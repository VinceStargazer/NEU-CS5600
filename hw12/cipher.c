#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cipher.h"

/* This function returns an encoded version of the string plaintext using the Polybius table */
char *pbEncode(const char *plaintext, table_t **table) {
    size_t len = strlen(plaintext);
    char *ciphertext = (char*)malloc(2 * len * sizeof(char) + 1);
    for (size_t i = 0; i < len; i++) {
        char key = plaintext[i];
        // lower case letters are treated as upper case
        if (key >= 'a' && key <= 'z') {
            key = (char) (key - 'a' + 'A');
        }
        table_t **temp = table;
        while (*temp != NULL) {
            table_t *tp = *temp;
            if (tp->key == key) {
                int value = tp->value;
                int d1 = value / 10, d2 = value % 10;
                ciphertext[2 * i] = (char) (d1 + '0');
                ciphertext[2 * i + 1] = (char) (d2 + '0');
                break;
            }
            temp++;
        }
        if (*temp == NULL) {
            ciphertext[2 * i] = key;
            ciphertext[2 * i + 1] = '1';
        }
    }
    ciphertext[2 * len] = '\0';
    return ciphertext;
}

/* This function returns a decoded version of the string ciphertext using the Polybius table */
char *pbDecode(const char *ciphertext, table_t **table) {
    size_t len = strlen(ciphertext);
    char *plaintext = (char*)malloc(len * sizeof(char) / 2 + 1);
    for (size_t i = 0; i < len / 2; i++) {
        char c1 = ciphertext[2 * i], c2 = ciphertext[2 * i + 1];
        if (c1 < '1' || c1 > '9' || c2 < '1' || c2 > '9') {
            plaintext[i] = c1;
            continue;
        }
        int value = (c1 - '0') * 10 + c2 - '0';
        table_t **temp = table;
        while (*temp != NULL) {
            table_t *tp = *temp;
            if (tp->value == value) {
                plaintext[i] = tp->key;
                break;
            }
            temp++;
        }
        if (*temp == NULL) {
            printf("Fail to find the corresponding value in the table.\n");
            free(plaintext);
            return NULL;
        }
    }
    plaintext[len / 2] = '\0';
    return plaintext;
}

/* Function to create a Polybius table and conduct encryption */
char* cipher(const char* word) {
    // Initialize the Polybius table for encryption
    table_t** table = (table_t**)malloc(81 * sizeof(table_t*));
    if (table == NULL) {
        printf("Fail to allocate heap memory.\n");
        return NULL;
    }
    int rows[9] = {7, 3, 1, 9, 4, 2, 8, 5, 6}, cols[9] = {5, 4, 6, 1, 8, 3, 2, 9, 7};
    // Special characters are added to the table
    for (char c = ' '; c <= '`'; c++) {
        int idx = c - ' ', row = idx / 9, col = idx % 9;
        table[idx] = (table_t*)malloc(sizeof(table_t));
        if (table[idx] == NULL) {
            printf("Fail to allocate heap memory.\n");
            return NULL;
        }
        table[idx]->key = c;
        table[idx]->value = rows[row] * 10 + cols[col];
    }
    for (int i = '`' - ' ' + 1; i < '`' - ' ' + 5; i++) {
        int row = i / 9, col = i % 9;
        table[i] = (table_t*)malloc(sizeof(table_t));
        if (table[i] == NULL) {
            printf("Fail to allocate heap memory.\n");
            return NULL;
        }
        table[i]->key = (char) ('{' + i - '`' + ' ' - 1);
        table[i]->value = rows[row] * 10 + cols[col];
    }

    char *edittext = pbEncode(word, table);
    // free the allocated heap memory
    for (int i = 0; i < 81; i++) {
        free(table[i]);
    }
    free(table);
    return edittext;
}
