#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "mem.h"

////////////////////////////////////////////////////////////////////////////////
// Dice related things
typedef struct {
  uint32_t number;
  uint32_t sides;
  char* tag;
} Dice;

typedef struct {
  size_t count;
  size_t capacity;
  Dice* dice;
} DiceSet;

void init_dice_set(DiceSet* set) {
  set->count = 0;
  set->capacity = 0;
  set->dice = NULL;
}

void free_dice_set(DiceSet* set) {
  FREE_ARRAY(Dice, set->dice, set->capacity);
  init_dice_set(set);
}

void add_to_dice_set(DiceSet* set, Dice dice) {
  if (set->capacity < set->count + 1) {
    size_t oldCapacity = set->capacity;
    set->capacity = GROW_CAPACITY(oldCapacity);
    set->dice = GROW_ARRAY(Dice, set->dice,
        oldCapacity, set->capacity);
  }

  set->dice[set->count] = dice;
  set->count++;
}

bool do_dice(char* line) {
  DiceSet set;
  init_dice_set(&set);
  for (size_t i = 0; line[i] && (i < LineMax); i++) {
    // remove whitespace
    while (isspace(line[i])) {
      i++; // yeah, i'm doin' the crazy!
    }

    uint32_t n = 0; // default value
    if (isdigit(line[i])) {
      n = atoi(line[i]);
    } else {
      n = 1; // default
    }

    // (Dice){.number = number, .sides = sides, .tag = tag}
  }
}

bool parse(char* line) {
  size_t start = 0;
  while (isspace(line[start])) {
    start++; // yeah, i'm doin' the crazy!
  }

  if (isdigit(line[start]) || 'd' == line[start]) {
    return do_dice(line[start]);
  }

  // check for exit... duh!
  if (!strncmp(line[start], "exit", 4)) {
    loop = false;
    puts("bye");
    return true;
  }

  printf("'%s'\n", line[start]);
  return false;
}

void usage() {
  puts("This is the usage statement... it tells you how to use this...");
}
