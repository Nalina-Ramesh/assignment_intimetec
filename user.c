#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct User {
    int id;
    char name[50];
    int age;
};

void CREATE_USERS() {
    struct User user;
    FILE *file = fopen("users.txt", "a");

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter user ID: ");
    scanf("%d", &user.id);
    printf("Enter user name: ");
    scanf("%49s", user.name);
    printf("Enter user age: ");
    scanf("%d", &user.age);

    fprintf(file,
            "{\n"
            "    \"id\": %d,\n"
            "    \"name\": \"%s\",\n"
            "    \"age\": %d\n"
            "}\n",
            user.id, user.name, user.age);

    fclose(file);
    printf("User created successfully!\n");
}

void DISPLAY_USERS() {
    FILE *file = fopen("users.txt", "r");
    char line[200];

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    fclose(file);
}

void UPDATE_USERS() {
    int id, found = 0;
    struct User user;
    FILE *file;
    FILE *temp;

    printf("Enter the user ID you want to update: ");
    scanf("%d", &id);

    file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Error opening temp file!\n");
        fclose(file);
        return;
    }

    while (fscanf(file, "{\n    \"id\": %d,\n    \"name\": \"%49[^\"]\",\n    \"age\": %d\n}\n",
                  &user.id, user.name, &user.age) == 3) {
        if (user.id == id) {
            found = 1;
            printf("Enter new name: ");
            scanf("%49s", user.name);
            printf("Enter new age: ");
            scanf("%d", &user.age);
        }

        fprintf(temp,
                "{\n"
                "    \"id\": %d,\n"
                "    \"name\": \"%s\",\n"
                "    \"age\": %d\n"
                "}\n",
                user.id, user.name, user.age);
    }

    fclose(file);
    fclose(temp);

    if (!found) {
        printf("User ID %d not found.\n", id);
        remove("temp.txt");
        return;
    }

    remove("users.txt");
    rename("temp.txt", "users.txt");
    printf("User updated successfully!\n");
}

void DELETE_USERS() {
    int id, found = 0;
    struct User user;
    FILE *file;
    FILE *temp;

    printf("Enter the user ID you want to delete: ");
    scanf("%d", &id);

    file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Error opening temp file!\n");
        fclose(file);
        return;
    }

    while (fscanf(file, "{\n    \"id\": %d,\n    \"name\": \"%49[^\"]\",\n    \"age\": %d\n}\n",
                  &user.id, user.name, &user.age) == 3) {
        if (user.id != id) {
            fprintf(temp,
                    "{\n"
                    "    \"id\": %d,\n"
                    "    \"name\": \"%s\",\n"
                    "    \"age\": %d\n"
                    "}\n",
                    user.id, user.name, user.age);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp);

    if (!found) {
        printf("User ID %d not found.\n", id);
        remove("temp.txt");
        return;
    }

    remove("users.txt");
    rename("temp.txt", "users.txt");
    printf("User deleted successfully!\n");
}

int main() {
    while (1) {
        int choice;
        printf("1.CREATE USERS\n");
        printf("2.DISPLAY USERS\n");
        printf("3.UPDATE USER\n");
        printf("4.DELETE USER\n");
        printf("5.EXIT\n");
        printf("enter your choice : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                CREATE_USERS();
                break;
            case 2:
                DISPLAY_USERS();
                break;
            case 3:
                UPDATE_USERS();
                break;
            case 4:
                DELETE_USERS();
                break;
            case 5:
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("invalid choice\n");
        }
    }

    return 0;
}
