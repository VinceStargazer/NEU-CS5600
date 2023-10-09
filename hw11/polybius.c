#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "polybius.h"

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

/* This function returns a decoded version of the string ciphertext using the Polybius table */
char *pbDecode(const char *ciphertext, table_t *table) {
    size_t len = strlen(ciphertext);
    char *plaintext = (char*)malloc(len * sizeof(char) / 2 + 1);
    for (size_t i = 0; i < len / 2; i++) {
        char c1 = ciphertext[2 * i], c2 = ciphertext[2 * i + 1];
        if (c1 < '1' || c1 > '9' || c2 < '1' || c2 > '9') {
            plaintext[i] = c1;
            continue;
        }
        int value = (c1 - '0') * 10 + c2 - '0';
        int j = 0;
        while (j++ < TABLE_SIZE) {
            if (table[j].value == value) {
                plaintext[i] = table[j].key;
                break;
            }
        }
        // Value not found
        if (j == TABLE_SIZE) {
            printf("Fail to find the corresponding value in the table.\n");
            free(plaintext);
            return NULL;
        }
    }
    plaintext[len / 2] = '\0';
    return plaintext;
}
