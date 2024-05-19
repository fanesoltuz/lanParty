#include "team.h"

// TeamList methods

Team *team_new() {
  Team *new_team = (Team *)malloc(sizeof(Team));
  new_team->name[0] = '\0';
  new_team->next = NULL;
  new_team->player_list = NULL;
  return new_team;
}

void team_free(Team *head) {
  if (!head)
    return;
  Team *current = head;
  Team *next;
  while (current != NULL) {
    next = current->next;
    playerlist_free(current->player_list);
    free(current);
    current = next;
  }
}

void team_add_front(Team **head_ref) {
  Team *new_head = team_new();
  new_head->next = *head_ref;
  *head_ref = new_head;
}

void team_print(Team *head) {
  for (Team *current = head; current; current = current->next) {
    printf("\nEchipa: %s\n", current->name);
    playerlist_print(current->player_list);
    printf("Scor total: %d\n", playerlist_get_points(current->player_list));
  }
}

void team_write_names(Team *head, FILE *fp) {
  for (Team *current = head; current; current = current->next) {
    fprintf(fp, "%s\n", current->name);
  }
}

void team_remove_min(Team **head) {
  if (!head || !*head)
    return;
  int min = INT_MAX;
  Team *min_team = NULL;
  Team *pmin_team = NULL;
  Team *pcurrent = NULL;
  for (Team *current = *head; current; current = current->next) {
    int psum = playerlist_get_points(current->player_list);
    if (psum < min) {
      min = psum;
      min_team = current;
      pmin_team = pcurrent;
    }
    pcurrent = current;
  }
  if (min_team) {
    if (min_team == *head) {
      *head = (*head)->next;
    } else if (pmin_team) {
      pmin_team->next = min_team->next;
    }
    playerlist_free(min_team->player_list);
    free(min_team);
  }
}

// TeamQueue methods

TeamQueue teamqueue_new(int size) {
  TeamQueue tq;
  tq.size = 0;
  tq.rear = -1;
  tq.front = 0;
  tq.teams = (Team **)malloc(sizeof(Team *) * size);
  return tq;
}

void teamqueue_free(TeamQueue *tq) {
  tq->size = 0;
  tq->rear = -1;
  tq->front = 0;
  free(tq->teams);
}

void teamqueue_clear(TeamQueue *tq) {
  tq->front = 0;
  tq->rear = -1;
  tq->size = 0;
}

void teamqueue_insert(TeamQueue *tq, Team *team) {
  tq->rear++;
  tq->teams[tq->rear] = team;
  tq->size++;
}

void teamqueue_print(TeamQueue tq) {
  for (int i = tq.front; i <= tq.rear; i++) {
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

void matchqueue_clear(MatchQueue *mq) {
  mq->rear = -1;
}

void matchqueue_insert(MatchQueue *mq, Match match) {
  mq->matches[++(mq->rear)] = match;
  mq->size++;
}

void matchqueue_build(MatchQueue *mq, TeamQueue *tq) {
  matchqueue_clear(mq);
  for (int i = tq->front; i < tq->rear; i += 2) {
    matchqueue_insert(mq, (Match){tq->teams[i], tq->teams[i + 1]});
  }
}
void matchqueue_match(MatchQueue *mq, TeamQueue *winners, TeamQueue *losers) {
  winners->rear = -1;
  losers->rear = -1;
  for (int i = mq->front; i <= mq->rear; i++) {
    if (playerlist_get_score(mq->matches[i].team1->player_list) > playerlist_get_score(mq->matches[i].team2->player_list)) {
      teamqueue_insert(winners, mq->matches[i].team1);
      teamqueue_insert(losers, mq->matches[i].team2);
      } else {
        teamqueue_insert(winners, mq->matches[i].team2);
      teamqueue_insert(losers, mq->matches[i].team1);

      
    }
  }
}
