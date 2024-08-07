#ifndef TYPES_H
#define TYPES_H

typedef enum ErrorType {
    MEMORY_ERROR,
    INPUT_ERROR,
    NOT_FOUND,
} ErrorType;

typedef enum ExitCode {
    SUCCESS = 0,
    FAILURE = 1,
    ENOENT = 2,
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

#endif