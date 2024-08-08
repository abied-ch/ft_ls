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
} ExitCode;

typedef struct Error {
    ErrorType type;
    char*     msg;
} Error;

typedef enum ContentType {
    NONE,
    ERROR,
    CHAR_PTR,
    CHAR_DOUBLE_PTR,
    INT_PTR,
    STRUCT_PTR,
} ContentType;

typedef struct Result {
    void*       content;
    ContentType type;
    Error*      err;
} Result;

typedef struct Directory {
    char** file_paths;
    char*  name;
} Directory;

typedef struct DirectoryList {
    Directory* dir;
    Directory* next;
} DirectoryList;

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