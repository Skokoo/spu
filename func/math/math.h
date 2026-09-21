#ifndef MATH_H
#define MATH_H

#include "../str.h"

extern "C" {
double pdouble(const char* s) {
    double val = 0.0;
    double div = 1.0;
    int i = 0;
    bool neg = false;
    if (s[i] == '-') { neg = true; i++; }
    while (s[i] >= '0' && s[i] <= '9') {
        val = val * 10.0 + (s[i] - '0');
        i++;
    }
    if (s[i] == '.') {
        i++;
        while (s[i] >= '0' && s[i] <= '9') {
            val = val * 10.0 + (s[i] - '0');
            div *= 10.0;
            i++;
        }
    }
    val /= div;
    return neg ? -val : val;
}
void pdouble_out(double val) {
    if (val < 0) {
        sp("-");
        val = -val;
    }
    long long part = (long long)val;
    double frac = val - (double)part;
    char buf[64];
    int idx = 0;
    if (part == 0) {
        buf[idx++] = '0';
    } else {
        char temp[64];
        int t = 0;
        while (part > 0) {
            temp[t++] = '0' + (part % 10);
            part /= 10;
        }
        while (t > 0) {
            buf[idx++] = temp[--t];
        }
    }
    buf[idx++] = '.';
    for (int i = 0; i < 6; i++) {
        frac *= 10.0;
        int digit = (int)frac;
        buf[idx++] = '0' + digit;
        frac -= digit;
    }
    buf[idx] = '\0';
    sp(buf);
}

double msin(double x) {
    double res = 0.0;
    double term = x;
    double num = x * x;
    int sign = 1;
    double fact = 1.0;
    for (int i = 1; i <= 9; i += 2) {
        if (i > 1) {
            term *= num;
            fact *= (i - 1) * i;
        }
        res += sign * (term / fact);
        sign = -sign;
    }
    return res;
}

double qpsi(double x) {
    double l = 100.0;
    double k = 3.141592653589793 / l;
    double psi = msin(k * x);
    return psi * psi;
}

double mlog(double x) {
    if (x <= 0.0) return 0.0;
    double num = (x - 1.0) / (x + 1.0);
    double denom = num * num;
    double res = 0.0;
    double series = num;
    for (int i = 1; i < 20; i += 2) {
        res += series / i;
        series *= denom;
    }
    return 2.0 * res;
}

double mlog2(double x) {
    return mlog(x) / 0.6931471805599453;
}
double mcalc(double n1, const char* op, double n2) {
    char c = *op;
    return (c == '+') ? (n1 + n2) : 
           (c == '-') ? (n1 - n2) : 
           (c == '*') ? (n1 * n2) : 
           (c == '/') ? ((n2 == 0.0) ? 0.0 : (n1 / n2)) : 0.0;
}
}

#endif