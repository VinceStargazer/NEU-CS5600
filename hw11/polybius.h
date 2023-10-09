#ifndef POLYBIUS_H
#define POLYBIUS_H

#define TABLE_SIZE 81

typedef struct {
    char key;
    int value;
} table_t;

char* pbEncode(const char* plaintext, table_t* table);
char* pbDecode(const char* ciphertext, table_t* table);

#endif