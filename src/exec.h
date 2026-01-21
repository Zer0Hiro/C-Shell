#ifndef EXTERNAL_H
#define EXTERNAL_H

/*
    Allows to find and run external program with arguments
*/

int find_exe(char* line, char* command_name);
char **parse_args(char *command, int *size);

#endif