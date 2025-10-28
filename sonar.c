#include <stdio.h>
#include <stdlib.h>

int isValidValue(int value) {
    if (value < 0 || value > 255) {
        printf("\nEnter value between 0 and 255: ");
        return 0;
    }
    return 1;
}

int isValidMatrixSize(int size) {
    if (size < 2 || size > 10) {
        printf("\nEnter value between 2 and 10: ");
        return 0;
    }
    return 1;
}

void getValidInput(const int size, int (*matrix)[size], int row, int col) {
    int isValid = 0;
    while (!isValid) {
        scanf("%d", (*(matrix + row) + col));
        isValid = isValidValue(*(*(matrix + row) + col));
    }
}

void takeMatrixInput(const int size, int (*matrix)[size]) {
    printf("\nEnter the values of %d x %d matrix:\n", size, size);
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            getValidInput(size, matrix, row, col);
        }
    }
}

void printMatrix(const int size, int (*matrix)[size]) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            printf("%4d", *(*(matrix + row) + col));
        }
        printf("\n");
    }
}

void transposeMatrix(const int size, int (*matrix)[size]) {
    for (int row = 0; row < size; row++) {
        for (int col = row; col < size; col++) {
            int temp = *(*(matrix + row) + col);
            *(*(matrix + row) + col) = *(*(matrix + col) + row);
            *(*(matrix + col) + row) = temp;
        }
    }
}

void swapRows(const int size, int (*matrix)[size]) {
    for (int row = 0; row < size; row++) {
        int start = 0, end = size - 1;
        while (start < end) {
            int temp = *(*(matrix + row) + start);
            *(*(matrix + row) + start) = *(*(matrix + row) + end);
            *(*(matrix + row) + end) = temp;
            start++;
            end--;
        }
    }
}

void rotateMatrix(const int size, int (*matrix)[size]) {
    transposeMatrix(size, matrix);
    swapRows(size, matrix);
    printf("\nRotated:\n");
    printMatrix(size, matrix);
}

void applyFilterToElement(const int size, int (*matrix)[size], int row, int col) {
    int count = 0, sum = 0;
    for (int filterRow = -1; filterRow <= 1; filterRow++) {
        for (int filterCol = -1; filterCol <= 1; filterCol++) {
            int neighborRow = row + filterRow;
            int neighborCol = col + filterCol;
            if (neighborRow >= 0 && neighborRow < size && neighborCol >= 0 && neighborCol < size) {
                sum += *(*(matrix + neighborRow) + neighborCol) & 0xFF;
                count++;
            }
        }
    }
    int avg = sum / count;
    *(*(matrix + row) + col) |= (avg << 8);
}

void applyFilter(const int size, int (*matrix)[size]) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            applyFilterToElement(size, matrix, row, col);
        }
    }

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            *(*(matrix + row) + col) >>= 8;
        }
    }
}

int main() {
    int size = 0;
    int isValidSize = 0;

    while (!isValidSize) {
        printf("\nEnter the N value: ");
        scanf("%d", &size);
        isValidSize = isValidMatrixSize(size);
    }

    int (*matrix)[size] = malloc(size * sizeof(*matrix));
    if (matrix == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    takeMatrixInput(size, matrix);

    printf("\nOriginal:\n");
    printMatrix(size, matrix);

    rotateMatrix(size, matrix);
    applyFilter(size, matrix);

    printf("\nFiltered matrix:\n");
    printMatrix(size, matrix);

    free(matrix);
    matrix = NULL;

    return 0;
}
