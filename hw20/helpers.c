#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

// Function to exit the program with the given message
void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

// Function to send a string from socket
void sendString(int sockfd, const char *str)
{
    size_t len = strlen(str);
    if (send(sockfd, &len, sizeof(len), 0) < 0)
    {
        error("Error sending string length");
    }
    if (send(sockfd, str, len, 0) < 0)
    {
        error("Error sending string data");
    }
}

// Function to receive a string from socket
void receiveString(int sockfd, char **str)
{
    size_t len;
    if (recv(sockfd, &len, sizeof(len), 0) < 0)
    {
        error("Error receiving string length");
    }

    *str = (char *)malloc(len + 1); // +1 for null terminator
    if (*str == NULL)
    {
        error("Error allocating memory for string");
    }

    if (recv(sockfd, *str, len, 0) < 0)
    {
        error("Error receiving string data");
    }

    (*str)[len] = '\0'; // Null-terminate the received string
}