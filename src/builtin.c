#include "builtin.h"

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "main.h"

// Builtin Commands Dictionary
builtin_commands builtins[] = {
    {"echo", &shell_echo}, {"exit", &shell_exit}, {"quit", &shell_exit},
    {"type", &shell_type}, {"pwd", &shell_pwd},
};

// Size of Dictionary
int builtins_size = sizeof(builtins) / sizeof(builtin_commands);

// Print inside terminal argument after echo command
int shell_echo(char* command)
{
    char* command_ptr = &command[strcspn(command, "echo")];
    command_ptr += 5;
    command_ptr += strspn(command_ptr, " ");
    printf("%s\n", command_ptr);
    return 0;
}

// Allows to exit shell
int shell_exit(char* command)
{
    if (command != NULL &&
        (!(strcmp(command, "exit")) || !(strcmp(command, "quit"))))
        exit(EXIT_SUCCESS);
}

// Print type of command
int shell_type(char* command)
{
    char* command_ptr = &command[strcspn(command, "type")];
    command_ptr += 5;
    command_ptr = strtok(command_ptr, " ");
    // Builtin
    for (int i = 0; i < builtins_size; i++)
    {
        if (strcmp(command_ptr, builtins[i].name) == 0)
        {
            printf("%s is a shell builtin\n", builtins[i].name);
            return 0;
        }
    }
    // In PATH
    char* path = in_path(command_ptr);
    if (path != NULL)
    {
        //<command> is <full_path>
        printf("%s is %s\n", command_ptr, path);
    }
    else
    {
        // <command>: not found
        printf("%s: not found\n", command_ptr);
        return 1;
    }
}

// Print working directory
int shell_pwd(char* command)
{
    char pwd[1024];

    if (getcwd(pwd, sizeof(pwd))) 
        printf("%s\n", pwd);
    return 0;
}
