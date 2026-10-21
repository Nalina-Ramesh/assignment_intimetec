#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Employee
{
    int employee_id;
    char employee_name[50];
    char employee_designation[50];
    int reporting_manager_id;
    int reports[100];
    int num_reports;
};

void add_employee(struct Employee *employees, int *count)
{
    struct Employee new_employee;
    new_employee.num_reports = 0;
    printf("Enter Employee ID: ");
    scanf("%d", &new_employee.employee_id);
    getchar();

    printf("Enter Employee Name: ");
    fgets(new_employee.employee_name, sizeof(new_employee.employee_name), stdin);
    new_employee.employee_name[strcspn(new_employee.employee_name, "\n")] = '\0';

    printf("Enter Employee Designation: ");
    fgets(new_employee.employee_designation, sizeof(new_employee.employee_designation), stdin);
    new_employee.employee_designation[strcspn(new_employee.employee_designation, "\n")] = '\0';

    printf("Enter Reporting Manager ID: ");
    scanf("%d", &new_employee.reporting_manager_id);
    for (int i = 0; i < *count; i++)
    {
        if (employees[i].employee_id == new_employee.reporting_manager_id)
        {
            employees[i].reports[employees[i].num_reports] = new_employee.employee_id;
            employees[i].num_reports++;
            break;
        }
    }
    employees[*count] = new_employee;
    (*count)++;

    printf("Employee added successfully!\n");
}

void delete_employee(struct Employee *employees, int *count, int employee_id)
{
    int index = -1;
    for (int i = 0; i < *count; i++)
    {
        if (employees[i].employee_id == employee_id)
        {
            index = i;
            if(strcmp(employees[i].employee_designation, "ceo") == 0)
            {
                printf("Cannot delete the CEO.\n");
                return;
            }
            break;
        }
    }

    if (index == -1)
    {
        printf("Employee with ID %d not found.\n", employee_id);
        return;
    }

    for (int i = index; i < *count - 1; i++)
    {
        employees[i] = employees[i + 1];
    }

    (*count)--;
    printf("Employee with ID %d deleted successfully!\n", employee_id);
}

void display_complete_hierarchy(struct Employee *employees, int count, int manager_id, int level)
{
    for (int i = 0; i < count; i++)
    {
        if (employees[i].reporting_manager_id == manager_id)
        {
            for (int j = 0; j < level; j++)
            {
                printf("  ");
            }
            printf("%d - %s - %s \n", employees[i].employee_id, employees[i].employee_name, employees[i].employee_designation);
            display_complete_hierarchy(employees, count, employees[i].employee_id, level + 1);
        }
    }
}

void display_employee_hierarchy(struct Employee *employees, int count)
{
    printf("enter the id of the employee whose hierarchy you want to display: ");
    int employee_id;
    scanf("%d", &employee_id);

    display_complete_hierarchy(employees, count, employee_id, 0);
}

int main()
{
    struct Employee employees[100];
    int count = 0;
    int choice;

    while (1)
    {
        printf("\nEmployee Hierarchy Management System\n");
        printf("1. Add Employee\n");
        printf("2. Delete Employee\n");
        printf("3. Display complete Hierarchy\n");
        printf("4. Display Employee Hierarchy\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            add_employee(employees, &count);
            break;
        case 2:
        {
            int employee_id;
            printf("Enter Employee ID to delete: ");
            scanf("%d", &employee_id);
            delete_employee(employees, &count, employee_id);
            break;
        }
        case 3:
            display_complete_hierarchy(employees, count, -1, 0);
            break;
        case 4:
            display_employee_hierarchy(employees, count);
            break;
        case 5:
            exit(0);
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}