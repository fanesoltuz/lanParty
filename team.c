#include "team.h"

// Utility functions
static void trim_end_whitespace(char *str) {
  if (str == NULL)
    return;
  int i = 0;
  while (str[i] != '\0')
    i++;
  i--;
  while (i >= 0 && isspace((unsigned char)str[i])) {
    str[i] = '\0';
    i--;
  }
}

// TeamList methods

Team *team_new() {
  Team *new_team = (Team *)malloc(sizeof(Team));
  new_team->name[0] = '\0';
  new_team->next = NULL;
  new_team->player_list = NULL;
  return new_team;
}

Team *team_from_file(FILE *fp) {
  Team *echipe = team_new();
  int nr_echipe = 0;
  int nr_echipe2 = 1;
  fscanf(fp, "%d", &nr_echipe);

  for (int i = 0; i < nr_echipe; i++) {
    PlayerNode *jucator_curent = NULL;
    int nr_jucatori = 0;
    char tempname[64];

    fscanf(fp, "%d %[^\n]\n", &nr_jucatori, tempname);
    trim_end_whitespace(tempname);
    strcpy(echipe->name, tempname);

    echipe->player_list = playernode_new();
    jucator_curent = echipe->player_list;

    for (int j = 0; j < nr_jucatori; j++) {
      fscanf(fp, "%s %s %d\n", jucator_curent->data.name1,
             jucator_curent->data.name2, &jucator_curent->data.points);
      if (j != nr_jucatori - 1) {
        jucator_curent->next = playernode_new();
        jucator_curent = jucator_curent->next;
      }
    }

    fscanf(fp, "\n");
    if (i != nr_echipe - 1)
      team_add_front(&echipe);
  }
  return echipe;
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

int teamlist_size(Team *team) {
  int s = 0;
  for (Team *current = team; current; current = current->next)
    s++;
  return s;
}

void teamlist_prepare(Team **head_ref) {
  int nr_echipe = teamlist_size(*head_ref), nr_echipe2 = 1;
  while (nr_echipe2 * 2 <= nr_echipe)
    nr_echipe2 *= 2;
  for (; nr_echipe != nr_echipe2; nr_echipe--)
    team_remove_min(head_ref);
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

void teamlist_write_names(Team *head, FILE *fp) {
  for (Team *current = head; current; current = current->next) {
    fprintf(fp, "%s\n", current->name);
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
      for (PlayerNode *jucator = win.teams[i]->player_list; jucator;
           jucator = jucator->next) {
        jucator->data.points++;
      }
      fprintf(fp, "%-32s  -  %.2f\n", win.teams[i]->name,
              playerlist_get_score(win.teams[i]->player_list));
    }
  }

  teamstack_free(&win);
  teamstack_free(&lose);
  matchqueue_free(&meciuri);
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
