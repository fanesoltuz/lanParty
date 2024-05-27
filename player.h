#pragma once
#include <stdio.h>
#include <stdlib.h>

struct player_t {
  char *name1;
  char *name2;
  int points;
};
typedef struct player_t Player; // Struct player

Player player_new();             // Constructor player
void player_free(Player player); // Destructor player

struct playernode_t {
  struct playernode_t *next;
  Player data;
};
typedef struct playernode_t PlayerNode; // Struct lista playeri

PlayerNode *playernode_new();           // Constructor
void playerlist_free(PlayerNode *head); // Destructor
void playerlist_print(PlayerNode *head);
float playerlist_get_score(PlayerNode *head); // Calculare scor lista player
int playerlist_get_points(PlayerNode *head);
