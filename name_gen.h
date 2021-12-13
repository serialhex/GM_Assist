
#include <stdio.h>
#include <stdint.h>

#ifndef name_gen_h
#define name_gen_h

// enumeration of gender
#define REGISTER_ENUM(x) x,
typedef enum {
  #include "gender.h"
  Gender_Num
} Gender;
#undef REGISTER_ENUM

// strings of gender
#define REGISTER_ENUM(x) #x,
static const char* gender_names_text[] = {
  #include "gender.h"
  "Invalid"
};
#undef REGISTER_ENUM

typedef struct {
  char name[32]; // names are a max of 32 bytes long... cause i don't wanna malloc
  Gender gender;
} Name_Info;

typedef struct {
  size_t count;
  size_t capacity;
  char location[32];
  Name_Info* names;
} Name_Set;

typedef struct {
  size_t count;
  size_t capacity;
  Name_Set** sets;
} Set_of_Name_sets;

// Prototypes
void do_name_gen();
#endif

////////////////////////////////////////////////////////////////////////////////
// Implementation

#ifdef name_gen_implementation

#include "dirent.h"
#include "mem.h"
#include "quickrng.h"

////////////////////////////////////////////////////////////////////////////////
// GLOBS!!!!
static Set_of_Name_sets* name_sets = NULL;
static pcg32_random_t rng = { .state = 42, .inc = 107 };

////////////////////////////////////////////////////////////////////////////////
// NameSet init & stuff
Name_Set* init_name_set() {
  Name_Set* set = (Name_Set*)malloc(sizeof(Name_Set));
  set->count = 0;
  set->capacity = 0;
  memset(set->location, 0, sizeof(set->location));
  set->names = NULL;

  return set;
}

void free_name_set(Name_Set* set) {
  FREE_ARRAY(Name_Set, set->names, set->capacity);
  init_name_set(set);
}

void add_to_name_set(Name_Set* set, Name_Info name) {
  if (set->capacity < set->count + 1) {
    size_t oldCapacity = set->capacity;
    set->capacity = GROW_CAPACITY(oldCapacity);
    set->names = GROW_ARRAY(Name_Info, set->names,
        oldCapacity, set->capacity);
  }

  set->names[set->count] = name;
  set->count++;
}

char* random_name_from_set(Name_Set* set) {
  uint32_t r = pcg32_random_r(&rng);
  return set->names[fastrange(r, set->count)].name;
}

// Sets_of_Name_sets init & stuff
void init_set_of_name_sets(Set_of_Name_sets* set) {
  set->count = 0;
  set->capacity = 0;
  set->sets = NULL;
}

void free_set_of_name_sets(Set_of_Name_sets* set) {
  FREE_ARRAY(Set_of_Name_sets, set->sets, set->capacity);
  init_set_of_name_sets(set);
}

void add_to_set_of_name_sets(Set_of_Name_sets* sets, Name_Set* names) {
  if (names == NULL) { return; }
  if (sets->capacity < sets->count + 1) {
    size_t oldCapacity = sets->capacity;
    sets->capacity = GROW_CAPACITY(oldCapacity);
    sets->sets = GROW_ARRAY(Name_Set*, sets->sets,
        oldCapacity, sets->capacity);
  }

  sets->sets[sets->count] = names;
  sets->count++;
}

////////////////////////////////////////////////////////////////////////////////
// funcitonal functionality!
static Name_Set* load_btn(char* filename) {
  char path[1024];
  strcpy(path, "names/btn/"); // make sure it's fresh!
  strcat(path, filename);
  puts(path);

  FILE* file = fopen(path, "rb");
  if (file == NULL) {
    printf("Error opening file `%s`; quitting!", path);
    exit(1);
  }

  Name_Set* names = init_name_set();

  #define Buffer_Size 4096
  char buffer[Buffer_Size - 1];
  fgets(buffer, Buffer_Size, file); // grab the first line and toss it, as it's CSV stuff

  while (fgets(buffer, Buffer_Size, file)) {
    Name_Info info;
    char gender;

    memset(info.name, 0, sizeof(info.name));

    // NAME	SCRIPT	LOCATION	MEANING	GENDER
    sscanf(buffer, "%32s", info.name);
    int len = strlen(buffer);
    gender = buffer[len - 3];

    switch (gender) {
      case 'm': {
        info.gender = male;
      } break;

      case 'f': {
        info.gender = female;
      } break;

      case 'a': {
        info.gender = either;
      } break;

      default: {
        info.gender = unknown;
      } break;
    }

    add_to_name_set(names, info);
  }

  #undef Buffer_Size
  fclose(file);
  return names;
}

