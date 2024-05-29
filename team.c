#include "team.h"

// Utility functions

void trim_end_whitespace(char *str) {
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
//elimina caractere de la finalul unui string(caractere spatiu)

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
  fscanf(fp, "%d", &nr_echipe);

  for (int i = 0; i < nr_echipe; i++) {
    PlayerNode *jucator_curent = NULL;
    int nr_jucatori = 0;
    char tempname[64];

    fscanf(fp, "%d %[^\n]\n", &nr_jucatori, tempname); //citeste scor si nume echipa folosind scanset(in loc de fgets)
    trim_end_whitespace(tempname); 
    strcpy(echipe->name, tempname);

    echipe->player_list = playernode_new();
    jucator_curent = echipe->player_list;

    for (int j = 0; j < nr_jucatori; j++) {
      fscanf(fp, "%s %s %d\n", jucator_curent->data.name1, jucator_curent->data.name2, &jucator_curent->data.points);
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
