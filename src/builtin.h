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

char* argument_parser(char* command, char* name);

int shell_echo(char* command);
int shell_exit(char* command);
int shell_type(char* command);
int shell_pwd(char* command);
int shell_cd(char* command);

extern builtin_commands builtins[];
extern int builtins_size;

#endif