#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
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
status_code check_action(int action)
{
    if(action < 1 || action > 4)
    {
        return INVALID_INPUT;
    }

    return SUCCESS;
}
void print_student(Student* students, int found)
{ 
    if(found == -1)
    {
        printf("There is no such student\n");
        return;
    }
    else
    {
       printf("information about found student: \n");
       printf("%d\n%s\n%s\n%s\n", students[found].id, students[found].name, students[found].surname, students[found].group); //TODO: avarage grade
       printf("Grades:\n");
       for(int i = 0; i < 5; i++)
       {
            printf("%d\n", students[found].grades[i]);
       }
    }
    
}
int find_student_by_surname(Student* students, int count_of_students, char* buffer)
{
    for(int i = 0; i < count_of_students; i++)
    {
        if(strcmp(students[i].surname, buffer) == 0)
        {
            return i;
        }
    }

    return -1;
}
int find_student_by_id(Student* students, int count_of_students, int id_for_search)
{
    for (int found = 0; found < count_of_students; found ++)
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
    int res_surname = 0;
    int id_for_search = 0;
    char buffer[128]; //name surname etc
    scanf("%d", &action);
    if(check_action(action) == INVALID_INPUT)
    {
        printf("invalid input\n");
        return INVALID_INPUT;
    }  
    switch(action)
    {
        case 1:
            
            printf("Enter the id of student to search: ");
            scanf("%d", &id_for_search);
            res_id = find_student_by_id(students, count_of_students, id_for_search);
            print_student(students, res_id);
            break;

        case 2:
            printf("Enter the surname of student to search\n");
            scanf("%s", buffer);
            res_surname = find_student_by_surname(students, count_of_students, buffer);
            print_student(students, res_surname);
            break;
            
    }
    free(students);
    return 0;
}
