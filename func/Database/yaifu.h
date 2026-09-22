#ifndef YAIFU_H
#define YAIFU_H

#include "../str.h"

extern "C" {
char* _s(char* str, unsigned long len) {
    for (unsigned long i = 0; i < len; i++) {
        str[i] ^= 0x5A;
    }
    return str;
}

unsigned long long get_hw() {
    unsigned long long h = 0xDE7A10543210ULL;
    long fd = sysopen("/proc/sys/kernel/random/boot_id", 0, 0);
    if (fd >= 0) {
        char buf[64];
        long n = sysread(fd, buf, 64);
        for (long i = 0; i < n; i++) h = (h * 33) ^ buf[i];
        sysclose(fd);
    }
    return h;
}

void lorenz(unsigned long long sd, int len, unsigned char* out) {
    double x = 0.1, y = 0.0, z = 0.0, dt = 0.01;
    double sg = 10.0, rh = 28.0, bt = 8.0 / 3.0;
    x += (double)(sd & 0xFF) / 255.0;
    y += (double)((sd >> 8) & 0xFF) / 255.0;
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < 10; j++) {
            double dx = sg * (y - x) * dt;
            double dy = (x * (rh - z) - y) * dt;
            double dz = (x * y - bt * z) * dt;
            x += dx; y += dy; z += dz;
        }
        unsigned long long r;
        char* src = (char*)&x;
        char* dst = (char*)&r;
        for (unsigned long c = 0; c < 8; c++) dst[c] = src[c];
        out[i] = (r >> 16) & 0xFF;
    }
}

unsigned char sbox(unsigned char v, unsigned char k, int idx, int enc) {
    unsigned char s = v;
    unsigned char mod = (k ^ idx) | 1;
    if (enc) {
        s ^= k;
        s = (s * mod) & 0xFF;
        s = ((s << 3) | (s >> 5)) & 0xFF;
        return ~s;
    } else {
        s = ~s;
        s = ((s >> 3) | (s << 5)) & 0xFF;
        unsigned char inv = 0;
        for (int i = 0; i < 256; i++) {
            if (((i * mod) & 0xFF) == 1) { inv = i; break; }
        }
        return ((s * inv) & 0xFF) ^ k;
    }
}
void core(unsigned char* data, int len, const char* key, unsigned long long salt, int enc) {
    unsigned long long sd = salt ^ get_hw();
    for (int i = 0; key[i] != '\0'; i++) sd = (sd * 6364136223846793005ULL) + key[i];
    unsigned char lz[4096];
    if (len > 4096) len = 4096;
    lorenz(sd, len, lz);
    if (enc) {
        unsigned char csd = (sd >> 24) & 0xFF;
        for (int i = 0; i < len; i++) {
            unsigned char st1 = sbox(data[i], lz[i], i, 1);
            unsigned char xor_v = st1 ^ lz[i] ^ csd;
            int sh = (lz[i] ^ i) % 8;
            data[i] = ((xor_v << sh) | (xor_v >> (8 - sh))) & 0xFF;
            csd = data[i];
        }
    } else {
        unsigned char csd = (sd >> 24) & 0xFF;
        for (int i = len - 1; i >= 0; i--) {
            int sh = (lz[i] ^ i) % 8;
            unsigned char unr = ((data[i] >> sh) | (data[i] << (8 - sh))) & 0xFF;
            unsigned char prv = (i > 0) ? data[i - 1] : csd;
            unsigned char st1 = unr ^ lz[i] ^ prv;
            data[i] = sbox(st1, lz[i], i, 0);
        }
    }
}
void get_key(char* out) {
    const char ch[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    long fd = sysopen("/dev/urandom", 0, 0);
    if (fd >= 0) {
        char buf[64];
        sysread(fd, buf, 64);
        for (int i = 0; i < 64; i++) {
            unsigned char r = buf[i];
            out[i] = ch[r % 62];
        }
        sysclose(fd);
    } else {
        unsigned long long seed = (unsigned long long)out;
        for (int i = 0; i < 64; i++) {
            seed = (seed * 6364136223846793005ULL) + 1;
            out[i] = ch[(seed >> 32) % 62];
        }
    }
    out[64] = '\0';
}

void db_save(const char* fn, const char* txt, const char* ky) {
    long fd = sysopen(fn, 577, 438);
    if (fd < 0) return;
    unsigned long long sl = 0;
    long ufd = sysopen("/dev/urandom", 0, 0);
    if (ufd >= 0) { sysread(ufd, (char*)&sl, 8); sysclose(ufd); }
    unsigned char head[10] = {'P', 'K', 0x03, 0x04, 0x14, 0x00, 0x08, 0x00, 0x08, 0x00};
    syswrite(fd, (char*)head, 10);
    syswrite(fd, (char*)&sl, 8);
    unsigned long len = slen(txt);
    int ilen = (int)len;
    syswrite(fd, (char*)&ilen, 4);
    char buf[4096];
    for (unsigned long i = 0; i < len; i++) buf[i] = txt[i];
    core((unsigned char*)buf, ilen, ky, sl, 1);
    syswrite(fd, buf, ilen);
    sysclose(fd);
}
void db_load(const char* fn, const char* ky) {
    long fd = sysopen(fn, 0, 0);
    if (fd < 0) { sp("Error: File open failed\n"); return; }
    char head[10];
    sysread(fd, head, 10);
    unsigned long long sl;
    int len;
    sysread(fd, (char*)&sl, 8);
    sysread(fd, (char*)&len, 4);
    if (len > 4096) len = 4096;
    char buf[4097];
    sysread(fd, buf, len);
    sysclose(fd);
    core((unsigned char*)buf, len, ky, sl, 0);
    buf[len] = '\0';
    bool val = true;
    for (int i = 0; i < len; i++) {
        if (buf[i] < 32 || buf[i] > 126) { val = false; break; }
    }
    if (val) {
        sp(buf);
        sp("\n");
        } else {
        sp("WARNING: KEY DEFUNCT.\n");
    } 
}
}
#endif