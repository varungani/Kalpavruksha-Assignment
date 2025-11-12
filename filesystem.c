#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 5000
#define NAME_SIZE 50

char virtual_disk[NUM_BLOCKS][BLOCK_SIZE];

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

free_block *free_list_head = NULL;
file_node *root = NULL;
file_node *current_dir = NULL;

int find_length(const char *name) {
    int i = 0;
    while (name[i] != '\0') {
        i++;
    }
    return i;
}

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

int main() {
    insert_free_blocks();
    add_root_directory();
    while (1) {
        printf("\n%s > ", current_dir->name);
        char input[10000];
        fgets(input, sizeof(input), stdin);
        char parsed[3][1000];
        parse_string(input, parsed);
        if (!valid_name(parsed[1]) && strcmp(parsed[0], "exit") != 0 && strcmp(parsed[0], "ls") != 0) {
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
