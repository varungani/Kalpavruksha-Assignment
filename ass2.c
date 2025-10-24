#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum MenuChoice {
    CREATE_USER = 1,
    DISPLAY_USERS,
    UPDATE_USER,
    DELETE_USER,
    EXIT
};

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
        if (!filePtr) {
            printf("Error opening file for writing.\n");
            return;
        }
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
        printf("No records found. File may not exist yet.\n");
        return;
    }
    printf("\nList of Users\n");
    while (fscanf(filePtr, "%d %s %d", &user.id, user.name, &user.age) == 3) {
        printf("ID: %d | Name: %s | Age: %d\n", user.id, user.name, user.age);
    }
    printf("--- End of List ---\n");
    fclose(filePtr);
}

void updateUser() {
    FILE *filePtr = fopen("users.txt", "r");
    if (!filePtr) {
        printf("No records found to update.\n");
        return;
    }
    FILE *tempPtr = fopen("temp.txt", "w");
    if (!tempPtr) {
        printf("Error creating temporary file.\n");
        fclose(filePtr);
        return;
    }

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
    if (!tempPtr) {
        printf("Error creating temporary file.\n");
        fclose(filePtr);
        return;
    }

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
    bool start = true;

    while (start) {
        printf("\n USER MANAGEMENT SYSTEM \n");
        printf("1. Create User\n");
        printf("2. Display All Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case CREATE_USER:
                addUser();
                break;
            case DISPLAY_USERS:
                displayUsers();
                break;
            case UPDATE_USER:
                updateUser();
                break;
            case DELETE_USER:
                deleteUser();
                break;
            case EXIT:
                printf("Exiting program.\n");
                start = false;
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
