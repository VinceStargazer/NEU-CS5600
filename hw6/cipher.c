#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "polybius.h"

int main(int argc, char *argv[]) {
    // validate the arguments
    if (argc != 3) {
        printf("Usage: %s [-e/-d] \"string\"\n", argv[0]);
        return -1;
    }
    if (strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-d") != 0) {
        printf("Usage: %s [-e/-d] \"%s\"\n", argv[0], argv[2]);
        return -1;
    }

    // initialize the Polybius table for encryption
    table_t** table = (table_t**)malloc(81 * sizeof(table_t*));
    if (table == NULL) {
        printf("Fail to allocate heap memory.\n");
        return -1;
    }
    int rows[9] = {7, 3, 1, 9, 4, 2, 8, 5, 6}, cols[9] = {5, 4, 6, 1, 8, 3, 2, 9, 7};
    // special characters are added to the table
    for (char c = ' '; c <= '`'; c++) {
        int idx = c - ' ', row = idx / 9, col = idx % 9;
        table[idx] = (table_t*)malloc(sizeof(table_t));
        if (table[idx] == NULL) {
            printf("Fail to allocate heap memory.\n");
            return -1;
        }
        table[idx]->key = c;
        table[idx]->value = rows[row] * 10 + cols[col];
    }
    for (int i = '`' - ' ' + 1; i < '`' - ' ' + 5; i++) {
        int row = i / 9, col = i % 9;
        table[i] = (table_t*)malloc(sizeof(table_t));
        if (table[i] == NULL) {
            printf("Fail to allocate heap memory.\n");
            return -1;
        }
        table[i]->key = (char) ('{' + i - '`' + ' ' - 1);
        table[i]->value = rows[row] * 10 + cols[col];
    }

    if (strcmp(argv[1], "-e") == 0) {
        char *ciphertext = pbEncode(argv[2], table);
        if (ciphertext == NULL) {
            return -1;
        }
        printf("%s\n", ciphertext);
        free(ciphertext);
    } else {
        char *plaintext = pbDecode(argv[2], table);
        if (plaintext == NULL) {
            return -1;
        }
        printf("%s\n", plaintext);
        free(plaintext);
    }

    // free the allocated heap memory
    for (int i = 0; i < 81; i++) {
        free(table[i]);
    }
    free(table);
    return 0;
}