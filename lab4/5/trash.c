#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    success,
    memory_error,
    error_with_opening_file,
    error_with_opening_output_file,
    invalid_input

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

status_code proccess_expression(int argc, char* argv[])
{
    FILE* file = NULL;
    char* output_file = NULL;
    FILE* out = NULL;
    char buffer[100];
    char sym;
    int index = 0;
    int line = 0;
    int brackets_counter = 0;

    for(int i = 1; i < argc; i++)
    {
        file = fopen(argv[i], "r");
        if(!file)
        {
            return error_with_opening_file;
        }

        //check brackets balance
        while((sym = getc(file)) != EOF && sym != '\n')
        {
            buffer[index] = sym;
            index++;

            if(sym == '(')
            {
                brackets_counter++;
            }

            else if(sym == ')')
            {
                brackets_counter--;
            }
            else if(sym == '\n')
            {
                line++;
            }
        }
        buffer[index] = '\0';
        printf("expression: %s\n", buffer);

        if(brackets_counter != 0)
        {
            printf("brackets are not balanced\n");
            output_file = create_error_file(argv[i]);
            out = fopen(output_file, "a");
            if(!out)
            {   
                fclose(file);
                return error_with_opening_output_file;
            }
            fprintf(out, "In expression %s in line %d detected wrong bracket balance\n",buffer, line);
        }

        index = 0;
        brackets_counter = 0;
    }

    fclose(file);
    return success;
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("Invalid input!\nPlease enter <file_1.txt ... file_n.txt>\n");
        return invalid_input;
    }

    status_code result = proccess_expression(argc, argv);

    switch(result)
    {
        case error_with_opening_file:
            printf("Error with opening file\n");
            return error_with_opening_file;

        case error_with_opening_output_file:
            printf("Error with opening output file\n");
            return error_with_opening_output_file;

        default:
            break;
    }

    return 0;
}
