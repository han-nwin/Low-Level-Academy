#include <stdio.h>
#include <sys/_types/_socklen_t.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLIENTS 1024
#define PORT 8080
#define BUFF_SIZE 4096

typedef enum {
  STATE_NEW,
  STATE_CONNECTED,
  STATE_DISCONNECTED,
} state_e;

//Structure to hold client state
typedef struct {
  int fd;
  state_e state;
  char buffer[4096];
} clientstate_t;


clientstate_t clientStates[MAX_CLIENTS];

//Initialize client array
void init_client() {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    clientStates[i].fd = -1;
    clientStates[i].state = STATE_NEW;
    memset(clientStates[i].buffer, '\0', BUFF_SIZE);
  }
}


//Find slot for new client
int find_free_slot() {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clientStates[i].fd == -1) {
      return i; //return the index of the free slot
    }
  }
  return -1; //full
}


//Main function
int main() {
  
  int num_fds; //track number of fd
  int freeSlot; //track free slot

  //Initialize sock address in | need to cast to sockaddr later
  struct sockaddr_in server_address; 
  struct sockaddr_in client_address; 
  socklen_t client_size = sizeof(client_address);

  //Initialize memory for serverInfo and clientInfo
  memset(&server_address, 0, sizeof(server_address)); //more dynamic
  memset(&client_address, 0, sizeof(client_address)); //more dynamic

  //Initialize sets of read and write
  fd_set read_fds;
  fd_set write_fds;


  //Set connection protocol and port
  server_address.sin_family = AF_INET; //IPv4
  server_address.sin_addr.s_addr = INADDR_ANY;
  /** Set address using a string
   *if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
      perror("Invalid address / Address not supported");
      return -1;
    }   
    serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
    */
  server_address.sin_port = htons(PORT); // 0x12345678 0x87654321


  //Open a socket
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0); // IPv4, SOCK_STREAM

  if(listen_fd == -1) {
    perror("Socket failed");
    return -1;
  }


  //bind
  if (bind(listen_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1){
    perror("Bind Failed");
    return -1;
  }


  //listen. Backlog 10 connections
  if (listen(listen_fd, 10) == -1) {
    perror("listen failed");
    return -1;
  }

  //converting ip address to human-readable form
  char ip_str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &server_address.sin_addr, ip_str, INET_ADDRSTRLEN);
  printf("Server is listening to IP: %s port %d\n", ip_str, PORT);


  while (1) {

    //Clear the FD sets
    //000000000000000
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);

    // Add the listening socket to the read set
    FD_SET(listen_fd, &read_fds); //000000010010100
    num_fds = listen_fd + 1; //set number of fd to the maximum value of fd + 1

    //Add active clients to read set
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (clientStates[i].fd != -1) {
        FD_SET(clientStates[i].fd, &read_fds);
        if (clientStates[i].fd > num_fds) {
          num_fds = clientStates[i].fd + 1; // update num fd if needed
        }
      }
    }

    //Wait for an activity on one of the sockets
    if (select (num_fds, &read_fds, &write_fds, NULL, NULL) == -1) {
      perror("select");
      return -1;
    }

    //Check for new connection then connect(accept)
    if (FD_ISSET(listen_fd, &read_fds)) {
      if (accept(listen_fd, (struct sockaddr*)&client_address, &client_size) == -1) {
        perror("accept");
        return -1;
      }
    }


  }
  //return 0;
}
