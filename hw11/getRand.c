#include <stdio.h>
#include <string.h>
#include <time.h>

// LCG by glibc standard
#define LCG_MULTIPLIER 1103515245
#define LCG_INCREMENT 12345
#define LCG_MODULUS 0xFFFFFFFF

unsigned lcgSeed;

/*  This function returns a random value within the interval [floor, ceil] */
unsigned getRand(unsigned floor, unsigned ceil) {
    lcgSeed = (LCG_MULTIPLIER * lcgSeed + LCG_INCREMENT) % LCG_MODULUS;
    return floor + lcgSeed % (ceil - floor + 1);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s (-a) <number> <file_name>\n", argv[0]);
        return -1;
    }
    if (argc == 4 && strcmp(argv[1], "-a") != 0) {
        printf("Usage: %s (-a) <number> <file_name\n", argv[0]);
        return -1;
    }
    char *numStr = argc == 3 ? argv[1] : argv[2];
    int num = 0;
    for (char *pt = numStr; *pt != '\0'; pt++) {
        if (*pt >= '0' && *pt <= '9') {
            num = 10 * num + *pt - '0';
        } else {
            printf("Usage: %s (-a) <number> <file_name>\n", argv[0]);
            return -1;
        }
    }
    char *file_name = argc == 3 ? argv[2] : argv[3];
    FILE *file = argc == 3 ? fopen(file_name, "w") : fopen(file_name, "a");
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }
    // Open Unix built-in dictionary
    FILE *dict = fopen("/usr/share/dict/words", "r");
    if (dict == NULL) {
        perror("Failed to open dictionary");
        return -1;
    }
    // Calculate the words in dictionary
    int lineCount = 0;
    char buffer[BUFSIZ];
    while (fgets(buffer, sizeof(buffer), dict) != NULL) {
        lineCount++;
    }

    // Set the seed with current time
    lcgSeed = time(NULL);
    for (int i = 0; i < num; i++) {
        // Generate a random line number in the file
        int randomLine = getRand(0, lineCount - 1);
        // Read the word at the random line number
        fseek(dict, 0, SEEK_SET);
        char word[100];
        for (int j = 0; j < randomLine; j++) {
            if (fgets(word, sizeof(word), dict) == NULL) {
                perror("Error reading dictionary file");
                fclose(dict);
                fclose(file);
                return -1;
            }
        }
        // Print the random word
        fprintf(file, "%s", word);
    }
    fclose(dict);
    fclose(file);
    return 0;
}