#ifndef _EMPLOYEE_H_
#define _EMPLOYEE_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <limits.h>
#include <stdio_ext.h> // __fpurge(stdin);

#define MAX_EMPLOYEES 50

struct Date {
    int day;
    int month;
    int year;
};

struct Employee {
    int id;
    char* full_name;
    char* department;
    double salary;
    struct Date start_date;
    struct Employee* next;
};

typedef enum {
    TMP,
    ADD_EMPLOYEE,
    REMOVE_BY_ID,
    REMOVE_BY_FULL_NAME,
    SHOW_EMPLOYEE,
    SORT_OPTION,
    EXIT_PROGRAM
}option_t;

typedef enum {
    TEMP,
    SORT_BY_SALARY_INCREASING,
    SORT_BY_SALARY_DERCEASING,
    SORT_BY_ALPHABET_FULL_NAME,
    EXIT_SORT
}option_sort;

typedef struct Date Date;
typedef struct Employee Employee;

void input_number_of_employee( int *number);

Employee* init_employee(int number);

Employee* create_employee(Employee *emp, int number);

void insert_employee(Employee** head, Employee* new_employee);

Employee* input_employee_infomation_list(int number, Employee** head);

void print_employee_table(Employee* head);

void free_employee_list(Employee* head);

void add_employee(Employee** employees);

void remove_employee_by_id(struct Employee** head);

void remove_employee_by_full_name(Employee** head);

double validate_num_input(bool flag);
void merge_sort(Employee** head, int (*compare)(const void*, const void*));

int compare_increase_salary(const void* left, const void* right);

int compare_decrease_salary(const void* left, const void* right);

int compare_by_name(const void* left, const void* right);

#endif