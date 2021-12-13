#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define vanilla_array_implementation
#include "mem.h"
#undef vanilla_array_implementation

#define quickrng_implementation
#include "quickrng.h"
#undef quickrng_implementation

// some constants
#define LineMax 4096

// enumeration of functionality
#define REGISTER_ENUM(x) x,
typedef enum {
  #include "functionality.h"
  Functionality_Num
} Functionality;
#undef REGISTER_ENUM

// strings of functionality
#define REGISTER_ENUM(x) #x,
static const char* functionality_names_text[] = {
  #include "functionality.h"
  "Invalid"
};
#undef REGISTER_ENUM

Functionality parse(char* line);
void usage();

// various functionality
#define name_gen_implementation
#include "name_gen.h"
#undef name_gen_implementation

bool loop; // globals are fun!

void main() {
  loop = true;
  char line[LineMax];

  printf("Welcome! What is your request? ");

  while (loop) {
    gets(line);
    switch (parse(line)) {

      case name: {
        do_name_gen();
      } break;

      case dice: {
        puts("no dice...");
      } break;

      case quit: {
        puts("mkay!");
        loop = false;
        continue;
      } break;

      default: {
        usage();
      } break;
    }

    printf("Next request please: ");
  }
  // ze end!
}



Functionality parse(char* line) {
  for (uint64_t i = 0; i < Functionality_Num; i++) {
    if (strcmp(line, functionality_names_text[i]) == 0) {
      return i;
    }
  }

  return Functionality_Num;
}

void usage() {
  puts("\tI don't know what that is, maybe this will help?");
  puts("\tThis is what I can do:");
  for (uint64_t i = 0; i < Functionality_Num; i++) {
    printf("\t\t");
    puts(functionality_names_text[i]);
  }
  puts("\tand that's it");
}
