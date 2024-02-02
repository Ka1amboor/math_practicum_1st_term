#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum
{
    SUCCESS,
    INVALID_INPUT,
    MEMORY_ERROR,
    FILE_ERROR,
    WRONG_GRADE

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
    if(action < 0 ||  action > 10)
    {
        return INVALID_INPUT;
    }

    return SUCCESS;
}

status_code check_students_grades(Student* students, int count_of_students)
{
    for(int i = 0; i < count_of_students; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            if(students[i].grades[j] < 2 || students[i].grades[j] > 5)
            {
                return WRONG_GRADE;
            }
        }
    }

    return SUCCESS;
}
float calculate_average_grade(Student* student)
{
    float sum = 0;
    for(int i = 0; i < 5; i++)
    {
        sum += student->grades[i];
    }
    return sum / 5.0;
}
status_code load_average_all_to_file(Student* students, int count_of_students, char* file)
{
    FILE* file_out = fopen(file, "w");
    if(!file_out)
    {
        return FILE_ERROR;
    }
    float average_all = 0.0;
    for(int i = 0; i < count_of_students; i++)
    {
        float average_grade = calculate_average_grade(&students[i]);
        average_all += average_grade;
    }
    average_all = average_all / count_of_students;
    fprintf(file_out, "Average all: %f\n", average_all);
    for (int i = 0; i < count_of_students; i++)
    {
        float average_grade = calculate_average_grade(&students[i]);
        if (average_grade > average_all)
        {
            fprintf(file_out, "%s %s %f ", students[i].name, students[i].surname, average_grade);
        }
        printf("\n");
    }
    fclose(file_out);
    return SUCCESS;
}

void print_students(Student* students, int count_of_students)
{
    for(int i = 0; i < count_of_students; i++)
    {
        printf("ID: %d\n", students[i].id);
        printf("Name: %s\n", students[i].name);
        printf("Surname: %s\n", students[i].surname);
        printf("Group: %s\n", students[i].group);
        
    }
}
status_code print_student_in_file_by_id(Student* students, int found, char* file, float average_grade)
{
    FILE* file_out = fopen(file, "w");
    if(!file_out)
    {
        return FILE_ERROR;
    }
    if(found == -1)
    {
        fclose(file_out);
        return INVALID_INPUT;
    }
    else
    {
        printf("information about student successly loaded\n"); 
        fprintf(file_out,"%s\n%s\n%s\n%f\n", students[found].name, students[found].surname, students[found].group, average_grade);
    }
    fclose(file_out);
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
       printf("%d\n%s\n%s\n%s\n", students[found].id, students[found].name, students[found].surname, students[found].group);
       printf("Grades:\n");
       for(int i = 0; i < 5; i++)
       {    
            printf("%d\n", students[found].grades[i]);
       }
    }
    
}
int compare_students_by_group(const void* a, const void* b)
{
    Student* student_1 = (Student*)a;
    Student* student_2 = (Student*)b;

    return (strcmp(student_1->group, student_2->group));
}
int compare_students_by_id(const void* a, const void* b)
{
    Student* student_1 = (Student*)a;
    Student* student_2 = (Student*)b;

    return student_1->id - student_2->id;
}

