#include <stdio.h>
#include <aio.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 1024
#define TOKEN_BUF_SIZE 64
#define TOKEN_DELIM " \t\n"

char *read_line()
{
    int buf_size = SIZE;
    char *buffer = malloc(sizeof(char) * buf_size);
    int position = 0;
    int c;
    if (!buffer)
    {
        fprintf(stderr, "allocation error\n");
        exit(1);
    }
    while (true)
    {
        c = getchar();
        if (c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = c;
        }
        position++;
        if (position >= buf_size)
        {
            buf_size += SIZE;
            buffer = realloc(buffer, buf_size * sizeof(char));
            if (!buffer)
            {
                fprintf(stderr, "reallocation error\n");
                exit(1);
            }
        }
    }
}


char **tokenize(char *line)
{
    int buf_size = TOKEN_BUF_SIZE, position = 0;
    char **tokens = malloc(sizeof(char *) * buf_size);
    char *token;
    if (!tokens)
    {
        fprintf(stderr, "memory allocation error\n");
        exit(1);
    }
    token = strtok(line, TOKEN_DELIM);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;
        if (position >= buf_size)
        {
            buf_size += TOKEN_BUF_SIZE;
            tokens = realloc(tokens, buf_size * sizeof(char *));
            if (!tokens)
            {
                fprintf(stderr, "reallocation error\n");
                exit(1);
            }
        }
        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

void helper(char **tokens)
{
    char **ptr = tokens;
    for (char *c = *ptr; c; c = *++ptr)
    {
        printf("%s\n", c);
    }
}

char *get_current_dir()
{
    char *buffer;
    buffer = getcwd(NULL, 0);
    if (buffer == NULL)
    {
        fprintf(stderr, "getcwd errror\n");
        exit(1);
    }
    return buffer;
}

int execute(char **args)
{
    if (args[0] == NULL)
    {
        return 1;
    }
    if (strcmp(args[0], "exit") == 0)
    {
        printf("Bye!\n");
        exit(0);
    }

    else if (strcmp(args[0], "cd") == 0)
    {
        execute_cd(args[1]);
    }
    else {
        launch_child(args);
    }
    return 1;
}

int execute_cd(char *path)
{
    if (path == NULL)
    {
        fprintf(stderr, "Provide argument for cd command\n");
        return 1;
    }
    if (chdir(path) != 0)
    {
        perror("cd");
    }
    return 1;
}

int launch_child(char **args)
{
    pid_t child = fork();
    if (child == -1)
    {
        fprintf(stderr, "oh no my child!\n");
        return 1;
    }
    else if (child == 0)
    {
        if (execvp(args[0], args) < 0)
        {
            fprintf(stderr, "failed to execute commmand\n");
        }
    }
    else
    {
        wait(NULL);
    }
    return 1;
}

void main_loop()
{
    char *line;
    char **args;
    bool status = true;
    char *dir = get_current_dir();
    while(true) {
        printf("%s > ", dir);
        line = read_line();
        args = tokenize(line);
        execute(args);
        free(line);
        free(args);
        free(dir);
        dir = get_current_dir();
    }
}

int main(int argc, char **argv)
{
    main_loop();
}
