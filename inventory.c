#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN_PRODUCT_ID 1
#define MAX_PRODUCT_ID 10000
#define NAME_LENGTH 50
#define MIN_PRICE 0
#define MAX_PRICE 100000
#define MIN_QUANTITY 0
#define MAX_QUANTITY 1000000

typedef enum {
    ADD_ITEM = 1,
    VIEW_ITEMS,
    UPDATE_QUANTITY,
    SEARCH_ITEM_ID,
    SEARCH_ITEM_NAME,
    SEARCH_ITEM_PRICE,
    DELETE_ITEM,
    EXIT_SYSTEM
} MenuAction;

typedef struct {
    int id;
    char name[NAME_LENGTH];
    float price;
    int quantity;
} Product;

int stringLength(const char *str) {
    if (!str) {
        return 0;
    }
    int len = 0, spaces = 0;
    while (str[len] != '\n' && str[len] != '\0') {
        if (str[len] == ' ') {
            spaces++;
        }
        len++;
    }
    if (len == 0 || spaces == len) {
        return 0;
    } else {
        return len;
    }
}

bool isIdDuplicate(const int id, const Product *items, const int total) {
    for (int i = 0; i < total; i++) {
        if (items[i].id == id) {
            return true;
        }
    }
    return false;
}

char toLowerCase(const char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 'a';
    } else {
        return c;
    }
}

bool isSameIgnoreCase(const char a, const char b) {
    return toLowerCase(a) == toLowerCase(b);
}

bool startsWithIgnoreCase(const char *prefix, const char *word) {
    int i = 0;
    while (prefix[i] != '\0' && word[i] != '\0') {
        if (!isSameIgnoreCase(prefix[i], word[i])) {
            return false;
        }
        i++;
    }
    if (prefix[i] == '\0') {
        return true;
    } else {
        return false;
    }
}

void inputProductName(Product *item) {
    bool valid = false;
    while (!valid) {
        printf("Product Name: ");
        if (fgets(item->name, NAME_LENGTH, stdin) == NULL) {
            clearerr(stdin);
            continue;
        }
        int len = stringLength(item->name);
        item->name[len] = '\0';
        if (len > 0) {
            valid = true;
        } else {
            printf("Invalid name. Try again.\n");
        }
    }
}

void inputProductId(Product *item, const Product *items, const int total) {
    bool valid = false;
    int id;
    while (!valid) {
        printf("Product ID: ");
        if (scanf("%d", &id) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid input. Enter integer.\n");
            continue;
        }
        int c = getchar();
        (void)c;
        if (id < MIN_PRODUCT_ID || id > MAX_PRODUCT_ID) {
            printf("ID out of range.\n");
        } else if (isIdDuplicate(id, items, total)) {
            printf("ID already exists. Try another.\n");
        } else {
            valid = true;
        }
    }
    item->id = id;
}

void inputProductPrice(Product *item) {
    bool valid = false;
    float price;
    while (!valid) {
        printf("Product Price: ");
        if (scanf("%f", &price) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid input. Enter number.\n");
            continue;
        }
        int c = getchar();
        (void)c;
        if (price < MIN_PRICE || price > MAX_PRICE) {
            printf("Invalid price range.\n");
        } else {
            valid = true;
        }
    }
    item->price = price;
}

void inputProductQuantity(Product *item) {
    bool valid = false;
    int qty;
    while (!valid) {
        printf("Product Quantity: ");
        if (scanf("%d", &qty) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid input. Enter integer.\n");
            continue;
        }
        int c = getchar();
        (void)c;
        if (qty < MIN_QUANTITY || qty > MAX_QUANTITY) {
            printf("Quantity out of range.\n");
        } else {
            valid = true;
        }
    }
    item->quantity = qty;
}

void createProduct(Product *items, const int total, const int index) {
    inputProductId(&items[index], items, total);
    inputProductName(&items[index]);
    inputProductPrice(&items[index]);
    inputProductQuantity(&items[index]);
}

void displayProduct(const Product *item) {
    printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
           item->id, item->name, item->price, item->quantity);
}

void displayAllProducts(const Product *items, const int total) {
    printf("\n--- PRODUCT LIST ---\n");
    for (int i = 0; i < total; i++) {
        displayProduct(&items[i]);
    }
}

int getProductIndexById(const Product *items, const int total, const int id) {
    for (int i = 0; i < total; i++) {
        if (items[i].id == id) {
            return i;
        }
    }
    printf("No product found with ID %d.\n", id);
    return -1;
}

