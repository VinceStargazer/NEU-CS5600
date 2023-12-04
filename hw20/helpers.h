#ifndef HELPERS_H
#define HELPERS_H

void error(const char *msg);
void sendString(int sockfd, const char *str);
void receiveString(int sockfd, char **str);
int isFileExist(const char *file_name);
char *findFilePrefix(const char *file_name, char delimiter);
char *findFileSuffix(const char *file_name, char delimiter);
void createFileName(char *new_file, char *prev_file, int ver_num);

#endif