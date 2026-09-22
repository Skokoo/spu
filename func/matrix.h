#ifndef MATRIX_H
#define MATRIX_H

#include "math.h"

extern "C" {
static int put_int(char *buf, int off, int v){
    char tmp[10];
    int t = 0;
    if(v==0) tmp[t++]='0';
    else while(v>0){ tmp[t++]='0'+(v%10); v/=10; }
    while(t>0) buf[off++] = tmp[--t];
    return off;
}

void run_matrix(int duration) {
    syswrite(1, "\033[2J\033[?25l", 10);
    int pos[80];
    for(int i=0;i<80;i++) pos[i]=-(i%15);

    for(int loop=0; loop<duration*50; loop++){
        for(int i=0;i<80;i++){
            if(pos[i]>=0 && pos[i]<24){
                char buf[32];
                int n=0;
                buf[n++]='\033'; buf[n++]='[';
                n=put_int(buf,n,pos[i]+1); buf[n++]=';';
                n=put_int(buf,n,i+1); buf[n++]='H';
                buf[n++]='\033'; buf[n++]='['; buf[n++]='3'; buf[n++]='8';
                buf[n++]=';'; buf[n++]='2'; buf[n++]=';';
                buf[n++]='0'; buf[n++]=';';
                n=put_int(buf,n,255); buf[n++]=';';
                buf[n++]='0'; buf[n++]='m';
                buf[n++]='!' + (loop+i)%93;
                syswrite(1,buf,n);
            }
            if(pos[i]-1>=0 && pos[i]-1<24){
                char b2[16]; int c=0;
                b2[c++]='\033'; b2[c++]='[';
                c=put_int(b2,c,pos[i]); b2[c++]=';';
                c=put_int(b2,c,i+1); b2[c++]='H'; b2[c++]=' ';
                syswrite(1,b2,c);
            }
            pos[i]++;
            if(pos[i]>=24) pos[i]=0;
        }
        for(volatile long d=0; d<8000000; d++);
    }
    syswrite(1, "\033[?25h\033[2J\033[H", 11);
}

}
#endif