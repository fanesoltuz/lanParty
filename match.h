#pragma once
#include "team.h"

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

void matchqueue_build(MatchQueue *mq, TeamStack *tq);
void matchqueue_match(MatchQueue *mq, TeamStack *winners, TeamStack *losers);
void teamlist_write_matches(Team *head, FILE *fp);
