#include <stdbool.h>
#include <stdio.h>
#include "../libft/include/libft.h"

typedef enum ErrorType {
    SUCCESS,
    MEMORY_ERROR,
    ARGUMENT_ERROR,
} ErrorType;

typedef struct Error {
    ErrorType type;
    char*     msg;
} Error;

typedef enum ContentType {
    CHAR_PTR,
    INT_PTR,
    STRUCT_PTR,
} ContentType;

typedef struct Result {
    void*       content;
    ContentType type;
    Error       err;
} Result;

Result result(void* content, ContentType contentType, Error err) {
    struct Result res;

    res.content = content;
    res.type = contentType;
    res.err = err;
    return res;
}

Error newError(ErrorType type, char* message) {
    struct Error err;

    err.type = type;
    err.msg = message;
    return err;
}

typedef struct Arguments {
    bool   a;
    bool   l;
    bool   R;
    bool   r;
    bool   t;
    char** targetPaths;
} Arguments;

Result parseArguments(char** av) {
    Arguments args;

    for (int i = 0; av[i]; ++i) {
        for (int j = 0; av[i][j]; ++j) {
            if (av[i][0] == '-' && ft_strlen(av[i]) > 1) {
                // TODO: handle file arguments
            }
        }
    }
}

Result function() { return result("Hello, World!", CHAR_PTR, newError(SUCCESS, NULL)); }

int main(int ac, char** av) {
    if (ac > 1) {
        Result res = parseArguments(&av[1]);
        if (res.err.type != SUCCESS) {
        }
    }
}
