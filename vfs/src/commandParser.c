#include "../inc/commandParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

int find_length(const char *name) {
    int i = 0;
    while (name[i] != '\0') {
        i++;
    }
    return i;
}

void parse_string(const char *str, char parsed[3][1000]) {
    int i = 0, j = 0;
    for (int x = 0; x < 3; x++) {
        parsed[x][0] = '\0';
    }
    while (str[i] != '\0' && str[i] != '\n' && j < 3) {
        while (str[i] == ' ') {
            i++;
        }
        if (str[i] == '\0' || str[i] == '\n') {
            break;
        }
        int k = 0;
        if (str[i] == '"') {
            i++;
            while (str[i] != '"' && str[i] != '\0' && str[i] != '\n' && k < 999) {
                parsed[j][k++] = str[i++];
            }
            if (str[i] == '"') {
                i++;
            }
        } else {
            while (str[i] != ' ' && str[i] != '\0' && str[i] != '\n' && k < 999) {
                parsed[j][k++] = str[i++];
            }
        }
        parsed[j][k] = '\0';
        j++;
    }
}

int valid_name(const char *name) {
    if (name[0] == '\0') {
        printf("\nInvalid name.");
        return 0;
    }
    const char *reserved[] = {"cwd", "mkdir", "create", "write", "read", "pwd", "rmdir", "ls", "cd", "df", "exit", NULL};
    for (int i = 0; reserved[i] != NULL; i++) {
        if (strcmp(name, reserved[i]) == 0) {
            printf("\nReserved word.");
            return 0;
        }
    }
    if (strlen(name) > NAME_SIZE) {
        printf("\nName too long.");
        return 0;
    }
    return 1;
}