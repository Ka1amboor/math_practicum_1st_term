#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

typedef enum 
{
    SUCCESS,
    MEMORY_ERROR,
    INVALID,
    INVALID_INPUT,
    ERROR_WITH_OPENING_FILE
    
}Status_code;

typedef enum
{
    OK,
    NOT_OK

}for_table;

typedef struct Macros_item 
{
    char* key; //PI
    char* value; //3.14
    unsigned long hash;
    struct Macros_item* next;

} Macros_item;

typedef struct Hash_table 
{
    Macros_item** items;
    int size;

}Hash_table;

unsigned long hash(char *key) 
{
    unsigned long hash = 0;
    int length = strlen(key);

    for(int i = 0; i < length; i++)
    {
        if(key[i] >= '0' && key[i] <= '9')
        {
            hash = hash * 62 + key[i] - '0';
        }
        else if (key[i] >= 'A' && key[i] <= 'Z')
        {
           hash = hash * 62 + key[i] - 'A' + 10;  
        }
        else if(key[i] >= 'a' && key[i] <= 'z')
        {
            hash = hash * 62 + key[i] - 'a' + 36;
        }
    }
    return hash;
}
for_table check(Hash_table* hash_table, int* count)
{
    int min = INT_MAX;
    int max = INT_MIN;
    for (int i = 0; i < hash_table->size; i++)
    {
        int m = 0;
        Macros_item* cur = hash_table->items[i];
        while (cur != NULL)
        {
            m++;
            (*count)++;
            cur = cur->next;
        }
        if (m > max)
        {
            max = m;
        }
        if (m < min)
        {
            min = m;
        }
    }
    if (min == 0) 
    {
        if (max >= 2)
        {
            return NOT_OK;
        }
        else return OK; 
    }
    if (max / min >= 2) 
    {
        return NOT_OK;
    }
    return OK;
}
Status_code create_hash_table(Hash_table** hash_table, int size) 
{
	*hash_table = (Hash_table*)malloc(sizeof(Hash_table));

	if (!*hash_table) 
    {
		return MEMORY_ERROR;
	}

	(*hash_table)->size = size;
	(*hash_table)->items = (Macros_item**)malloc(sizeof(Macros_item*) * (*hash_table)->size);

	if (!(*hash_table)->items) 
    {
		free((*hash_table)->items);
		(*hash_table)->items = NULL;
		return MEMORY_ERROR;
	}

	for (int i = 0; i < (*hash_table)->size; i++) 
    {
		(*hash_table)->items[i] = NULL;
	}

	return SUCCESS;
}
Status_code insert_macros(Hash_table* hash_table, char *key, char *value) 
{

    unsigned long hash_value = hash(key);
    unsigned long index = hash_value % hash_table->size;
    Macros_item* new_macros = (Macros_item*)malloc(sizeof(Macros_item));
    if (!new_macros)
    {
        return MEMORY_ERROR;
    }
    new_macros->hash = hash_value;
    new_macros->key = strdup(key);
    new_macros->value = strdup(value);
    new_macros->next = hash_table->items[index];
    hash_table->items[index] = new_macros;
    return SUCCESS;
}

char* search_macros(Hash_table *hash_table, char *key) 
{
    unsigned long hash_value = hash(key);
    unsigned long index = hash_value % hash_table->size;
    Macros_item *macros = hash_table->items[index];
    while(macros != NULL) 
    {
        if(strcmp(macros->key, key) == 0) 
        {
            return macros->value;
        }
        macros = macros->next;
    }
    return NULL;
}

Status_code read_define(FILE* file, Hash_table* hash_table)
{
    char line[100];
    char define[8];
    char define_name[100];
    char define_value[100];
    fscanf(file, "%s", define);
    while (strcmp(define, "#define") == 0)
    {
        fscanf(file, "%s %s\n", define_name, define_value);
        if (insert_macros(hash_table, define_name, define_value) != SUCCESS)
        {
            return MEMORY_ERROR;
        }
        fscanf(file, "%s", define);
    }
    fseek(file, ftell(file) - strlen(define), SEEK_SET);
    return SUCCESS;
}


