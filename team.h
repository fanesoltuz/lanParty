#pragma once
#include "player.h"
#include <limits.h>
#include <stdlib.h>

struct team_t {
  struct team_t *next;
  char name[64];
  PlayerNode *player_list;
};
typedef struct team_t Team;

Team *team_new();
void team_free(Team *head);
void team_print(Team *head);
void team_add_front(Team **head_ref);
void team_write_names(Team *head, FILE *fp);
void team_remove_min(Team **head);

struct teamqueue_t {
  Team **teams;
  int size;
  int front, rear;
};
typedef struct teamqueue_t TeamQueue;

TeamQueue teamqueue_new(int size);
void teamqueue_free(TeamQueue *tq);
void teamqueue_clear(TeamQueue *tq);
void teamqueue_insert(TeamQueue *tq, Team *team);
void teamqueue_print(TeamQueue tq);

struct match_t {
  Team *team1, *team2;
};
typedef struct match_t Match;

struct matchqueue_t {
  Match *matches;
  int size;
  int front, rear;
};
typedef struct matchqueue_t MatchQueue;

MatchQueue matchqueue_new(int size);
void matchqueue_free(MatchQueue *mq);
void matchqueue_clear(MatchQueue *mq);
void matchqueue_insert(MatchQueue *mq, Match match);

void matchqueue_build(MatchQueue *mq, TeamQueue *tq);
void matchqueue_match(MatchQueue *mq, TeamQueue *winners, TeamQueue *losers);
