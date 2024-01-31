#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

typedef enum
{
    success,
    memory_error,
    invalid_arguments,
    invalid_input,
    overflow

}status_code;

status_code Roman (const char* buf, int* num) 
{
    int res = 0;
    int i = 0;
    int cnt = 0;
    while (buf[i] == 'M') 
    {
        res += 1000;
        i++;
        cnt++;
    }
    if (cnt > 3) 
    {
        return invalid_arguments;
    }
    cnt = 0;

    if (buf[i] == 'C' && buf[i + 1] == 'M') 
    {
        res += 900;
        i += 2;
    }

    if (buf[i] == 'D') 
    {
        res += 500;
        i++;
    }

    if (buf[i] == 'C' && buf[i + 1] == 'D') 
    {
        res += 400;
        i += 2;
    }

    while (buf[i] == 'C') 
    {
        res += 100;
        i++;
        cnt++;
    }
    if (cnt > 3) 
    {
        return invalid_arguments;
    }
    cnt = 0;

    if (buf[i] == 'X' && buf[i + 1] == 'C') 
    {
        res += 90;
        i += 2;
    }

    if (buf[i] == 'L') 
    {
        res += 50;
        i++;
    }

    if (buf[i] == 'X' && buf[i + 1] == 'L') 
    {
        res += 40;
        i += 2;
    }

    while (buf[i] == 'X') 
    {
        res += 10;
        i++;
        cnt++;
    }
    if (cnt > 3) 
    {
        return invalid_arguments;
    }
    cnt = 0;

    if (buf[i] == 'I' && buf[i + 1] == 'X') 
    {
        res += 9;
        i += 2;
    }

    if (buf[i] == 'V') 
    {
        res += 5;
        i++;
    }

    if (buf[i] == 'I' && buf[i + 1] == 'V') 
    {
        res += 4;
        i += 2;
    }

    while (buf[i] == 'I') 
    {
        res += 1;
        i++;
        cnt++;
    }
    if (cnt > 3) 
    {
        return invalid_arguments;
    }

    if (buf[i] != '\0') 
    {
        return invalid_arguments;
    } 

    *num = res;

    return success;
}


int check_number(char* number)
{
    char* ptr = NULL;
    if (*number == '-') 
    {
        ptr = number + 1;
    }
    else 
    {
        ptr = number;
    }
    while (*ptr) 
    {
        if (isalnum(*ptr)) 
        {
           if(isalpha(*ptr))
           {
                if((*ptr < 'a' || *ptr > 'z'))
                {
                    return -1;
                }
           }

        }
        else  
        {
            return -1;
        }
        ptr++;
    }

    return 1;
}

status_code Cv(int* cv_res, char* number, int base) 
{
    if(!number)
    {
        return invalid_arguments;
    }
    if (base < 2 || base > 36) 
    {
        base = 10;
    }
    if(check_number(number) == -1)
    {
        return invalid_arguments;
    }
    //////////convert

    int sign = 1;
    char* ptr = NULL;
    int result = 0;

    if(*number == '-')
    {
        sign = -1;
        ptr = number + 1;
    }
    else
    {
        ptr = number;
    }

    while(*ptr)
    {
        result = result * base + (isdigit(*ptr) ? - '0' : *ptr - 'a' + 10);
        ptr++;
    }

    
    *cv_res = sign * result;
    return success;
}


status_code Zeckendorf(char* num, unsigned int* res) 
{
    int len = strlen(num);
    unsigned int cur = 0;
    unsigned int prev = 0;
    unsigned int next = 0;
    *res = 0;

    if (num[len - 1] != '1') 
    {
        return invalid_arguments;
    }

    if (len == 0) 
    {
        return invalid_arguments;
    } 
    else if (len == 1) 
    {
        *res = 0;
        return success;
    } 
    else 
    {
        prev = 0; 
        next = 1;
    }

    len--;
    for (int i = 0; i < len; i++) 
    {
        if (num[i] != '0' && num[i] != '1') 
        {
            return invalid_arguments;
        }

        if (num[i] == '1') 
        {
            (*res) += prev + next;
        }

        cur = prev;
        prev = next;
        next += cur;
    }
    
    return success;
}


int oversscanf(char* stream, const char* format, ...) 
{
    int record = 0;
    int idx_format = 0;
    int idx_buf = 0;
    int rec = 0;
    int idx_stream = 0; 
    int size = 8;
    int* num;
    int* res_Cv;
    int base = 0;
    unsigned int* res_zeckendorf = 0;
    void* std;
    char flag[10];
    int idx_flag = 0;

    char* buffer = (char*)malloc(sizeof(char) * size);
    if(!buffer)
    {
        return -1;
    }

    va_list args;
    va_start(args, format);

    while(format[idx_format] != '\0')
    {
        if(format[idx_format] == '%' && format[idx_format + 1] != '\0')
        {
            idx_buf = 0;
            buffer[0] = '\0';


        while(stream[idx_stream] != EOF && stream[idx_stream] != ' ' && stream[idx_stream] != '\n' && stream[idx_stream] != '\t')
        {
            if(idx_buf >= size - 1)
            {
                size *= 2;
                char* for_realloc = (char*)realloc(buffer, size);
                if(!for_realloc)
                {
                    free(buffer);
                    va_end(args);
                    return -1;
                }

                buffer = for_realloc;
            }

            buffer[idx_buf] = stream[idx_stream];
            idx_buf++;
            idx_stream++;
        }
        idx_stream++;
        buffer[idx_buf] = '\0';

        if(format[idx_format + 1] == 'R' && format[idx_format + 2] == 'o')
        {
            num = va_arg(args, int*);
            if(Roman(buffer, num) == invalid_arguments)
            {
                free(buffer);
                return -1;
            }

            record += 1;
            idx_format += 2;
        }
        else if(format[idx_format + 1] == 'Z' && format[idx_format + 2] == 'r')
        {
            res_zeckendorf = va_arg(args, unsigned int*);

            if(Zeckendorf(buffer, res_zeckendorf) == invalid_arguments)
            {
                free(buffer);
                return -1;
            }
            record += 1;
            idx_format += 2;
        }
        else if(format[idx_format + 1] == 'C' && format[idx_format + 2] == 'v')
        {
            res_Cv = va_arg(args, int*);
            base = va_arg(args, int);
            if(Cv(res_Cv, buffer, base) != success)
            {
                free(buffer);
                return -1;
            }

            record += 1;
            idx_format += 2;
            
        }
        else
        {
            std = va_arg(args, void*);
            flag[0] = '%';
            idx_flag = 1;

            while(format[idx_format + 1] != '%' && format[idx_format + 1] != '\0')
            {
                flag[idx_flag] = format[idx_format + 1];
                idx_format++;
                idx_flag++;
            }
            flag[idx_flag] = '\0';

            rec = sscanf(buffer, flag, std);
            if(rec == -1)
            {
                return -1;
            }

            record += rec;

        }
        }
        else if( format[idx_format] == '%')
        {
            free(buffer);
            return -1;
        }
        idx_format++;
    }

    va_end(args);
    free(buffer);
    return record;
}

int main()
{
   int roman = 0;
   int num = 0;
   int cv;
   int base = 16;
   char stream[] = "XXVII 52 1001011 -a"; //19 101001->1001011
   unsigned int zeckendorf;
   if(oversscanf(stream, "%Ro%d%Zr%Cv", &roman, &num, &zeckendorf, &cv, base) == -1)
   {
        printf("error\n");
        return invalid_input;
   }

   printf("%d\n%d\n%d\n%d\n", roman, num, zeckendorf, cv);

    return 0;
}
