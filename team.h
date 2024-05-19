#pragma once
#include "player.h"
#include <limits.h>
#include <stdlib.h>

struct team_t {
  struct team_t *next;
  char name[64];
  Player *players;
};

typedef struct team_t Team;

Team *team_new();
void team_free(Team *head);
void team_print(Team *head);
void team_add_front(Team **head_ref);
void team_write_names(Team *head, FILE *fp);
void team_remove_min(Team **head);
