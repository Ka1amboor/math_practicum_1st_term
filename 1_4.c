#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum status_code
{
    SUCCESS = 0,
    INVALID_INPUT = 1,
    MEMORY_ERROR = 2,
    FILES_ISSUES = 3
};

enum status_code delete_arabic_numbers(char* input_file, char* output_file)
{
    FILE* file_in = fopen(input_file, "r");
    
    if (!file_in)
    {
        return FILES_ISSUES;
    }

    FILE* file_out = fopen(output_file, "w");
    
    if (!file_out)
    {
        fclose(file_in);
        return FILES_ISSUES;
    }

    int c;

    while ((c = fgetc(file_in)) != EOF)
    {
        if(!isdigit(c))
        {
            fputc(c, file_out);
        }
    }

    fclose(file_in);
    fclose(file_out);

    return SUCCESS;
}

enum status_code count_of_letter(char* input_file, char* output_file)
{
    FILE* file_in = fopen(input_file, "r");
    
    if (!file_in)
    {
        return FILES_ISSUES;
    }

    FILE* file_out = fopen(output_file, "w");
    
    if (!file_out)
    {
        fclose(file_in);
        return FILES_ISSUES;
    }

    int c;
    int count_str = 1, count_letter = 0;

    while(!feof(file_in))
    {
        c = fgetc(file_in);
        if (c == EOF)
        {
            fprintf(file_out, "%d %d\n", count_str, count_letter);
        }

        if (c == '\n')
        {
            fprintf(file_out, "%d %d\n", count_str, count_letter);
            count_str++;
            count_letter = 0;
        }

        else if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        {
            count_letter++;
        }
    }

    fclose(file_in);
    fclose(file_out);
    return SUCCESS;

}

enum status_code count_sym(char* input_file, char* output_file)
{
    FILE* file_in = fopen(input_file, "r");

    if (!file_in)
    {
        return FILES_ISSUES;
    }

    FILE* file_out = fopen(output_file, "w");
    
    if (!file_out)
    {
        fclose(file_in);
        return FILES_ISSUES;
    }

    int c;
    int count_sym = 0;
    int count_str = 1;

    while(!feof(file_in))
    {
        c = fgetc(file_in);

        if (c == EOF)
        {
            fprintf(file_out, "%d %d\n", count_str, count_sym);
        }

        if (isalnum(c) || c == ' ')
        {
            continue;
        }

        else if (c == '\n')
        {
            fprintf(file_out, "%d %d\n", count_str, count_sym);
            count_str++;
            count_sym = 0;
        }

        else
        {
            count_sym++;
        }
    }

    fclose(file_in);
    fclose(file_out);
    return SUCCESS;
}

enum status_code replace_sym(char* input_file, char* output_file)
{
    FILE* file_in = fopen(input_file, "r");

    if(!file_in)
    {
        return FILES_ISSUES;
    }

    FILE* file_out = fopen(output_file, "w");
    
    if (!file_out)
    {
        fclose(file_in);
        return FILES_ISSUES;
    }

    int c;
    
    while((c = fgetc(file_in)) != EOF)
    {
        if ((c >= '0' && c <= '9' )|| c == '\n')
        {
            fputc(c, file_out);
        }
        else
        {
            fprintf(file_out, "%X", c);
        }
    }

    fclose(file_in);
    fclose(file_out);

    return SUCCESS;

}

int my_strcmp(char* str_1, char* str_2)
{
    int i = 0, flag = 0;

    while (flag == 0)
    {
        if (str_1[i] > str_2[i])
        {
            flag = 1;
        }

        else if(str_1[i] < str_2[i])
        {
            flag = -1;
        }

        else
        {
            i++;
        }
    }

    return flag;
}

enum status_code check_input(int argc, char* argv[])
{
    if (argc < 3 || argc > 4)
    {
        return INVALID_INPUT;
    }

    if(argv[1][0] != '-' && argv[1][0] != '/')
    {
        return INVALID_INPUT;
    }

    if (argv[1][1] == 'n' && argc != 4 && strlen(argv[1]) != 3)
    {
        return INVALID_INPUT;
    }

    if (argv[1][1] == 'n')
    {
        if(my_strcmp(argv[2], argv[3]) == 0)
        {
            return INVALID_INPUT;
        }
    }

    return SUCCESS;
}

int main(int argc, char* argv[])
{
    if (check_input(argc, argv) == INVALID_INPUT)
    {
        printf("invalid input\n");
        return INVALID_INPUT;
    }

    char flag;
    
    char* input_file = argv[2];
    char* output_file = NULL;

    if (argv[1][1] == 'n')
    {
        flag = argv[1][2];
        output_file = argv[3];
    }
    else
    {
        flag = argv[1][1];
        output_file = (char*)malloc(strlen(argv[2]) + 5);
        if (!output_file)
        {
            printf("failed to allocate memory");
            return MEMORY_ERROR;
        }

        strcpy(output_file, "out_"); //copy "out_"
        strcat(output_file, argv[2]); //add 

    }
    
    switch (flag)
    {
        case 'd':
            if((delete_arabic_numbers(input_file, output_file)) == FILES_ISSUES)
            {
                printf("error with opening files");
                return FILES_ISSUES;
            }
            break;

        case 'i':

            if (count_of_letter(input_file, output_file) == FILES_ISSUES)
            {
                printf("error with opening files");
                return FILES_ISSUES;
            }
            break;

        case 's':
            if(count_sym(input_file, output_file) == FILES_ISSUES)
            {
                printf("error with opening files");
                return FILES_ISSUES;
            }
            break;

        case 'a':
            if(replace_sym(input_file, output_file) == FILES_ISSUES)
            {
                printf("error with opening files");
                return FILES_ISSUES;
            }
            break;

        default:
            printf("invalid flag\n");
            break;
        
    }
    free(output_file);

    return 0;
}
