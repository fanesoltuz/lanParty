#include "bst.h"

TeamBSTNode *teambstnode_new(float score, char *name) {
  TeamBSTNode *new_node = (TeamBSTNode *)malloc(sizeof(TeamBSTNode));
  new_node->team.name = (char *)malloc(sizeof(char) * strlen(name) + 1);
  new_node->team.score = score;
  strcpy(new_node->team.name, name);
  new_node->left = NULL;
  new_node->right = NULL;
  return new_node;
}

void teambst_free(TeamBSTNode *root) {
  if (!root)
    return;
  teambst_free(root->left);
  teambst_free(root->right);
  free(root->team.name);
  free(root);
}

void teambst_insert(TeamBSTNode **root_ref, float score, char *name) {
  if (!*root_ref) {
    *root_ref = teambstnode_new(score, name);
    return;
  }

  if (score < (*root_ref)->team.score) {
    teambst_insert(&((*root_ref)->left), score, name);
  } else if (score > (*root_ref)->team.score) {
    teambst_insert(&((*root_ref)->right), score, name);
  } else {
    if (strcmp(name, (*root_ref)->team.name) < 0) {
      teambst_insert(&((*root_ref)->left), score, name);
    } else {
      teambst_insert(&((*root_ref)->right), score, name);
    }
  }
}

void teambst_write_traverse(TeamBSTNode *root, FILE *fp) {
  if (root) {
    teambst_write_traverse(root->right, fp);
    fprintf(fp, "%-32s  -  %.2f\n", root->team.name, root->team.score);
    teambst_write_traverse(root->left, fp);
  }
}

TeamBSTNode *teambst_generate(Team *head, int team_cnt) {
  TeamStack win = teamstack_new(teamlist_size(head));
  TeamStack lose = teamstack_new(teamlist_size(head));
  MatchQueue meciuri = matchqueue_new(teamlist_size(head) / 2);
  TeamBSTNode *root = NULL;
  Team *current = head;

  while (current) {
    teamstack_insert(&win, current);
    current = current->next;
  }

  while (win.size > team_cnt) {
    matchqueue_build(&meciuri, &win);
    matchqueue_match(&meciuri, &win, &lose);
    for (int i = 0; i < win.size; i++)
      for (PlayerNode *jucator = win.teams[i]->player_list; jucator; jucator = jucator->next)
        jucator->data.points++;
  }

  for (int i = 0; i <= win.top; i++) {
    teambst_insert(&root, playerlist_get_score(win.teams[i]->player_list), win.teams[i]->name);
  }
  teamstack_free(&win);
  teamstack_free(&lose);
  matchqueue_free(&meciuri);
  return root;
}

void teambst_write_first_teams(Team *teams, int team_cnt, FILE *fp) {
  TeamBSTNode *root = teambst_generate(teams, 8);
  fprintf(fp, "\nTOP %d TEAMS:\n", team_cnt);
  teambst_write_traverse(root, fp);
  teambst_free(root);
}
