#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "commands.h"

#define SIZE 1024
#define TOKEN_BUF_SIZE 64
#define TOKEN_DELIM " \t\n"

char *read_line() {
    int buf_size = SIZE;
    char *buffer = malloc(sizeof(char) * buf_size);
    int position = 0;
    int c;
    if (!buffer) {
        fprintf(stderr, "allocation error");
        exit(1);
    }
    while (1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;
        if (position >= buf_size) {
            buf_size += SIZE;
            buffer = realloc(buffer, buf_size * sizeof(char));
            if (!buffer) {
                fprintf(stderr, "reallocation error");
                exit(1);
            }
        }
    }
}

char **tokenize(char *line) {
    int buf_size = TOKEN_BUF_SIZE, position = 0;
    char **tokens = malloc(sizeof(char*) * buf_size);
    char *token;
    if (!tokens) {
        fprintf(stderr, "memory allocation error");
        exit(1);
    }
    token = strtok(line, TOKEN_DELIM);
    while(token != NULL) {
        tokens[position] = token;
        position++;
        if (position >= buf_size) {
            buf_size += TOKEN_BUF_SIZE;
            tokens = realloc(tokens, buf_size * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "reallocation error");
                error(1);
            }
        }
        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

void helper(char **tokens) {
    char **ptr = tokens;
    for (char *c = *ptr; c; c = *++ptr) {
        printf("%s\n", c);
    }
}

char* get_current_dir() {
    char *buffer;
    buffer = getcwd(NULL, 0);
    if (buffer == NULL) {
        fprintf(stderr, "getcwd errro");
        exit(1);
    }
    return buffer;
}

int execute(char **args) {
    if (args[0] == NULL) {
        return -1;
    }
    if(strcmp(args[0], "exit") == 0) {
        printf("Bye!");
        exit(0);
    }
}

void main_loop() {
    char *line;
    char **args;
    bool status = true;
    char *dir = get_current_dir();
    do {
       printf("%s > ", dir); 
       line = read_line();
       args = tokenize(line);
       execute(args);
    } while (status);
    free(dir);
}

int main(int argc, char** argv) {
    main_loop();
}