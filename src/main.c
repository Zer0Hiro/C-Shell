#include "main.h"

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"
#include "exec.h"

// Enviroment Path
const char *ENV_VARIABLE = "PATH";

int main(int argc, char *argv[])
{
    while (true)
    {
        char input[BUFFER];
        char *command_word;
        int builtin = 0;

        // Flush after every printf
        setbuf(stdout, NULL);
        printf("$ ");

        // Parse Command
        fgets(input, BUFFER, stdin);
        input[strcspn(input, "\n")] = '\0';

        // Make a copy for strtok
        char *input_copy = strdup(input);

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
        if (builtin)
            continue;

        // Execute external program
        if (!find_exe(input, command_word))
            continue;
        // Not valid command
        printf("%s: command not found\n", input);

        free(input_copy);
    }

    return 0;
}

// Returns directory in PATH
char *in_path(char *command)
{
    char *env_p = getenv(ENV_VARIABLE);
    if (env_p == NULL)
        return NULL;

    char *copy = strdup(env_p);
    char *token = strtok(copy, ":");

    while (token)
    {
        // Calculate: dir + '/' + command + '\0'
        int path_len = strlen(token) + 1 + strlen(command) + 1;
        char *full_path = malloc(path_len);

        if (full_path)
        {
            // Build the path
            strcpy(full_path, token);
            strcat(full_path, "/");
            strcat(full_path, command);

            // Check if file exists and is executable
            if (access(full_path, X_OK) == 0)
            {
                free(copy);
                return full_path;
            }
            free(full_path); // Not the right directory, free and continue
        }
        token = strtok(NULL, ":");
    }
    free(copy);
    return NULL;
}