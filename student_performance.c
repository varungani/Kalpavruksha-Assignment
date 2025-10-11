#include <stdio.h>

typedef struct
{
    int rollNumber;
    char name[50];
    int marks[3];
    int total;
    float average;
    char grade;
} Student;

int calculateTotal(int marks[])
{
    int total = 0;
    for (int i = 0; i < 3; i++)
    {
        total += marks[i];
    }
    return total;
}

float calculateAverage(int total)
{
    return total / 3.0f;
}

char assignGrade(float average)
{
    if (average >= 85)
        return 'A';
    else if (average >= 70)
        return 'B';
    else if (average >= 50)
        return 'C';
    else if (average >= 35)
        return 'D';
    else
        return 'F';
}

void printPerformance(char grade)
{
    int stars = 0;

    switch (grade)
    {
    case 'A':
        stars = 5;
        break;
    case 'B':
        stars = 4;
        break;
    case 'C':
        stars = 3;
        break;
    case 'D':
        stars = 2;
        break;
    default:
        return;
    }

    for (int i = 0; i < stars; i++)
    {
        printf("*");
    }
}

void printRollNumbers(Student students[], int n, int index)
{
    if (index == n)
    {
        return;
    }

    printf("%d ", students[index].rollNumber);
    printRollNumbers(students, n, index + 1);
}

int main()
{
    int n;
    printf("Enter number of students: ");
    scanf("%d", &n);

    Student students[n];

    for (int i = 0; i < n; i++)
    {
        printf("\nEnter details for student %d:\n", i + 1);
        scanf("%d %s %d %d %d",
              &students[i].rollNumber,
              students[i].name,
              &students[i].marks[0],
              &students[i].marks[1],
              &students[i].marks[2]);

        students[i].total = calculateTotal(students[i].marks);
        students[i].average = calculateAverage(students[i].total);
        students[i].grade = assignGrade(students[i].average);
    }

    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf("Roll: %d\n", students[i].rollNumber);
        printf("Name: %s\n", students[i].name);
        printf("Total: %d\n", students[i].total);
        printf("Average: %.2f\n", students[i].average);
        printf("Grade: %c\n", students[i].grade);

        if (students[i].average < 35)
        {
            continue;
        }

        printf("Performance: ");
        printPerformance(students[i].grade);
        printf("\n\n");
    }

    printf("List of Roll Numbers (via recursion): ");
    printRollNumbers(students, n, 0);
    printf("\n");

    return 0;
}
