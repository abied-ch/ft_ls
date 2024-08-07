#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "../libft/include/libft.h"
#include "types.h"

Result result(void* content, ContentType contentType, Error* err) {
    struct Result res;

    res.content = content;
    res.type = contentType;
    res.err = err;
    return res;
}

Result newError(ErrorType type, char* message) {
    Error* err = ft_calloc(1, sizeof(Error));
    if (!err) {
        return result(NULL, ERROR, NULL);
    }

    err->type = type;
    err->msg = ft_strdup(message);
    if (!err->msg) {
        free(err);
        return result(NULL, ERROR, NULL);
    }

    Result res;
    res.err = err;
    res.type = ERROR;
    res.content = NULL;
    return res;
}

int displayError(Error* err) {
    if (!err) {
        return FAILURE;
    }

    int type = err->type;

    ft_putendl_fd(err->msg, STDERR_FILENO);
    free(err->msg);
    free(err);

    if (type == NOT_FOUND) {
        return ENOENT;
    }
    return FAILURE;
}

typedef struct Arguments {
    bool   a;
    bool   l;
    bool   R;
    bool   r;
    bool   t;
    char** targetPaths;
} Arguments;

Result parseArgumentString(Arguments* args, char* arg) {
    char errorTemplate[29] = "ft_ls: invalid option -- '_'";

    for (int idx = 0; arg[idx]; ++idx) {
        switch (arg[idx]) {
            case 'a':
                args->a = true;
                break;
            case 'l':
                args->l = true;
                break;
            case 'R':
                args->R = true;
                break;
            case 'r':
                args->r = true;
                break;
            case 't':
                args->t = true;
                break;
            default:
                errorTemplate[26] = arg[idx];
                return newError(INPUT_ERROR, errorTemplate);
        }
    }
    return result(NULL, NONE, NULL);
}

Result parseArguments(char** av) {
    Arguments args = {0};

    for (int idx = 0; av[idx]; ++idx) {
        if (av[idx][0] == '-') {
            if (ft_strlen(av[idx]) == 1) {
                return newError(NOT_FOUND, "ft_ls: cannot access '-': No such file or directory");
            }
            Result res = parseArgumentString(&args, &av[idx][1]);
            if (res.err != NULL) {
                return res;
            }
        }
    }
    return result(&args, STRUCT_PTR, NULL);
}

int main(int ac, char** av) {
    if (ac > 1) {
        Result res = parseArguments(&av[1]);
        if (res.type == ERROR) {
            return displayError(res.err);
        }
    }
    return 0;
}
