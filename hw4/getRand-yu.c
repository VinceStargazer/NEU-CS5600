#include <stdio.h>
#include "mystring.h"

// LCG by glibc standard
#define LCG_MULTIPLIER 1103515245
#define LCG_INCREMENT 12345
#define LCG_MODULUS 0xFFFFFFFF

unsigned lcgSeed;

/*  This function returns a random value within the interval [floor, ceil] */
unsigned getRand(unsigned floor, unsigned ceil)
{
    lcgSeed = (LCG_MULTIPLIER * lcgSeed + LCG_INCREMENT) % LCG_MODULUS;
    return floor + lcgSeed % (ceil - floor + 1);
}

/*  This function reads the seed from an existing output file */
void readSeedFromFile(FILE *file)
{
    int lastNumber, currNumber;
    while (fscanf(file, "%d", &currNumber) != EOF)
    {
        lastNumber = currNumber;
    }
    lcgSeed = lastNumber;
}

/*  This function calculates the seed by hashing user input */
void hashSeed(unsigned num, const char *str)
{
    unsigned hash = 0;
    while (*str != '\0')
    {
        hash = hash * 31 + *str;
        str++;
    }
    lcgSeed = hash ^ num;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s (-a) <number> <fileName.txt>\n", argv[0]);
        return -1;
    }
    if (argc == 4 && isStringEqual(argv[1], "-a") == 0)
    {
        printf("Usage: %s (-a) <number> <fileName.txt>\n", argv[0]);
        return -1;
    }
    char *numStr = argc == 3 ? argv[1] : argv[2];
    int num = 0;
    for (char *pt = numStr; *pt != '\0'; pt++)
    {
        if (*pt >= '0' && *pt <= '9')
        {
            num = 10 * num + *pt - '0';
        }
        else
        {
            printf("Usage: %s (-a) <number> <fileName.txt>\n", argv[0]);
            return -1;
        }
    }
    char *fileName = argc == 3 ? argv[2] : argv[3];
    if (stringEndsWith(fileName, ".txt") == 0)
    {
        printf("Usage: %s (-a) <number> <fileName.txt>\n", argv[0]);
        return -1;
    }
    FILE *fileR = fopen(fileName, "r");
    if (fileR != NULL)
    {
        // Set the seed as the previous random value if file exists
        readSeedFromFile(fileR);
        fclose(fileR);
    }
    else
    {
        // otherwise set the seed by hashing the user input (number and file name)
        hashSeed(num, fileName);
    }
    FILE *file = argc == 3 ? fopen(fileName, "w") : fopen(fileName, "a");
    if (file == NULL)
    {
        perror("Filed to open file!");
        return -1;
    }
    for (int i = 0; i < num; i++)
    {
        fprintf(file, "%d ", getRand(1, 1000));
    }
    fprintf(file, "\n");
    fclose(file);
    return 0;
}