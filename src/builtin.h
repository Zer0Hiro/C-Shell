#ifndef BUILTIN_H
#define BUILTIN_H

#define BUFFER 1024

typedef struct
{
    char* name;
    int (*b_func)(char* args);
} builtin_commands;

struct dirent* entry;

int shell_echo(char* command);
int shell_exit(char* command);
int shell_type(char* command);

builtin_commands builtins[] = {
    {"echo", &shell_echo},
    {"exit", &shell_exit},
    {"quit", &shell_exit},
    {"type", &shell_type},
};

int builtins_size = sizeof(builtins) / sizeof(builtin_commands);

#endif