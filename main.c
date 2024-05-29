#include "avl.h"
#include "bst.h"
#include "team.h"
#include <stdio.h>

int main(int argc, char **argv) {
  int cerinta = -1;

  if (argc < 4) { // Verificare nr argumente
    puts("Argumente invalide!");
    return -1;
  }

  FILE *fp_cer = fopen(argv[1], "r"); // Se citesc cerintele
  for (int i = 0; i < 5; i++) {
    int cerinta_nr;
    fscanf(fp_cer, "%d", &cerinta_nr);
    if (cerinta_nr)
      cerinta = i;
  }
  fclose(fp_cer);

  FILE *fp_in = fopen(argv[2], "r");
  Team *echipe = team_from_file(fp_in); // Citire echipe + jucatori

  fclose(fp_in);

  FILE *fp_out = fopen(argv[3], "w");
  switch (cerinta) {
  case 0:
    teamlist_write_names(echipe, fp_out);
    break;
  case 1:
    teamlist_prepare(&echipe); // Sterge echipele cu punctaj mic pana la N^2
    teamlist_write_names(echipe, fp_out);
    break;
  case 2:
    teamlist_prepare(&echipe);
    teamlist_write_names(echipe, fp_out);
    teamlist_write_matches(echipe, fp_out); // Se scriu meciurile / castigatorii
    break;
  case 3:
    teamlist_prepare(&echipe);
    teamlist_write_names(echipe, fp_out);
    teamlist_write_matches(echipe, fp_out);
    teambst_write_first_teams(echipe, 8, fp_out); // Se scriu primele 8 echipe
    break;
  case 4:
    teamlist_prepare(&echipe);
    teamlist_write_names(echipe, fp_out);
    teamlist_write_matches(echipe, fp_out);
    teambst_write_first_teams(echipe, 8, fp_out);
    teamavl_write_teams_level(echipe, 8, 2, fp_out); // Se scriu echipele de pe nivelul 2
    break;
  default: // Caz cerinta invalida
    team_free(echipe);
    fclose(fp_out);
    puts("Cerinta invalida!");
    return -1;
  }
//functile aferente cerintelor elibereaza memoria 
  team_free(echipe);
  fclose(fp_out);
  return 0;
}
