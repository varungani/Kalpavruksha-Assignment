#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

int validInput(int a) {
  if(a < 0 || a > 255)  {
    printf("\nEnter value between 0 and 255:");
    return 0;
  }
  return 1;
}

void getValidInput(int n, int (*matrix)[n], int i, int j) {
  do
      {
        scanf("%d", (*(matrix + i) + j));
      } while (!validInput(*(*(matrix + i) + j)));
}

int validNValue(int n)  {
  if(n < 2 || n > 10) {
    printf("\nEnter value between 2 and 10: ");
    return 0;
  }
  return 1;
}

void  takeInput(int n, int (*matrix)[n]) {
  printf("\nEnter the values of %d * %d matrix:\n", n, n);

  for(int i = 0;i < n;i++)  {
    for(int j = 0;j < n;j++)  {
      getValidInput(n, matrix, i, j);
    }
  }
}

void printInput(int n, int (*matrix)[n]) {

  //printf("\n\nOriginal Matrix:\n");
  for(int i = 0;i < n;i++)  {
    for(int j = 0;j < n;j++)  {
      printf("%d ", *(*(matrix + i) + j));
    }
    printf("\n");
  }
}

void rotateMatrix(int n, int (*matrix)[n])  {

  //transpose
  for(int i = 0;i < n;i++)  {
    for(int j = i;j < n;j++) {
      int temp = *(*(matrix + i) + j);
      *(*(matrix + i) + j) = *(*(matrix + j) + i);
      *(*(matrix + j) + i) = temp; 
    }
  }
  //printf("\nTransposed:\n");
  //printInput(n, matrix);

  //row swap
  for(int i = 0;i < n;i++)  {
    int start = 0, end = n - 1;
    while(start < end)  {
      int temp = *(*(matrix + i) + start);
      *(*(matrix + i) + start) = *(*(matrix + i) + end);
      *(*(matrix + i) + end) = temp;
      start++;
      end--;
    }
  }
  printf("\nRotated:\n");
  printInput(n, matrix);
}


void calculateFilter(int n, int (*matrix)[n], int i, int j) {
  int count = 0, sum = 0;
  for(int filterI = -1;filterI <= 1;filterI++)  {
    for(int filterJ = -1;filterJ <= 1;filterJ++)  {
      int k = i + filterI, m = j + filterJ;
      if(k >= 0 && k < n && m >= 0 && m < n)  {
        sum += *(*(matrix + k) + m) & 0xFF;
        count++;
      }
    }
  }

  int res = sum / count;
  *(*(matrix + i) + j) |= (res << 8);
}
void filterMatrix(int n, int (*matrix)[n])  {
  for(int i = 0;i < n;i++)  {
    for(int j = 0;j < n;j++)  {
      calculateFilter(n, matrix, i, j);
    }
  }

  //shifting bits to filtered value
  for(int i = 0;i < n;i++)  {
    for(int j = 0;j < n;j++)  {
      *(*(matrix + i) + j) = *(*(matrix + i) + j) >> 8;
    }
  }
}

int main()  {

  int n;

  do
  {
    printf("\nEnter the N value:");
    scanf("%d", &n);
  } while (!validNValue(n));
  


  int matrix[n][n];

  takeInput(n, matrix);

  printf("\nOriginal:\n");
  printInput(n, matrix);
  
  rotateMatrix(n, matrix);
  //printInput(n, matrix);

  filterMatrix(n, matrix);
  printf("\nFiltered matrix:\n");
  printInput(n, matrix);
  return 0;
}