// main.c
#include "minixsql.h"
#include "utils.h"
#include <stdio.h>

#define TEST_TBL_EMPLOYEES "employees"
#define TEST_TBL_DEPARTMENTS "departments"
#define TEST_TBL_PROJECTS "projects"

int main() {
    Column cols_employees[] = {
        {"id", INT_32, 0},
        {"name", VARCHAR, 50},
        {"age", INT_32, 0},
        {"salary", DOUBLE, 0},
        {"hire_date", TIMESTAMP, 0}
    };
    CreateTable(TEST_TBL_EMPLOYEES, cols_employees, 5);

    Column cols_departments[] = {
        {"id", INT_32, 0},
        {"name", VARCHAR, 50},
        {"manager_id", INT_32, 0}
    };
    CreateTable(TEST_TBL_DEPARTMENTS, cols_departments, 3);

    Column cols_projects[] = {
        {"id", INT_32, 0},
        {"name", VARCHAR, 100},
        {"start_date", TIMESTAMP, 0},
        {"end_date", TIMESTAMP, 0},
        {"budget", DOUBLE, 0}
    };
    CreateTable(TEST_TBL_PROJECTS, cols_projects, 5);

    for (int i = 0; i < 100; i++) {
        int32_t id = i + 1;
        char name[51];
        snprintf(name, sizeof(name), "Employee %d", id);
        int32_t age = 20 + (rand() % 40);
        double salary = 30000 + (rand() % 70000);
        time_t hire_date = time(NULL) - (rand() % (3600 * 24 * 365 * 5));

        void* data[] = {&id, name, &age, &salary, &hire_date};
        InsertTuple(TEST_TBL_EMPLOYEES, data);
    }

    const char* dept_names[] = {"HR", "IT", "Finance", "Marketing", "Sales"};
    for (int i = 0; i < 5; i++) {
        int32_t id = i + 1;
        int32_t manager_id = 1 + (rand() % 100);
        void* data[] = {&id, (void*)dept_names[i], &manager_id};
        InsertTuple(TEST_TBL_DEPARTMENTS, data);
    }

    const char* project_names[] = {"Website Redesign", "Mobile App Development", "Cloud Migration", "AI Integration", "Security Upgrade"};
    for (int i = 0; i < 5; i++) {
        int32_t id = i + 1;
        time_t start_date = time(NULL) - (rand() % (3600 * 24 * 365));
        time_t end_date = start_date + (rand() % (3600 * 24 * 365));
        double budget = 100000 + (rand() % 900000);
        void* data[] = {&id, (void*)project_names[i], &start_date, &end_date, &budget};
        InsertTuple(TEST_TBL_PROJECTS, data);
    }

    printf("Employees Table:\n");
    PrintTable(TEST_TBL_EMPLOYEES);

    printf("\nDepartments Table:\n");
    PrintTable(TEST_TBL_DEPARTMENTS);
    
    printf("\nProjects Table:\n");
    PrintTable(TEST_TBL_PROJECTS);

    DeleteTable(TEST_TBL_EMPLOYEES);
    DeleteTable(TEST_TBL_DEPARTMENTS);
    DeleteTable(TEST_TBL_PROJECTS);

    printf("\nAll tables deleted.\n");

    return 0;
}