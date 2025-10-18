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

    for (int index = 0; index < stars; index++)
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
    int totalStudents;
    printf("Enter number of students: ");
    scanf("%d", &totalStudents);

    Student students[totalStudents];

    for (int index = 0; index < totalStudents; index++)
    {
        printf("\nEnter details for student %d:\n", index + 1);
        scanf("%d %s %d %d %d",
              &students[index].rollNumber,
              students[index].name,
              &students[index].marks[0],
              &students[index].marks[1],
              &students[index].marks[2]);

        students[index].total = calculateTotal(students[index].marks);
        students[index].average = calculateAverage(students[index].total);
        students[index].grade = assignGrade(students[index].average);
    }

    printf("\n");
    for (int index = 0; index < totalStudents; index++)
    {
        printf("Roll: %d\n", students[index].rollNumber);
        printf("Name: %s\n", students[index].name);
        printf("Total: %d\n", students[index].total);
        printf("Average: %.2f\n", students[index].average);
        printf("Grade: %c\n", students[index].grade);

        if (students[index].average < 35)
        {
            continue;
        }

        printf("Performance: ");
        printPerformance(students[index].grade);
        printf("\n\n");
    }

    printf("List of Roll Numbers (via recursion): ");
    printRollNumbers(students, totalStudents, 0);
    printf("\n");

    return 0;
}
