#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User {
    int id;
    char name[50];
    int age;
};

void addUser() {
    FILE *filePtr = fopen("users.txt", "r");
    struct User user;
    int newId, found = 0;
    printf("Enter ID: ");
    scanf("%d", &newId);

    if (filePtr) {
        while (fscanf(filePtr, "%d %s %d", &user.id, user.name, &user.age) == 3) {
            if (user.id == newId) {
                printf("Error: User ID already exists.\n");
                found = 1;
                break;
            }
        }
        fclose(filePtr);
    }

    if (!found) {
        filePtr = fopen("users.txt", "a");
        user.id = newId;
        printf("Enter Name: ");
        scanf("%s", user.name);
        printf("Enter Age: ");
        scanf("%d", &user.age);
        fprintf(filePtr, "%d %s %d\n", user.id, user.name, user.age);
        fclose(filePtr);
        printf("User added successfully\n");
    }
}

void displayUsers() {
    FILE *filePtr = fopen("users.txt", "r");
    struct User user;
    if (!filePtr) {
        printf("No records found.\n");
        return;
    }
    printf("\n List of Users \n");
    while (fscanf(filePtr, "%d %s %d", &user.id, user.name, &user.age) == 3) {
        printf("ID: %d | Name: %s | Age: %d\n", user.id, user.name, user.age);
    }
    fclose(filePtr);
}

void updateUser() {
    FILE *filePtr = fopen("users.txt", "r");
    if (!filePtr) {
        printf("No records found to update.\n");
        return;
    }
    FILE *tempPtr = fopen("temp.txt", "w");
    struct User user;
    int id, found = 0;
    printf("Enter ID to update: ");
    scanf("%d", &id);

    while (fscanf(filePtr, "%d %s %d", &user.id, user.name, &user.age) == 3) {
        if (user.id == id) {
            printf("Enter new Name: ");
            scanf("%s", user.name);
            printf("Enter new Age: ");
            scanf("%d", &user.age);
            found = 1;
        }
        fprintf(tempPtr, "%d %s %d\n", user.id, user.name, user.age);
    }

    fclose(filePtr);
    fclose(tempPtr);
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        printf("User updated successfully\n");
    else
        printf("ID not found.\n");
}

void deleteUser() {
    FILE *filePtr = fopen("users.txt", "r");
    if (!filePtr) {
        printf("No records found to delete\n");
        return;
    }
    FILE *tempPtr = fopen("temp.txt", "w");
    struct User user;
    int id, found = 0;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    while (fscanf(filePtr, "%d %s %d", &user.id, user.name, &user.age) == 3) {
        if (user.id == id) {
            found = 1;
            continue;
        }
        fprintf(tempPtr, "%d %s %d\n", user.id, user.name, user.age);
    }

    fclose(filePtr);
    fclose(tempPtr);
    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        printf("User deleted successfully!\n");
    else
        printf("ID not found.\n");
}

int main() {
    int choice;
    while (1) {
        printf("\n USER MANAGEMENT SYSTEM \n");
        printf("1. Create User\n");
        printf("2. Display All Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addUser(); break;
            case 2: displayUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: printf("Exiting program.\n"); return 0;
            default: printf("Invalid choice. Try again.\n");
        }
    }
}
