#ifndef CRYPTO_H
#define CRYPTO_H
#include "../str.h"

extern "C" {

bool mregex(const char* pat, const char* txt) {
    if (pat[0] == '\0') return txt[0] == '\0';
    if (pat[0] == '*' && pat[1] == '\0') return true;
    if (pat[0] == '*') {
        if (mregex(pat + 1, txt)) return true;
        if (txt[0] != '\0' && mregex(pat, txt + 1)) return true;
        return false;
    }
    if (txt[0] != '\0' && (pat[0] == '.' || pat[0] == txt[0])) {
        return mregex(pat + 1, txt + 1);
    }
    return false;
}

int b64v(char c) {
    return (c >= 'A' && c <= 'Z') ? (c - 'A') : 
           (c >= 'a' && c <= 'z') ? (c - 'a' + 26) : 
           (c >= '0' && c <= '9') ? (c - '0' + 52) : 
           (c == '+') ? 62 : (c == '/') ? 63 : 0;
}

void b64e(const char* src, char* dst) {
    const char* map = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int i = 0, j = 0;
    unsigned long len = slen(src);
    while (i < len) {
        unsigned int b = (unsigned char)src[i++] << 16;
        b |= (i < len) ? (unsigned char)src[i++] << 8 : 0;
        b |= (i < len) ? (unsigned char)src[i++] : 0;
        dst[j++] = map[(b >> 18) & 0x3F];
        dst[j++] = map[(b >> 12) & 0x3F];
        dst[j++] = (i > len + 1) ? '=' : map[(b >> 6) & 0x3F];
        dst[j++] = (i > len) ? '=' : map[b & 0x3F];
    }
    dst[j] = '\0';
}

void b64d(const char* src, char* dst) {
    int i = 0, j = 0;
    unsigned long len = slen(src);
    while (i < len) {
        unsigned int b = b64v(src[i++]) << 18;
        b |= b64v(src[i++]) << 12;
        b |= (src[i] != '=') ? b64v(src[i++]) << 6 : (i++, 0);
        b |= (src[i] != '=') ? b64v(src[i++]) : (i++, 0);
        dst[j++] = (b >> 16) & 0xFF;
        if (src[i - 2] != '=') dst[j++] = (b >> 8) & 0xFF;
        if (src[i - 1] != '=') dst[j++] = b & 0xFF;
    }
    dst[j] = '\0';
}

void run_fnv1a_hash(const char* text) {
    unsigned long long hash = 14695981039346656037ULL;
    int i = 0;
    while (text[i] != '\0') {
        hash ^= (unsigned char)text[i];
        hash *= 1099511628211ULL;
        i++;
    }
    sp("0x");
    char buf[32];
    const char* chars = "0123456789abcdef";
    for (int j = 15; j >= 0; --j) {
        buf[j] = chars[hash & 0xF];
        hash >>= 4;
    }
    buf[16] = '\0';
    sp(buf);
    sp("\n");
}

}
#endif