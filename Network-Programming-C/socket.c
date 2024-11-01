#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#define PORT 5555

int main() {
  
  //Initialize sock address in | need to cast to sockaddr later
  struct sockaddr_in serverInfo = {0}; // 0 out server info -> if not might fail at bind

  //Client socket 
  struct sockaddr_in clientInfo = {0};
  int clientSize = sizeof(clientInfo);

  serverInfo.sin_family = AF_INET; //IPv4
  serverInfo.sin_addr.s_addr = 0;
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

  //accept
  int cfd = accept(fd, (struct sockaddr*)&clientInfo, (socklen_t*)&clientSize);
  if (cfd == -1) {
    perror("Accept failed");
    return -1;
  }

  close(cfd); //close client fd

  return 0;
}
