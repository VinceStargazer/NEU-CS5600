#include <stdio.h>
#include "mystring.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s [-u/-l/-cap] <text>\n", argv[0]);
        return -1;
    }
    size_t len = 0;
    for (size_t i = 2; i < argc; i++)
    {
        len += stringLen(argv[i]) + 1;
    }
    char result[len];
    result[0] = '\0';
    for (size_t i = 2; i < argc; i++)
    {
        appendString(result, argv[i]);
        if (i < argc - 1)
        {
            appendString(result, " ");
        }
    }
    if (isStringEqual(argv[1], "-u") == 1)
    {
        stringToUpper(result);
    }
    else if (isStringEqual(argv[1], "-l") == 1)
    {
        stringToLower(result);
    }
    else if (isStringEqual(argv[1], "-cap") == 1)
    {
        stringToCap(result);
    }
    else
    {
        printf("Usage: %s [-u/-l/-cap] <text>\n", argv[0]);
        return -1;
    }
    printf("%s\n", result);
    return 0;
}