#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN_ID 1
#define MAX_ID 10000
#define NAME_LIMIT 50
#define MIN_COST 0
#define MAX_COST 100000
#define MIN_STOCK 0
#define MAX_STOCK 1000000

typedef enum {
    ADD_PRODUCT = 1,
    VIEW_PRODUCTS,
    UPDATE_STOCK,
    SEARCH_BY_ID,
    SEARCH_BY_NAME,
    SEARCH_BY_PRICE,
    DELETE_PRODUCT,
    EXIT_APP
} MenuOption;

typedef struct {
    int id;
    char name[NAME_LIMIT];
    float price;
    int stock;
} Product;

int getLength(const char *s) {
    if (!s) return 0;
    int i = 0, spaces = 0;
    while (s[i] != '\n' && s[i] != '\0') {
        if (s[i] == ' ') spaces++;
        i++;
    }
    return (i == 0 || spaces == i) ? 0 : i;
}

bool idExists(const int id, const Product *list, const int count) {
    for (int i = 0; i < count; i++)
        if (list[i].id == id)
            return true;
    return false;
}

char lower(const char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A' + 'a';
    return c;
}

bool sameIgnoreCase(const char a, const char b) {
    return lower(a) == lower(b);
}

bool startsWithIgnoreCase(const char *query, const char *name) {
    int i = 0;
    while (query[i] != '\0' && name[i] != '\0') {
        if (!sameIgnoreCase(query[i], name[i])) return false;
        i++;
    }
    return query[i] == '\0';
}

void getName(Product *item) {
    bool ok = false;
    while (!ok) {
        printf("Product Name: ");
        if (fgets(item->name, NAME_LIMIT, stdin) == NULL) {
            clearerr(stdin);
            continue;
        }
        int len = getLength(item->name);
        item->name[len] = '\0';
        ok = len > 0;
        if (!ok) printf("Invalid name. Try again.\n");
    }
}

void getId(Product *item, const Product *list, const int count) {
    bool ok = false;
    int id;
    while (!ok) {
        printf("Product ID: ");
        if (scanf("%d", &id) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Enter integer.\n");
            continue;
        }
        int c = getchar(); (void)c;
        if (id < MIN_ID || id > MAX_ID) {
            printf("ID out of range.\n");
        } else if (idExists(id, list, count)) {
            printf("ID already exists. Try another.\n");
        } else ok = true;
    }
    item->id = id;
}

void getPrice(Product *item) {
    bool ok = false;
    float cost;
    while (!ok) {
        printf("Product Price: ");
        if (scanf("%f", &cost) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Enter number.\n");
            continue;
        }
        int c = getchar(); (void)c;
        if (cost < MIN_COST || cost > MAX_COST)
            printf("Invalid price range.\n");
        else ok = true;
    }
    item->price = cost;
}

void getStock(Product *item) {
    bool ok = false;
    int qty;
    while (!ok) {
        printf("Product Quantity: ");
        if (scanf("%d", &qty) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Enter integer.\n");
            continue;
        }
        int c = getchar(); (void)c;
        if (qty < MIN_STOCK || qty > MAX_STOCK)
            printf("Quantity out of range.\n");
        else ok = true;
    }
    item->stock = qty;
}

void fillProduct(Product *list, const int count, const int idx) {
    getId(&list[idx], list, count);
    getName(&list[idx]);
    getPrice(&list[idx]);
    getStock(&list[idx]);
}

void showProduct(const Product *p) {
    printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
           p->id, p->name, p->price, p->stock);
}

void showAll(const Product *list, const int count) {
    printf("\n--- PRODUCT LIST ---\n");
    for (int i = 0; i < count; i++)
        showProduct(&list[i]);
}

int findIndexById(const Product *list, const int count, const int id) {
    for (int i = 0; i < count; i++)
        if (list[i].id == id)
            return i;
    printf("No product found with ID %d.\n", id);
    return -1;
}

void addProduct(Product **list, int *count, int *cap) {
    if (*count == *cap) {
        *cap = (*cap == 0) ? 1 : (*cap * 2);
        Product *tmp = realloc(*list, (*cap) * sizeof(Product));
        if (!tmp) {
            printf("Memory allocation failed.\n");
            return;
        }
        *list = tmp;
    }
    fillProduct(*list, *count, *count);
    (*count)++;
    printf("Product added successfully.\n");
}

