#pragma once
#include "match.h"
#include "team.h"
#include <stdlib.h>

struct teambstnode_t {
  struct {
    char *name;
    float score;
  } team;
  struct teambstnode_t *left;
  struct teambstnode_t *right;
};

typedef struct teambstnode_t TeamBSTNode;

TeamBSTNode *teambstnode_new(float score, char *name);
void teambst_free(TeamBSTNode *root);
void teambst_insert(TeamBSTNode **root_ref, float score, char *name);
TeamBSTNode *teambst_generate(Team *head, int team_cnt);
void teambst_write_traverse(TeamBSTNode *root, FILE *fp);
void teambst_write_first_teams(Team *teams, int team_cnt, FILE *fp);
