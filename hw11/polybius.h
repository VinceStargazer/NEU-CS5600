typedef struct {
    char key;
    int value;
} table_t;

char *pbEncode(const char *plaintext, table_t **table);
char *pbDecode(const char *ciphertext, table_t **table);
char *handleInput(const char *input, const char *option);