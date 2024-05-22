#pragma once
#include "match.h"
#include "team.h"
#include <stdlib.h>

struct teamavlnode_t {
  Team *team_ref;
  struct teamavlnode_t *left;
  struct teamavlnode_t *right;
  int height;
};

typedef struct teamavlnode_t TeamAVLNode;

TeamAVLNode *teamavlnode_new();
void teamavl_free(TeamAVLNode *root);
void teamavl_insert(TeamAVLNode **root_ref, Team *team_ref);
TeamAVLNode *teamavl_generate(Team *head, int team_cnt);
void teamavl_write_teams_level(Team *teams, int team_cnt, int height, FILE *fp);
