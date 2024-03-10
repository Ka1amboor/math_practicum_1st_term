//
// Created by 79257 on 06.03.2024.
//

#ifndef INC_4_9_STRUCTURES_H
#define INC_4_9_STRUCTURES_H


typedef enum
{
    SUCCESS,
    MEMORY_ERROR,
    OPENING_FILE_ERROR,
    INVALID_INPUT,
    INVALID_STRUCTURE

}status_code;



typedef struct My_time
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

}My_time;

typedef enum Structure_for_application
{
    BINARY_HEAP,
    BINOMIAL_HEAP,
    FIBONACCI_HEAP,
    SKEW_HEAP,
    LEFTIST_HEAP,
    TREAP

}Structure_for_application;

typedef enum Second_structure
{
    HASH_SET,
    DYNAMIC_ARRAY,
    BINARY_SEARCH_TREE,
    TRIE

}Second_structure;

typedef struct Application
{
    char time_of_receipt[20]; //время поступления
    int priority_of_app; //приоритет
    char id_of_department[20]; //идентификатор отделения
    char* message; //текст заявки

}Application;

void free_application(Application* application)
{
    if(!application)
    {
        return;
    }
    free(application->message);
    free(application);
    application = NULL;
}

typedef struct Operator
{
    int proccess_time; //время обработки заявки
    char name_operator[20];// имя оператора
    int start_time;// время в которое он взял крайнюю заявку
    Application* application;// текущая заявка, которая находится в обработке

}Operator;

typedef struct Department //отделение
{
    int count_operators; //количество операторов
    int count_free_applications; //количество свободных заявок, которые можно взять оператору (они не в обработке)
    int count_real_applications; //количество оставшихся заявок (свободны + которые сейчас в обработке)
    void* heap;                     //хранеие заявок
    Operator* operators;
    int is_overload; //перегружено ли отделение
    double coefficient_load; //коэффициент загруженности

}Department;

#endif //INC_4_9_STRUCTURES_H
