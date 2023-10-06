/* This function calculates the length of a string */
int stringLen(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

/* This function appends a source string to a destination string */
void appendString(char *dest, const char *src)
{
    while (*dest != '\0')
    {
        dest++;
    }
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

/* This function assesses whether two strings are equal */
int isStringEqual(const char *str1, const char *str2)
{
    while (*str1 == *str2)
    {
        if (*str1 == '\0')
        {
            return 1;
        }
        str1++;
        str2++;
    }
    return 0;
}

/* This function converts a string in all upper case */
void stringToUpper(char *str)
{
    while (*str != '\0')
    {
        if (*str >= 'a' && *str <= 'z')
        {
            *str = *str - 'a' + 'A';
        }
        str++;
    }
}

/* This function converts a string in all lower case */
void stringToLower(char *str)
{
    while (*str != '\0')
    {
        if (*str >= 'A' && *str <= 'Z')
        {
            *str = *str - 'A' + 'a';
        }
        str++;
    }
}

/* This function converts a string with the first letter of each word
    capitalized and all other characters of the word in lower case */
void stringToCap(char *str)
{
    char prev = ' ';
    while (*str != '\0')
    {
        if (prev == ' ')
        {
            if (*str >= 'a' && *str <= 'z')
            {
                *str = *str - 'a' + 'A';
            }
        }
        else if (*str >= 'A' && *str <= 'Z')
        {
            *str = *str - 'A' + 'a';
        }
        prev = *str;
        str++;
    }
}