void modifyStock(Product *list, const int count) {
    int id;
    printf("Enter Product ID: ");
    if (scanf("%d", &id) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); return; }
    int c = getchar(); (void)c;
    int idx = findIndexById(list, count, id);
    if (idx != -1) {
        getStock(&list[idx]);
        printf("Quantity updated.\n");
    }
}

void searchById(const Product *list, const int count) {
    int id;
    printf("Enter Product ID: ");
    if (scanf("%d", &id) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); return; }
    int c = getchar(); (void)c;
    int idx = findIndexById(list, count, id);
    if (idx != -1) showProduct(&list[idx]);
}

void searchByName(const Product *list, const int count) {
    int c; while ((c = getchar()) != '\n' && c != EOF) { break; }
    char query[NAME_LIMIT];
    printf("Enter name (partial allowed): ");
    if (fgets(query, NAME_LIMIT, stdin) == NULL) { clearerr(stdin); return; }
    int len = getLength(query);
    query[len] = '\0';
    bool found = false;
    for (int i = 0; i < count; i++) {
        if (startsWithIgnoreCase(query, list[i].name)) {
            if (!found) printf("Matching products:\n");
            found = true;
            showProduct(&list[i]);
        }
    }
    if (!found) printf("No match found for '%s'.\n", query);
}

void searchByPrice(const Product *list, const int count) {
    float min, max;
    printf("Enter minimum price: ");
    if (scanf("%f", &min) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); return; }
    printf("Enter maximum price: ");
    if (scanf("%f", &max) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); return; }
    int c = getchar(); (void)c;
    bool found = false;
    for (int i = 0; i < count; i++) {
        if (list[i].price >= min && list[i].price <= max) {
            if (!found) printf("Products in range:\n");
            found = true;
            showProduct(&list[i]);
        }
    }
    if (!found) printf("No products between %.2f and %.2f.\n", min, max);
}

void swap(Product *a, Product *b) {
    Product t = *a;
    *a = *b;
    *b = t;
}

void removeProduct(Product *list, int *count) {
    int id;
    printf("Enter Product ID to delete: ");
    if (scanf("%d", &id) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); return; }
    int c = getchar(); (void)c;
    int idx = findIndexById(list, *count, id);
    if (idx != -1) {
        for (int i = idx + 1; i < *count; i++)
            swap(&list[i - 1], &list[i]);
        (*count)--;
        printf("Product deleted.\n");
    }
}

void manageInventory(const int initialCount) {
    Product *list = calloc(initialCount, sizeof(Product));
    if (!list) {
        printf("Memory allocation failed.\n");
        return;
    }

    for (int i = 0; i < initialCount; i++) {
        printf("\nEnter details for product %d\n", i + 1);
        fillProduct(list, initialCount, i);
    }

    int count = initialCount, cap = initialCount, choice = 0;
    bool running = true;

    while (running) {
        printf("\n--- INVENTORY MENU ---\n");
        printf("1. Add Product\n2. View Products\n3. Update Quantity\n4. Search by ID\n5. Search by Name\n6. Search by Price Range\n7. Delete Product\n8. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); continue; }
        int c = getchar(); (void)c;

        switch (choice) {
            case ADD_PRODUCT: addProduct(&list, &count, &cap); break;
            case VIEW_PRODUCTS: showAll(list, count); break;
            case UPDATE_STOCK: modifyStock(list, count); break;
            case SEARCH_BY_ID: searchById(list, count); break;
            case SEARCH_BY_NAME: searchByName(list, count); break;
            case SEARCH_BY_PRICE: searchByPrice(list, count); break;
            case DELETE_PRODUCT: removeProduct(list, &count); break;
            case EXIT_APP: running = false; break;
            default: printf("Invalid choice.\n");
        }
    }

    free(list);
    printf("Exiting... Memory freed.\n");
}

int main() {
    int count = 0;
    while (count < 1 || count > 100) {
        printf("Enter initial number of products (1 - 100): ");
        if (scanf("%d", &count) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); continue; }
        int c = getchar(); (void)c;
    }
    manageInventory(count);
    return 0;
}
