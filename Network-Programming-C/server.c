#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#define PORT 5555
#define BUFF_SIZE 4096

typedef enum {
  PROTO_HELLO,
  PROTO_WORLD,

} proto_type_e;

//TLV (type length value) system
typedef struct {
  proto_type_e type;
  unsigned short len;

} proto_header_t;

void handle_client(int fd) {
  char buff[BUFF_SIZE] = {0};
  proto_header_t * header = (proto_header_t *)buff; //work with stack space
  header->type = htons(PROTO_HELLO);
  header->len = htons(header->len);
  
  //turn header into array of data
  int *data = (int *)((char *)header + sizeof(proto_header_t)); //Point to the next memory address after the header
  //Set value for data
  *data = htons(1); //test sending data = (int) 1
  
  //write header and int data
  write(fd, header, sizeof(proto_header_t) + sizeof(int));
}

int main() {
  
  //Initialize sock address in | need to cast to sockaddr later
  struct sockaddr_in serverInfo = {0}; // 0 out server info -> if not might fail at bind

  //Client socket 
  struct sockaddr_in clientInfo = {0};
  int clientSize = sizeof(clientInfo);

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
  
  int fd = socket(AF_INET, SOCK_STREAM, 0); // IPv4, SOCK_STREAM
  if(fd == -1) {
    perror("Socket failed");
    return -1;
  }
  //bind
  if (bind(fd, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) == -1){
    perror("Bind Failed");
    return -1;
  }

  //listen
  if (listen(fd, 0) == -1) {
    perror("listen failed");
    return -1;
  }
  //converting ip address to human-readable form
  char ip_str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &serverInfo.sin_addr, ip_str, INET_ADDRSTRLEN);

  printf("Server is listening to IP: %s port %d\n", ip_str, PORT);

  while (1) {
    printf("Server waiting for connection...\n");
    //accept
    int cfd = accept(fd, (struct sockaddr*)&clientInfo, (socklen_t*)&clientSize);
    if (cfd == -1) {
      perror("Accept failed");
      return -1;
    }
    printf("A client connected...\n");
    handle_client(cfd);

    close(cfd); //close client fd
    printf("A client disconnected\n");
    printf("\n");
    sleep(1);
  }
  //return 0;
}
