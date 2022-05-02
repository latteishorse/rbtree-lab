#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  // TODO: initialize struct if needed
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = (node_t *)calloc(1, sizeof(node_t));

  p->root = p->nil;
  p->nil->color = RBTREE_BLACK;

  return p;
}

node_t *left_rotate(rbtree *t, node_t *x){
    node_t *y = x->right;
    x->right = y->left;
    if (y->left != t->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == t->nil) {
        t->root = y;
    } 
    else if (x == x->parent->left) {
        x->parent->left = y;    
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    return;
}

node_t *right_rotate(rbtree *t, node_t *x){
    node_t *y = x->left;
    x->left = y->right;
    if (y->right != t->nil) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == t->nil) {
        t->root = y;
    }
    else if (x == x->parent->right) {
        x->parent->right = y;
    }
    else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
    return;  

}
node_t *rb_insert_Fixup(rbtree *t, node_t *z) {
  node_t *uncle;
  //node_t *uncle;
  while (z->parent->color == RBTREE_RED){
      if(z->parent == z->parent->parent->left){
          uncle = z->parent->parent->right;
          //경우1
          if ((uncle != t->nil) && (uncle->color == RBTREE_RED)){
              z->parent->color = RBTREE_BLACK;
              uncle->color = RBTREE_BLACK;
              z->parent->parent->color = RBTREE_RED;
              z = z->parent->parent;
          }
          //경우2
          else {

              if(z == z->parent->right){
                  z = z->parent;
                  left_rotate(t,z);
              }
              if (z != t->root && z->parent != t->root) {
                  z->parent->color = RBTREE_BLACK;
                  z->parent->parent->color = RBTREE_RED;
                  right_rotate(t, z->parent->parent);
              }
          }
      }
      //반대로
      else{
          uncle = z->parent->parent->left;
          //경우1
          if (uncle != t->nil && uncle->color == RBTREE_RED){
              z->parent->color = RBTREE_BLACK;
              uncle->color = RBTREE_BLACK;
              z->parent->parent->color = RBTREE_RED;
              z = z->parent->parent;
          }
          //경우2
          else {
              if(z == z->parent->left){
                  z = z->parent;
                  right_rotate(t,z);
              }
              if (z != t->root && z->parent != t->root) {
                  //경우3
                  z->parent->color = RBTREE_BLACK;
                  
                  z->parent->parent->color = RBTREE_RED;
                  left_rotate(t, z->parent->parent);
              }
          }
      }
  }
  t->root->color = RBTREE_BLACK;
  return NULL;
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  //t->nil = NULL;
  node_t *parent = t->nil;
  node_t *p = t->root;
  //새 노드가 들어갈 자리 찾기
  while (p != t->nil)
  {
      parent = p;
      if (p->key > key)
      {
          p = p->left;
      }
      else
      {
          p = p->right;
      }
  }
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  new_node->parent = parent;
  // 트리의 첫 노드일 때 - 루트가 된다
  if (parent == t->nil)
  {
      t->root = new_node;
      new_node->color = RBTREE_BLACK;
  }
  //찾은 부모노드보다 새로운 노드가 작을때 - 왼쪽 자식으로
  else if (key < parent->key)
  {
      parent->left = new_node;
      new_node->color = RBTREE_RED;
  }
  //크거나 같을때 - 오른쪽 자식으로
  else
  {
      parent->right = new_node;
      new_node->color = RBTREE_RED;
  }
  //새로운 노드 설정
  new_node->key = key;
  new_node->left = t->nil;
  new_node->right = t->nil;
  rb_insert_Fixup(t, new_node); 
  return t->root;

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *r = t->root;
  while (r != t->nil) {
    if (r->key == key) 
      return r;
    else if (r->key < key) // BST 특성이랑 동일
      r = r->right;
    else
      r = r->left;
  }
  return NULL; // not found 
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *r = t->root;
  if (r == t->nil) 
    return r;
  while (r->left != t->nil) {
    r = r -> left;
  }
  return r;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *r = t->root;
  if (r == t->nil) 
    return r;
  while (r->right != t->nil) {
    r = r -> right;
  }
  return r;
}

// --------------------------------------------

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}


int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
