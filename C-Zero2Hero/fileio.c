#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return -1;
  }

  int fd = open(argv[1], O_CREAT | O_RDONLY, 0644);
  if (fd == -1) {
    perror("open");
    exit(1);
  }

  return 0;

}

