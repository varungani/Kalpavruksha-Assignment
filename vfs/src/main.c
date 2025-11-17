#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/directory.h"
#include "../inc/file.h"
#include "../inc/commandParser.h"

int main() {
    insert_free_blocks();
    add_root_directory();
    
    while (1) {
        printf("\n%s > ", current_dir->name);
        char input[10000];
        fgets(input, sizeof(input), stdin);
        
        char parsed[3][1000];
        parse_string(input, parsed);
        
        
        if (strcmp(parsed[0], "exit") != 0 && 
            strcmp(parsed[0], "ls") != 0 && 
            strcmp(parsed[0], "df") != 0 && 
            !valid_name(parsed[1])) {
            continue;
        }

        int choice = -1;
        if (strcmp(parsed[0], "mkdir") == 0) choice = 1;
        else if (strcmp(parsed[0], "create") == 0) choice = 2;
        else if (strcmp(parsed[0], "write") == 0) choice = 3;
        else if (strcmp(parsed[0], "read") == 0) choice = 4;
        else if (strcmp(parsed[0], "delete") == 0) choice = 5;
        else if (strcmp(parsed[0], "rmdir") == 0) choice = 6;
        else if (strcmp(parsed[0], "ls") == 0) choice = 7;
        else if (strcmp(parsed[0], "cd") == 0) choice = 8;
        else if (strcmp(parsed[0], "df") == 0) choice = 9;
        else if (strcmp(parsed[0], "exit") == 0) choice = 10;

        switch (choice) {
            case 1: make_directory(parsed[1]); break;
            case 2: create_file(parsed[1]); break;
            case 3: write_file(parsed[1], parsed[2]); break;
            case 4: read_file(parsed[1]); break;
            case 5: {
                file_node *temp = find_child(parsed[1]);
                if (temp && !temp->is_directory) {
                    delete_file_node(temp);
                } else {
                    printf("\nFile not found.");
                }
                break;
            }
            case 6: remove_directory(parsed[1]); break;
            case 7: list_files(); break;
            case 8: change_directory(parsed[1]); break;
            case 9: show_disk_usage(); break;
            case 10: exit_program(); break;
            default: printf("\nUnknown command."); break;
        }
    }
    return 0;
}