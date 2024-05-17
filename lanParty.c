#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct player {
  char firstName[64];
  char secondName[64];
  int points;
};
typedef struct player Player;

struct playerListNode {
  Player data;
  struct playerListNode *next;
};
typedef struct playerListNode PlayerListNode;

// Constructor pentru PlayerListNode
PlayerListNode *PlayerListNode_new() {
  PlayerListNode *node = (PlayerListNode *)malloc(sizeof(PlayerListNode));
  node->next = NULL;
  return node;
}

// Destructor recursiv pentru PlayerListNode
void PlayerListNode_delete(PlayerListNode *head) {
  if (head == NULL)
    return;
  PlayerListNode *current = head;
  PlayerListNode *next;
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
}

struct teamListNode {
  PlayerListNode *playerListHead;
  char name[64];
  struct teamListNode *next;
};
typedef struct teamListNode TeamListNode;

// Constructor pentru TeamListNode
TeamListNode *TeamListNode_new() {
  TeamListNode *node = (TeamListNode *)malloc(sizeof(TeamListNode));
  node->playerListHead = NULL;
  node->next = NULL;
  return node;
}

// Destructor recursiv pentru TeamListNode
void TeamListNode_delete(TeamListNode *head) {
  if (head == NULL)
    return;
  TeamListNode *current = head;
  TeamListNode *next;
  while (current != NULL) {
    next = current->next;
    PlayerListNode_delete(current->playerListHead);
    free(current);
    current = next;
  }
}

// Adauga un nod la inceputul unei liste
void TeamListNode_add_front(TeamListNode **head){
  TeamListNode *new_head = TeamListNode_new();
  new_head->next = *head;
  *head=new_head;
}

int main(int argc, char **argv) {
  int cerinte[5] = {0, 0, 0, 0, 0};

  if (argc < 3) {
    puts("Nu au fost specificate suficiente fisiere!");
    return -1;
  }

  FILE *fpcer = fopen(argv[1], "r");
  for (int i = 0; i < 5; i++) {
    fscanf(fpcer, "%d", &cerinte[i]);
  }
  fclose(fpcer);

  FILE *fpin = fopen(argv[2], "r");
  FILE *fpout = fopen(argv[3], "w");

  TeamListNode *listaEchipe = TeamListNode_new();

  int nrEchipe = 0;
  fscanf(fpin, "%d", &nrEchipe);

  for (int i = 0; i < nrEchipe; i++) {
    PlayerListNode *jucatorCurent = NULL;
    int nrJucatori = 0;

    fscanf(fpin, "%d %[^\n]", &nrJucatori, listaEchipe->name); // Citire string cu spatii cu scanset (toate caracterele pana la \n)

    listaEchipe->playerListHead = PlayerListNode_new();
    jucatorCurent = listaEchipe->playerListHead;

    for (int j = 0; j < nrJucatori; j++) {
      jucatorCurent->next = PlayerListNode_new();
      jucatorCurent = jucatorCurent->next;
      fscanf(fpin, "%s %s %d", jucatorCurent->data.firstName, jucatorCurent->data.secondName, &jucatorCurent->data.points);
    }

    if(i != nrEchipe - 1)
      TeamListNode_add_front(&listaEchipe); // Inserare element in la inceput
  }

  if (cerinte[0]) {
    TeamListNode *echipaCurenta = listaEchipe;
    while(echipaCurenta){
      fprintf(fpout,"%s\n", echipaCurenta->name);
      echipaCurenta= echipaCurenta->next;
    }
  }

  TeamListNode_delete(listaEchipe);
  fclose(fpin);
  fclose(fpout);
  return 0;
}
