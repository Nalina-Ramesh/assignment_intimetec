
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define MAX_NAME 51
#define MAX_SIZE (BLOCK_SIZE * NUM_BLOCKS)

// Virtual disk
char disk[NUM_BLOCKS][BLOCK_SIZE];

// Free block management using an array
int freeBlocks[NUM_BLOCKS];
int freeCount = 0;
int usedBlocks = 0;

// File and directory structure
typedef struct FileNode
{
    char name[MAX_NAME];
    int isDirectory;
    int size;
    int blockCount;
    int *blocks;

    struct FileNode *parent;
    struct FileNode *child;
    struct FileNode *next;

} FileNode;

FileNode *root;
FileNode *cwd;

// Initialize disk
void initializeDisk()
{
    for (int i = 0; i < NUM_BLOCKS; i++)
        freeBlocks[freeCount++] = i;
}

// Allocate a block
int allocateBlock()
{
    if (freeCount == 0)
        return -1;

    usedBlocks++;
    return freeBlocks[--freeCount];
}

// Release a block
void freeBlock(int index)
{
    freeBlocks[freeCount++] = index;
    usedBlocks--;
}

// Create a file or directory
FileNode *createNode(char *name, int isDirectory)
{
    FileNode *node = calloc(1, sizeof(FileNode));

    if (node == NULL)
        return NULL;

    strcpy(node->name, name);
    node->isDirectory = isDirectory;
    node->parent = cwd;

    return node;
}

// Find a file or directory
FileNode *findNode(FileNode *dir, char *name)
{
    FileNode *temp = dir->child;

    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
            return temp;

        temp = temp->next;
    }

    return NULL;
}

// Insert a node into a directory
void insertNode(FileNode *dir, FileNode *node)
{
    node->next = dir->child;
    dir->child = node;
}

// Remove a node from a directory
void removeNode(FileNode *dir, FileNode *node)
{
    FileNode *temp = dir->child;
    FileNode *prev = NULL;

    while (temp != NULL)
    {
        if (temp == node)
        {
            if (prev == NULL)
                dir->child = temp->next;
            else
                prev->next = temp->next;

            return;
        }

        prev = temp;
        temp = temp->next;
    }
}

// Validate file and directory names
int validName(char *name)
{
    return strlen(name) > 0 && strlen(name) < MAX_NAME && strchr(name, '/') == NULL && strcmp(name, ".") != 0 && strcmp(name, "..") != 0;
}

// Initialize file system
void initializeVFS()
{
    initializeDisk();
    root = calloc(1, sizeof(FileNode));
    if (!root)
    {
        printf("Initialization failed.\n");
        exit(1);
    }
    strcpy(root->name, "/");
    root->isDirectory = 1;

    cwd = root;
}

// Create directory
void mkdirCommand(char *name)
{
    if (!validName(name))
    {
        printf("Invalid directory name.\n");
        return;
    }

    if (findNode(cwd, name))
    {
        printf("Name already exists.\n");
        return;
    }

    FileNode *node = createNode(name, 1);

    if (!node)
        return;

    insertNode(cwd, node);
    printf("Directory created successfully.\n");
}

// Create file
void createCommand(char *name)
{
    if (!validName(name))
    {
        printf("Invalid file name.\n");
        return;
    }

    if (findNode(cwd, name))
    {
        printf("Name already exists.\n");
        return;
    }

    FileNode *node = createNode(name, 0);

    if (!node)
        return;

    insertNode(cwd, node);

    printf("File created successfully.\n");
}

// Write data into file
void writeCommand(char *name, char *content)
{
    FileNode *file = findNode(cwd, name);

    if (!file || file->isDirectory)
    {
        printf("File not found.\n");
        return;
    }

    int size = strlen(content);
    int required = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    if (required > freeCount)
    {
        printf("Disk full.\n");
        return;
    }

    int *newBlocks = NULL;

    if (required > 0)
    {
        newBlocks = malloc(required * sizeof(int));

        if (!newBlocks)
            return;
    }

    // Allocate blocks
    for (int i = 0; i < required; i++)
        newBlocks[i] = allocateBlock();

    // Write content into disk
    for (int i = 0; i < required; i++)
    {
        int offset = i * BLOCK_SIZE;
        int bytes = size - offset;
        if (bytes > BLOCK_SIZE)
            bytes = BLOCK_SIZE;

        memcpy(disk[newBlocks[i]], content + offset, bytes);
    }

    // Release previous blocks
    for (int i = 0; i < file->blockCount; i++)
        freeBlock(file->blocks[i]);

    free(file->blocks);

    file->blocks = newBlocks;
    file->blockCount = required;
    file->size = size;

    printf("Data written successfully.\n");
}

// Read file
void readCommand(char *name)
{
    FileNode *file = findNode(cwd, name);

    if (!file || file->isDirectory)
    {
        printf("File not found.\n");
        return;
    }

    for (int i = 0; i < file->blockCount; i++)
    {
        int bytes = file->size - i * BLOCK_SIZE;

        if (bytes > BLOCK_SIZE)
            bytes = BLOCK_SIZE;

        fwrite(disk[file->blocks[i]], 1, bytes, stdout);
    }

    printf("\n");
}

