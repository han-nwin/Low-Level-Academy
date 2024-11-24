
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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

void handle_server(int fd) {
  char buff[BUFF_SIZE] = {0};
  proto_header_t * header = (proto_header_t *)buff; //work with stack space
  //read header and int data
  read(fd, buff, sizeof(proto_header_t) + sizeof(int));
  
  header->type = ntohs(header->type);
  header->len = ntohs(header->len);
  
  int *data = (int *)((char *)header + sizeof(proto_header_t)); //Point to the next memory address after the header
  *data = ntohs(*data);

  if (header->type != PROTO_HELLO) {
    printf("Wrong prototype %s\n", (char *) header->type);
    return;
  }

  if (*data != 1){
    printf("Wrong version %d\n", *data);
    return;
  }

  printf("Server connected successfully. Prototype: %d Version: %d\n", header->type, *data);
  
}

int main(int argc, char **argv) {

  if (argc != 2) {
    printf("Usage: %s <ip of the host>\n", argv[0]);
    return 1;
  }

  struct sockaddr_in serverInfo = {0};
  /**
   * memset(&serverInfo, 0, sizeof(serverInfo)); //more dynamic 
   * */

  serverInfo.sin_family = AF_INET; //IPv4
  // Set address using a string
   if (inet_pton(AF_INET, argv[1], &serverInfo.sin_addr) <= 0) {
      perror("Invalid address / Address not supported");
      return -1;
    }   
  serverInfo.sin_port = htons(PORT);

  //Create socket
  int fd = socket(AF_INET, SOCK_STREAM, 0); //Ipv4
  if (fd == -1) {
    perror("Socket failed");
    return -1;
  }

  //Connect
  if (connect(fd, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) == -1) {
    perror("Connect failed");
    close(fd);
    return -1;
  }
  printf("Connected successfully!\n");

  handle_server(fd);
  
  close(fd);//close socket

  return 0;
}
