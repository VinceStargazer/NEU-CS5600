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

// Function to handle write operation from the server side
void handleWrite(int client_sock)
{
  // Receive client's remote file path
  char *local_file;
  receiveString(client_sock, &local_file);

  // Find the correct version number
  char file_name[strlen(local_file) + 3];
  sprintf(file_name, "%s", local_file);
  int ver_num = 1;
  while (isFileExist(file_name))
  {
    sprintf(file_name, "%s_%d", local_file, ver_num);
    ver_num++;
  }
  
  // Open local file
  FILE *fp = fopen(file_name, "w");
  if (fp == NULL)
  {
    error("Error opening local file for writing");
  }

  // Read data from the client and write to the local file
  char *buffer[MAX_BUFFER_SIZE];
  ssize_t bytesRead;
  while ((bytesRead = recv(client_sock, buffer, sizeof(buffer), 0)) > 0)
  {
    fwrite(buffer, 1, bytesRead, fp);
  }

  // Free memory and close file
  free(local_file);
  fclose(fp);
}

// Function to handle get operation from the server side
void handleGet(int client_sock)
{
  // Receive client's remote file path
  char *local_file;
  receiveString(client_sock, &local_file);

  // Open local file
  FILE *fp = fopen(local_file, "r");
  if (fp == NULL)
  {
    error("Error opening local file for reading");
  }

  // Read from the local file and write to the remote file
  char buffer[MAX_BUFFER_SIZE];
  size_t bytesRead;
  while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0)
  {
    if (send(client_sock, buffer, bytesRead, 0) < 0)
    {
      error("Error sending data to the server");
    }
  }

  // Free memory and close file
  free(local_file);
  fclose(fp);
}

// Function to handle remove operation from the server side
void handleRemove(int client_sock)
{
  // Receive client's remote file path
  char *local_path;
  receiveString(client_sock, &local_path);

  // Use the system command to execute the remove operation
  char command[256];
  sprintf(command, "rm -rf %s", local_path);
  if (system(command) == 0)
  {
    printf("File '%s' removed successfully.\n", local_path);
  }
  else
  {
    error("Error removing file");
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
    error("Error getting file information");
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

  // Free memory and close socket
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
  server_addr.sin_port = htons(2000);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

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

  // Accept an incoming connection:
  client_size = sizeof(client_addr);
  client_sock = accept(socket_desc, (struct sockaddr *)&client_addr, &client_size);

  if (client_sock < 0)
  {
    error("Can't accept\n");
  }
  printf("Client connected at IP: %s and port: %i\n",
         inet_ntoa(client_addr.sin_addr),
         ntohs(client_addr.sin_port));

  // Receive client's action string
  char *action;
  receiveString(client_sock, &action);
  printf("Action from client: %s\n", action);

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
  else
  {
    error("Invalid action");
  }

  free(action);

  // Closing the socket:
  close(client_sock);
  close(socket_desc);

  return 0;
}
