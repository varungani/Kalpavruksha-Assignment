#include "../inc/file.h"
#include "../inc/commandParser.h" // For find_length()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_file(const char *name) {
    if (find_child(name) != NULL) {
        printf("\nFile '%s' already exists.", name);
        return;
    }
    file_node *temp = (file_node *)malloc(sizeof(file_node));
    strcpy(temp->name, name);
    temp->size = 0;
    temp->num_blocks = 0;
    temp->is_directory = 0;
    temp->block_pointers = NULL;
    temp->parent = current_dir;
    temp->child = NULL;
    temp->next = temp;
    temp->prev = temp;
    if (current_dir->child == NULL) {
        current_dir->child = temp;
    } else {
        file_node *head = current_dir->child;
        file_node *tail = head->prev;
        temp->next = head;
        temp->prev = tail;
        tail->next = temp;
        head->prev = temp;
    }
    printf("File '%s' created.\n", name);
}

void write_file(const char *name, const char *text) {
    file_node *temp = find_child(name);
    if (temp == NULL) {
        printf("\nFile '%s' not found.", name);
        return;
    }
    if (temp->is_directory) {
        printf("\nError: '%s' is a directory.", name);
        return;
    }
    int len = find_length(text);
    int i = 0;
    while (i < len) {
        int index = get_index();
        if (index == -1) {
            return;
        }
        int k = 0;
        while (i < len && k < BLOCK_SIZE) {
            virtual_disk[index][k++] = text[i++];
        }
        temp->size += k;
        temp->num_blocks++;
        temp->block_pointers = (int *)realloc(temp->block_pointers, temp->num_blocks * sizeof(int));
        temp->block_pointers[temp->num_blocks - 1] = index;
    }
    printf("Data written successfully.\n");
}

void read_file(const char *name) {
    file_node *temp = find_child(name);
    if (temp == NULL) {
        printf("\nFile '%s' not found.", name);
        return;
    }
    if (temp->is_directory) {
        printf("\n'%s' is a directory.", name);
        return;
    }
    printf("\nData in '%s':\n", name);
    int remaining = temp->size;
    for (int i = 0; i < temp->num_blocks; i++) {
        int index = temp->block_pointers[i];
        int to_print = (remaining > BLOCK_SIZE) ? BLOCK_SIZE : remaining;
        for (int j = 0; j < to_print; j++) {
            putchar(virtual_disk[index][j]);
        }
        remaining -= to_print;
    }
    printf("\n");
}

void delete_file_node(file_node *temp) {
    if (temp == NULL) {
        return;
    }
    char name[50];
    strcpy(name, temp->name);
    if (temp->parent->child == temp) {
        if (temp->next == temp) {
            temp->parent->child = NULL;
        } else {
            temp->parent->child = temp->next;
        }
    }
    for (int i = 0; i < temp->num_blocks; i++) {
        restore_free_block(temp->block_pointers[i]);
    }
    free(temp->block_pointers);
    if (temp->next == temp) {
        free(temp);
    } else {
        temp->next->prev = temp->prev;
        temp->prev->next = temp->next;
        free(temp);
    }
    printf("\nFile '%s' deleted.", name);
}