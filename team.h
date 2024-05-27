#pragma once
#include "player.h"
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

struct team_t {
  struct team_t *next;
  char name[64];
  PlayerNode *player_list;
};
typedef struct team_t Team;

Team *team_new();
Team *team_from_file(FILE *fp); // Constructor din fisier
void team_free(Team *head);
void team_print(Team *head);
void team_add_front(Team **head_ref);
void team_remove_min(Team **head);
int teamlist_size(Team *team);
void teamlist_prepare(Team **head_ref);
void teamlist_write_names(Team *head, FILE *fp);
void teamlist_write_matches(Team *head, FILE *fp);

struct teamstack_t {
  Team **teams;
  int size;
  int top;
};
typedef struct teamstack_t TeamStack;

TeamStack teamstack_new(int size);
void teamstack_free(TeamStack *tq);
void teamstack_clear(TeamStack *tq);
void teamstack_insert(TeamStack *tq, Team *team);
void teamstack_print(TeamStack tq);
void teamstack_pop(TeamStack *tq);
