#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "polybius.h"

int main(int argc, char *argv[]) {
    // Validate the arguments
    if (argc != 2 || strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-d") != 0) {
        printf("Usage: %s [-e/-d]\n", argv[0]);
        return -1;
    }
    // Initialize the Polybius table for encryption
    table_t table[TABLE_SIZE];
    if (table == NULL) {
        printf("Fail to allocate heap memory.\n");
        return -1;
    }
    int rows[9] = {7, 3, 1, 9, 4, 2, 8, 5, 6}, cols[9] = {5, 4, 6, 1, 8, 3, 2, 9, 7};
    // Special characters are added to the table
    for (char c = ' '; c <= '`'; c++) {
        int idx = c - ' ', row = idx / 9, col = idx % 9;
        table[idx].key = c;
        table[idx].value = rows[row] * 10 + cols[col];
    }
    for (int i = '`' - ' ' + 1; i < '`' - ' ' + 5; i++) {
        int row = i / 9, col = i % 9;
        table[i].key = (char) ('{' + i - '`' + ' ' - 1);
        table[i].value = rows[row] * 10 + cols[col];
    }
    
    // Cipher each word passed from stdin
    char word[100];
    while (scanf("%s", word) != EOF) {
        char *edittext = strcmp(argv[1], "-e") == 0 ? pbEncode(word, table) : pbDecode(word, table);
        if (edittext == NULL) {
            return -1;
        }
        printf("%s\n", edittext);
        free(edittext);
    }

    return 0;
}