#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

int buffer_size;
uint8_t *buffer;

typedef struct memory
{
    int starting_offset;
    int block_size;
    int allocation_id;
    bool is_allocated;
} memory;

memory *data_array = NULL;
int num_blocks = 0;
int next_allocation_id = 1;

void first_fit();
void best_fit();
void worst_fit();

void allocation_strategy(int *fit_choice)
{
    printf("Select allocation strategy(1.First Fit , 2. Best Fit , 3. Worst Fit ):");
    scanf("%d", fit_choice);
    if (*fit_choice == 1)
    {
        printf("Strategy set to First fit\n");
    }
    else if (*fit_choice == 2)
    {
        printf("Strategy set to Best fit\n");
    }
    else if (*fit_choice == 3)
    {
        printf("Strategy set to Worst fit\n");
    }
    else
    {
        printf("Please select the proper option among them (1-3)\n");
        allocation_strategy(fit_choice);
    }
}

void initialize_pool()
{
    printf("enter the memory pool size(1-1000000)\n");
    scanf("%d", &buffer_size);
    if (buffer_size < 1 || buffer_size > 1000000)
    {
        printf("the buffer size is invalid please try entering a proper buffer size again(1-1000000)\n");
        initialize_pool();
    }

    buffer = (uint8_t *)malloc(buffer_size * sizeof(uint8_t));
    if (buffer == NULL)
    {
        printf("memory not alloctaed\n");
        free(buffer);
        return;
    }
    data_array = (memory *)malloc(sizeof(memory));
    if (data_array == NULL)
    {
        printf("Metadata allocation failed\n");
        free(buffer);
        exit(1);
    }
    data_array[0].starting_offset = 0;
    data_array[0].block_size = buffer_size;
    data_array[0].allocation_id = 0;
    data_array[0].is_allocated = false;
    num_blocks = 1;
}

void allocate_memory(int *fit_choice)
{
    if (*fit_choice == 1)
    {
        first_fit();
    }
    else if (*fit_choice == 2)
    {
        best_fit();
    }
    else
    {
        worst_fit();
    }
}

void split_and_allocate(int index, int size)
{
    if (data_array[index].block_size > size)
    {
        data_array = (memory *)realloc(data_array, (num_blocks + 1) * sizeof(memory));

        for (int i = num_blocks; i > index + 1; i--)
        {
            data_array[i] = data_array[i - 1];
        }

        data_array[index + 1].starting_offset = data_array[index].starting_offset + size;
        data_array[index + 1].block_size = data_array[index].block_size - size;
        data_array[index + 1].allocation_id = 0;
        data_array[index + 1].is_allocated = false;

        num_blocks++;
    }

    data_array[index].block_size = size;
    data_array[index].allocation_id = next_allocation_id++;
    data_array[index].is_allocated = true;

    printf("Memory allocated successfully. ID: %d, Offset: %d, Size: %d\n",
           data_array[index].allocation_id, data_array[index].starting_offset, size);
}

void first_fit()
{
    printf("enter the size of memory to be allocated\n");
    int size;
    scanf("%d", &size);

    if (size <= 0)
    {
        printf("Invalid allocation size\n");
        return;
    }

    for (int i = 0; i < num_blocks; i++)
    {
        if (!data_array[i].is_allocated && data_array[i].block_size >= size)
        {
            split_and_allocate(i, size);
            return;
        }
    }
    printf("Error: No suitable free block found (First Fit)\n");
}

void best_fit()
{
    printf("enter the size of memory to be allocated\n");
    int size;
    scanf("%d", &size);

    if (size <= 0)
    {
        printf("Invalid allocation size\n");
        return;
    }

    int best_index = -1;
    int min_rem_size = buffer_size + 1;

    for (int i = 0; i < num_blocks; i++)
    {
        if (!data_array[i].is_allocated && data_array[i].block_size >= size)
        {
            int rem_size = data_array[i].block_size - size;
            if (rem_size < min_rem_size)
            {
                min_rem_size = rem_size;
                best_index = i;
            }
        }
    }

    if (best_index != -1)
    {
        split_and_allocate(best_index, size);
    }
    else
    {
        printf("Error: No suitable free block found (Best Fit)\n");
    }
}

void worst_fit()
{
    printf("enter the size of memory to be allocated\n");
    int size;
    scanf("%d", &size);

    if (size <= 0)
    {
        printf("Invalid allocation size\n");
        return;
    }

    int worst_index = -1;
    int max_rem_size = -1;

    for (int i = 0; i < num_blocks; i++)
    {
        if (!data_array[i].is_allocated && data_array[i].block_size >= size)
        {
            int rem_size = data_array[i].block_size - size;
            if (rem_size > max_rem_size)
            {
                max_rem_size = rem_size;
                worst_index = i;
            }
        }
    }

    if (worst_index != -1)
    {
        split_and_allocate(worst_index, size);
    }
    else
    {
        printf("Error: No suitable free block found (Worst Fit)\n");
    }
}

void coalesce_free_blocks()
{
    for (int i = 0; i < num_blocks - 1; i++)
    {
        if (!data_array[i].is_allocated && !data_array[i + 1].is_allocated)
        {
            data_array[i].block_size += data_array[i + 1].block_size;

            for (int j = i + 1; j < num_blocks - 1; j++)
            {
                data_array[j] = data_array[j + 1];
            }
            num_blocks--;
            data_array = (memory *)realloc(data_array, num_blocks * sizeof(memory));
            i--;
        }
    }
}

void release_memory()
{
    int id;
    printf("Enter the Allocation ID to release: ");
    scanf("%d", &id);

    for (int i = 0; i < num_blocks; i++)
    {
        if (data_array[i].is_allocated && data_array[i].allocation_id == id)
        {
            data_array[i].is_allocated = false;
            data_array[i].allocation_id = 0;
            printf("Allocation ID %d released successfully.\n", id);
            coalesce_free_blocks();
            return;
        }
    }
    printf("Error: Allocation ID %d not found.\n", id);
}

