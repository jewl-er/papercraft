#ifndef PLATFORM_COMPAT_H
#define PLATFORM_COMPAT_H

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

#define read _read
#define open _open
#define close _close

struct option {
    const char *name;
    int has_arg;
    int *flag;
    int val;
};

#define no_argument 0
#define required_argument 1
#define optional_argument 2

extern char *optarg;
extern int optind;
int getopt_long(int argc, char * const argv[], const char *optstring,
    const struct option *longopts, int *longindex);

static inline void errx(int eval, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
    exit(eval);
}


#define getpid _getpid
#define srand48(x) srand((unsigned int)(x))
#define lrand48() ((long) rand())

#define PACKED
#else
#include <unistd.h>
#include <getopt.h>
#include <err.h>
#define PACKED __attribute__((__packed__))
#endif

#endif
