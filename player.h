#pragma once
#include <stdio.h>
#include <stdlib.h>

struct player_t {
  struct player_t *next;
  char name1[64], name2[64];
  int points;
};

typedef struct player_t Player;

Player *player_new();
void player_free(Player *head);
void player_print(Player *head);
int player_point_sum(Player *head);
