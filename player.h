#pragma once
#include <stdio.h>
#include <stdlib.h>

struct player_t {
  char *name1;
  char *name2;
  int points;
};
typedef struct player_t Player;

Player player_new();
void player_free(Player player);

struct playernode_t {
  struct playernode_t *next;
  Player data;
};
typedef struct playernode_t PlayerNode;

PlayerNode *playernode_new();
void playerlist_free(PlayerNode *head);
void playerlist_print(PlayerNode *head);
float playerlist_get_score(PlayerNode *head);
int playerlist_get_points(PlayerNode *head);
