#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int capacity;
struct Node
{
    int key;
    char data[100];
    struct Node *next;
};
struct Node *queue;
struct Node *arr[100] = {NULL};
int get_index(int token)
{
    for (int i = 0; i < capacity; i++)
    {
        int index = (token + i) % capacity;
        if (arr[index] == NULL)
        {
            return index;
        }
    }
    return -1;
}

void put_data(int token, char value[])
{
    int index = get_index(token);
    if (index == -1)
    {
        printf("no place available\n");
        return;
    }
    if (arr[index] == NULL)
    {
        arr[index] = malloc(sizeof(struct Node));

        if (arr[index] == NULL)
        {
            printf("Memory allocation failed\n");
            return;
        }

        arr[index]->key = token;
    }
    strcpy(arr[index]->data, value);
    return;
}

void get_data(int token)
{
    for (int i = 0; i < capacity; i++)
    {
        int index = (token + i) % capacity;
        struct Node *temp = arr[index];
        if (temp->key == token)
        {
            printf("the data associated with the given token is :%s\n", temp->data);
            printf("index is %d\n", index);
            return;
        }
    }
    printf("token not available");
    printf("NULL\n");
    return;
}

int main()
{
    printf("enter the capacity:");
    scanf("%d", &capacity);
    int choice;
    while (1)
    {
        printf("operations that can be performed:\n");
        printf("1.Put data\n");
        printf("2.Get data\n");
        printf("3.Exit\n");
        printf("enter the choice:");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
        {
            int token;
            char value[100];
            printf("enter the key that needs to be insetred:");
            scanf("%d", &token);
            getchar();
            printf("enter the value that needs to be insetred:");
            fgets(value, sizeof(value), stdin);
            value[strcspn(value, "\n")] = '\0';
            put_data(token, value);
            break;
        }
        case 2:
        {
            int token;
            printf("enter the key of the data tht u want to get\n");
            scanf("%d", &token);
            get_data(token);
            break;
        }
        }
    }
}
