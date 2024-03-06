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
    INVALID_INPUT

}status_code;



typedef struct Application
{
    char time_of_receipt[20]; //время поступления
    int priority_of_app; //приоритет
    char id_of_department[20]; //идентификатор отделения
    char* message; //текст заявки



}Application;

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
    void* heap;                     //
    Operator* operators;
    int is_overload;
    double coefficient_load;

}Department;

#endif //INC_4_9_STRUCTURES_H
