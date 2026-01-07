#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"

// Enviroment Path
const char* ENV_VARIABLE = "PATH";

int main(int argc, char* argv[])
{
    while (true)
    {
        char input[BUFFER];
        char input_copy[BUFFER];
        char* command_word;
        int builtin = 0;

        // Flush after every printf
        setbuf(stdout, NULL);
        printf("$ ");

        // Parse Command
        fgets(input, BUFFER, stdin);
        input[strcspn(input, "\n")] = '\0';

        // Make a copy for strtok
        strcpy(input_copy, input);

        // Remove Whitespaces
        command_word = strtok(input_copy, " ");

        // Find if command is builtin
        for (int i = 0; i < builtins_size; i++)
        {
            if (command_word != NULL &&
                strcmp(command_word, builtins[i].name) == 0)
            {
                builtins[i].b_func(input);
                builtin = 1;
                break;
            }
        }

        // Not valid command
        if (!builtin) printf("%s: command not found\n", input);
    }

    return 0;
}

int shell_echo(char* command)
{
    char* command_ptr = &command[strcspn(command, "echo")];
    command_ptr += 5;
    command_ptr += strspn(command_ptr, " ");
    printf("%s\n", command_ptr);
    return 0;
}

int shell_exit(char* command)
{
    if (command != NULL &&
        (!(strcmp(command, "exit")) || !(strcmp(command, "quit"))))
        exit(EXIT_SUCCESS);
}

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
    char* env_p = getenv(ENV_VARIABLE);
    if (getenv != NULL)
    {
        char* copy = strdup(env_p);
        char* token = strtok(copy, ":");
        while (token)
        {
            // Open directory from PATH
            DIR* dir = opendir(token);
            if (dir)
            {
                // Parse if command is there
                while (entry = readdir(dir))
                {
                    if (entry->d_name[0] == '.') continue;
                    if (strcmp(entry->d_name, command_ptr) == 0)
                    {
                        // command + / + token + \0
                        char* full_path =
                            malloc(strlen(command_ptr) + strlen(token) + 2);
                        strcpy(full_path, token);
                        strcat(full_path, "/");
                        strcat(full_path, entry->d_name);

                        // Check permissions
                        if (access(full_path, X_OK) == 0)
                        {
                            //<command> is <full_path>
                            printf("%s is %s\n", command_ptr, full_path);
                            free(copy);
                            return 0;
                        }
                    }
                }
                closedir(dir);
            }
            token = strtok(NULL, ":");
        }
        free(copy);
    }
    // <command>: not found
    printf("%s: not found\n", command_ptr);
    return 1;
}