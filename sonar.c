#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int validateMatrixSize(const int size) {
    int isValid = 1;
    if (size < 2 || size > 10) {
        printf("Matrix size must be between 2 and 10.\n");
        isValid = 0;
    }
    return isValid;
}

void generateRandomMatrix(const int size, int (*matrix)[size]) {
    printf("\nGenerated %d x %d matrix:\n", size, size);
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            *(*(matrix + row) + col) = rand() % 256;
            printf("%4d", *(*(matrix + row) + col));
        }
        printf("\n");
    }
}

void printMatrix(const int size, const int (*matrix)[size]) {
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

void reverseRows(const int size, int (*matrix)[size]) {
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

void rotateMatrixClockwise(const int size, int (*matrix)[size]) {
    transposeMatrix(size, matrix);
    reverseRows(size, matrix);
    printf("\nMatrix after rotation:\n");
    printMatrix(size, matrix);
}

void applyMeanFilter(const int size, int (*matrix)[size]) {
    int tempMatrix[size][size];

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            int sum = 0, count = 0;

            for (int dRow = -1; dRow <= 1; dRow++) {
                for (int dCol = -1; dCol <= 1; dCol++) {
                    int nRow = row + dRow;
                    int nCol = col + dCol;

                    if (nRow >= 0 && nRow < size && nCol >= 0 && nCol < size) {
                        sum += *(*(matrix + nRow) + nCol);
                        count++;
                    }
                }
            }
            tempMatrix[row][col] = sum / count;
        }
    }

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            *(*(matrix + row) + col) = tempMatrix[row][col];
        }
    }

    printf("\nMatrix after applying mean filter:\n");
    printMatrix(size, matrix);
}

int main() {
    srand((unsigned) time(NULL));

    int matrixSize = 0;
    int isValidSize = 0;

    while (!isValidSize) {
        printf("Enter matrix size (2 to 10): ");
        scanf("%d", &matrixSize);
        isValidSize = validateMatrixSize(matrixSize);
    }

    int (*matrix)[matrixSize] = malloc(matrixSize * sizeof(*matrix));
    if (matrix == NULL) {
        printf("Memory allocation failed.\n");
        return 0;
    }

    generateRandomMatrix(matrixSize, matrix);
    rotateMatrixClockwise(matrixSize, matrix);
    applyMeanFilter(matrixSize, matrix);

    free(matrix);
    matrix = NULL;

    return 0;
}
