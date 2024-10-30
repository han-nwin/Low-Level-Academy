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

union Data {
  int intValue;
  float floatValue;
  char charValue;
};

int main () {
  union Data data;

  data.intValue = 10;
  




  return 0;
}
