#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum
{
    SUCCESS,
    INVALID_INPUT,
    MEMORY_ERROR,
    FILE_ERROR
} status_code;

typedef struct
{
    unsigned int id;
    char name[256];
    char surname[256];
    char group[256];
    unsigned char* grades;
} Student;

void print_student(Student* students, int found)
{ 
    if(found == -1)
    {
        printf("There is nosuch student\n");
        return;
    }
    else
    {
       printf("information about found student: \n");
       printf("%d\n%s\n%s\n%s\n", students[found].id, students[found].name, students[found].surname, students[found].group); //TODO: avarage grade
    }
    
}

int find_student_by_id(Student* students, int count_of_students, int id_for_search)
{
    for (int found = 0; found < count_of_students; found++)
    {
        if(students[found].id == id_for_search)
        {
            return found;
        }
    }

    return  -1;
}

status_code read_from_file(const char* input_file, Student** students, int* count_of_students)
{
    FILE* file = fopen(input_file, "r");
    if (!file) 
    {
        return FILE_ERROR;
    }
    char buffer[512];

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {   
        (*count_of_students)++;
        *students = realloc(*students, (*count_of_students) * sizeof(Student));
        if (*students == NULL)
        {
            fclose(file);
            return MEMORY_ERROR;
        }
        (*students)[*count_of_students - 1].grades = malloc(5 * sizeof(unsigned char));
        if (!(*students)[*count_of_students - 1].grades)
        {
            fclose(file);
            return MEMORY_ERROR;
        }

        sscanf(buffer, "%u %s %s %s %hhu %hhu %hhu %hhu %hhu", 
            &(*students)[*count_of_students - 1].id,
            (*students)[*count_of_students - 1].name, 
            (*students)[*count_of_students - 1].surname,
            (*students)[*count_of_students - 1].group, 
            &(*students)[*count_of_students - 1].grades[0],
            &(*students)[*count_of_students - 1].grades[1], 
            &(*students)[*count_of_students - 1].grades[2],
            &(*students)[*count_of_students - 1].grades[3], 
            &(*students)[*count_of_students - 1].grades[4]);
    }

    fclose(file);
    return SUCCESS;
}

void print_choice()
{
    printf("****************************************\n");
    printf("*     Please select an action           *\n");
    printf("*                                       *\n");
    printf("*  Enter <1> to find student by ID      *\n");
    printf("*  Enter <2> to find student by surname *\n");
    printf("*  Enter <3> to find student by name    *\n");
    printf("*  Enter <4> to find student by group   *\n");
    printf("*                                       *\n");
    printf("****************************************\n");
}


void print_students(Student* students, int count_of_students)
{
    for (int found = 0; found < count_of_students; found++)
    {
        printf("Student %d\n", found + 1);
        printf("ID: %u\n", students[found].id);
        printf("Name: %s\n", students[found].name);
        printf("Surname: %s\n", students[found].surname);
        printf("Group: %s\n", students[found].group);
        printf("Grades: ");
        for (int j = 0; j < 5; j++)
        {
            printf("%hhu ", students[found].grades[j]);
        }
        printf("\n\n");
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Invalid input! Please enter <input file> <output file>\n"); 
        return INVALID_INPUT;
    }

    Student* students = NULL;
    int count_of_students = 0;

    status_code read_status;
    read_status = read_from_file(argv[1], &students, &count_of_students);
    print_choice();
    int action = 0;
    int res_id = 0;
    int id_for_search = 0;
    scanf("%d", &action);

    switch(action)
    {
        case 1:
            
            printf("Enter the id of student to search: ");
            scanf("%d", &id_for_search);
            res_id = find_student_by_id(students, count_of_students, id_for_search);
            print_student(students, res_id);
            break;
    }

    free(students);
    return 0;
}