void resize_memory(int *fit_choice)
{
    int id, new_size;
    printf("Enter the Allocation ID to resize: ");
    if (scanf("%d", &id) != 1)
        return;
    printf("Enter the new size: ");
    if (scanf("%d", &new_size) != 1)
        return;

    if (new_size <= 0)
    {
        printf("Invalid size request.\n");
        return;
    }

    int target_index = -1;
    for (int i = 0; i < num_blocks; i++)
    {
        if (data_array[i].is_allocated && data_array[i].allocation_id == id)
        {
            target_index = i;
            break;
        }
    }

    if (target_index == -1)
    {
        printf("Error: Allocation ID %d not found.\n", id);
        return;
    }

    int current_size = data_array[target_index].block_size;

    // Case 1: Shrinking the block
    if (new_size <= current_size)
    {
        int diff = current_size - new_size;
        if (diff > 0)
        {
            data_array[target_index].block_size = new_size;

            data_array = (memory *)realloc(data_array, (num_blocks + 1) * sizeof(memory));
            for (int i = num_blocks; i > target_index + 1; i--)
            {
                data_array[i] = data_array[i - 1];
            }
            data_array[target_index + 1].starting_offset = data_array[target_index].starting_offset + new_size;
            data_array[target_index + 1].block_size = diff;
            data_array[target_index + 1].allocation_id = 0;
            data_array[target_index + 1].is_allocated = false;
            num_blocks++;

            coalesce_free_blocks();
        }
        printf("Block resized (shrunk) successfully.\n");
        return;
    }

    // Case 2: Expanding the block into the immediate next free block
    int diff = new_size - current_size;
    if (target_index + 1 < num_blocks && !data_array[target_index + 1].is_allocated && data_array[target_index + 1].block_size >= diff)
    {
        data_array[target_index].block_size = new_size;
        if (data_array[target_index + 1].block_size == diff)
        {
            for (int i = target_index + 1; i < num_blocks - 1; i++)
            {
                data_array[i] = data_array[i + 1];
            }
            num_blocks--;
            data_array = (memory *)realloc(data_array, num_blocks * sizeof(memory));
        }
        else
        {
            data_array[target_index + 1].starting_offset += diff;
            data_array[target_index + 1].block_size -= diff;
        }
        printf("Block expanded in-place successfully.\n");
        return;
    }

    // Case 3: In-place expansion failed -> Attempt Relocation
    printf("In-place expansion failed.\n");

    int alt_index = find_fit_index(*fit_choice, new_size);
    if (alt_index != -1)
    {
        int old_offset = data_array[target_index].starting_offset;

        split_and_allocate(alt_index, new_size);

        int new_target_index = -1;
        for (int i = 0; i < num_blocks; i++)
        {
            if (data_array[i].is_allocated && data_array[i].starting_offset != old_offset && data_array[i].block_size == new_size)
            {
                new_target_index = i;
            }
        }

        for (int i = 0; i < num_blocks; i++)
        {
            if (data_array[i].starting_offset == old_offset)
            {
                data_array[i].is_allocated = false;
                data_array[new_target_index].allocation_id = id;
                data_array[i].allocation_id = 0;
                break;
            }
        }

        next_allocation_id--;
        coalesce_free_blocks();
        printf("Block successfully relocated to a new memory offset.\n");
        return;
    }
}
void display_memory_map()
{
    printf("\n--- MEMORY MAP ---\n");
    printf("%-10s %-10s %-10s %-12s\n", "Offset", "Size", "Status", "ID");
    for (int i = 0; i < num_blocks; i++)
    {
        printf("%-10d %-10d %-10s %-12d\n",
               data_array[i].starting_offset,
               data_array[i].block_size,
               data_array[i].is_allocated ? "ALLOCATED" : "FREE",
               data_array[i].allocation_id);
    }
    printf("-------------------\n");
}

void display_memory_statistics()
{
    int allocated_bytes = 0;
    int free_bytes = 0;
    int allocated_blocks = 0;
    int free_blocks = 0;

    for (int i = 0; i < num_blocks; i++)
    {
        if (data_array[i].is_allocated)
        {
            allocated_bytes += data_array[i].block_size;
            allocated_blocks++;
        }
        else
        {
            free_bytes += data_array[i].block_size;
            free_blocks++;
        }
    }
    printf("memory statistics\n");
    printf("Total Pool Size     : %d bytes\n", buffer_size);
    printf("Allocated Memory    : %d bytes (%d blocks)\n", allocated_bytes, allocated_blocks);
    printf("Free Memory         : %d bytes (%d blocks)\n", free_bytes, free_blocks);

    printf("-------------------------\n");
}

int main()
{
    int fit_choice;
    initialize_pool();
    allocation_strategy(&fit_choice);

    int choice;
    while (1)
    {
        printf("1.Allocate Memory\n");
        printf("2.Release Memory\n");
        printf("3.Resize Alloctaion\n");
        printf("4.Display memory map\n");
        printf("5.Display memory allocation\n");
        printf("6.change allocation strategy\n");
        printf("7.Exit\n");
        printf("Enter the choice:");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            allocate_memory(&fit_choice);
            break;
        case 2:
            release_memory();
            break;
        case 3:
            resize_memory(&fit_choice);
            break;
        case 4:
            display_memory_map();
            break;
        case 5:
            display_memory_statistics();
            break;
        case 6:
            allocation_strategy(&fit_choice);
            break;
        case 7:
            exit(1);
        default:
            printf("please enter a valid choice (1-7)\n");
        }
    }
}
