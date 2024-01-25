#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum
{
    success,
    invalid_input,
    memory_error,
    error_with_opening_file

}status_code;

char* create_error_file(char* filename)
{
    char* output_file = NULL;
    output_file = (char*)malloc(strlen(filename) + 8); //errors_
    if(!output_file)
    {
        return NULL;
    }
    strcpy(output_file, "errors_");
    strcat(output_file, filename);

    return output_file;
}

status_code proccess_bracket_epression(FILE* input_file, FILE* output_file)
{
    char sym;
    char string[256];
    int flag = 1;
    int line = 0;
    int index = 0;
    int brackets_counter = 0;

    while(flag)
    {
        int index = 0;
        int brackets_counter = 0;
        //check brackets balance
        while((sym = getc(input_file)) != EOF && sym != '\n')
        {
            string[index] = sym;
            index++;

            if(sym == '(')
            {
                brackets_counter++;
            }
            else if(sym == ')')
            {
                brackets_counter--;
            }
        }
        string[index] = '\0';
        printf("Expression: %s\n", string);

        if(brackets_counter != 0)
        {
            fprintf(output_file,"Expression: %s is not balanced with brackets in line: %d\n",string, line);
        }
        else
        {
            printf("The expression: %s is balanced!\n", string);
        }
        line++;
        if(sym == EOF)
        {
            flag = 0;
        }

    }

    return success;
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("invalid input\n");
        return invalid_input;
    }

    FILE* input_file = NULL;
    char* out = NULL;
    FILE* output_file = NULL;

    for(int i = 1; i < argc; i++)
    {
        input_file = fopen(argv[i], "r");
        if(!input_file)
        {
            printf("error with opening file\n");
            return error_with_opening_file;
        }

        printf("file: %s\n", argv[i]);
        
        out = create_error_file(argv[i]);
        output_file = fopen(out, "a");
        if(!output_file)
        {
            printf("error with opening output file\n");
            fclose(input_file);
            return error_with_opening_file;
        }

        status_code proccess_status = proccess_bracket_epression(input_file, output_file);
        switch(proccess_status)
        {
            case success:
                printf("everything ok\n");
                break;
        }

        fclose(input_file);
        fclose(output_file);
    }

    
    return 0;
}
