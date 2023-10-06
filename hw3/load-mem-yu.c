#include <stdio.h>
#include <stdlib.h>

#define MAXSTRING 200

/*  The data structure to store an array of integers and its size */
typedef struct
{
    int *data;
    size_t size;
} dynBlock;

/*  This function allocates a single instance of a dynBlock from the heap by the given size
    and returns a pointer to that allocated object.*/
dynBlock *allocDynBlock(size_t size)
{
    dynBlock *block = (dynBlock *)malloc(sizeof(dynBlock));
    if (block == NULL)
    {
        perror("Failed to allocate heap memory of a dynBlock.\n");
        exit(1);
    }
    block->data = (int *)malloc(size * sizeof(int));
    if (block->data == NULL)
    {
        fprintf(stderr, "Failed to allocate heap memory of %zu integers.\n", size);
        exit(1);
    }
    block->size = size;
    return block;
}

/*  This functions stores an array of integers in a dynamic block (a dynBlock object)
    previously allocated */
void storeMem2Blk(int *data, dynBlock *block)
{
    if (block == NULL || data == NULL)
    {
        perror("Null input!\n");
        return;
    }
    for (size_t i = 0; i < block->size; i++)
    {
        block->data[i] = data[i];
    }
}

/*  This function takes parses a given string into an array of integers, 
    then stores the array in a new dynamic block which will be returned */
dynBlock *readLine2Blk(char *line)
{
    int cnt = 0;
    char prev = ' ';
    for (char *pt = line; *pt != '\n'; pt++)
    {
        if (prev == ' ' && *pt >= '0' && *pt <= '9')
        {
            cnt++;
        }
        prev = *pt;
    }
    int data[cnt];
    int i = 0, num = 0;
    for (char *pt = line; *pt != '\n'; pt++)
    {
        if (*pt == ' ')
        {
            data[i++] = num;
            num = 0;
        }
        else if (*pt >= '0' && *pt <= '9')
        {
            num = num * 10 + *pt - '0';
        }
    }
    data[i++] = num;
    dynBlock *block = allocDynBlock(cnt);
    storeMem2Blk(data, block);
    return block;
}

int main()
{
    FILE *file = fopen("blocks.data", "r");
    if (file == NULL)
    {
        perror("Failed to open file!\n");
        return -1;
    }
    char line[MAXSTRING];
    fgets(line, MAXSTRING, file);
    while (feof(file) == 0)
    {
        printf("Input line: %s", line);
        dynBlock *block = readLine2Blk(line);
        printf("Block data: ");
        for (size_t i = 0; i < block->size; i++)
        {
            printf("%d ", block->data[i]);
        }
        printf("\n\n");
        free(block->data);
        free(block);
        fgets(line, MAXSTRING, file);
    }
    fclose(file);
    return 0;
}