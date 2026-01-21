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
    {"cd", &shell_cd},
};

// Size of Dictionary
int builtins_size = sizeof(builtins) / sizeof(builtin_commands);

char* argument_parser(char* command, char* name)
{
    char* command_ptr = &command[strcspn(command, name)];
    command_ptr += strlen(name) + 1;
    command_ptr += strspn(command_ptr, " ");
    return command_ptr;
}

// Print inside terminal argument after echo command
int shell_echo(char* command)
{
    char* command_ptr = argument_parser(command, "echo");
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
    char* command_ptr = argument_parser(command, "type");
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

    if (getcwd(pwd, sizeof(pwd))) printf("%s\n", pwd);
    return 0;
}

// Allows to change the current working directory
int shell_cd(char* command)
{
    char* adress = argument_parser(command, "cd");

    // Change directory
    // Home directory
    if (strcmp(adress, "~") == 0)
    {
        adress = getenv("HOME");
    }

    // relative to cur directory
    else if (adress[0] == '.' && adress[1] == '/')
        adress += 2;

    // Check if directory exist
    if (chdir(adress) != 0)
    {
        printf("cd: %s: No such file or directory\n", adress);
        return 1;
    }
    return 0;
}
