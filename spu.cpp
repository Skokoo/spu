#include "func/str.h"
#include "func/math/math.h"
#include "func/rgb.h"
#include "func/crypto/crypto.h"

extern "C" {
void man(int argc, char** argv) {
    if (__builtin_expect(argc < 2, 0)) {
        syswrite(1, "\n", 1);
        sysexit(0);
    }

    const char* flag = argv[1];

    if (mstr(flag, "-h")) {
        sp("\nUsage: spu \033[34m[flag] [args]\033[0m\n\n");
        sp("Option:\n");
        sp("  -h    : Show help menu\n");
        sp("  -ex   : Show Example\n");
        sp("  -i    : Read stdin stream\n");
        sp("  -re   : Regex match -> sp -re [pattern] [text]\n");
        sp("  -hash : FNV-1a 64-bit Checksum -> sp -hash [text]\n");
        sp("  -m    : Math solver -> sp -m [func] [val]\n");
        sp("  -L    : Loop execution -> sp -L [init] [cond] [text] [step]\n");
        sp("  -F    : Conditional IF -> sp -F [v1] [op] [v2] (and/or ...) [text]\n");
        sp("  -b64e : Base64 Encode -> sp -b64e [text]\n");
        sp("  -b64d : Base64 Decode -> sp -b64d [hash]\n");
        sp("  -qnt  : Randomness generator & wave simulation -> sp -qnt [pos]\n\n");
        sysexit(0);
    }

    if (mstr(flag, "-hash")) {
        if (argc < 3) sysexit(1);
        run_fnv1a_hash(argv[2]);
        sysexit(0);
    }

    if (match_str(flag, "-rgb")) {
        if (argc < 3) sys_exit(1);
        rgb_out(argv[2]);
        print_raw("\n");
        sys_exit(0);
    }

    if (mstr(flag, "-qnt")) {
        if (argc < 3) sysexit(1);
        double pos = pdouble(argv[2]);
        if (pos < 0.0 || pos > 100.0) {
            sp("0.000000\n");
            sysexit(0);
        }
        double prob = qpsi(pos);
        pdouble_out(prob);
        sp("\n");
        sysexit(0);
    }

    if (mstr(flag, "-b64e")) {
        if (argc < 3) sysexit(1);
        char out[8192];
        b64e(argv[2], out);
        sp(out);
        sp("\n");
        sysexit(0);
    }

    if (mstr(flag, "-b64d")) {
        if (argc < 3) sysexit(1);
        char out[8192];
        b64d(argv[2], out);
        sp(out);
        sp("\n");
        sysexit(0);
    }
    if (mstr(flag, "-re")) {
        if (argc < 4) sysexit(1);
        if (mregex(argv[2], argv[3])) {
            sp("true\n");
        } else {
            sp("false\n");
        }
        sysexit(0);
    }

    if (mstr(flag, "-L")) {
        if (argc < 6) sysexit(1);
        int start = 0, limit = 0, step = 1;
        pinit(argv[2], &start);
        plimit(argv[3], &limit);
        pstep(argv[5], &step);
        char raw[4096];
        unsigned long rlen = tansi(raw, argv[4]);
        char space[4096];
        unsigned long slen_val = tansi(space, " ");
        static char io[65536];
        unsigned long curr = 0;
        for (int i = start; i < limit; i += step) {
            if (curr + rlen + slen_val >= 65536) {
                syswrite(1, io, curr);
                curr = 0;
            }
            cstr(io + curr, raw, rlen);
            curr += rlen;
            cstr(io + curr, space, slen_val);
            curr += slen_val;
        }
        if (curr > 0) syswrite(1, io, curr);
        sp("\n");
        sysexit(0);
    }
    if (mstr(flag, "-F")) {
        if (argc < 6) sysexit(1);
        int base = 2;
        bool res = esingle(argv[base], argv[base + 1], argv[base + 2]);
        base += 3;
        while (base + 3 < argc) {
            const char* op = argv[base];
            bool next = esingle(argv[base + 1], argv[base + 2], argv[base + 3]);
            if (mstr(op, "and")) res = res && next;
            else if (mstr(op, "or")) res = res || next;
            base += 4;
        }
        if (res) {
            char out[4096];
            unsigned long olen = tansi(out, argv[argc - 1]);
            syswrite(1, out, olen);
            sp("\n");
        }
        sysexit(0);
    }

    if (mstr(flag, "-i")) {
        char buf[256];
        long bytes = sysread(0, buf, 255);
        if (bytes > 0) {
            buf[bytes] = '\0';
            syswrite(1, buf, bytes);
        }
        sysexit(0);
    }

    if (mstr(flag, "-m")) {
        if (argc < 4) sysexit(1);
        double res = 0.0;
        int idx = 2;

        if (mstr(argv[idx], "sin")) {
            res = msin(pdouble(argv[idx + 1]));
            idx += 2;
        } else if (mstr(argv[idx], "log")) {
            res = mlog(pdouble(argv[idx + 1]));
            idx += 2;
        } else if (mstr(argv[idx], "log2")) {
            res = mlog2(pdouble(argv[idx + 1]));
            idx += 2;
        } else {
            res = pdouble(argv[idx]);
            idx += 1;
        }

        while (idx + 1 < argc) {
            const char* op = argv[idx];
            double next = 0.0;

            if (mstr(argv[idx + 1], "sin")) {
                next = msin(pdouble(argv[idx + 2]));
                idx += 3;
            } else if (mstr(argv[idx + 1], "log")) {
                next = mlog(pdouble(argv[idx + 2]));
                idx += 3;
            } else if (mstr(argv[idx + 1], "log2")) {
                next = mlog2(pdouble(argv[idx + 2]));
                idx += 3;
            } else {
                next = pdouble(argv[idx + 1]);
                idx += 2;
            }

            res = mcalc(res, op, next);
        }

        pdouble_out(res);
        sp("\n");
        sysexit(0);
    }

    for (int i = 1; i < argc; i++) {
        char out[256];
        unsigned long olen = tansi(out, argv[i]);
        syswrite(1, out, olen);
        if (i < argc - 1) syswrite(1, " ", 1);
    }
    syswrite(1, "\n", 1);
    sysexit(0);
}

__attribute__((naked)) void _start() {
    asm volatile(
        "ldr x0, [sp]\n\t"
        "add x1, sp, #8\n\t"
        "b man\n\t"
    );
}

}