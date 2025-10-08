#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User {
    int id;
    char name[50];
    int age;
};

int main() {
    FILE *filePtr, *tempPtr;
    struct User currentUser;
    int choice;
    int searchId;
    int recordFound;

    while (1) {
        printf("\n=== USER MANAGEMENT SYSTEM ===\n");
        printf("1. Create User\n");
        printf("2. Display All Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                filePtr = fopen("users.txt", "r");
                int newUserId;
                printf("Enter ID: ");
                scanf("%d", &newUserId);
                recordFound = 0;

                if (filePtr) {
                    while (fscanf(filePtr, "%d %s %d", &currentUser.id, currentUser.name, &currentUser.age) == 3) {
                        if (currentUser.id == newUserId) {
                            printf("Error: User ID already exists.\n");
                            recordFound = 1;
                            break;
                        }
                    }
                    fclose(filePtr);
                }

                if (!recordFound) {
                    filePtr = fopen("users.txt", "a");
                    currentUser.id = newUserId;
                    printf("Enter Name: ");
                    scanf("%s", currentUser.name);
                    printf("Enter Age: ");
                    scanf("%d", &currentUser.age);
                    fprintf(filePtr, "%d %s %d\n", currentUser.id, currentUser.name, currentUser.age);
                    fclose(filePtr);
                    printf("User added successfully!\n");
                }
                break;
            }

            case 2: {
                filePtr = fopen("users.txt", "r");
                if (!filePtr) {
                    printf("No records found.\n");
                    break;
                }
                printf("\n--- List of Users ---\n");
                while (fscanf(filePtr, "%d %s %d", &currentUser.id, currentUser.name, &currentUser.age) == 3) {
                    printf("ID: %d | Name: %s | Age: %d\n", currentUser.id, currentUser.name, currentUser.age);
                }
                fclose(filePtr);
                break;
            }

            case 3: {
                filePtr = fopen("users.txt", "r");
                if (!filePtr) {
                    printf("No records found to update.\n");
                    break;
                }
                tempPtr = fopen("temp.txt", "w");
                printf("Enter ID to update: ");
                scanf("%d", &searchId);
                recordFound = 0;

                while (fscanf(filePtr, "%d %s %d", &currentUser.id, currentUser.name, &currentUser.age) == 3) {
                    if (currentUser.id == searchId) {
                        printf("Enter new Name: ");
                        scanf("%s", currentUser.name);
                        printf("Enter new Age: ");
                        scanf("%d", &currentUser.age);
                        recordFound = 1;
                    }
                    fprintf(tempPtr, "%d %s %d\n", currentUser.id, currentUser.name, currentUser.age);
                }

                fclose(filePtr);
                fclose(tempPtr);
                remove("users.txt");
                rename("temp.txt", "users.txt");

                if (recordFound)
                    printf("User updated successfully!\n");
                else
                    printf("ID not found.\n");
                break;
            }

            case 4: {
                filePtr = fopen("users.txt", "r");
                if (!filePtr) {
                    printf("No records found to delete.\n");
                    break;
                }
                tempPtr = fopen("temp.txt", "w");
                printf("Enter ID to delete: ");
                scanf("%d", &searchId);
                recordFound = 0;

                while (fscanf(filePtr, "%d %s %d", &currentUser.id, currentUser.name, &currentUser.age) == 3) {
                    if (currentUser.id == searchId) {
                        recordFound = 1;
                        continue;
                    }
                    fprintf(tempPtr, "%d %s %d\n", currentUser.id, currentUser.name, currentUser.age);
                }

                fclose(filePtr);
                fclose(tempPtr);
                remove("users.txt");
                rename("temp.txt", "users.txt");

                if (recordFound)
                    printf("User deleted successfully!\n");
                else
                    printf("ID not found.\n");
                break;
            }

            case 5:
                printf("Exiting program.\n");
                return 0;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
