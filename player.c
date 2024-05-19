#include "player.h"

Player *player_new() {
  Player *new_player = (Player *)malloc(sizeof(Player));
  new_player->next = NULL;
  new_player->name1[0] = '\0';
  new_player->name2[0] = '\0';
  new_player->points = 0;
  return new_player;
}

void player_free(Player *head) {
  if (!head)
    return;
  Player *current = head;
  Player *next;
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
}

void player_print(Player *head) {
  for (Player *current = head; current; current = current->next) {
    printf("Nume: %s %s Scor: %d\n", current->name1, current->name2, current->points);
  }
}

int player_point_sum(Player *head) {
  if (!head)
    return 0;
  int s = 0;
  Player *current = head;
  while (current) {
    s += current->points;
    current = current->next;
  }
  return s;
}
