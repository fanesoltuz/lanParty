#include "team.h"

Team *team_new() {
  Team *new_team = (Team *)malloc(sizeof(Team));
  new_team->name[0] = '\0';
  new_team->next = NULL;
  new_team->players = NULL;
  return new_team;
}

void team_free(Team *head) {
  if (!head)
    return;
  Team *current = head;
  Team *next;
  while (current != NULL) {
    next = current->next;
    player_free(current->players);
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
    player_print(current->players);
    printf("Scor total: %d\n", player_point_sum(current->players));
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
    int psum = player_point_sum(current->players);
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
    player_free(min_team->players);
    free(min_team);
  }
}
