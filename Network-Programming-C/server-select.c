#include <stdio.h>
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
  struct sockaddr_in serverInfo; 
  struct sockaddr_in clientInfo; 
  int clientSize = sizeof(clientInfo);

  //Initialize memory for serverInfo and clientInfo
  memset(&serverInfo, 0, sizeof(serverInfo)); //more dynamic
  memset(&clientInfo, 0, sizeof(clientInfo)); //more dynamic

  //Initialize sets of read and write
  fd_set read_fds;
  fd_set write_fds;


  //Set connection protocol and port
  serverInfo.sin_family = AF_INET; //IPv4
  serverInfo.sin_addr.s_addr = INADDR_ANY;
  /** Set address using a string
   *if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
      perror("Invalid address / Address not supported");
      return -1;
    }   
    serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
    */
  serverInfo.sin_port = htons(PORT); // 0x12345678 0x87654321


  //Open a socket
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0); // IPv4, SOCK_STREAM

  if(listen_fd == -1) {
    perror("Socket failed");
    return -1;
  }


  //bind
  if (bind(listen_fd, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) == -1){
    perror("Bind Failed");
    return -1;
  }


  //listen
  if (listen(listen_fd, 0) == -1) {
    perror("listen failed");
    return -1;
  }

  //converting ip address to human-readable form
  char ip_str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &serverInfo.sin_addr, ip_str, INET_ADDRSTRLEN);
  printf("Server is listening to IP: %s port %d\n", ip_str, PORT);


  while (1) {

    //Clear the FD sets
    //000000000000000
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);

    // Add the listening socket to the read set
    FD_SET(listen_fd)


  }
  //return 0;
}
