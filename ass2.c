#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User {
    int id;
    char name[50];
    int age;
};

int main() {
    int choice;
    FILE *fp, *temp;
    struct User u;
    int id, found;

    do {
        printf("\n1. Create User\n");
        printf("2. Read Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            fp = fopen("users.txt", "r");
            int newId;
            printf("Enter ID: "); 
            scanf("%d", &newId);
            found = 0;

            if (fp) {
                while (fscanf(fp, "%d %s %d", &u.id, u.name, &u.age) == 3) {
                    if (u.id == newId) {
                        printf("Error: User ID already exists.\n");
                        found = 1;
                        break;
                    }
                }
                fclose(fp);
            }

            if (!found) {
                fp = fopen("users.txt", "a");
                u.id = newId;
                printf("Enter Name: "); 
                scanf("%s", u.name);
                printf("Enter Age: "); 
                scanf("%d", &u.age);
                fprintf(fp, "%d %s %d\n", u.id, u.name, u.age);
                fclose(fp);
                printf("User added successfully!\n");
            }
        }
        else if (choice == 2) {
            fp = fopen("users.txt", "r");
            if (!fp) { 
            printf("No records found.\n"); 
            continue; 
            }
            printf("\n--- Users ---\n");
            while (fscanf(fp, "%d %s %d", &u.id, u.name, &u.age) == 3) {
                printf("%d %s %d\n", u.id, u.name, u.age);
            }
            fclose(fp);
        }
        else if (choice == 3) {
            fp = fopen("users.txt", "r");
            temp = fopen("temp.txt", "w");
            printf("Enter ID to update: ");
            scanf("%d", &id);
            found = 0;
            while (fscanf(fp, "%d %s %d", &u.id, u.name, &u.age) == 3) {
                if (u.id == id) {
                    printf("Enter new Name: "); scanf("%s", u.name);
                    printf("Enter new Age: "); scanf("%d", &u.age);
                    found = 1;
                }
                fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
            }
            fclose(fp); fclose(temp);
            remove("users.txt");
            rename("temp.txt", "users.txt");
            if (!found) printf("ID not found.\n");
        }
        else if (choice == 4) {
            fp = fopen("users.txt", "r");
            temp = fopen("temp.txt", "w");
            printf("Enter ID to delete: ");
            scanf("%d", &id);
            found = 0;
            while (fscanf(fp, "%d %s %d", &u.id, u.name, &u.age) == 3) {
                if (u.id == id) {
                    found = 1; continue;
                }
                fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
            }
            fclose(fp); fclose(temp);
            remove("users.txt");
            rename("temp.txt", "users.txt");
            if (!found){ 
                printf("ID not found.\n");
            }
        }

    } while (choice != 5);

    return 0;
}
