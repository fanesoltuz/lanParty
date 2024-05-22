#include "match.h"

// TeamStack methods

TeamStack teamstack_new(int size) {
  TeamStack tq;
  tq.size = 0;
  tq.top = -1;
  tq.teams = (Team **)malloc(sizeof(Team *) * size);
  return tq;
}

void teamstack_free(TeamStack *tq) {
  tq->size = 0;
  tq->top = -1;
  free(tq->teams);
}

void teamstack_clear(TeamStack *tq) {
  tq->top = -1;
  tq->size = 0;
}

void teamstack_insert(TeamStack *tq, Team *team) {
  tq->top++;
  tq->teams[tq->top] = team;
  tq->size++;
}

void teamstack_pop(TeamStack *tq) {
  if (tq->top == -1)
    return;
  tq->top--;
  tq->size--;
}

void teamstack_print(TeamStack tq) {
  for (int i = 0; i <= tq.top; i++) {
    printf("%s\n", tq.teams[i]->name);
  }
}

// MatchQueue methods

MatchQueue matchqueue_new(int size) {
  MatchQueue mq;
  mq.size = 0;
  mq.rear = -1;
  mq.front = 0;
  mq.matches = (Match *)malloc(sizeof(Match) * size);
  return mq;
}

void matchqueue_free(MatchQueue *mq) {
  mq->size = 0;
  mq->rear = -1;
  mq->front = 0;
  free(mq->matches);
}

void matchqueue_clear(MatchQueue *mq) { mq->rear = -1; }

void matchqueue_insert(MatchQueue *mq, Match match) {
  mq->matches[++(mq->rear)] = match;
  mq->size++;
}

void matchqueue_build(MatchQueue *mq, TeamStack *tq) {
  matchqueue_clear(mq);
  for (int i = 0; i < tq->top; i += 2) {
    matchqueue_insert(mq, (Match){tq->teams[i], tq->teams[i + 1]});
  }
}

void matchqueue_match(MatchQueue *mq, TeamStack *winners, TeamStack *losers) {
  teamstack_clear(winners);
  teamstack_clear(losers);
  for (int i = mq->rear; i >= mq->front; i--) {
    if (playerlist_get_score(mq->matches[i].team1->player_list) >
        playerlist_get_score(mq->matches[i].team2->player_list)) {
      teamstack_insert(winners, mq->matches[i].team1);
      teamstack_insert(losers, mq->matches[i].team2);
    } else {
      teamstack_insert(winners, mq->matches[i].team2);
      teamstack_insert(losers, mq->matches[i].team1);
    }
  }
}

void teamlist_write_matches(Team *head, FILE *fp) {
  TeamStack win = teamstack_new(teamlist_size(head));
  TeamStack lose = teamstack_new(teamlist_size(head));
  MatchQueue meciuri = matchqueue_new(teamlist_size(head) / 2);
  int round = 1;

  Team *current = head;
  while (current) {
    teamstack_insert(&win, current);
    current = current->next;
  }

  while (win.size > 1) {
    fprintf(fp, "\n--- ROUND NO:%d\n", round);
    matchqueue_build(&meciuri, &win);
    for (int i = meciuri.front; i <= meciuri.rear; i++) {
      fprintf(fp, "%-32s - %32s\n", meciuri.matches[i].team1->name,
              meciuri.matches[i].team2->name);
    }
    matchqueue_match(&meciuri, &win, &lose);
    fprintf(fp, "\nWINNERS OF ROUND NO:%d\n", round++);
    for (int i = 0; i < win.size; i++) {
      fprintf(fp, "%-32s  -  %.2f\n", win.teams[i]->name, playerlist_get_score(win.teams[i]->player_list) + round - 1.0f);
    }
  }

  teamstack_free(&win);
  teamstack_free(&lose);
  matchqueue_free(&meciuri);
}


