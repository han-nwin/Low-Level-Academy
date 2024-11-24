#include <stdio.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>


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

void init_client() {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    
  }
}
