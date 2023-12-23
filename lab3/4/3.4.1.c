#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    SUCCESS,
    MEMORY_ERROR
} status_code;

typedef struct
{
    char* str;
    int length;
} String;

String create_string(const char* str)
{
    String created_string;

    if (str == NULL)
    {
        created_string.length = 0;
        created_string.str = NULL;
        return created_string;
    }

    int len = strlen(str);
    created_string.str = (char*)malloc(sizeof(char) * (len + 1));

    if (!created_string.str)
    {
        created_string.length = 0;
        return created_string;
    }

    strcpy(created_string.str, str);
    created_string.length = len;
    return created_string;
}

void print_string(const String string)
{
    if (string.str == NULL || string.length == 0) 
    {
        printf("This string doesn't exist\n");
    }
    else
    {
        printf("%s\n", string.str);
    }
}

void delete_string(String* string)
{
    if (string != NULL && string->str != NULL)
    {
        free(string->str);
        string->str = NULL;
        string->length = 0;
    }
}

int compare_strings(const String string_1, const String string_2)
{
    if (string_1.length > string_2.length)
    {
        return 1;
    }
    else if (string_1.length < string_2.length)
    {
        return -1;
    }
    else
    {
        return strcmp(string_1.str, string_2.str);
    }
}

int is_equal(const String string_1, const String string_2)
{
    if (string_1.str == NULL || string_2.str == NULL) 
    {
        return -2;
    }

    return strcmp(string_1.str, string_2.str);
}

status_code copy_to_exist(String* exist, const String source)
{
    free(exist->str);
    exist->str = (char*)malloc(sizeof(char) * (source.length + 1));

    if(!exist->str)
    {
        return MEMORY_ERROR;
    }

    strcpy(exist->str, source.str);
    return SUCCESS;
}

String copy_to_new(const String source)
{
    String new_string;
    new_string.str = (char*)malloc(sizeof(char) * (source.length + 1));
    
    if (!new_string.str)
    {
        new_string.length = 0;
        return new_string;
    }

    strcpy(new_string.str, source.str);
    new_string.length = source.length;

    return new_string;
}

status_code concatination(String string_1, const String string_2) 
{
    int length = string_1.length + string_2.length;

    char* new_string = (char*)malloc((length + 1) * sizeof(char));

    if (!new_string)
    {
        return MEMORY_ERROR;
    }

    strcpy(new_string, string_1.str);
    strcat(new_string, string_2.str);
    
    free(string_1.str);
    
    string_1.length = length;
    string_1.str = new_string;
    
    return SUCCESS;
}

int main()
{
    const char* string_1 = "mama";
    const char* string_2 = "papa";
    const char* string_3 = "mama";
    const char* string_5 = "sister";

    String res_string_1 = create_string(string_1);
    String res_string_2 = create_string(string_2);
    String res_string_3 = create_string(string_3);
    String res_string_5 = create_string(string_5);

    print_string(res_string_2); //papa
    delete_string(&res_string_2);
    
    print_string(res_string_2); //This string doesn't exist
    printf("Result of compare strings: %d\n", compare_strings(res_string_1, res_string_3)); //0

    String res_string_4 = copy_to_new(res_string_1);
    
    print_string(res_string_4);//mama

    copy_to_exist(&res_string_1, res_string_5);
    
    print_string(res_string_1);//sister
    
    if(concatination(res_string_3, res_string_5) == SUCCESS)
     {
        printf("Result of concatination: %s", res_string_3.str);
     }

     return 0;
}
