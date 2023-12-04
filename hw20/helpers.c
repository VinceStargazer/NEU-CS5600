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

// Function to determine whether a file name already exists
int isFileExist(const char *file_name)
{
  FILE *fp = fopen(file_name, "r");
  if (fp == NULL)
  {
    return 0;
  }
  fclose(fp);
  return 1;
}

// Function to find and return the prefix of a file name by delimiter
char *findFilePrefix(const char *file_name, char delimiter)
{
  const char *dot = strrchr(file_name, delimiter);
  if (dot == NULL || dot == file_name)
  {
    return NULL;
  }
  size_t len = dot - file_name;
  char *prefix = (char *)malloc(len + 1);
  if (prefix == NULL)
  {
    error("Memory allocation failed");
  }
  strncpy(prefix, file_name, len);
  prefix[len] = '\0';
  return prefix;
}

// Function to find and return the suffix of a file name by delimiter
char *findFileSuffix(const char *file_name, char delimiter)
{
  const char *dot = strrchr(file_name, delimiter);
  if (dot == NULL || dot == file_name)
  {
    return NULL;
  }
  char *suffix = (char *)malloc(strlen(dot + 1) + 1);
  if (suffix == NULL)
  {
    error("Memory allocation failed");
  }
  strcpy(suffix, dot + 1);
  suffix[strlen(dot + 1)] = '\0';
  return suffix;
}

// Function to create a versioned file name by name (prefix) and version number (suffix)
void createFileName(char *new_file, char *prev_file, int ver_num)
{
  if (ver_num == 0)
  {
    sprintf(new_file, "%s", prev_file);
    new_file[strlen(prev_file) + 1] = '\0';
    return;
  }
  char *prefix = findFilePrefix(prev_file, '.');
  if (prefix == NULL)
  { // When there is no file suffix
    sprintf(new_file, "%s_%d", prev_file, ver_num);
  }
  else
  { // When there is a file suffix
    char *suffix = findFileSuffix(prev_file, '.');
    sprintf(new_file, "%s_%d.%s", prefix, ver_num, suffix);
    free(prefix);
    free(suffix);
  }
}