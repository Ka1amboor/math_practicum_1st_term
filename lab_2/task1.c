#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

enum status_code
{
    SUCCESS = 0,
    INVALID_INPUT = 1,
    MEMORY_ERROR = 2
};

enum status_code valid_flag(char flag[])
{
    int flags_count = 5;
    char flags[5] = {'l', 'r', 'u', 'n', 'c'};

    if (flag[0] == '-')
    {
        for( int i = 0; i < flags_count; i++)
        {
            if(flag[1] == flags[i])
            {
                return SUCCESS;
            }
        }
    }
    return INVALID_INPUT;
}

enum status_code my_strlen(int* res, char str[])
{
    for(int i = 0; str[i] != '\0'; i++)
    {
        (*res)++;
    }
    return SUCCESS;
}

enum status_code reverse_string(int length, char* str, char** res)
{
    *res = (char*)malloc(sizeof(char) * (length + 1));
    if (*res == NULL)
    {
        return MEMORY_ERROR;
    }
    (*res)[length] = '\0';

    length--;
    int index = 0;
    while (length >= 0)
    {
        (*res)[index] = str[length];
        index++;
        length--;
    }
    return SUCCESS;

}

enum status_code caps(char* str, char** res)
{   
    *res = (char*)malloc(sizeof(str));
    if (*res == NULL)
    {
        return MEMORY_ERROR;
    }

    int index = 0;
    while (str[index] != '\0')
    {
        if (!(isalpha(str[index])))
        {
            return INVALID_INPUT;
        }
        if (index & 1)
        {
            (*res)[index] = toupper(str[index]);
            
        }
        else
        {
            (*res)[index] = str[index];
        }
        index++;
        
    }
    (*res)[index] = '\0';
    return SUCCESS;
}

enum status_code sorting(int len, char** res, char* str)
{   
    *res = (char*)malloc(sizeof(str));
    if (*res == NULL)
    {
        return MEMORY_ERROR;
    }
    
    int digit_count = 0;
    int letter_count = 0;
    int etc_count = 0;

    for (int i = 0; i < len; i++)
    {
        char sym = str[i];
        if (isdigit(sym))
        {
            digit_count++;
        }
        else if (isalpha(sym))
        {
            letter_count++;
        }
        else
        {
            etc_count++;
        }
    }

    int first_pos = 0;
    int second_pos = digit_count;
    int third_pos = digit_count + letter_count;

    for (int i = 0; i < len; i++)
    {
        char s = str[i];
        if (isdigit(s))
        {
            (*res)[first_pos] = s;
            first_pos++;
        }
        else if(isalpha(s))
        {
            (*res)[second_pos] = s;
            second_pos++;
        }
        else
        {
            (*res)[third_pos] = s;
            third_pos++;
        }
    }
    (*res)[len] = '\0';
    return SUCCESS;

    
}

int generate_order(int* arr, int seed, int size) {

    if (size < 1) {
        return INVALID_INPUT;
    }

    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }

    srand(seed);

    // Fischer-Yates shuffle
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }

    return SUCCESS;
}

void concatination_string(int* last, char st[], char ch[]) {
    for (int i = 0; st[i] != '\0'; i++) {
        ch[(*last)++] = st[i]; 
    }
}




int main(int argc, char*argv[])
{
    if (argc < 3)
{
    printf("Not enough arguments\n");
    return INVALID_INPUT;
    
}
    enum status_code valid_flag_res = valid_flag(argv[1]);
    if (valid_flag_res == INVALID_INPUT)
    {
        printf("Invalid flag");
        return INVALID_INPUT;
    }

    char flag = argv[1][1];
    
    switch(flag)
    {
        case 'l':
        {
            int result = 0;
            enum status_code res_my_strlen =  my_strlen(&result, argv[2]);
               if (res_my_strlen == SUCCESS)
            {
                printf("The length of string: %d\n", result);
                break;
                
            }
                
            
        }
        case 'r':
        {
            int length = 0;
            my_strlen(&length, argv[2]);
        
            char* reverse = argv[2];
            char* res;

            enum status_code res_reverse_string = reverse_string(length, reverse,&res);
            if (res_reverse_string == SUCCESS)
            {
                printf("The result of reverse: %s\n", res);
                free(res);
                break;
            }
            else
            {
                printf("failed to allocate memory");
                free(res);
                return MEMORY_ERROR;
            }
        }

        case 'u':
        {
            char* arr_caps = argv[2];
            char* res_str;
            enum status_code res_caps = caps(arr_caps, &res_str);
            if (res_caps == SUCCESS)
            {
                printf("The result of caps: %s\n", res_str);
                free(res_str);
                break;
            }
            else if (res_caps == INVALID_INPUT)
            {
                printf("invalid input!\n");
                free(res_str);
                break;
            }
            else if (res_caps == MEMORY_ERROR)
            {
                printf("failed to allocate memory");
                free(res_str);
                break;
            }

        }

        case 'n': 

        {   
            char* res_n;
            int lngth = 0;
            my_strlen(&lngth, argv[2]);
            char res[lngth];
            enum status_code res_sorting = sorting(lngth, &res_n, argv[2]);
            if (res_sorting == SUCCESS)
            {
                printf("The result of flag -n: %s\n", res_n);
                free(res_n);
                break;
            }
            else if (res_sorting == MEMORY_ERROR)
            {
                printf("failed to allocate memory");
                free(res_n);
                break;
            }
        }

        

        case 'c': // <-c> <seed> <count of string> <strings>

        {
            
            int count = atoi(argv[3]); // count of strings for concat
            if (argc != count + 4)
            {
                printf("invalid input\n");
                return INVALID_INPUT;
            }

            int seed = atoi(argv[2]);


            int* arr_generate_order = (int*)malloc(sizeof(int) * count);
            if (arr_generate_order == NULL)
            {
                printf("failed to allocate memory\n");
                return MEMORY_ERROR;
            }
            //generated order of strings
            generate_order(arr_generate_order, seed, count);

            //len of all strings sum of strings

            int all_len = 0;
            for (int i = 0; i < count; i++)
            {
                int le = 0;
                enum status_code sc = my_strlen(&le, argv[i+4]);
                if (sc == SUCCESS)
                {
                    all_len += 1;
                }
               
            }

            char* arr_concat = (char*)malloc(sizeof(char) * all_len);
            if (arr_concat == NULL)
            {
                printf("failed to aloocate memory\n");
                return MEMORY_ERROR;
            }

            int last = 0;
            for (int i = 0; i < count; i++)
            {
                concatination_string(&last, argv[arr_generate_order[i] + 4], arr_concat);

            }

            printf("Result of concatination %s\n", arr_concat);

            free(arr_generate_order);
            free(arr_concat);
            break;

        }
        
        default:
            printf("invalid flag\n");
            break;
    }

    



return SUCCESS;
}