Status_code replace(Hash_table* hash_table, FILE* file)
{
    int start_index = ftell(file);
    char str[256];
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file) - start_index + 1;
    fseek(file, start_index, SEEK_SET);

    char *buffer = (char *)malloc(fileSize);
    if (buffer == NULL) {
        return MEMORY_ERROR;
    }
    int index_buf = 0;
    char c = fgetc(file);
    int flag = 0;
    while(c != EOF && !isalnum(c))
    {
        if (index_buf == fileSize)
        {
            char* temp = realloc(buffer, fileSize * 2);
            if (!temp)
            {
                return MEMORY_ERROR;
            }    
            buffer = temp;
            fileSize *= 2;
        }
        buffer[index_buf] = c;
        index_buf++;
        c = fgetc(file);
    }
    if (c == EOF)
    {
        flag = 1;
    }
    fseek(file, -1, SEEK_CUR);
    while (!flag && fscanf(file, "%s", str) == 1)
    {
        if (index_buf == fileSize)
        {
            char* temp = realloc(buffer, fileSize * 2);
            if (!temp)
            {
                return MEMORY_ERROR;
            }    
            buffer = temp;
            fileSize *= 2;
        }
        char *value = search_macros(hash_table, str);
        if (value != NULL)
        {
            if (index_buf + strlen(value) >= fileSize)
            {
                char *temp = realloc(buffer, fileSize * 2);
                if (!temp)
                {
                    return MEMORY_ERROR;
                }
                buffer = temp;
                fileSize *= 2;
            }
            for (int j = 0; j < strlen(value); j++)
            {
                buffer[index_buf] = value[j];
                index_buf++;
            }
        }
        else
        {
            if (index_buf + strlen(str) >= fileSize)
            {
                char *temp = realloc(buffer, fileSize * 2);
                if (!temp)
                {
                    return MEMORY_ERROR;
                }
                buffer = temp;
                fileSize *= 2;
            }
            index_buf += sprintf(&(buffer[index_buf]), "%s", str);
        }
        c = fgetc(file);
        while (c != EOF && !isalnum(c))
        {
            if (index_buf == fileSize)
            {
                char *temp = realloc(buffer, fileSize * 2);
                if (!temp)
                {
                    return MEMORY_ERROR;
                }
                buffer = temp;
                fileSize *= 2;
            }
            buffer[index_buf] = c;
            index_buf++;
            c = fgetc(file);
        }
        if (c == EOF)
        {
            flag = 1;
        }
        fseek(file, -1, SEEK_CUR);
    }
    printf("%s\n", buffer);
    fseek(file, start_index, SEEK_SET);
    for (int i = 0; i < index_buf; i++)
    {
        fprintf(file, "%c", buffer[i]);
    }
    while( ( c = fgetc(file)) != EOF )
    {
        fprintf(file, "%c", ' ');
    } 
    free(buffer);
    return SUCCESS; 
}
void print_hash_table(Hash_table* hash_table)
{
    for (int i = 0; i < hash_table->size; i++)
    {
        if (hash_table->items[i] != NULL)
        {
            Macros_item* current = hash_table->items[i];
            while (current != NULL)
            {
                printf("%s ", current->value);
                current = current->next;
            }
            printf("\n");
        }
    }
}
void destroy_table(Hash_table* hash_table)
{
    for (int i = 0; i < hash_table->size; i++)
    {
        if (hash_table->items[i] != NULL)
        {
            Macros_item* current = hash_table->items[i];
            while(current != NULL)
            {
                Macros_item* previous = current;
                current = current->next;
                free(previous->key);
                free(previous->value);
                free(previous);
            }
            free(current);
        }
    }
    free(hash_table->items);
    free(hash_table);
}

Status_code change_size_of_table(Hash_table** table, int new_size)
{
    Hash_table* new_table;
    if (create_hash_table(&new_table, new_size) != SUCCESS) return MEMORY_ERROR;
    for (int i = 0; i < (*table)->size; i++)
    {
        if ((*table)->items[i] != NULL)
        {
            Macros_item* cur = (*table)->items[i];
            while (cur != NULL)
            {
                unsigned long hash = cur->hash;
                int new_index = hash % new_size;

                Macros_item* new_macros = (Macros_item*)malloc(sizeof(Macros_item));
                if (!new_macros)
                {
                    return MEMORY_ERROR;
                }
                new_macros->hash = hash;
                new_macros->key = strdup(cur->key);
                new_macros->value = strdup(cur->value);
                new_macros->next = new_table->items[new_index];
                new_table->items[new_index] = new_macros;
                cur = cur->next;
            }
        }
    }
    destroy_table(*table);
    *table = new_table;
    return SUCCESS;
}
int is_prime(int n) 
{
    if (n <= 1) 
    {
        return 0;
    }
    for (int i = 2; i * i <= n; i++) 
    {
        if (n % i == 0) 
        {
            return 0;
        }
    }
    return 1;
}
int main(int argc, char*argv[])
{
    if (argc < 2)
    {
        printf("invalid input! please enter <input_file>\n");
        INVALID_INPUT;
    }

    FILE* input_file;
    if (!(input_file = fopen(argv[1], "r+")))
    {
        printf("error with opening file\n");
        return ERROR_WITH_OPENING_FILE;
    }
    int size_of_hash_table = 128;
    Hash_table* my_table;
    create_hash_table(&my_table, size_of_hash_table);
    Macros_item** HashTable = my_table->items;
    if (read_define(input_file, my_table) != SUCCESS)
    {
        printf("failed to allocate memory\n");
        destroy_table(my_table);
        return MEMORY_ERROR;
    }

    int waiting_count = 0;
    while (check(my_table, &waiting_count) != OK)
    {
        int new_size = waiting_count * 2;
        while (!is_prime(new_size)) new_size++;
        printf("%d\n", new_size);

        if (change_size_of_table(&my_table, new_size) != SUCCESS)
        {
            printf("ERROR\n");
            destroy_table(my_table);
            return INVALID;
        }
    }
    

    if (replace(my_table, input_file) != SUCCESS)
    {
        printf("failed to allocate memory\n");
        destroy_table(my_table);
        return MEMORY_ERROR;
    }

    fclose(input_file);
    destroy_table(my_table);
    return 0;
}
