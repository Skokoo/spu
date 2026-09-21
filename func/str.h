#ifndef STR_H
#define STR_H

#include "sys/syscall.h"

extern "C" {
int pint(const char* s) {
    while (*s == ' ' || *s == '\t') s++;
    int sign = (*s == '-') ? -1 : 1;
    if (*s == '-' || *s == '+') s++;
    int res = 0;
    while (*s >= '0' && *s <= '9') res = (res << 3) + (res << 1) + (*s++ - '0');
    return res * sign;
}
bool pinit(const char* s, int* val) {
    while (*s && *s != '=') s++;
    return (*s == '=') ? (*val = pint(s + 1), true) : false;
}

void cstr(char* dst, const char* src, unsigned long n) {
    while (n--) *dst++ = *src++;
}

bool pstep(const char* step, int* tg) {
    while (*step && *step != '+' && *step != '-') step++;
    if (*step == '+' && *(step + 1) == '=') return (*tg = pint(step + 2), true);
    if (*step == '-' && *(step + 1) == '=') return (*tg = -pint(step + 2), true);
    return false;
}
unsigned long slen(const char* s) {
    const char* p = s;
    while (*p) p++;
    return (unsigned long)(p - s);
}

bool esingle(const char* v1, const char* op, const char* v2) {
    int n1 = pint(v1), n2 = pint(v2);
    return (*(op + 1) == 'e') ? (n1 == n2) : 
           (*(op + 1) == 'n') ? (n1 != n2) : 
           (*(op + 1) == 'g') ? ((*(op + 2) == 'e') ? (n1 >= n2) : (n1 > n2)) : 
           (*(op + 1) == 'l') ? ((*(op + 2) == 'e') ? (n1 <= n2) : (n1 < n2)) : false;
}
void sp(const char* str) {
    syswrite(1, str, slen(str));
}

bool mstr(const char* s1, const char* s2) {
    for (; *s1 == *s2; s1++, s2++) if (*s1 == '\0') return true;
    return false;
}
bool plimit(const char* tok, int* lim) {
    while (*tok && *tok != '-') tok++;
    return (*tok == '-') ? (*lim = pint(tok + 3), true) : false;
}
unsigned long tansi(char* buf, const char* raw) {
    char* start = buf;
    int state = 0;
    while (true) {
        char c = *raw++;
        if (!c) break;
        if (state == 0) {
        if (c == '\\') state = 1;
        else *buf++ = c;
            } else {
        if (c == '0' && *raw == '3' && *(raw + 1) == '3') {
        *buf++ = '\033'; raw += 2;
         } else if (c == 'x' && *raw == '1' && *(raw + 1) == 'b') {
         *buf++ = '\033'; raw += 2;
         } else {
         *buf++ = (c == 'n') ? '\n' : c;
             }
         state = 0;
                 }
                    }
    *buf = '\0';
    return (unsigned long)(buf - start);
}
}


#endif