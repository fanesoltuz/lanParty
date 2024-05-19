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
    Player *jucator_curent = NULL;
    int nr_jucatori = 0;

    fscanf(fp_in, "%d %[^\n]\n", &nr_jucatori, echipe->name);

    echipe->players = player_new();
    jucator_curent = echipe->players;

    for (int j = 0; j < nr_jucatori; j++) {
      fscanf(fp_in, "%s %s %d\n", jucator_curent->name1, jucator_curent->name2, &jucator_curent->points);
      if (j != nr_jucatori - 1) {
        jucator_curent->next = player_new();
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
    for (nr_echipe2; nr_echipe2 < nr_echipe; nr_echipe2++)
      team_remove_min(&echipe);
    team_write_names(echipe, fp_out);
    break;
  case 2:
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
