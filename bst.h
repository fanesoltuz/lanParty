#pragma once
#include "team.h"
#include "match.h"
#include <stdlib.h>

struct teambstnode_t{
  Team* team_ref;
  struct teambstnode_t* left;
  struct teambstnode_t* right;
};

typedef struct teambstnode_t TeamBSTNode;

TeamBSTNode* teambstnode_new();
void teambst_free(TeamBSTNode* root);
void teambst_insert(TeamBSTNode** root_ref, Team* team_ref);
TeamBSTNode* teambst_generate(Team *head, int team_cnt);
void teambst_write_traverse(TeamBSTNode* root, FILE* fp);
void teambst_write_first_teams(Team* teams, int team_cnt, FILE* fp);