void addNewProduct(Product **items, int *total, int *capacity) {
    if (*total == *capacity) {
        *capacity = (*capacity == 0) ? 1 : (*capacity * 2);
        Product *temp = realloc(*items, (*capacity) * sizeof(Product));
        if (!temp) {
            printf("Memory allocation failed.\n");
            return;
        } else {
            *items = temp;
        }
    }
    createProduct(*items, *total, *total);
    (*total)++;
    printf("Product added successfully.\n");
}

void updateProductQuantity(Product *items, const int total) {
    int id;
    printf("Enter Product ID: ");
    if (scanf("%d", &id) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        return;
    }
    int c = getchar();
    (void)c;
    int index = getProductIndexById(items, total, id);
    if (index != -1) {
        inputProductQuantity(&items[index]);
        printf("Quantity updated.\n");
    }
}

void searchProductById(const Product *items, const int total) {
    int id;
    printf("Enter Product ID: ");
    if (scanf("%d", &id) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        return;
    }
    int c = getchar();
    (void)c;
    int index = getProductIndexById(items, total, id);
    if (index != -1) {
        displayProduct(&items[index]);
    }
}

void searchProductByName(const Product *items, const int total) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        break;
    }
    char input[NAME_LENGTH];
    printf("Enter name (partial allowed): ");
    if (fgets(input, NAME_LENGTH, stdin) == NULL) {
        clearerr(stdin);
        return;
    }
    int len = stringLength(input);
    input[len] = '\0';
    bool found = false;
    for (int i = 0; i < total; i++) {
        if (startsWithIgnoreCase(input, items[i].name)) {
            if (!found) {
                printf("Matching products:\n");
            }
            found = true;
            displayProduct(&items[i]);
        }
    }
    if (!found) {
        printf("No match found for '%s'.\n", input);
    }
}

void searchProductByPrice(const Product *items, const int total) {
    float min, max;
    printf("Enter minimum price: ");
    if (scanf("%f", &min) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        return;
    }
    printf("Enter maximum price: ");
    if (scanf("%f", &max) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        return;
    }
    int c = getchar();
    (void)c;
    bool found = false;
    for (int i = 0; i < total; i++) {
        if (items[i].price >= min && items[i].price <= max) {
            if (!found) {
                printf("Products in range:\n");
            }
            found = true;
            displayProduct(&items[i]);
        }
    }
    if (!found) {
        printf("No products between %.2f and %.2f.\n", min, max);
    }
}

void swapProducts(Product *a, Product *b) {
    Product temp = *a;
    *a = *b;
    *b = temp;
}

void deleteProduct(Product *items, int *total) {
    int id;
    printf("Enter Product ID to delete: ");
    if (scanf("%d", &id) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        return;
    }
    int c = getchar();
    (void)c;
    int index = getProductIndexById(items, *total, id);
    if (index != -1) {
        for (int i = index + 1; i < *total; i++) {
            swapProducts(&items[i - 1], &items[i]);
        }
        (*total)--;
        printf("Product deleted.\n");
    }
}

void runInventorySystem(const int initialCount) {
    Product *items = calloc(initialCount, sizeof(Product));
    if (!items) {
        printf("Memory allocation failed.\n");
        return;
    }

    for (int i = 0; i < initialCount; i++) {
        printf("\nEnter details for product %d\n", i + 1);
        createProduct(items, initialCount, i);
    }

    int total = initialCount, capacity = initialCount, choice = 0;
    bool active = true;

    while (active) {
        printf("\n--- INVENTORY MENU ---\n");
        printf("1. Add Product\n2. View Products\n3. Update Quantity\n4. Search by ID\n5. Search by Name\n6. Search by Price Range\n7. Delete Product\n8. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }
        int c = getchar();
        (void)c;

        switch (choice) {
            case ADD_ITEM: {
                addNewProduct(&items, &total, &capacity);
                break;
            }
            case VIEW_ITEMS: {
                displayAllProducts(items, total);
                break;
            }
            case UPDATE_QUANTITY: {
                updateProductQuantity(items, total);
                break;
            }
            case SEARCH_ITEM_ID: {
                searchProductById(items, total);
                break;
            }
            case SEARCH_ITEM_NAME: {
                searchProductByName(items, total);
                break;
            }
            case SEARCH_ITEM_PRICE: {
                searchProductByPrice(items, total);
                break;
            }
            case DELETE_ITEM: {
                deleteProduct(items, &total);
                break;
            }
            case EXIT_SYSTEM: {
                active = false;
                break;
            }
            default: {
                printf("Invalid choice.\n");
                break;
            }
        }
    }

    free(items);
    printf("Exiting... Memory freed.\n");
}

int main() {
    int initialCount = 0;
    while (initialCount < 1 || initialCount > 100) {
        printf("Enter initial number of products (1 - 100): ");
        if (scanf("%d", &initialCount) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }
        int c = getchar();
        (void)c;
    }
    runInventorySystem(initialCount);
    return 0;
}
