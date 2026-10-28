#include <stdio.h>
#include <stdlib.h>

void create_matrix(int **matrix, int n)
{
    int min = 1, max = 250;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            *(*(matrix + i) + j) = (rand() % (max - min + 1)) + min;
        }
    }
}

void rotate_matrix(int **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i; j < n; j++)
        {
            int temp = *(*(matrix + i) + j);
            *(*(matrix + i) + j) = *(*(matrix + j) + i);
            *(*(matrix + j) + i) = temp;
        }
    }
    int i = 0;
    int j = n - 1;
    while (i < j)
    {
        for (int k = 0; k < n; k++)
        {
            int temp = *(*(matrix + k) + i);
            *(*(matrix + k) + i) = *(*(matrix + k) + j);
            *(*(matrix + k) + j) = temp;
        }
        i++;
        j--;
    }
}

void smoothing_filter(int **matrix, int n)
{
    int **result = malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++)
        *(result + i) = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int sum = 0;
            int count = 0;
            for (int x = i - 1; x <= i + 1; x++)
            {
                for (int y = j - 1; y <= j + 1; y++)
                {
                    if (x >= 0 && x < n && y >= 0 && y < n && !(x == i && y == j))
                    {
                        sum += *(*(matrix + x) + y);
                        count++;
                    }
                }
            }

            *(*(result + i) + j) = sum / count;
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            *(*(matrix + i) + j) = *(*(result + i) + j);
        }
        free(*(result + i));
    }

    free(result);
}
void print_matrix(int **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", *(*(matrix + i) + j));
        }
        printf("\n");
    }
}
int main()
{
    int n;
    printf("enter the size of the matrix:");
    scanf("%d", &n);
    int **matrix = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        *(matrix + i) = (int *)malloc(n * sizeof(int));
    }

    create_matrix(matrix, n);
    print_matrix(matrix, n);
    printf("-------------------\n");
    rotate_matrix(matrix, n);
    print_matrix(matrix, n);
    printf("---------------------\n");
    smoothing_filter(matrix, n);
    print_matrix(matrix, n);
}
