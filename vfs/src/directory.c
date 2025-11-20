#include "../inc/directory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char virtual_disk[NUM_BLOCKS][BLOCK_SIZE];
free_block *free_list_head = NULL;
file_node *root = NULL;
file_node *current_dir = NULL;

void initialize_free_blocks() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        free_block *new_block = (free_block *)malloc(sizeof(free_block));
        new_block->index = i;
        
        if (free_list_head == NULL) {
            free_list_head = new_block;
            free_list_head->next = free_list_head;
            free_list_head->prev = free_list_head;
        } else {
           
            free_block *tail = free_list_head->prev;
            
            tail->next = new_block;
            new_block->prev = tail;
            new_block->next = free_list_head;
            free_list_head->prev = new_block;
        }
    }
}

void initialize_root_directory() {
    root = (file_node *)malloc(sizeof(file_node));
    strcpy(root->name, "/");
    root->is_directory = 1;
    root->parent = NULL;
    root->child = NULL;
    root->next = root;
    root->prev = root;
    current_dir = root;
}

int allocate_free_block_index() {
    if (free_list_head == NULL) {
        printf("\nError: Not enough space on disk!");
        return -1;
    }
    int allocated_index = free_list_head->index;
    
    if (free_list_head->next == free_list_head) {
        
        free(free_list_head);
        free_list_head = NULL;
        return allocated_index;
    }
    
    free_block *node_to_remove = free_list_head;
    free_block *new_head = free_list_head->next;
    free_block *tail = free_list_head->prev;
    
    new_head->prev = tail;
    tail->next = new_head;
    free_list_head = new_head;
    
    free(node_to_remove);
    return allocated_index;
}

void release_block_index(const int index) {
    free_block *returned_block = (free_block *)malloc(sizeof(free_block));
    returned_block->index = index;

    if (free_list_head == NULL) {
        returned_block->next = returned_block;
        returned_block->prev = returned_block;
        free_list_head = returned_block;
        return;
    }
    
    
    free_block *tail = free_list_head->prev;
    
    tail->next = returned_block;
    returned_block->prev = tail;
    returned_block->next = free_list_head;
    free_list_head->prev = returned_block;
    
    
}

file_node *find_child_node(const char *name) {
    if (current_dir->child == NULL) {
        return NULL;
    }
    file_node *iterator = current_dir->child;
    do {
        if (strcmp(iterator->name, name) == 0) {
            return iterator;
        }
        iterator = iterator->next;
    } while (iterator != current_dir->child);
    return NULL;
}

void make_directory(const char *name) {
    if (find_child_node(name) != NULL) {
        printf("\nDirectory '%s' already exists.", name);
        return;
    }
    file_node *new_dir = (file_node *)malloc(sizeof(file_node));
    strcpy(new_dir->name, name);
    new_dir->is_directory = 1;
    new_dir->parent = current_dir;
    new_dir->child = NULL;
    
    
    new_dir->next = new_dir;
    new_dir->prev = new_dir;

    if (current_dir->child == NULL) {
        current_dir->child = new_dir;
    } else {
        file_node *head = current_dir->child;
        file_node *tail = head->prev;
        
       
        tail->next = new_dir;
        new_dir->prev = tail;
        new_dir->next = head;
        head->prev = new_dir;
    }
    printf("Directory '%s' created.\n", name);
}

void remove_directory(const char *name) {
    if (strcmp(name, "/") == 0) {
        printf("\nCannot delete root directory.");
        return;
    }
    file_node *target_node = find_child_node(name);
    if (target_node == NULL) {
        printf("\nDirectory '%s' not found.", name);
        return;
    }
    if (target_node->child != NULL) {
        printf("\nCannot delete non-empty directory.");
        return;
    }

   
    if (target_node->parent->child == target_node) {
        if (target_node->next == target_node) {
           
            target_node->parent->child = NULL;
        } else {
            
            target_node->parent->child = target_node->next;
        }
    }

    
    if (target_node->next != target_node) {
        target_node->prev->next = target_node->next;
        target_node->next->prev = target_node->prev;
    }

    free(target_node);
    printf("\nDirectory '%s' deleted.", name);
}

void list_directory_contents() {
    file_node *iterator = current_dir->child;
    printf("\n");
    if (iterator == NULL) {
        printf("(empty directory)");
        return;
    }
    do {
        printf("%s ", iterator->name);
        iterator = iterator->next;
    } while (iterator != current_dir->child);
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
    
    
    if (strcmp(name, ".") == 0) {
         return;
    }

    if (current_dir->child == NULL) {
        printf("\nNo subdirectories found.");
        return;
    }
    
    file_node *iterator = current_dir->child;
    do {
        if (strcmp(iterator->name, name) == 0 && iterator->is_directory) {
            current_dir = iterator;
            printf("\nMoved to '%s'.", name);
            return;
        }
        iterator = iterator->next;
    } while (iterator != current_dir->child);
    
    printf("\nDirectory not found.");
}

void show_disk_usage() {
    int free_count = 0;
    if (free_list_head != NULL) {
        free_block *iterator = free_list_head;
        do {
            free_count++;
            iterator = iterator->next;
        } while (iterator != free_list_head);
    }
    
    int used_blocks = NUM_BLOCKS - free_count;
    float usage_percent = ((float)used_blocks / NUM_BLOCKS) * 100.0;
    
    printf("\nTotal Blocks: %d", NUM_BLOCKS);
    printf("\nUsed Blocks:  %d", used_blocks);
    printf("\nFree Blocks:  %d", free_count);
    printf("\nUsage:        %.2f%%", usage_percent);
}

void free_virtual_memory() {
    if (free_list_head == NULL) return;
    
  
    free_list_head->prev->next = NULL; 
    
    free_block *current = free_list_head;
    while (current != NULL) {
        free_block *next_node = current->next;
        free(current);
        current = next_node;
    }
    free_list_head = NULL;
}

void recursive_free_nodes(file_node *node) {
    if (node == NULL) return;
    
    if (node->child != NULL) {
        file_node *child_iter = node->child;
      
        child_iter->prev->next = NULL;
        
        while (child_iter != NULL) {
            file_node *next_child = child_iter->next;
            recursive_free_nodes(child_iter); 
            child_iter = next_child;
        }
    }
   
    if (node->block_pointers != NULL) {
        free(node->block_pointers);
    }
    free(node);
}

void cleanup_system() {
    recursive_free_nodes(root);
    root = NULL;
    free_virtual_memory();
}
