#include "player.h"
#include "team.h"
#include <stdio.h>
#include <stdlib.h>

// #define DEBUG

int main(int argc, char **argv) {
  int cerinta = -1;

  if (argc < 4) {
    puts("Argumente invalide!");
    return -1;
  }

  FILE *fp_cer = fopen(argv[1], "r");
  for (int i = 0; i < 5; i++) {
    int cerinta_nr;
    fscanf(fp_cer, "%d", &cerinta_nr);
    if (cerinta_nr)
      cerinta = i;
  }
  fclose(fp_cer);

  Team *echipe = team_new();

  FILE *fp_in = fopen(argv[2], "r");
  int nr_echipe = 0;
  int nr_echipe2 = 1;
  fscanf(fp_in, "%d", &nr_echipe);

  for (int i = 0; i < nr_echipe; i++) {
    PlayerNode *jucator_curent = NULL;
    int nr_jucatori = 0;

    fscanf(fp_in, "%d %[^\n]\n", &nr_jucatori, echipe->name);

    echipe->player_list = playernode_new();
    jucator_curent = echipe->player_list;

    for (int j = 0; j < nr_jucatori; j++) {
      fscanf(fp_in, "%s %s %d\n", jucator_curent->data.name1, jucator_curent->data.name2, &jucator_curent->data.points);
      if (j != nr_jucatori - 1) {
        jucator_curent->next = playernode_new();
        jucator_curent = jucator_curent->next;
      }
    }

    fscanf(fp_in, "\n");
    if (i != nr_echipe - 1)
      team_add_front(&echipe);
  }
  fclose(fp_in);

#ifdef DEBUG
  team_print(echipe);
  printf("Numar echipe:%d\n", nr_echipe);
#endif

  FILE *fp_out = fopen(argv[3], "w");
  switch (cerinta) {
  case 0:
    team_write_names(echipe, fp_out);
    break;
  case 1:
    while (nr_echipe2 * 2 <= nr_echipe) {
      nr_echipe2 *= 2;
    }
    for (nr_echipe; nr_echipe != nr_echipe2; nr_echipe--)
      team_remove_min(&echipe);
    team_write_names(echipe, fp_out);
    break;
  case 2: {
    while (nr_echipe2 * 2 <= nr_echipe) {
      nr_echipe2 *= 2;
    }
    for (nr_echipe; nr_echipe != nr_echipe2; nr_echipe--)
      team_remove_min(&echipe);
    team_write_names(echipe, fp_out);

    TeamQueue win = teamqueue_new(nr_echipe);
    TeamQueue lose = teamqueue_new(nr_echipe);
    MatchQueue meciuri = matchqueue_new(nr_echipe / 2);
    int round = 1;

    Team *current = echipe;
    while (current) {
      teamqueue_insert(&win, current);
      current = current->next;
    }

    while (win.rear >= 1) {
      fprintf(fp_out, "\n--- ROUND NO:%d\n", round);
      matchqueue_build(&meciuri, &win);
      for (int i = meciuri.rear; i >= meciuri.rear; i--) {
        fprintf(fp_out, "%-32s - %32s\n", meciuri.matches[i].team1->name, meciuri.matches[i].team2->name);
      }
      matchqueue_match(&meciuri, &win, &lose);
      fprintf(fp_out, "\nWINNERS OF ROUND NO:%d\n", round++);
      for (int i = win.rear; i >= win.front; i--) {
        for(PlayerNode *jucator = win.teams[i]->player_list; jucator; jucator = jucator->next){
          jucator->data.points++;
        }
        fprintf(fp_out, "%-32s  -  %.2f\n", win.teams[i]->name, playerlist_get_score(win.teams[i]->player_list));
      }
    }

    teamqueue_free(&win);
    teamqueue_free(&lose);
    matchqueue_free(&meciuri);
  } break;
  case 3:
  case 4:
    break;
  default:
    puts("Cerinta invalida!");
    return -1;
  }

  team_free(echipe);

  fclose(fp_out);
  return 0;
}