int compare_students_by_surname(const void* a, const void* b)
{
    Student* student_1 = (Student*)a;
    Student* student_2 = (Student*)b;

    return (strcmp(student_1->surname, student_2->surname));
}
int compare_students_by_name(const void* a, const void* b)
{
    Student* student_1 = (Student*)a;
    Student* student_2 = (Student*)b;

    return (strcmp(student_1->name, student_2->name));
}
int find_student_by_name(Student* students, int count_of_students, char* buffer)
{
    for(int i = 0; i < count_of_students; i++)
    {
        if(strcmp(students[i].name, buffer) == 0)
        {
            return i;
        }
    }

    return -1;
}
int find_student_by_group(Student* students, int count_of_students, char* buffer)
{
    for(int i = 0; i < count_of_students; i++)
    {
        if(strcmp(students[i].group, buffer) == 0)
        {
            return i;
        }
    }

    return -1;
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
        Student* tmp; //no more leak
        tmp = realloc(*students, (*count_of_students) * sizeof(Student));
        if (tmp == NULL)
        {
            fclose(file);
            free(students);
            return MEMORY_ERROR;
        }
        *students = tmp;
        (*students)[*count_of_students - 1].grades = malloc(5 * sizeof(unsigned char));
        if (!(*students)[*count_of_students - 1].grades)
        {
            fclose(file);
            free(students);
            return MEMORY_ERROR;
        }

        if (sscanf(buffer, "%u %s %s %s %hhu %hhu %hhu %hhu %hhu", 
            &(*students)[*count_of_students - 1].id,
            (*students)[*count_of_students - 1].name, 
            (*students)[*count_of_students - 1].surname,
            (*students)[*count_of_students - 1].group, 
            &(*students)[*count_of_students - 1].grades[0],
            &(*students)[*count_of_students - 1].grades[1], 
            &(*students)[*count_of_students - 1].grades[2],
            &(*students)[*count_of_students - 1].grades[3], 
            &(*students)[*count_of_students - 1].grades[4]) != 9)
            {
                for(int i = 0; i < (*count_of_students); i++)
                {
                    free(students[i]->grades);
                }
                free(students);
                return INVALID_INPUT;
            }
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
    printf("*  Enter <5> to sort student by ID      *\n");
    printf("*  Enter <6> to sort student by surname *\n");
    printf("*  Enter <7> to sort student by name    *\n");
    printf("*  Enter <8> to sort student by group   *\n");
    printf("*  Enter <9> to load information about  *\n");
    printf("*  student by ID into output file with  *\n");
    printf("*         average grade                 *\n");
    printf("*  Enter <0> to load information about  *\n");
    printf("* the average grade of the best student* \n");
    printf("*   Enter <10> if you want to exit      *\n");
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
    if(read_status == FILE_ERROR)
    {
        printf("error with opening file\n");
        return FILE_ERROR;
    }
    if(read_status == INVALID_INPUT)
    {
        printf("invalid input\n");
        return INVALID_INPUT;
    }

    int action = 0;
    while(1)
    {   
        
        print_choice();
        if(check_students_grades(students, count_of_students) == WRONG_GRADE)
        {
            printf("grade must be from 2 to 5\n");
            return WRONG_GRADE;

        }
        int res_id = 0;
        int res_surname = 0;
        int res_name = 0;
        int id_for_search = 0;
        int res_group = 0;
        float average_grade = 0.0;
        int res_load = 0;
        char buffer[128]; //name surname etc
        if(scanf("%d", &action) != 1)
        {
            printf("invalid input\n");
            return INVALID_INPUT;
        }
        if(check_action(action) == INVALID_INPUT)
        {
            printf("invalid input\n");
            return INVALID_INPUT;
        }
        if(action == 10)
        {
            break;
        }
        switch(action)
        {
            case 1:
                
                printf("Enter the id of student to search: ");
                if(scanf("%d", &id_for_search)!= 1)
                {
                    printf("invalid input\n");
                    return INVALID_INPUT;
                }
                res_id = find_student_by_id(students, count_of_students, id_for_search);
                print_student(students, res_id);
                break;

            case 2:
                printf("Enter the surname of student to search\n");
                scanf("%s", buffer);
                res_surname = find_student_by_surname(students, count_of_students, buffer);
                print_student(students, res_surname);
                break;
            
            case 3:
                printf("Enter the name of student to search: ");
                scanf("%s", buffer);
                res_name = find_student_by_name(students, count_of_students, buffer);
                print_student(students, res_name);
                break;

            case 4:
                printf("Enter the number of group for search: ");
                scanf("%s", buffer);
                res_group = find_student_by_group(students, count_of_students, buffer);
                print_student(students, res_group);
                break;

            case 5:
                qsort(students, count_of_students, sizeof(Student), compare_students_by_id);
                printf("Sorted by ID\n");
                print_students(students, count_of_students);
                break;
            case 6:
                qsort(students, count_of_students, sizeof(Student), compare_students_by_surname);
                printf("Sorted by surname\n");
                print_students(students, count_of_students);
                break;
            
            case 7:
                qsort(students, count_of_students, sizeof(Student), compare_students_by_name);
                printf("Sorted by name\n");
                print_students(students, count_of_students);
                break;

            case 8:
                qsort(students, count_of_students, sizeof(Student), compare_students_by_group);
                printf("Sorted by group\n");
                print_students(students, count_of_students);
                break;
            case 9:
                printf("Enter the id of student to search: ");
                scanf("%d", &id_for_search);
                res_id = find_student_by_id(students, count_of_students, id_for_search);
                average_grade = calculate_average_grade(&students[res_id]);
                res_load = print_student_in_file_by_id(students, res_id, argv[2], average_grade);
                if(res_load ==  INVALID_INPUT)
                {
                    printf("There is no such student\n");
                    return INVALID_INPUT;
                }
                if(res_load == FILE_ERROR)
                {
                    printf("error with opening file\n");
                }
                break;

            case 0:
                if(load_average_all_to_file(students, count_of_students, argv[2]) == FILE_ERROR)
                {
                    printf("erroe with opening file\n");
                    return FILE_ERROR;
                }
                else
                {
                    printf("information successfully loaded!\n");
                }     
    }
    }
    free(students);
    return 0;
}
