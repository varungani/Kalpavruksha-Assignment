#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/directory.h"
#include "../inc/file.h"
#include "../inc/commandParser.h"


void run_file_system_loop() {
    int system_running = 1; 

    while (system_running) {
        printf("\n%s > ", current_dir->name);
        
        char input_buffer[10000];
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break; 
        }

        char parsed_args[3][1000];
        parse_input_string(input_buffer, parsed_args);

        
        if (strcmp(parsed_args[0], "exit") != 0 && 
            strcmp(parsed_args[0], "ls") != 0 && 
            strcmp(parsed_args[0], "df") != 0 && 
            !is_valid_name(parsed_args[1])) {
            continue;
        }

       
        int command_id = -1;
        if (strcmp(parsed_args[0], "mkdir") == 0) command_id = 1;
        else if (strcmp(parsed_args[0], "create") == 0) command_id = 2;
        else if (strcmp(parsed_args[0], "write") == 0) command_id = 3;
        else if (strcmp(parsed_args[0], "read") == 0) command_id = 4;
        else if (strcmp(parsed_args[0], "delete") == 0) command_id = 5;
        else if (strcmp(parsed_args[0], "rmdir") == 0) command_id = 6;
        else if (strcmp(parsed_args[0], "ls") == 0) command_id = 7;
        else if (strcmp(parsed_args[0], "cd") == 0) command_id = 8;
        else if (strcmp(parsed_args[0], "df") == 0) command_id = 9;
        else if (strcmp(parsed_args[0], "exit") == 0) command_id = 10;

        switch (command_id) {
            case 1: make_directory(parsed_args[1]); break;
            case 2: create_file(parsed_args[1]); break;
            case 3: write_to_file(parsed_args[1], parsed_args[2]); break;
            case 4: read_from_file(parsed_args[1]); break;
            case 5: {
                file_node *target_file = find_child_node(parsed_args[1]);
                if (target_file && !target_file->is_directory) {
                    delete_file_node(target_file);
                } else {
                    printf("\nFile not found or is a directory.");
                }
                break;
            }
            case 6: remove_directory(parsed_args[1]); break;
            case 7: list_directory_contents(); break;
            case 8: change_directory(parsed_args[1]); break;
            case 9: show_disk_usage(); break;
            case 10: 
                cleanup_system(); 
                system_running = 0; 
                printf("\nExiting...\n");
                break;
            default: printf("\nUnknown command."); break;
        }
    }
}

int main() {
    initialize_free_blocks();
    initialize_root_directory();
    
    run_file_system_loop();
    
    return 0;
}
