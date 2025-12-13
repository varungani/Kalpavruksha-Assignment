#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<limits.h>

#define MAX_SIZE 1000
#define MAX_PRIME 997
#define MAX_HASH_SIZE 1000

typedef struct Node {
    int key;
    char data[MAX_SIZE];
    struct Node *front;
    struct Node *rear;
} Node;

typedef struct hashNode {
    int key;
    Node *value;
    struct hashNode *next;
} hashNode;

hashNode *map[MAX_HASH_SIZE];
Node *head = NULL;
Node *tail = NULL;
int capacity, count;
int hash = MAX_PRIME;

int hashCode(int key) {
    if (key < 0) key = -key;
    return key % hash;
}

void initializeHash() {
    for (int i = 0; i < MAX_HASH_SIZE; i++) map[i] = NULL;
}

void putInMap(int key, Node *node) {
    int idx = hashCode(key);
    hashNode *p = map[idx];
    while (p) {
        if (p->key == key) {
            p->value = node;
            return;
        }
        p = p->next;
    }
    hashNode *n = malloc(sizeof(hashNode));
    n->key = key;
    n->value = node;
    n->next = map[idx];
    map[idx] = n;
}

Node* getFromMap(int key) {
    int idx = hashCode(key);
    hashNode *t = map[idx];
    while (t) {
        if (t->key == key) return t->value;
        t = t->next;
    }
    return NULL;
}

void removeFromMap(int key) {
    int idx = hashCode(key);
    hashNode *c = map[idx], *p = NULL;
    while (c) {
        if (c->key == key) {
            if (p == NULL) map[idx] = c->next;
            else p->next = c->next;
            free(c);
            return;
        }
        p = c;
        c = c->next;
    }
}

void insertAtHead(Node *node) {
    Node *h = head->front;
    head->front = node;
    node->rear = head;
    h->rear = node;
    node->front = h;
}

void deleteNode(Node *node) {
    removeFromMap(node->key);
    node->rear->front = node->front;
    node->front->rear = node->rear;
    free(node);
}

char* get(int key) {
    Node *n = getFromMap(key);
    if (!n) return NULL;
    n->front->rear = n->rear;
    n->rear->front = n->front;
    insertAtHead(n);
    return n->data;
}

void put(int key, char value[]) {
    Node *n = getFromMap(key);
    if (n) {
        n->front->rear = n->rear;
        n->rear->front = n->front;
        strcpy(n->data, value);
        insertAtHead(n);
        return;
    }

    if (count == capacity) {
        deleteNode(tail->rear);
    } else count++;

    n = malloc(sizeof(Node));
    n->key = key;
    strcpy(n->data, value);
    insertAtHead(n);
    putInMap(key, n);
}

void createCache(int cap) {
    capacity = cap;
    count = 0;
    initializeHash();

    head = malloc(sizeof(Node));
    tail = malloc(sizeof(Node));

    head->key = -1;
    tail->key = -1;

    head->rear = NULL;
    head->front = tail;
    tail->rear = head;
    tail->front = NULL;
}

void exitProgram() {
    Node *c = head->front, *p = NULL;
    while (c) {
        p = c;
        c = c->front;
        deleteNode(p);
    }
    free(head);
    free(tail);
    exit(0);
}

void parseInput(char *input, char out[3][MAX_SIZE]) {
    int i = 0, k = 0;
    while (input[i] && k < 3) {
        while (input[i] == ' ') i++;
        int j = 0;
        while (input[i] && input[i] != ' ' && input[i] != '\n') {
            out[k][j++] = input[i++];
        }
        out[k][j] = '\0';
        k++;
    }
}

int main() {
    char input[3 * MAX_SIZE];
    char parsed[3][MAX_SIZE];

    while (1) {
        fgets(input, sizeof(input), stdin);
        int n = strlen(input);
        if (n && input[n - 1] == '\n') input[n - 1] = '\0';

        parseInput(input, parsed);

        if (!strcmp(parsed[0], "createCache")) {
            createCache(atoi(parsed[1]));
        } else if (!strcmp(parsed[0], "put")) {
            put(atoi(parsed[1]), parsed[2]);
        } else if (!strcmp(parsed[0], "get")) {
            printf("\n%s\n", get(atoi(parsed[1])));
        } else if (!strcmp(parsed[0], "exit")) {
            exitProgram();
        }
    }
    return 0;
}
