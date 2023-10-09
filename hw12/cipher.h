#ifndef CIPHER_H
#define CIPHER_H

typedef struct {
    char key;
    int value;
} table_t;

char *pbEncode(const char *plaintext, table_t* table);
char* cipher(const char *word);

#endif