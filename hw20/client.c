/*
 * client.c -- TCP Socket Client
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

// Function to create a socket and send the action
int create_socket(char* action) {
  int socket_desc;
  struct sockaddr_in server_addr;
  // Create socket:
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  
  if (socket_desc < 0) {
    error("Unable to create socket\n");
  }
  printf("Socket created successfully\n");
  
  // Set port and IP the same as server-side:
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(2000);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // Send connection request to server:
  if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    error("Unable to connect\n");
  }
  printf("Connected with server successfully\n");

  // Send action type to server
  sendString(socket_desc, action);
  return socket_desc;
}

// Function to handle write operation
void handleWrite(char* local_file, char* remote_file) {
    // Open local file
    FILE* fp = fopen(local_file, "r"); 
    if (fp == NULL) {
      error("Error opening local file for reading");
    }

    // Create a socket
    int sockfd = create_socket("WRITE");
    sendString(sockfd, remote_file);

    // Close file and socket
    fclose(fp);
    close(sockfd);
}

int main(int argc, char* argv[]) {
  // Validate arguments
  if (argc < 3) {
    error("Insufficient arguments");
  }
  char* action = argv[1];

  if (strcmp(action, "WRITE") == 0) { // Question 1
    if (argc != 4) {
      error("Usage: ./rfs WRITE <local-file-path> <remote-file-path>");
    }
    handleWrite(argv[2], argv[3]);
  } else if (strcmp(action, "GET") == 0) { // Question 2
    if (argc != 4) {
      error("Usage: ./rfs GET <remote-file-path> <local-file-path>");
    }

    // Create a socket
    int sockfd = create_socket(action);

    close(sockfd);
  } else if (strcmp(action, "RM") == 0) { // Question 3
    if (argc != 3) {
      error("Usage: ./rfs RM <remote-file-path>");
    }

    // Create a socket
    int sockfd = create_socket(action);

    close(sockfd);
  } else if (strcmp(action, "LS") == 0) { // Question 6
    if (argc != 3) {
      error("Usage: ./rfs LS <remote-file-path>");
    }

    // Create a socket
    int sockfd = create_socket(action);

    close(sockfd);
  } else {
    error("Invalid action");
  }
  
  // // Send the message to server:
  // if (send(socket_desc, client_message, strlen(client_message), 0) < 0) {
  //   printf("Unable to send message\n");
  //   return -1;
  // }
  
  // // Receive the server's response:
  // if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0) {
  //   printf("Error while receiving server's msg\n");
  //   return -1;
  // }
  
  // printf("Server's response: %s\n",server_message);
  
  // // Close the socket:
  // close(socket_desc);
  
  return 0;
}
