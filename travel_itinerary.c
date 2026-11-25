#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Node
{
    char source[100];
    char destination[100];
    struct Node *next;
};

struct Node *createNode(char *source, char *destination)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));

    if (newNode != NULL)
    {
        strcpy(newNode->source, source);
        strcpy(newNode->destination, destination);
        newNode->next = NULL;
    }
    return newNode;
}

int main()
{
    int n;
    char input[1000];
    char src[100], dest[100];
    struct Node *tickets[100] = {NULL};
    printf("enter the number of tickets\n");
    scanf("%d", &n);

    printf("enter the source and destination in format(source->destination) of all the tickets to start from\n");
    for (int i = 0; i <= n; i++)
    {
        if (fgets(input, sizeof(input), stdin) != NULL)
        {
            input[strcspn(input, "\n")] = '\0';
            char *arrow = strstr(input, "->");
            if (arrow != NULL)
            {
                *arrow = '\0';
                char *src = input;
                char *dest = arrow + 2;
                tickets[i] = createNode(src, dest);
            }
            else
            {
                printf("Invalid format. Try again:\n");
                i--;
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j && strcmp(tickets[i]->destination, tickets[j]->source) == 0)
            {
                tickets[i]->next = tickets[j];
                break;
            }
        }
    }
    struct Node *start = NULL;
    for (int i = 0; i < n; i++)
    {
        int isDestination = 0;
        for (int j = 0; j < n; j++)
        {
            if (strcmp(tickets[i]->source, tickets[j]->destination) == 0)
            {
                isDestination = 1;
                break;
            }
        }
        if (!isDestination)
        {
            start = tickets[i];
            break;
        }
    }

    printf("\nFull Itinerary:\n");
    struct Node *curr = start;
    while (curr != NULL)
    {
        printf("%s -> %s\n", curr->source, curr->destination);
        curr = curr->next;
    }

    for (int i = 0; i < n; i++)
    {
        free(tickets[i]);
    }

    return 0;
}

