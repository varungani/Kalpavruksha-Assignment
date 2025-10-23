#include <stdio.h>

typedef struct
{
    int rollNumber;
    char name[50];
    int marks[3];
    int totalMarks;
    float averageMarks;
    char grade;
} Student;

int calculateTotalMarks(const int marks[])
{
    int total = 0;
    for (int i = 0; i < 3; i++)
    {
        total += marks[i];
    }
    return total;
}

float calculateAverageMarks(const int totalMarks)
{
    return totalMarks / 3.0f;
}

char determineGrade(const float averageMarks)
{
    if (averageMarks >= 85)
    {
        return 'A';
    }
    else if (averageMarks >= 70)
    {
        return 'B';
    }
    else if (averageMarks >= 50)
    {
        return 'C';
    }
    else if (averageMarks >= 35)
    {
        return 'D';
    }
    else
    {
        return 'F';
    }
}

void displayPerformanceStars(const char grade)
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
        break;
    }

    for (int i = 0; i < stars; i++)
    {
        printf("*");
    }
}

void displayRollNumbersRecursively(const Student students[], const int totalStudents, const int currentIndex)
{
    if (currentIndex == totalStudents)
    {
        return;
    }

    printf("%d ", students[currentIndex].rollNumber);
    displayRollNumbersRecursively(students, totalStudents, currentIndex + 1);
}

void readStudentDetails(Student *studentPtr, const int studentNumber)
{
    printf("\nEnter details for student %d:\n", studentNumber);
    scanf("%d %s %d %d %d",
          &studentPtr->rollNumber,
          studentPtr->name,
          &studentPtr->marks[0],
          &studentPtr->marks[1],
          &studentPtr->marks[2]);

    studentPtr->totalMarks = calculateTotalMarks(studentPtr->marks);
    studentPtr->averageMarks = calculateAverageMarks(studentPtr->totalMarks);
    studentPtr->grade = determineGrade(studentPtr->averageMarks);
}

int main()
{
    int totalStudents;
    printf("Enter number of students: ");
    scanf("%d", &totalStudents);

    Student students[totalStudents];

    for (int i = 0; i < totalStudents; i++)
    {
        readStudentDetails(&students[i], i + 1);
    }

    printf("\n");

    for (int i = 0; i < totalStudents; i++)
    {
        printf("Roll: %d\n", students[i].rollNumber);
        printf("Name: %s\n", students[i].name);
        printf("Total Marks: %d\n", students[i].totalMarks);
        printf("Average: %.2f\n", students[i].averageMarks);
        printf("Grade: %c\n", students[i].grade);

        if (students[i].averageMarks >= 35)
        {
            printf("Performance: ");
            displayPerformanceStars(students[i].grade);
            printf("\n\n");
        }
        else
        {
            printf("Failed.\n\n");
        }
    }

    printf("List of Roll Numbers (via recursion): ");
    displayRollNumbersRecursively(students, totalStudents, 0);
    printf("\n");

    return 0;
}
