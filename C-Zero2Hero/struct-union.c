#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_IDS 32
#define MAX_EMPLPOYEES 100
struct employee_t {
  int id;
  char firstname[64];
  char lastname[64];
  float income;
  bool ismanager;

};

int main () {

  return 0;
}
