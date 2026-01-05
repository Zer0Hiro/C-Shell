#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define BUFFER 1024

typedef struct
{
  char *name;
  int (*b_func)(char *args);
} builtin_commands;

int shell_echo(char *command);
int shell_exit(char *command);
int shell_type(char *command);

builtin_commands builtins[] = {
    {"echo", &shell_echo},
    {"exit", &shell_exit},
    {"quit", &shell_exit},
    {"type", &shell_type},
};

int builtins_size = sizeof(builtins) / sizeof(builtin_commands);

int main(int argc, char *argv[])
{
  while (true)
  {
    // Flush after every printf
    setbuf(stdout, NULL);
    printf("$ ");

    // Parse Command
    char input[BUFFER];
    fgets(input, BUFFER, stdin);
    input[strcspn(input, "\n")] = '\0';

    // Make a copy for strtok
    char input_copy[BUFFER];
    strcpy(input_copy, input);

    // Remove Whitespaces
    char *command_word = strtok(input_copy, " ");

    // Find if command is builtin
    int builtin = 0;
    for (int i = 0; i < builtins_size; i++)
    {
      if (command_word != NULL && strcmp(command_word, builtins[i].name) == 0)
      {
        builtins[i].b_func(input);
        builtin = 1;
        break;
      }
    }

    // Not valid command
    if (!builtin)
      printf("%s: command not found\n", input);
  }

  return 0;
}

int shell_echo(char *command)
{
  char *command_ptr = &command[strcspn(command, "echo")];
  command_ptr += 5;
  command_ptr += strspn(command_ptr, " ");
  printf("%s\n", command_ptr);
  return 0;
}

int shell_exit(char *command)
{
  if (command != NULL && (!(strcmp(command, "exit")) || !(strcmp(command, "quit"))))
    exit(EXIT_SUCCESS);
}

int shell_type(char *command)
{
  char *command_ptr = &command[strcspn(command, "type")];
  command_ptr += 5;
  command_ptr += strspn(command_ptr, " ");
  for (int i = 0; i < builtins_size; i++)
  {
    if (strcmp(command_ptr, builtins[i].name) == 0)
    {
      printf("%s is a shell builtin\n", builtins[i].name);
      return 0;
    }
  }
  printf("%s: not found\n",command_ptr);
  return 1;
}