static Name_Set* load_momj(char* filename) {
  char path[1024];
  strcpy(path, "names/momj/"); // make sure it's fresh!
  strcat(path, filename);
  puts(path);

  FILE* file = fopen(path, "rb");
  if (file == NULL) {
    printf("Error opening file `%s`; quitting!", path);
    exit(1);
  }

  Name_Set* names = init_name_set();
  strcpy(names->location, filename);
  names->location[strlen(filename) - 4] = 0;

  #define Buffer_Size 4096
  char buffer[Buffer_Size - 1];
  fgets(buffer, Buffer_Size, file); // grab the first line and toss it, as it's CSV stuff

  while (fgets(buffer, Buffer_Size, file)) {
    Name_Info info;
    char gender;

    memset(info.name, 0, sizeof(info.name));

    // NAMES	MEANING	GENDER
    sscanf(buffer, "%32s", info.name);
    int len = strlen(buffer);
    gender = buffer[len - 1];

    switch (gender) {
      case 'm': {
        info.gender = male;
      } break;

      case 'f': {
        info.gender = female;
      } break;

      case 'e': {
        info.gender = either;
      } break;

      default: {
        info.gender = unknown;
      } break;
    }

    add_to_name_set(names, info);
  }

  #undef Buffer_Size
  fclose(file);
  return names;
}

static void load_name_sets() {
  if (name_sets != NULL) {
    return;
  }

  name_sets = (Set_of_Name_sets*)malloc(sizeof(Set_of_Name_sets));
  init_set_of_name_sets(name_sets);

  DIR* dir;
  struct dirent* here;

  ////////////
  // names/btn
/*
  dir = opendir("names/btn");

  if (dir == NULL) {
    puts("couldn't open 'names/btn', bad things are happening!!!");
    exit(1);
  }

  while (here = readdir(dir)) {
    int len = strlen(here->d_name);

    if (len < 4) {
      continue;
    }

    if (!strcmp(&here->d_name[len - 4], ".tzv")){
      //The String ends with ".tzv"
      add_to_set_of_name_sets(name_sets, load_btn(here->d_name));
    }
  }

  closedir(dir);
*/

  /////////////
  // names/momj
  dir = opendir("names/momj");

  if (dir == NULL) {
    puts("couldn't open 'names/momj', bad things are happening!!!");
    exit(1);
  }

  while (here = readdir(dir)) {
    int len = strlen(here->d_name);

    if (len < 4) {
      continue;
    }

    if (!strcmp(&here->d_name[len - 4], ".tsv")){
      //The String ends with ".tzv"
      add_to_set_of_name_sets(name_sets, load_momj(here->d_name));
    }
  }

  closedir(dir);

}

size_t show_namesets() {
  size_t i = 0;
  for (; i < name_sets->count; i++) {
    printf("%zu - %s", i, name_sets->sets[i]->location);
    if (i & 1) {
      printf("\n");
    } else {
      printf("\t\t");
    }
  }

  printf("%zu - quit\n", i+1);
  return i;
}

void do_name_gen() {
  load_name_sets();

  printf("got %u name sets!\n", name_sets->count);

  const int64_t total_namesets = (int64_t)show_namesets();
  int64_t selection;
  char buffer[1024];

  while(1) {
    puts("What is your selection?");
    gets(buffer);
    selection = atoi(buffer);
    if (selection <= total_namesets) {
      printf("you selected: %s, here's 20 names\n", name_sets->sets[selection]->location);
      for (uint32_t i = 0; i < 20; i++) {
        printf("  %s\n", random_name_from_set(name_sets->sets[selection]));
      }
      show_namesets();
    } else {
      break;
    }
  }

}

#endif
