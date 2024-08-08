#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

Result new_error(ErrorType type, char* message) {
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

Result parse_arg(Arguments* args, char* arg) {
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
                return new_error(INPUT_ERROR, errorTemplate);
        }
    }
    return result(NULL, NONE, NULL);
}

void free_matrix(char** m) {
    if (!m) {
        return;
    }

    for (int idx = 0; m[idx]; ++idx) {
        free(m[idx]);
    }
    free(m);
}

// SAFETY:
// This function assumes that the matrix is null-terminated
Result append_string_to_matrix(char** old, char* s) {
    size_t oldsize = 0;

    while (old && old[oldsize]) {
        oldsize++;
    }

    char** new = ft_calloc(oldsize + 2, sizeof(char*));
    if (!new) {
        return new_error(MEMORY_ERROR, strerror(errno));
    }

    size_t idx = 0;
    while (idx < oldsize) {
        new[idx] = ft_strdup(old[idx]);
        if (!new[idx]) {
            free_matrix(new);
            return new_error(MEMORY_ERROR, strerror(errno));
        }
        idx++;
    }

    new[idx] = ft_strdup(s);
    if (!new[idx]) {
        free_matrix(new);
        return new_error(MEMORY_ERROR, strerror(errno));
    }
    new[idx + 1] = NULL;
    return result(new, CHAR_DOUBLE_PTR, NULL);
}

Result add_file_arg(Arguments* args, char* arg) {
    char** target_paths = ft_calloc(++args->n_target_paths + 1, sizeof(char*));
    size_t idx = 0;

    while (args->target_paths && args->target_paths[idx]) {
        target_paths[idx] = ft_strdup(args->target_paths[idx]);
        if (!target_paths[idx]) {
            free_matrix(target_paths);
            return new_error(MEMORY_ERROR, strerror(errno));
        }
        idx++;
    }

    target_paths[idx] = ft_strdup(arg);
    if (!target_paths[idx]) {
        free_matrix(target_paths);
        return new_error(MEMORY_ERROR, strerror(errno));
    }
    target_paths[idx + 1] = NULL;
    free_matrix(args->target_paths);
    args->target_paths = target_paths;
    return result(NULL, NONE, NULL);
}

Result parse_args(char** av) {
    Arguments* args = ft_calloc(1, sizeof(Arguments));
    if (!args) {
        return new_error(MEMORY_ERROR, strerror(errno));
    }

    for (int idx = 0; av[idx]; ++idx) {
        if (av[idx][0] == '-') {
            if (ft_strlen(av[idx]) == 1) {
                return new_error(NOT_FOUND, "ft_ls: cannot access '-': No such file or directory");
            }
            Result res = parse_arg(args, &av[idx][1]);
            if (res.err != NULL) {
                return res;
            }
        } else {
            Result res = add_file_arg(args, av[idx]);
            if (res.err != NULL) {
                return res;
            }
        }
    }
    return result(args, STRUCT_PTR, NULL);
}

// Result append_dir() {}

Result walk_dir(Arguments* args, char* dirname) {
    if (args && (args->a || args->target_paths)) {
        printf("%s:\n", dirname);
    }
    DIR* dir = opendir(dirname);
    if (!dir) {
        if (errno == ENOTDIR) {
            return result(NULL, NONE, NULL);
        }
        return new_error(DIRECTORY_ERROR, strerror(errno));
    }
    struct dirent* dir_data = readdir(dir);

    while (dir_data) {
        printf("%s ", dir_data->d_name);
        dir_data = readdir(dir);
    }
    closedir(dir);
    return result(NULL, NONE, NULL);
}

int main(int ac, char** av) {
    Arguments* args = NULL;

    if (ac > 1) {
        Result res = parse_args(&av[1]);
        if (res.type == ERROR) {
            return displayError(res.err);
        }
        args = (Arguments*)res.content;
    }
    DirectoryList* dir_list = ft_calloc(1, sizeof(DirectoryList));
    if (!dir_list) {
        perror("memory allocation for directory list failed");
        return EXIT_FAILURE;
    }
    Result res = walk_dir(args, ".");
    if (res.type == ERROR) {
        return displayError(res.err);
    }
    if (ac > 1) {
        free_matrix(args->target_paths);
        free(args);
    }
    return 0;
}
