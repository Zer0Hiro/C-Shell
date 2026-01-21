#include "exec.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "main.h"

int find_exe(char* line, char* command_name)
{
    char** arguments;
    int arg_amount;
    // Get directory of execution
    char* path = in_path(command_name);
    if (path != NULL)
    {
        pid_t pid = fork();

        // Failed
        if (pid == -1)
        {
            perror("fork");
            free(path);
            return 1;
        }
        // Child process
        else if (pid == 0)
        {
            // Parse commmand into an array
            arguments = parse_args(line, &arg_amount);

            if (arguments != NULL)
            {
                // Execute program
                execv(path, arguments);
                // Error
                perror("execv");
            }

            free(path);
            exit(1);
        }
        // Parent process
        else
        {
            int status;
            waitpid(pid, &status, 0);

            // Command was found and Executed
            free(path);
            return 0;
        }
    }
    return 1;
}

char** parse_args(char* command, int* size)
{
    // Start Capacity
    int capacity = 2;
    int i = 0;

    char** arguments = malloc(sizeof(char*) * capacity);
    if (!arguments)
    {
        perror("malloc");
        return NULL;
    }

    // Parse Args
    char* copy = strdup(command);

    char* token = strtok(copy, " ");
    while (token)
    {
        arguments[i] = token;
        token = strtok(NULL, " ");
        if (token != NULL)
        {
            capacity++;
            char** temp = realloc(arguments, sizeof(char*) * capacity);
            if (!temp)
            {
                perror("realloc");
                free(arguments);
                free(copy);
                return NULL;
            }
            arguments = temp;
        }
        i++;
    }
    arguments[i] = NULL;
    *size = i;
    return arguments;
}