#ifndef HELPERS_H
#define HELPERS_H

void error(const char *msg);
void sendString(int sockfd, const char *str);
void receiveString(int sockfd, char **str);

#endif