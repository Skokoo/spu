#ifndef VISUAL_H
#define VISUAL_H
#include "math.h"

extern "C" {

static void put_num(char *out, int *n, int val){
    char tmp[8];
    int t = 0;
    if(val==0) tmp[t++]='0';
    else while(val>0){ tmp[t++]='0'+(val%10); val/=10; }
    while(t>0) out[(*n)++] = tmp[--t];
}

void rgb_out(const char* s) {
    char out[64];
    int i = 0;
    while (s[i]) {
        if (s[i]==' ' || s[i]=='\t' || s[i]=='\n'){
            syswrite(1, &s[i++], 1);
            continue;
        }                
        double r_rad = 0.3 * i;
        double g_rad = 0.3 * i + 2.0;
        double b_rad = 0.3 * i + 4.0;
        
        while (r_rad > 6.283185307179586) r_rad -= 6.283185307179586;
        while (g_rad > 6.283185307179586) g_rad -= 6.283185307179586;
        while (b_rad > 6.283185307179586) b_rad -= 6.283185307179586;

        int r = (int)(msin(r_rad) * 127.0 + 128.0);
        int g = (int)(msin(g_rad) * 127.0 + 128.0);
        int b = (int)(msin(b_rad) * 127.0 + 128.0);

        int n = 0;
        out[n++]='\033'; out[n++]='['; out[n++]='3'; out[n++]='8';
        out[n++]=';'; out[n++]='2'; out[n++]=';';
        put_num(out,&n,r); out[n++]=';';
        put_num(out,&n,g); out[n++]=';';
        put_num(out,&n,b); out[n++]='m';
        out[n++]=s[i++];
        syswrite(1, out, n);
    }
    syswrite(1, "\033[0m", 4);
}

}
#endif