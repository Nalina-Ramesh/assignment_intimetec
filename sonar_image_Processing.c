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
        for (int j = i + 1; j < n; j++)
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
    int *prev = (int *)malloc(n * sizeof(int));
    int *curr = (int *)malloc(n * sizeof(int));

    if (prev == NULL || curr == NULL)
    {
        free(prev);
        free(curr);
        return;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            *(curr + j) = *(*(matrix + i) + j);
        }
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
                        int value = 0;

                        if (x == i)
                        {
                            value = *(curr + y);
                        }
                        else if (x == i - 1)
                        {
                            value = *(prev + y);
                        }
                        else
                        {
                            value = *(*(matrix + x) + y);
                        }

                        sum += value;
                        count++;
                    }
                }
            }

            if (count > 0)
            {
                *(*(matrix + i) + j) = sum / count;
            }
        }

        int *temp = prev;
        prev = curr;
        curr = temp;
    }

    free(prev);
    free(curr);
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

    for (int i = 0; i < n; i++)
    {
        free(*(matrix + i));
    }
    free(matrix);
}
