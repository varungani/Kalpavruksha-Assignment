#include "../inc/commandParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int calculate_string_length(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void parse_input_string(const char *input_string, char args_array[3][1000]) {
    int str_idx = 0;
    int arg_idx = 0;
    
 
    for (int x = 0; x < 3; x++) {
        args_array[x][0] = '\0';
    }

    while (input_string[str_idx] != '\0' && input_string[str_idx] != '\n' && arg_idx < 3) {
       
        while (input_string[str_idx] == ' ') {
            str_idx++;
        }
        if (input_string[str_idx] == '\0' || input_string[str_idx] == '\n') {
            break;
        }

        int char_idx = 0;
     
        if (input_string[str_idx] == '"') {
            str_idx++;
            while (input_string[str_idx] != '"' && input_string[str_idx] != '\0' && input_string[str_idx] != '\n' && char_idx < 999) {
                args_array[arg_idx][char_idx++] = input_string[str_idx++];
            }
            if (input_string[str_idx] == '"') {
                str_idx++;
            }
        } else {
           
            while (input_string[str_idx] != ' ' && input_string[str_idx] != '\0' && input_string[str_idx] != '\n' && char_idx < 999) {
                args_array[arg_idx][char_idx++] = input_string[str_idx++];
            }
        }
        args_array[arg_idx][char_idx] = '\0';
        arg_idx++;
    }
}

int is_valid_name(const char *name) {
    if (name == NULL || name[0] == '\0') {
        
        return 0;
    }
    const char *reserved_words[] = {"mkdir", "create", "write", "read", "rmdir", "delete", "ls", "cd", "df", "exit", NULL};
    
    for (int i = 0; reserved_words[i] != NULL; i++) {
        if (strcmp(name, reserved_words[i]) == 0) {
            printf("\nError: '%s' is a reserved word.", name);
            return 0;
        }
    }
    if (strlen(name) > NAME_SIZE) {
        printf("\nError: Name too long.");
        return 0;
    }
    return 1;
}
