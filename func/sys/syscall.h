#ifndef SYSCALL_H
#define SYSCALL_H

extern "C" {

long syswrite(long fd, const char* buf, unsigned long count) {
    long ret;
    register long r0 asm("x0") = fd;
    register const char* r1 asm("x1") = buf;
    register unsigned long r2 asm("x2") = count;
    register long r8 asm("x8") = 64;
    asm volatile("svc #0" : "=r"(ret) : "r"(r0), "r"(r1), "r"(r2), "r"(r8) : "memory");
    return ret;
}

void sysexit(int code) {
    register long r0 asm("x0") = code;
    register long r8 asm("x8") = 93;
    asm volatile("svc #0" : : "r"(r0), "r"(r8) : "memory");
}

long sysread(long fd, char* buf, unsigned long count) {
    long ret;
    register long r0 asm("x0") = fd;
    register char* r1 asm("x1") = buf;
    register unsigned long r2 asm("x2") = count;
    register long r8 asm("x8") = 63;
    asm volatile("svc #0" : "=r"(ret) : "r"(r0), "r"(r1), "r"(r2), "r"(r8) : "memory");
    return ret;
}

}
#endif