// Delete file
void deleteCommand(char *name)
{
    FileNode *file = findNode(cwd, name);

    if (!file || file->isDirectory)
    {
        printf("File not found.\n");
        return;
    }

    for (int i = 0; i < file->blockCount; i++)
        freeBlock(file->blocks[i]);

    free(file->blocks);

    removeNode(cwd, file);
    free(file);

    printf("File deleted successfully.\n");
}

// Remove empty directory
void rmdirCommand(char *name)
{
    FileNode *dir = findNode(cwd, name);
    if (!dir || !dir->isDirectory)
    {
        printf("Directory not found.\n");
        return;
    }
    if (dir->child != NULL)
    {
        printf("Directory is not empty.\n");
        return;
    }
    removeNode(cwd, dir);
    free(dir);

    printf("Directory removed successfully.\n");
}

// List files and directories
void lsCommand()
{
    FileNode *temp = cwd->child;

    if (!temp)
    {
        printf("(empty)\n");
        return;
    }

    while (temp != NULL)
    {
        if (temp->isDirectory)
            printf("[DIR] ");

        printf("%s\n", temp->name);

        temp = temp->next;
    }
}

// Change directory
void cdCommand(char *name)
{
    if (strcmp(name, "/") == 0)
    {
        cwd = root;
        return;
    }

    if (strcmp(name, "..") == 0)
    {
        if (cwd->parent)
            cwd = cwd->parent;

        return;
    }

    FileNode *dir = findNode(cwd, name);

    if (!dir || !dir->isDirectory)
    {
        printf("Directory not found.\n");
        return;
    }

    cwd = dir;
}

// Print current directory
void pwdCommand()
{
    FileNode *stack[NUM_BLOCKS];
    int count = 0;

    FileNode *temp = cwd;

    while (temp != root && count < NUM_BLOCKS)
    {
        stack[count++] = temp;
        temp = temp->parent;
    }

    printf("/");

    for (int i = count - 1; i >= 0; i--)
    {
        printf("%s", stack[i]->name);

        if (i > 0)
            printf("/");
    }

    printf("\n");
}

// Display disk information
void dfCommand()
{
    printf("Total Blocks: %d\n", NUM_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", freeCount);

    printf("Disk Usage: %.2f%%\n",
           usedBlocks * 100.0 / NUM_BLOCKS);
}

// Recursively release directory contents
void freeDirectory(FileNode *dir)
{
    while (dir->child != NULL)
    {
        FileNode *node = dir->child;

        if (node->isDirectory)
            freeDirectory(node);
        else
        {
            for (int i = 0; i < node->blockCount; i++)
                freeBlock(node->blocks[i]);

            free(node->blocks);
        }

        removeNode(dir, node);
        free(node);
    }
}

// Release all memory
void cleanup()
{
    freeDirectory(root);
    free(root);
}

int main()
{
    initializeVFS();

    char command[20];
    char name[MAX_NAME];
    char line[MAX_SIZE + 100];
    char content[MAX_SIZE + 1];

    printf("Virtual File System Ready.\n");

    while (1)
    {
        pwdCommand();
        printf("> ");

        if (!fgets(line, sizeof(line), stdin))
            break;

        int pos = 0;

        if (sscanf(line, "%19s", command) != 1)
            continue;

        if (strcmp(command, "exit") == 0)
            break;

        if (strcmp(command, "mkdir") == 0 || strcmp(command, "create") == 0 || strcmp(command, "read") == 0 || strcmp(command, "delete") == 0 || strcmp(command, "rmdir") == 0 || strcmp(command, "cd") == 0)
        {
            if (sscanf(line, "%19s %50s", command, name) != 2)
            {
                printf("Missing name.\n");
                continue;
            }

            if (strcmp(command, "mkdir") == 0)
                mkdirCommand(name);

            else if (strcmp(command, "create") == 0)
                createCommand(name);

            else if (strcmp(command, "read") == 0)
                readCommand(name);

            else if (strcmp(command, "delete") == 0)
                deleteCommand(name);

            else if (strcmp(command, "rmdir") == 0)
                rmdirCommand(name);

            else if (strcmp(command, "cd") == 0)
                cdCommand(name);
        }

        else if (strcmp(command, "write") == 0)
        {
            if (sscanf(line, "%19s %50s%n",
                       command, name, &pos) != 2)
            {
                printf("Missing file name.\n");
                continue;
            }

            char *p = line + pos;

            while (*p == ' ' || *p == '\t')
                p++;

            // Remove newline
            p[strcspn(p, "\n")] = '\0';

            // Remove surrounding quotes
            int len = strlen(p);

            if (len >= 2 && p[0] == '"' && p[len - 1] == '"')
            {
                p[len - 1] = '\0';
                p++;
            }

            strcpy(content, p);
            writeCommand(name, content);
        }

        else if (strcmp(command, "ls") == 0)
            lsCommand();

        else if (strcmp(command, "pwd") == 0)
            pwdCommand();

        else if (strcmp(command, "df") == 0)
            dfCommand();

        else
            printf("Unknown command.\n");
    }

    cleanup();

    printf("Memory released. Exiting program.\n");

    return 0;
}
