#include <stdio.h>
#include <stdlib.h.>
#include <string.h>

typedef enum
{
    SUCCESS,
    INVALID_INPUT,
    MEMORY_ERROR,
    FILE_ERROR

}status_code;


typedef struct
{
    unsigned int id;
    char name[256];
    char surname[256];
    double salary;

}Employee;

int compare_a(const void* a, const void* b)
{
    double epsilon = 0.0001;
    const Employee* worker_a = (const Employee*)a;
    const Employee* worker_b = (const Employee*)b;

    double salary_diff = worker_a->salary - worker_b->salary;

    if(salary_diff > epsilon)
    {
        return 1;
    }

    else if(salary_diff < epsilon)
    {
        return -1;
    }

    int surname_diff = strcmp(worker_a->surname, worker_b->surname);
    if(surname_diff != 0)
    {
        return surname_diff;
    }

    int name_diff = strcmp(worker_a->name, worker_b->name);
    if(name_diff != 0)
    {
        return name_diff;
    }

    int id_diff = worker_a->id - worker_b->id;
    if(id_diff > 0)
    {
        return 1;
    }

    else if(id_diff < 0)
    {
        return -1;
    }  
}

int compare_b(const void* a, const void* b)
{
    double epsilon = 0.0001;
    const Employee* worker_a = (const Employee*)a;
    const Employee* worker_b = (const Employee*)b;

    double salary_diff = worker_a->salary - worker_b->salary;

    if(salary_diff > epsilon)
    {
        return -1;
    }

    else if(salary_diff < epsilon)
    {
        return 1;
    }

    int surname_diff = strcmp(worker_a->surname, worker_b->surname);
    if(surname_diff != 0)
    {
        return -surname_diff;
    }

    int name_diff = strcmp(worker_a->name, worker_b->name);
    if(name_diff != 0)
    {
        return -name_diff;
    }

    int id_diff = worker_a->id - worker_b->id;
    if(id_diff > 0)
    {
        return -1;
    }

    else if(id_diff < 0)
    {
        return 1;
    }

}

int my_strlen(char* str)
{
    char* end_ptr = str;

    while(*end_ptr)
    {
        end_ptr++;
    }

    return end_ptr - str;
}

status_code check_input(int argc, char* argv[])
{
    if (argc != 4)
    {
        return INVALID_INPUT;
    }

    if ((argv[2][0]) != '-' && (argv[2][0]) != '/')
    {
        return INVALID_INPUT;
    }

    if ((argv[2][1]) != 'a' && (argv[2][1]) != 'd')
    {
        return INVALID_INPUT;
    }

    if(my_strlen(argv[2]) != 2)
    {
        return INVALID_INPUT;
    }

    return SUCCESS;
}

status_code read_from_file(const char* input_file, Employee** result, int* count_of_employees)
{
    FILE* file = fopen(input_file, "r");
    if(!file)
    {
        return FILE_ERROR;
    }

    *result = (Employee*)malloc(sizeof(Employee) * (*count_of_employees + 1));
    if(!(*result))
    {
        return MEMORY_ERROR;
    }

    char buffer[256];
    
    while(fgets(buffer, sizeof(buffer), file) != NULL)
    {
        Employee employee;

        sscanf(buffer,"%u %s %s %lf", &employee.id, employee.name, employee.surname, &employee.salary);
        *result = realloc(*result, (*count_of_employees + 1) * sizeof(Employee));

        if(!(*result))
        {
            fclose(file);
            free(result);
            return MEMORY_ERROR;
        }

        (*result)[*count_of_employees] = employee;
        (*count_of_employees)++;

    }

    fclose(file);
    return SUCCESS;
}
status_code write_to_file(const char* output_file, Employee* result, int count_of_employees)
{
    FILE* file = fopen(output_file, "w");
    if(!file)
    {
        return FILE_ERROR;
    }

    for(int i = 0; i < count_of_employees; i++)
    {
        fprintf(file, "%u %s %s %f\n", result[i].id, result[i].name, result[i].surname, result[i].salary);
    }

    fclose(file);
    return SUCCESS;
}



int main(int argc, char* argv[]) // argv[1] - input_file argv[2] - flag argv[3] - output_file
{

    if(check_input(argc, argv) == INVALID_INPUT)
    {
        printf("invalid input\n");
        return INVALID_INPUT;
    }

    Employee* employees = NULL;
    int count_of_employees = 0;
    status_code status_read;

    status_read = read_from_file(argv[1], &employees, &count_of_employees);

    switch(status_read)
    {
        case FILE_ERROR:
            printf("error with opening file\n");
            return FILE_ERROR;
            break;

        case MEMORY_ERROR:
            printf("failed to allocate memory\n");
            return MEMORY_ERROR;
            break;
    }

    char flag = argv[2][1];

    switch(flag)
    {
        case 'a':
           qsort(employees, count_of_employees, sizeof(Employee), compare_a);
           break;

        case 'd':
            qsort(employees, count_of_employees, sizeof(Employee), compare_b);
            break;

        default:
            printf("invalid value\n");
            break;
    }

    status_code status_write;

    status_write = write_to_file(argv[3], employees, count_of_employees);
    
    switch(status_write)
    {
        case FILE_ERROR:
            printf("error with opening file\n");
            free(employees);
            break;

        case SUCCESS:
            printf("have done!\n");
            free(employees);
            break;
    }

    return 0;
}
