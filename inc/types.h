#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdio.h>

typedef enum ErrorType {
    MEMORY_ERROR,
    INPUT_ERROR,
    NOT_FOUND,
    DIRECTORY_ERROR,
} ErrorType;

typedef enum ExitCode {
    SUCCESS = 0,
    FAILURE = 1,
    NOENT = 2,
} ExitCode;

typedef struct Error {
    ErrorType type;
    char*     msg;
} Error;

typedef enum ContentType {
    NONE,
    ERROR,
    CHAR_PTR,
    INT_PTR,
    STRUCT_PTR,
} ContentType;

typedef struct Result {
    void*       content;
    ContentType type;
    Error*      err;
} Result;

typedef struct Arguments {
    bool   a;
    bool   l;
    bool   R;
    bool   r;
    bool   t;
    char** target_paths;
    size_t n_target_paths;
} Arguments;

#endif