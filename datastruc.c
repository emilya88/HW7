#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50];
    char surname[50];
    int id;
    float grade;
} Student;

typedef struct {
    Student* students;
    int count;
} StudentList;

void addStudent(StudentList* list);
void removeStudentById(StudentList* list, int id);
void searchStudentById(StudentList* list, int id);
void displayAllStudents(StudentList* list);
void displayAverageGrade(StudentList* list);
void saveStudentsToFile(StudentList* list);
void loadStudentsFromFile(StudentList* list);
void freeMemory(StudentList* list);

int main() {
    StudentList studentList;
    studentList.students = NULL;
    studentList.count = 0;

    int choice;
    int id;

    do {
        printf("1. Add a new student\n");
        printf("2. Remove a student by ID\n");
        printf("3. Search a student by ID\n");
        printf("4. Display all student details\n");
        printf("5. Display average grade of all students\n");
        printf("6. Save student details into 'group_details.csv'\n");
        printf("7. Load student details from file\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1:
                addStudent(&studentList);
                break;
            case 2:
                printf("Enter student ID to remove: ");
                scanf("%d", &id);
                removeStudentById(&studentList, id);
                break;
            case 3:
                printf("Enter student ID to search: ");
                scanf("%d", &id);
                searchStudentById(&studentList, id);
                break;
            case 4:
                displayAllStudents(&studentList);
                break;
            case 5:
                displayAverageGrade(&studentList);
                break;
            case 6:
                saveStudentsToFile(&studentList);
                break;
            case 7:
                loadStudentsFromFile(&studentList);
                break;
            case 8:
                freeMemory(&studentList);
                printf("Exiting the program.\n");
                break;
            default:
                printf("Please try again.\n");
        }
    } while (choice != 8);

    return 0;
}

void addStudent(StudentList* list) {
    list->students = realloc(list->students, (list->count + 1) * sizeof(Student));
    if (list->students == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    Student* newStudent = &list->students[list->count];

    printf("Enter student name: ");
    fgets(newStudent->name, sizeof(newStudent->name), stdin);
    newStudent->name[strcspn(newStudent->name, "\n")] = '\0';  

    printf("Enter student surname: ");
    fgets(newStudent->surname, sizeof(newStudent->surname), stdin);
    newStudent->surname[strcspn(newStudent->surname, "\n")] = '\0';

    printf("Enter student ID: ");
    scanf("%d", &newStudent->id);
    getchar(); 

    printf("Enter student grade: ");
    scanf("%f", &newStudent->grade);
    getchar(); 

    list->count++;
    printf("Student added successfully.\n");
}

void removeStudentById(StudentList* list, int id) {
    int i, found = 0;
    for (i = 0; i < list->count; i++) {
        if (list->students[i].id == id) {
            found = 1;
            break;
        }
    }

    if (found) {
        for (int j = i; j < list->count - 1; j++) {
            list->students[j] = list->students[j + 1];
        }
        list->students = realloc(list->students, (list->count - 1) * sizeof(Student));
        if (list->students == NULL && list->count > 1) {
            printf("Memory allocation failed.\n");
            exit(1);
        }
        list->count--;
        printf("Student with ID %d removed.\n", id);
    } else {
        printf("Student with ID %d not found.\n", id);
    }
}

void searchStudentById(StudentList* list, int id) {
    int found = 0;
    for (int i = 0; i < list->count; i++) {
        if (list->students[i].id == id) {
            printf("Student found: %s %s (ID: %d, Grade: %.2f)\n", 
                    list->students[i].name, list->students[i].surname, 
                    list->students[i].id, list->students[i].grade);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student with ID %d not found.\n", id);
    }
}

void displayAllStudents(StudentList* list) {
    if (list->count == 0) {
        printf("No students to display.\n");
        return;
    }

    for (int i = 0; i < list->count; i++) {
        printf("Student %d: %s %s (ID: %d, Grade: %.2f)\n", 
                i + 1, list->students[i].name, list->students[i].surname, 
                list->students[i].id, list->students[i].grade);
    }
}

void displayAverageGrade(StudentList* list) {
    if (list->count == 0) {
        printf("No students available to calculate average grade.\n");
        return;
    }

    float totalGrade = 0;
    for (int i = 0; i < list->count; i++) {
        totalGrade += list->students[i].grade;
    }

    printf("Average grade of all students: %.2f\n", totalGrade / list->count);
}

void saveStudentsToFile(StudentList* list) {
    FILE* file = fopen("group_details.csv", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < list->count; i++) {
        fprintf(file, "%s,%s,%d,%.2f\n", 
                list->students[i].name, list->students[i].surname, 
                list->students[i].id, list->students[i].grade);
    }

    fclose(file);
    printf("Student details saved to 'group_details.csv'.\n");
}

void loadStudentsFromFile(StudentList* list) {
    FILE* file = fopen("group_details.csv", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    while (!feof(file)) {
        list->students = realloc(list->students, (list->count + 1) * sizeof(Student));
        if (list->students == NULL) {
            printf("Memory allocation failed.\n");
            fclose(file);
            exit(1);
        }

        Student* newStudent = &list->students[list->count];
        fscanf(file, "%49[^,],%49[^,],%d,%f\n", 
               newStudent->name, newStudent->surname, 
               &newStudent->id, &newStudent->grade);
        
        list->count++;
    }

    fclose(file);
    printf("Student details loaded from 'group_details.csv'.\n");
}

void freeMemory(StudentList* list) {
    free(list->students);
    list->students = NULL;
    list->count = 0;
}
