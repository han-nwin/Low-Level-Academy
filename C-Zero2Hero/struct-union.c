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
    printf("Int Value: %d\n", data.intValue);

    data.floatValue = 3.14;
    printf("Float Value: %.2f\n", data.floatValue);

    data.charValue = 'A';
    printf("Char Value: %c\n", data.charValue);

    // Notice that only the last value assigned (charValue) is correctly printed
    printf("\nAfter all assignments:\n");
    printf("Int Value: %d\n", data.intValue);
    printf("Float Value: %.2f\n", data.floatValue);
    printf("Char Value: %c\n", data.charValue);




  return 0;
}
