int stringLen(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

int stringEndsWith(const char *str1, const char *str2)
{
    int i = stringLen(str1) - 1, j = stringLen(str2) - 1;
    while (i >= 0 && j >= 0 && str1[i] == str2[j])
    {
        i--;
        j--;
    }
    return j < 0 ? 1 : 0;
}

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