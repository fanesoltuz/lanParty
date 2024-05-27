#include "avl.h"

TeamAVLNode *teamavlnode_new(float score, char *name) {
  TeamAVLNode *new_node = (TeamAVLNode *)malloc(sizeof(TeamAVLNode));
  new_node->team.name = (char *)malloc(sizeof(char) * 64 + 1);
  new_node->team.score = score;
  strcpy(new_node->team.name, name);
  new_node->left = NULL;
  new_node->right = NULL;
  new_node->height = 0;
  return new_node;
}

void teamavl_free(TeamAVLNode *root) {
  if (!root)
    return;
  teamavl_free(root->left);
  teamavl_free(root->right);
  free(root->team.name);
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
  if (!node || !node->right)
    return node;

  TeamAVLNode *new_root = node->right;
  TeamAVLNode *temp = new_root->left;

  new_root->left = node;
  node->right = temp;

  teamavl_update_height(node);
  teamavl_update_height(new_root);

  return new_root;
}

TeamAVLNode *teamavlnode_rot_right(TeamAVLNode *node) {
  if (!node || !node->left)
    return node;

  TeamAVLNode *new_root = node->left;
  TeamAVLNode *temp = new_root->right;

  new_root->right = node;
  node->left = temp;

  teamavl_update_height(new_root);
  teamavl_update_height(node);

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

void teamavl_insert(TeamAVLNode **root_ref, float score, char *name) {
  if (!*root_ref) {
    *root_ref = teamavlnode_new(score, name);
    return;
  }
  if (score < (*root_ref)->team.score) {
    teamavl_insert(&((*root_ref)->left), score, name);
  } else if (score > (*root_ref)->team.score) {
    teamavl_insert(&((*root_ref)->right), score, name);
  } else {
    if (strcmp(name, (*root_ref)->team.name) < 0) {
      teamavl_insert(&((*root_ref)->left), score, name);
    } else {
      teamavl_insert(&((*root_ref)->right), score, name);
    }
  }
  teamavl_balance(root_ref);
}

void teamavl_print_level(TeamAVLNode *root, int level) {
  if (!root)
    return;
  if (level == 0) {
    return;
  }
  teamavl_print_level(root->right, level - 1);
  teamavl_print_level(root->left, level - 1);
}

void teamavl_write_level(TeamAVLNode *root, int level, FILE *fp) {
  if (!root)
    return;
  teamavl_write_level(root->right, level - 1, fp);
  teamavl_write_level(root->left, level - 1, fp);
}

void bst_to_avl(TeamAVLNode **avl, TeamBSTNode *bst) {
  if (!bst)
    return;
  bst_to_avl(avl, bst->left);
  teamavl_insert(avl, bst->team.score, bst->team.name);
  bst_to_avl(avl, bst->right);
}

void teamavl_write_teams_level(Team *teams, int team_cnt, int level, FILE *fp) {
  TeamBSTNode *bst_root = teambst_generate(teams, team_cnt);
  fprintf(fp, "\nTHE LEVEL %d TEAMS ARE:\n", level);
  TeamAVLNode *avl_root = NULL;
  bst_to_avl(&avl_root, bst_root);
  teamavl_write_level(avl_root, level, fp);
  teamavl_print_level(avl_root, level);
  teamavl_free(avl_root);
}
