#ifndef DIRECTORY_H_
#define DIRECTORY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BLOCK_SIZE 512
#define NUM_BLOCKS 5000
#define NAME_SIZE 50


extern char virtual_disk[NUM_BLOCKS][BLOCK_SIZE];

typedef struct free_block {
    int index;
    struct free_block *prev;
    struct free_block *next;
} free_block;

typedef struct file_node {
    char name[NAME_SIZE];
    int is_directory;
    struct file_node *prev;
    struct file_node *next;
    struct file_node *parent;
    struct file_node *child;
    int size;
    int num_blocks;
    int *block_pointers;
} file_node;


extern free_block *free_list_head;
extern file_node *root;
extern file_node *current_dir;




void insert_free_blocks();
void add_root_directory();
int get_index();
void restore_free_block(const int index);
void show_disk_usage();
void free_virtual_memory();
void free_file_nodes(file_node *temp);
void exit_program();

file_node *find_child(const char *name);
void make_directory(const char *name);
void remove_directory(const char *name);
void list_files();
void change_directory(const char *name);

#endif