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
#include <arpa/inet.h>
#include <unistd.h>
#include "helpers.h"

#define MAX_BUFFER_SIZE 1024

void handleWrite(int client_sock) {
  // Receive client's remote file path
  char* local_file;
  receiveString(client_sock, &local_file);
  printf("File location: %s\n", local_file);

  // Open local file
  FILE* fp = fopen(local_file, "w"); 
  if (fp == NULL) {
    error("Error opening local file for writing");
  }

  // Free memory and close file
  free(local_file);
  fclose(fp);
  close(client_sock);
}

int main(void) {
  int socket_desc, client_sock;
  socklen_t client_size;
  struct sockaddr_in server_addr, client_addr;
  
  // Create socket:
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  
  if (socket_desc < 0) {
    error("Error while creating socket\n");
  }
  printf("Socket created successfully\n");
  
  // Set port and IP:
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(2000);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  // Bind to the set port and IP:
  if (bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    error("Couldn't bind to the port\n");
  }
  printf("Done with binding\n");
  
  // Listen for clients:
  if (listen(socket_desc, 1) < 0) {
    error("Error while listening\n");
  }
  printf("\nListening for incoming connections.....\n");
  
  // Accept an incoming connection:
  client_size = sizeof(client_addr);
  client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
  
  if (client_sock < 0) {
    error("Can't accept\n");
  }
  printf("Client connected at IP: %s and port: %i\n", 
         inet_ntoa(client_addr.sin_addr), 
         ntohs(client_addr.sin_port));
  
  // Receive client's action string
  char* action;
  receiveString(client_sock, &action);
  printf("Action from client: %s\n", action);
  free(action);
  
  if (strcmp(action, "WRITE") == 0) { // Question 1
    // Open local file
    handleWrite(client_sock);
  } else if (strcmp(action, "GET") == 0) { // Question 2

  } else if (strcmp(action, "RM") == 0) { // Question 3

  } else if (strcmp(action, "LS") == 0) { // Question 6

  }
  
  // Closing the socket:
  close(client_sock);
  close(socket_desc);
  
  return 0;
}
