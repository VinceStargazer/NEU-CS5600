/*
 * server.c -- TCP Socket Server
 *
 * adapted from:
 *   https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include "helpers.h"

#define MAX_BUFFER_SIZE 1024
#define VER_BUFFER_SIZE 256
#define PORT_NUMBER 2000
#define IP_ADDRESS "127.0.0.1"

// Function to determine whether a file name already exists
int isFileExist(char *file_name)
{
  FILE *fp = fopen(file_name, "r");
  if (fp == NULL)
  {
    return 0;
  }
  fclose(fp);
  return 1;
}

// Function to find and return the prefix of file name
char *findPrefix(const char *file_name)
{
  const char *dot = strrchr(file_name, '.');
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

// Function to find and return the suffix of file name
char *findSuffix(const char *file_name)
{
  const char *dot = strrchr(file_name, '.');
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
  char *prefix = findPrefix(prev_file);
  if (prefix == NULL)
  { // When there is no file suffix
    sprintf(new_file, "%s_%d", prev_file, ver_num);
  }
  else
  { // When there is a file suffix
    char *suffix = findSuffix(prev_file);
    sprintf(new_file, "%s_%d.%s", prefix, ver_num, suffix);
    free(prefix);
    free(suffix);
  }
}

// Function to get the latest version number by file name
int getLatestVersion(char *file_name)
{
  FILE *fp = fopen("remote_files/version_info", "r");
  if (fp == NULL)
  {
    error("Error opening version info file");
  }
  char line[VER_BUFFER_SIZE];
  while (fgets(line, VER_BUFFER_SIZE, fp))
  {
    // parse the input line by '=' to read key and value
    char *equals = strchr(line, '=');
    if (equals != NULL)
    {
      *equals = '\0';
      if (strcmp(line, file_name) == 0)
      {
        fclose(fp);
        return atoi(equals + 1);
      }
    }
  }
  fclose(fp);
  return 0;
}

// Function to update the latest version by file name
void updateLatestVersion(char *file_name, int ver_num)
{
  FILE *fp = fopen("remote_files/version_info", ver_num == 1 ? "a" : "r+");
  if (fp == NULL)
  {
    error("Error opening version info file");
  }

  if (ver_num == 1)
  {
    fprintf(fp, "%s=%d\n", file_name, ver_num);
    fclose(fp);
    return;
  }

  char search_key[strlen(file_name) + 1];
  sprintf(search_key, "%s=", file_name);
  char line[VER_BUFFER_SIZE];
  while (fgets(line, VER_BUFFER_SIZE, fp))
  {
    if (strncmp(line, search_key, strlen(search_key)) == 0)
    {
      // Rewind the file pointer to the beginning of the line
      fseek(fp, -strlen(line), SEEK_CUR);

      // Replace the old version with the new version
      fprintf(fp, "%s%d\n", search_key, ver_num);

      // Move the file pointer back to where it was
      fseek(fp, 0, SEEK_CUR);
      break;
    }
  }

  fclose(fp);
}

// Print a success message and send it to client
void handleSuccess(int client_sock, const char *message)
{
  printf("%s\n", message);
  sendString(client_sock, message);
}

// Print a failure message and send it to client
void handleFailure(int client_sock, const char *message)
{
  perror(message);
  sendString(client_sock, message);
}

// Function to handle write operation from the server side
void handleWrite(int client_sock)
{
  // Receive client's remote file path
  char *local_file;
  receiveString(client_sock, &local_file);

  // Find the latest version number (Question 5)
  char *file_name = (char *)malloc(strlen(local_file) + 3);
  if (file_name == NULL)
  {
    handleFailure(client_sock, "Memory allocation failed");
    return;
  }

  if (!isFileExist(local_file))
  {
    createFileName(file_name, local_file, 0);
  }
  else
  {
    int ver_num = getLatestVersion(local_file) + 1;
    createFileName(file_name, local_file, ver_num);
    updateLatestVersion(local_file, ver_num);
  }

  // Open local file
  FILE *fp = fopen(file_name, "w");
  if (fp == NULL)
  {
    handleFailure(client_sock, "Error opening local file for writing");
    return;
  }

  // Read data from the client and write to the local file
  char buffer[MAX_BUFFER_SIZE];
  ssize_t bytesRead;
  while ((bytesRead = recv(client_sock, buffer, sizeof(buffer), 0)) > 0)
  {
    fwrite(buffer, 1, bytesRead, fp);
  }
  printf("Successfully writing to file '%s'\n", file_name);

  // Free memory and close file
  free(local_file);
  free(file_name);
  fclose(fp);
}

// Function to handle get operation from the server side
void handleGet(int client_sock)
{
  // Receive client's remote file path
  char *local_file;
  receiveString(client_sock, &local_file);

  // Get the latest version of a file by default (Question 7)
  char *file_name = (char *)malloc(strlen(local_file) + 3);
  if (file_name == NULL)
  {
    error("Memory allocation failed");
  }
  int ver_num = getLatestVersion(local_file);
  createFileName(file_name, local_file, ver_num);

  // Open local file
  FILE *fp = fopen(file_name, "r");
  if (fp == NULL)
  {
    error(file_name);
    error("Error opening local file for reading");
    return;
  }

  // Read from the local file and write to the remote file
  char buffer[MAX_BUFFER_SIZE];
  size_t bytesRead;
  while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0)
  {
    if (send(client_sock, buffer, bytesRead, 0) < 0)
    {
      handleFailure(client_sock, "Error sending data to the client");
      return;
    }
  }
  printf("Successfully reading from file '%s'\n", file_name);

  // Free memory and close file
  free(local_file);
  free(file_name);
  fclose(fp);
}

// Function to handle remove operation from the server side
void handleRemove(int client_sock)
{
  // Receive client's remote file path
  char *local_path;
  receiveString(client_sock, &local_path);

  if (!isFileExist(local_path))
  {
    handleFailure(client_sock, "File not exist");
  }

  // Use the system command to execute the remove operation
  char command[256];
  sprintf(command, "rm -rf %s", local_path);

  if (system(command) == 0)
  {
    char message[VER_BUFFER_SIZE];
    sprintf(message, "File '%s' removed successfully", local_path);
    handleSuccess(client_sock, message);
  }
  else
  {
    handleFailure(client_sock, "Error removing file");
  }

  // Free memory
  free(local_path);
}

// Function to handle list operation from the server side
void handleList(int client_sock)
{
  // Receive client's remote file path
  char *local_file;
  receiveString(client_sock, &local_file);
  struct stat file_stat;

  // Get file information
  if (stat(local_file, &file_stat) < 0)
  {
    char err[] = "Error removing file";
    perror(err);
    sendString(client_sock, err);
    return;
  }

  // Construct versioning information
  char version_info[MAX_BUFFER_SIZE];
  snprintf(version_info, sizeof(version_info), "File: %s\nSize: %ld bytes\nCreated: %sModified: %sAccessed: %s",
           local_file,
           file_stat.st_size,
           ctime(&file_stat.st_ctime),
           ctime(&file_stat.st_mtime),
           ctime(&file_stat.st_atime));

  // Send versioning information to the client
  send(client_sock, version_info, strlen(version_info), 0);
  char message[VER_BUFFER_SIZE];
  sprintf(message, "Listing %s info successfully\n", local_file);
  handleSuccess(client_sock, message);

  // Free memory
  free(local_file);
}

int main(void)
{
  int socket_desc, client_sock;
  socklen_t client_size;
  struct sockaddr_in server_addr, client_addr;

  // Create socket:
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_desc < 0)
  {
    error("Error while creating socket\n");
  }
  printf("Socket created successfully\n");

  // Set port and IP:
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT_NUMBER);
  server_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

  // Bind to the set port and IP:
  if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    error("Couldn't bind to the port\n");
  }
  printf("Done with binding\n");

  // Listen for clients:
  if (listen(socket_desc, 1) < 0)
  {
    error("Error while listening\n");
  }
  printf("\nListening for incoming connections.....\n");

  // Accept incoming connections:
  while (1)
  {
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr *)&client_addr, &client_size);

    if (client_sock < 0)
    {
      perror("Can't accept\n");
      continue;
    }
    printf("\nClient connected at IP: %s and port: %i\n",
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    // Receive client's action string
    char *action;
    receiveString(client_sock, &action);
    printf("Action attempt from client: %s\n", action);

    if (strcmp(action, "WRITE") == 0)
    { // Question 1
      handleWrite(client_sock);
    }
    else if (strcmp(action, "GET") == 0)
    { // Question 2
      handleGet(client_sock);
    }
    else if (strcmp(action, "RM") == 0)
    { // Question 3
      handleRemove(client_sock);
    }
    else if (strcmp(action, "LS") == 0)
    { // Question 6
      handleList(client_sock);
    }
    else if (strcmp(action, "STOP") == 0)
    { // Turn off the server
      handleSuccess(client_sock, "Server terminated by client");
      free(action);
      close(client_sock);
      break;
    }
    else
    {
      perror("Invalid action");
    }

    free(action);

    // Closing the client socket:
    close(client_sock);
  }

  close(socket_desc);

  return 0;
}
