#include "../inc/file.h"
#include "../inc/commandParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_file(const char *name) {
    if (find_child_node(name) != NULL) {
        printf("\nError: '%s' already exists.", name);
        return;
    }
    file_node *new_file = (file_node *)malloc(sizeof(file_node));
    strcpy(new_file->name, name);
    new_file->size = 0;
    new_file->num_blocks = 0;
    new_file->is_directory = 0;
    new_file->block_pointers = NULL;
    new_file->parent = current_dir;
    new_file->child = NULL;
    
    
    new_file->next = new_file;
    new_file->prev = new_file;
    
    if (current_dir->child == NULL) {
        current_dir->child = new_file;
    } else {
        file_node *head = current_dir->child;
        file_node *tail = head->prev;
        
        tail->next = new_file;
        new_file->prev = tail;
        new_file->next = head;
        head->prev = new_file;
    }
    printf("File '%s' created.\n", name);
}

void write_to_file(const char *name, const char *text) {
    file_node *file_ptr = find_child_node(name);
    if (file_ptr == NULL) {
        printf("\nFile '%s' not found.", name);
        return;
    }
    if (file_ptr->is_directory) {
        printf("\nError: '%s' is a directory.", name);
        return;
    }
    
    int text_len = calculate_string_length(text);
    int current_char_pos = 0;
    
    while (current_char_pos < text_len) {
        int disk_block_index = allocate_free_block_index();
        if (disk_block_index == -1) {
            return; 
        }
        
        int byte_offset = 0;
        while (current_char_pos < text_len && byte_offset < BLOCK_SIZE) {
            virtual_disk[disk_block_index][byte_offset++] = text[current_char_pos++];
        }
        
        file_ptr->size += byte_offset;
        file_ptr->num_blocks++;
        file_ptr->block_pointers = (int *)realloc(file_ptr->block_pointers, file_ptr->num_blocks * sizeof(int));
        file_ptr->block_pointers[file_ptr->num_blocks - 1] = disk_block_index;
    }
    printf("Data written successfully.\n");
}

void read_from_file(const char *name) {
    file_node *file_ptr = find_child_node(name);
    if (file_ptr == NULL) {
        printf("\nFile '%s' not found.", name);
        return;
    }
    if (file_ptr->is_directory) {
        printf("\nError: '%s' is a directory.", name);
        return;
    }
    
    printf("\nData in '%s':\n", name);
    int bytes_remaining = file_ptr->size;
    
    for (int i = 0; i < file_ptr->num_blocks; i++) {
        int block_idx = file_ptr->block_pointers[i];
        int bytes_to_read = (bytes_remaining > BLOCK_SIZE) ? BLOCK_SIZE : bytes_remaining;
        
        for (int j = 0; j < bytes_to_read; j++) {
            putchar(virtual_disk[block_idx][j]);
        }
        bytes_remaining -= bytes_to_read;
    }
    printf("\n");
}

void delete_file_node(file_node *file_to_delete) {
    if (file_to_delete == NULL) return;

    char filename_backup[50];
    strcpy(filename_backup, file_to_delete->name);

    
    if (file_to_delete->parent->child == file_to_delete) {
        if (file_to_delete->next == file_to_delete) {
            file_to_delete->parent->child = NULL;
        } else {
            file_to_delete->parent->child = file_to_delete->next;
        }
    }
    
    
    for (int i = 0; i < file_to_delete->num_blocks; i++) {
        release_block_index(file_to_delete->block_pointers[i]);
    }
    free(file_to_delete->block_pointers);
    
   
    if (file_to_delete->next != file_to_delete) {
        file_to_delete->next->prev = file_to_delete->prev;
        file_to_delete->prev->next = file_to_delete->next;
    }
    
    free(file_to_delete);
    printf("\nFile '%s' deleted.", filename_backup);
}
