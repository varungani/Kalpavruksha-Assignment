#include "../inc/directory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char virtual_disk[NUM_BLOCKS][BLOCK_SIZE];
free_block *free_list_head = NULL;
file_node *root = NULL;
file_node *current_dir = NULL;



void insert_free_blocks() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        free_block *temp = (free_block *)malloc(sizeof(free_block));
        temp->index = i;
        temp->next = NULL;
        temp->prev = NULL;
        if (free_list_head == NULL) {
            free_list_head = temp;
            free_list_head->next = free_list_head;
            free_list_head->prev = free_list_head;
        } else {
            free_list_head->prev->next = temp;
            temp->next = free_list_head;
            temp->prev = free_list_head->prev;
            free_list_head->prev = temp;
        }
        free_list_head = temp;
    }
}

void add_root_directory() {
    root = (file_node *)malloc(sizeof(file_node));
    strcpy(root->name, "/");
    root->is_directory = 1;
    root->parent = NULL;
    root->child = NULL;
    root->next = root;
    root->prev = root;
    current_dir = root;
}

int get_index() {
    if (free_list_head == NULL) {
        printf("\nError: not enough space!");
        return -1;
    }
    int index = free_list_head->index;
    if (free_list_head->next == free_list_head) {
        free(free_list_head);
        free_list_head = NULL;
        return index;
    }
    free_block *temp = free_list_head;
    free_block *head = free_list_head->next;
    free_block *tail = free_list_head->prev;
    head->prev = tail;
    tail->next = head;
    free_list_head = head;
    free(temp);
    return index;
}

void restore_free_block(const int index) {
    if (free_list_head == NULL) {
        free_block *head = (free_block *)malloc(sizeof(free_block));
        head->index = index;
        head->next = head;
        head->prev = head;
        free_list_head = head;
        return;
    }
    free_block *head = free_list_head;
    free_block *tail = free_list_head->prev;
    free_block *temp = (free_block *)malloc(sizeof(free_block));
    temp->index = index;
    head->prev = temp;
    tail->next = temp;
    temp->prev = tail;
    temp->next = head;
}

file_node *find_child(const char *name) {
    file_node *temp = current_dir->child;
    if (temp == NULL) {
        return NULL;
    }
    do {
        if (strcmp(temp->name, name) == 0) {
            return temp;
        }
        temp = temp->next;
    } while (temp != current_dir->child);
    return NULL;
}

void make_directory(const char *name) {
    if (find_child(name) != NULL) {
        printf("\nDirectory '%s' already exists.", name);
        return;
    }
    file_node *temp = (file_node *)malloc(sizeof(file_node));
    strcpy(temp->name, name);
    temp->is_directory = 1;
    temp->parent = current_dir;
    temp->child = NULL;
    temp->next = temp;
    temp->prev = temp;
    if (current_dir->child == NULL) {
        current_dir->child = temp;
    } else {
        file_node *head = current_dir->child;
        file_node *tail = head->prev;
        head->prev = temp;
        temp->next = head;
        temp->prev = tail;
        tail->next = temp;
    }
    printf("Directory '%s' created.\n", name);
}

void remove_directory(const char *name) {
    if (strcmp(name, "/") == 0) {
        printf("\nCannot delete root directory.");
        return;
    }
    file_node *temp = find_child(name);
    if (temp == NULL) {
        printf("\nDirectory '%s' not found.", name);
        return;
    }
    if (temp->child != NULL) {
        printf("\nCannot delete non-empty directory.");
        return;
    }
    if (temp->parent->child == temp) {
        if (temp->next == temp) {
            temp->parent->child = NULL;
        } else {
            temp->parent->child = temp->next;
        }
    }
    if (temp->next == temp) {
        free(temp);
    } else {
        temp->next->prev = temp->prev;
        temp->prev->next = temp->next;
        free(temp);
    }
    printf("\nDirectory '%s' deleted.", name);
}

void list_files() {
    file_node *temp = current_dir->child;
    printf("\n");
    if (temp == NULL) {
        printf("(empty directory)");
        return;
    }
    do {
        printf("%s ", temp->name);
        temp = temp->next;
    } while (temp != current_dir->child);
}

void change_directory(const char *name) {
    if (strcmp(name, "..") == 0) {
        if (current_dir->parent == NULL) {
            printf("\nAlready at root.");
            return;
        }
        current_dir = current_dir->parent;
        printf("\nMoved to '%s'.", current_dir->name);
        return;
    }
    if (current_dir->child == NULL) {
        printf("\nNo subdirectories.");
        return;
    }
    file_node *temp = current_dir->child;
    do {
        if (strcmp(temp->name, name) == 0 && temp->is_directory) {
            current_dir = temp;
            printf("\nMoved to '%s'.", name);
            return;
        }
        temp = temp->next;
    } while (temp != current_dir->child);
    printf("\nDirectory not found.");
}

void show_disk_usage() {
    int count = 0;
    int used_blocks = 0;
    float usage = 0.0;
    if (free_list_head != NULL) {
        free_block *temp = free_list_head;
        do {
            count++;
            temp = temp->next;
        } while (temp != free_list_head);
        used_blocks = NUM_BLOCKS - count;
        usage = ((float)used_blocks / NUM_BLOCKS) * 100.0;
    } else {
        used_blocks = NUM_BLOCKS;
        count = 0;
        usage = 100.0;
    }
    printf("\nTotal: %d", NUM_BLOCKS);
    printf("\nUsed: %d", used_blocks);
    printf("\nFree: %d", count);
    printf("\nUsage: %.2f%%", usage);
}

void free_virtual_memory() {
    if (free_list_head == NULL) {
        return;
    }
    free_list_head->prev->next = NULL;
    free_block *head = free_list_head;
    while (head != NULL) {
        free_block *temp = head;
        head = head->next;
        free(temp);
    }
    free_list_head = NULL;
}

void free_file_nodes(file_node *temp) {
    if (temp == NULL) {
        return;
    }
    if (temp->child != NULL) {
        file_node *head = temp->child;
        head->prev->next = NULL;
        while (head != NULL) {
            file_node *next = head->next;
            free_file_nodes(head);
            head = next;
        }
    }
    free(temp);
}

void exit_program() {
    free_file_nodes(root);
    root = NULL;
    free_virtual_memory();
    printf("\nExiting...");
    exit(0);
}