#include "platform_compat.h"

#ifdef _WIN32

#include <string.h>

char *optarg;
int optind = 1;
static int optpos = 1;

static int shortopt_has_arg(const char *optstring, char c)
{
    const char *p = strchr(optstring, c);
    if (!p)
        return -1;
    return p[1] == ':' ? 1 : 0;
}

int getopt_long(int argc, char * const argv[], const char *optstring,
    const struct option *longopts, int *longindex)
{
    optarg = NULL;

    if (optind >= argc)
        return -1;

    char *arg = argv[optind];
    if (arg[0] != '-' || arg[1] == '\0')
        return -1;

    if (strcmp(arg, "--") == 0) {
        optind++;
        return -1;
    }

    if (arg[1] == '-') {
        const char *name = arg + 2;
        const char *eq = strchr(name, '=');
        size_t name_len = eq ? (size_t)(eq - name) : strlen(name);

        for (int i = 0; longopts && longopts[i].name; i++) {
            if (strncmp(name, longopts[i].name, name_len) != 0 || longopts[i].name[name_len] != '\0')
                continue;

            if (longindex)
                *longindex = i;

            if (longopts[i].has_arg == required_argument) {
                if (eq)
                    optarg = (char*)eq + 1;
                else if (optind + 1 < argc)
                    optarg = argv[++optind];
                else {
                    optind++;
                    return '?';
                }
            }

            optind++;
            if (longopts[i].flag) {
                *longopts[i].flag = longopts[i].val;
                return 0;
            }
            return longopts[i].val;
        }

        optind++;
        return '?';
    }

    char c = arg[optpos++];
    int has_arg = shortopt_has_arg(optstring, c);
    if (has_arg < 0) {
        if (arg[optpos] == '\0') {
            optind++;
            optpos = 1;
        }
        return '?';
    }

    if (has_arg) {
        if (arg[optpos] != '\0') {
            optarg = &arg[optpos];
            optind++;
        } else if (optind + 1 < argc) {
            optarg = argv[++optind];
            optind++;
        } else {
            optind++;
            optpos = 1;
            return '?';
        }
        optpos = 1;
    } else if (arg[optpos] == '\0') {
        optind++;
        optpos = 1;
    }

    return c;
}

#endif
