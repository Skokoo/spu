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
        int r = (int)(msin(0.3*i)*127+128);
        int g = (int)(msin(0.3*i+2)*127+128);
        int b = (int)(msin(0.3*i+4)*127+128);

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