#ifndef BUILTIN_H
#define BUILTIN_H

/*
    Includes all builtin commands
*/

typedef struct
{
    char* name;
    int (*b_func)(char* args);
} builtin_commands;

int shell_echo(char* command);
int shell_exit(char* command);
int shell_type(char* command);
int shell_pwd(char* command);

extern builtin_commands builtins[];
extern int builtins_size;

#endif