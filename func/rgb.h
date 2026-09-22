#ifndef VISUAL_H
#define VISUAL_H

#include "math.h"

extern "C" {
void rgb_out(const char* s) {
    char out[64];
    int i = 0;
    while (s[i]) {
        if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n') {
            syswrite(1, &s[i], 1);
            i++;
            continue;
        }
        int r = (int)(msin(0.3 * i) * 127 + 128);
        int g = (int)(msin(0.3 * i + 2) * 127 + 128);
        int b = (int)(msin(0.3 * i + 4) * 127 + 128);
       
        int n = 0;
        n += __builtin_sprintf(out+n, "\033[38;2;%d;%d;%dm", r, g, b);
        out[n++] = s[i++];
        syswrite(1, out, n);
    }
    syswrite(1, "\033[0m", 4);
}
}

#endif