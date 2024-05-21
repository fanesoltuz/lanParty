#include "player.h"

Player player_new() {
  Player new_player;
  new_player.name1 = (char *)malloc(sizeof(char) * 64);
  new_player.name2 = (char *)malloc(sizeof(char) * 64);
  new_player.name1[0] = '\0';
  new_player.name2[0] = '\0';
  new_player.points = 0;
  return new_player;
}

void player_free(Player player) {
  free(player.name1);
  free(player.name2);
  player.points = 0;
}

PlayerNode *playernode_new() {
  PlayerNode *new_playerlist = (PlayerNode *)malloc(sizeof(PlayerNode));
  new_playerlist->next = NULL;
  new_playerlist->data = player_new();
  return new_playerlist;
}

void playerlist_free(PlayerNode *head) {
  if (!head)
    return;
  PlayerNode *current = head;
  PlayerNode *next;
  while (current != NULL) {
    next = current->next;
    player_free(current->data);
    free(current);
    current = next;
  }
}

void playerlist_print(PlayerNode *head) {
  for (PlayerNode *current = head; current; current = current->next) {
    printf("Nume: %s %s Scor: %d\n", current->data.name1, current->data.name2,
           current->data.points);
  }
}

float playerlist_get_score(PlayerNode *head) {
  if (!head)
    return 0;
  int s = 0, c = 0;
  PlayerNode *current = head;
  while (current) {
    c++;
    s += current->data.points;
    current = current->next;
  }
  return (float)s / c;
}

int playerlist_get_points(PlayerNode *head) {
  if (!head)
    return 0;
  int s = 0;
  PlayerNode *current = head;
  while (current) {
    s += current->data.points;
    current = current->next;
  }
  return s;
}
