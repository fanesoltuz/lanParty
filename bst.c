#include "bst.h"

TeamBSTNode *teambstnode_new() {
  TeamBSTNode *new_node = (TeamBSTNode *)malloc(sizeof(TeamBSTNode));
  new_node->team_ref = NULL;
  new_node->left = NULL;
  new_node->right = NULL;
  return new_node;
}

void teambst_free(TeamBSTNode *root) {
  if (!root)
    return;
  teambst_free(root->right);
  teambst_free(root->left);
  free(root);
}

void teambst_insert(TeamBSTNode **root_ref, Team *team_ref) {
  if (!*root_ref) {
    *root_ref = teambstnode_new();
    (*root_ref)->team_ref = team_ref;
    return;
  }
  if (playerlist_get_points(team_ref->player_list) < playerlist_get_points((*root_ref)->team_ref->player_list)) {
    teambst_insert(&((*root_ref)->left), team_ref);
  } else if (playerlist_get_points(team_ref->player_list) > playerlist_get_points((*root_ref)->team_ref->player_list)) {
    teambst_insert(&((*root_ref)->right), team_ref);
  } else {
    if (strcmp(team_ref->name, (*root_ref)->team_ref->name) < 0) {
      teambst_insert(&((*root_ref)->left), team_ref);
    } else {
      teambst_insert(&((*root_ref)->right), team_ref);
    }
  }
}

void teambst_write_traverse(TeamBSTNode *root, FILE *fp) {
  if (root) {
    teambst_write_traverse(root->right, fp);
    if (root->team_ref)
      fprintf(fp, "%-32s  -  %.2f\n", root->team_ref->name,
              playerlist_get_score(root->team_ref->player_list));
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

  for (int i = 0; i < team_cnt; i++) {
    teambst_insert(&root, win.teams[i]);
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
