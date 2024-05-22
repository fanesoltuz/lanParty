#include "avl.h"

TeamAVLNode *teamavlnode_new() {
  TeamAVLNode *new_node = (TeamAVLNode *)malloc(sizeof(TeamAVLNode));
  new_node->team_ref = NULL;
  new_node->left = NULL;
  new_node->right = NULL;
  new_node->height = 1;
  return new_node;
}

void teamavl_free(TeamAVLNode *root) {
  if (!root)
    return;
  teamavl_free(root->right);
  teamavl_free(root->left);
  free(root);
}

int teamavl_get_height(TeamAVLNode *root) {
  if (!root)
    return 0;
  return root->height;
}

void teamavl_update_height(TeamAVLNode *root) {
  if (!root)
    return;
  int lh = teamavl_get_height(root->left);
  int rh = teamavl_get_height(root->right);
  root->height = ((lh > rh) ? lh : rh) + 1;
}

int teamavl_get_balance(TeamAVLNode *root) {
  if (!root)
    return 0;
  return teamavl_get_height(root->left) - teamavl_get_height(root->right);
}

TeamAVLNode *teamavlnode_rot_left(TeamAVLNode *node) {
  TeamAVLNode *new_root = node->right;
  node->right = new_root->left;
  new_root->left = node;
  teamavl_update_height(node);
  teamavl_update_height(new_root);
  return new_root;
}

TeamAVLNode *teamavlnode_rot_right(TeamAVLNode *node) {
  TeamAVLNode *new_root = node->left;
  node->left = new_root->right;
  new_root->right = node;
  teamavl_update_height(node);
  teamavl_update_height(new_root);
  return new_root;
}

void teamavl_balance(TeamAVLNode **root_ref) {
  if (!*root_ref)
    return;
  teamavl_update_height(*root_ref);
  int bf = teamavl_get_balance(*root_ref);
  if (bf > 1) {
    if (teamavl_get_balance((*root_ref)->left) < 0)
      (*root_ref)->left = teamavlnode_rot_left((*root_ref)->left);
    *root_ref = teamavlnode_rot_right(*root_ref);
  } else if (bf < -1) {
    if (teamavl_get_balance((*root_ref)->right) > 0)
      (*root_ref)->right = teamavlnode_rot_right((*root_ref)->right);
    *root_ref = teamavlnode_rot_left(*root_ref); 
  }
}

void teamavl_insert(TeamAVLNode **root_ref, Team *team_ref) {
  if (!*root_ref) {
    *root_ref = teamavlnode_new();
    (*root_ref)->team_ref = team_ref;
    return;
  }
  if (playerlist_get_points(team_ref->player_list) < playerlist_get_points((*root_ref)->team_ref->player_list)) {
    teamavl_insert(&((*root_ref)->left), team_ref);
  } else if (playerlist_get_points(team_ref->player_list) > playerlist_get_points((*root_ref)->team_ref->player_list)) {
    teamavl_insert(&((*root_ref)->right), team_ref);
  } else {
    if (strcmp(team_ref->name, (*root_ref)->team_ref->name) < 0) {
      teamavl_insert(&((*root_ref)->left), team_ref);
    } else {
      teamavl_insert(&((*root_ref)->right), team_ref);
    }
  }
  teamavl_balance(root_ref);
}

TeamAVLNode *teamavl_generate(Team *head, int team_cnt) {
  TeamStack win = teamstack_new(teamlist_size(head));
  TeamStack lose = teamstack_new(teamlist_size(head));
  MatchQueue meciuri = matchqueue_new(teamlist_size(head) / 2);
  TeamAVLNode *root = NULL;
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
    teamavl_insert(&root, win.teams[i]);
  }

  teamstack_free(&win);
  teamstack_free(&lose);
  matchqueue_free(&meciuri);
  return root;
}

void teamavl_write_level(TeamAVLNode *root, int level, FILE *fp) {
  if (!root)
    return;
  if (level == 0) {
    fprintf(fp, "%s\n", root->team_ref->name);
  } else {
    teamavl_write_level(root->left, level - 1, fp);
    teamavl_write_level(root->right, level - 1, fp);
  }
}

void teamavl_write_teams_level(Team *teams, int team_cnt, int level, FILE *fp) {
  TeamAVLNode *root = teamavl_generate(teams, team_cnt);
  fprintf(fp, "\nTHE LEVEL %d TEAMS ARE:\n", level);
  teamavl_write_level(root, level, fp);
  teamavl_free(root);
}
