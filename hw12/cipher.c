#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cipher.h"

#define TABLE_SIZE 81

/* This function returns an encoded version of the string plaintext using the Polybius table */
char *pbEncode(const char *plaintext, table_t* table) {
    size_t len = strlen(plaintext);
    char *ciphertext = (char*)malloc(2 * len * sizeof(char) + 1);
    for (size_t i = 0; i < len; i++) {
        char key = plaintext[i];
        // lower case letters are treated as upper case
        if (key >= 'a' && key <= 'z') {
            key = (char) (key - 'a' + 'A');
        }
        int j = 0;
        while (j++ < TABLE_SIZE) {
            if (table[j].key == key) {
                int value = table[j].value;
                int d1 = value / 10, d2 = value % 10;
                ciphertext[2 * i] = (char) (d1 + '0');
                ciphertext[2 * i + 1] = (char) (d2 + '0');
                break;
            }
        }
        // Key not found
        if (j == TABLE_SIZE) {
            ciphertext[2 * i] = key;
            ciphertext[2 * i + 1] = '1';
        }
    }
    ciphertext[2 * len] = '\0';
    return ciphertext;
}

/* Function to create a Polybius table and conduct encryption */
char* cipher(const char* word) {
    // Initialize the Polybius table for encryption
    table_t table[TABLE_SIZE];
    if (table == NULL) {
        printf("Fail to allocate heap memory.\n");
        return NULL;
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

    char *edittext = pbEncode(word, table);
    return edittext;
}
