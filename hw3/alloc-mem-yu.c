#include <stdio.h>
#include <stdlib.h>

/*  Allocate 5MB of storage from the static segment by global variable. The memory will be
    de-allocated once the program exits */
char staticMemory[5 * 1024 * 1024];

/*  This function allocates memory from the stack. The memory will be de-allocated
    automatically once the functions completes its execution */
void allocStackMemory()
{
    // Allocate 1MB of storage from the stack
    char stackMemory[1 * 1024 * 1024];
    printf("Allocated 1MB of stack memory.\n");
}

int main()
{
    printf("Allocated 5MB of static memory.\n");
    // Allocate 10MB of storage from the heap
    char *heapMemory = (char *)malloc(10 * 1024 * 1024);
    if (heapMemory == NULL)
    {
        perror("Failed to allocate 10MB of heap memory.\n");
        return -1;
    }
    printf("Allocated 10MB of heap memory.\n");
    // Free 10MB of heap memory
    free(heapMemory);
    printf("Freed 10MB of heap memory.\n");
    allocStackMemory();
    printf("Freed 1MB of stack memory.\n");
    printf("Freed 5MB of static memory.\n");
    return 0;
}