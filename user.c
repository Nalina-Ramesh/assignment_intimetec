#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "users.txt"
#define TEMP_FILE "temp.txt"
#define NAME_SIZE 50

struct User {
    int id;
    char name[NAME_SIZE];
    int age;
};

void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void read_string(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\r\n")] = '\0';
}

int read_int(const char *prompt) {
    char input[32];
    int value;

    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return 0;
        }

        if (sscanf(input, "%d", &value) == 1) {
            return value;
        }

        printf("Invalid input. Please enter a valid number.\n");
    }
}

void write_user_record(FILE *file, const struct User *user) {
    fprintf(file,
            "{\n"
            "    \"id\": %d,\n"
            "    \"name\": \"%s\",\n"
            "    \"age\": %d\n"
            "}\n",
            user->id, user->name, user->age);
}

int read_user_record(FILE *file, struct User *user) {
    char line[200];

    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }

    if (strcmp(line, "{\n") != 0 && strcmp(line, "{\r\n") != 0) {
        return 0;
    }

    if (fgets(line, sizeof(line), file) == NULL || sscanf(line, "    \"id\": %d", &user->id) != 1) {
        return 0;
    }

    if (fgets(line, sizeof(line), file) == NULL || sscanf(line, "    \"name\": \"%49[^\"]\"", user->name) != 1) {
        return 0;
    }

    if (fgets(line, sizeof(line), file) == NULL || sscanf(line, "    \"age\": %d", &user->age) != 1) {
        return 0;
    }

    if (fgets(line, sizeof(line), file) == NULL || (strcmp(line, "}\n") != 0 && strcmp(line, "}\r\n") != 0)) {
        return 0;
    }

    return 1;
}

void CREATE_USERS() {
    struct User user;
    FILE *file = fopen(FILE_NAME, "a");

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    user.id = read_int("Enter user ID: ");
    read_string("Enter user name: ", user.name, sizeof(user.name));
    user.age = read_int("Enter user age: ");

    write_user_record(file, &user);
    fclose(file);
    printf("User created successfully!\n");
}

void DISPLAY_USERS() {
    FILE *file = fopen(FILE_NAME, "r");
    struct User user;

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (read_user_record(file, &user)) {
        printf("User ID: %d\n", user.id);
        printf("Name: %s\n", user.name);
        printf("Age: %d\n\n", user.age);
    }

    fclose(file);
}

void UPDATE_USERS() {
    int id;
    int found = 0;
    struct User user;
    FILE *file;
    FILE *temp;

    id = read_int("Enter the user ID you want to update: ");

    file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    temp = fopen(TEMP_FILE, "w");
    if (temp == NULL) {
        printf("Error opening temp file!\n");
        fclose(file);
        return;
    }

    while (read_user_record(file, &user)) {
        if (user.id == id) {
            found = 1;
            read_string("Enter new name: ", user.name, sizeof(user.name));
            user.age = read_int("Enter new age: ");
        }

        write_user_record(temp, &user);
    }

    fclose(file);
    fclose(temp);

    if (!found) {
        printf("User ID %d not found.\n", id);
        remove(TEMP_FILE);
        return;
    }

    remove(FILE_NAME);
    rename(TEMP_FILE, FILE_NAME);
    printf("User updated successfully!\n");
}

void DELETE_USERS() {
    int id;
    int found = 0;
    struct User user;
    FILE *file;
    FILE *temp;

    id = read_int("Enter the user ID you want to delete: ");

    file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    temp = fopen(TEMP_FILE, "w");
    if (temp == NULL) {
        printf("Error opening temp file!\n");
        fclose(file);
        return;
    }

    while (read_user_record(file, &user)) {
        if (user.id == id) {
            found = 1;
            continue;
        }

        write_user_record(temp, &user);
    }

    fclose(file);
    fclose(temp);

    if (!found) {
        printf("User ID %d not found.\n", id);
        remove(TEMP_FILE);
        return;
    }

    remove(FILE_NAME);
    rename(TEMP_FILE, FILE_NAME);
    printf("User deleted successfully!\n");
}

int main() {
    int choice;

    while (1) {
        printf("\n1. CREATE USERS\n");
        printf("2. DISPLAY USERS\n");
        printf("3. UPDATE USER\n");
        printf("4. DELETE USER\n");
        printf("5. EXIT\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }

        clear_input_buffer();

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
                printf("Invalid choice\n");
        }
    }

    return 0;
}
