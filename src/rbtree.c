#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

// 
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = (node_t *)calloc(1, sizeof(node_t));

  p->root = p->nil;
  p->nil->color = RBTREE_BLACK;

  return p;
}

// free 
void freenode(node_t *p, node_t *nil) {
    if (p == nil) return;
 
    freenode(p->left,nil);
    freenode(p->right,nil);
    free(p);
}

// ITAR pseudo code 바탕으로 작성
void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;

  // y.left = T.nil이 될 때까지
  if (y->left != t->nil) {
      y->left->parent = x;
  }

  y->parent = x->parent; // x의 부모를 y root로
  if (x->parent == t->nil) {
      t->root = y; // x가 root였다면 y를 root로
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

// ITAR pseudo code 바탕으로 작성
void right_rotate(rbtree *t, node_t *x) {
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

node_t *rbtree_insert(rbtree *t, const key_t key) {

  node_t *parent = t->nil; // y
  node_t *p = t->root; // x

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

  // 첫 노드 -> root
  if (parent == t->nil)
  {
      t->root = new_node;
      new_node->color = RBTREE_BLACK;
  }
  // 새로운 노드 < 부모 노드 -> left node
  else if (key < parent->key)
  {
      parent->left = new_node;
      new_node->color = RBTREE_RED; //
  }
  // right node
  else
  {
      parent->right = new_node;
      new_node->color = RBTREE_RED;
  }
  // 
  new_node->key = key;

  new_node->left = t->nil;
  new_node->right = t->nil; 
  insert_fixup(t, new_node); // 검증
  return t->root;
}

// insert한 다음 검증하기 위한 부분
void insert_fixup(rbtree *t, node_t *z){
  node_t *uncle; 
  while (z->parent->color == RBTREE_RED){
      if(z->parent == z->parent->parent->left){
          uncle = z->parent->parent->right;
          
          //경우1
          if ((uncle != t->nil) == (uncle->color == RBTREE_RED)){
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
              if ((z != t->root) == (z->parent != t->root)) {
                  z->parent->color = RBTREE_BLACK;
                  z->parent->parent->color = RBTREE_RED;
                  right_rotate(t, z->parent->parent);
              }
          }
      }
      // 반대 경우
      else{
          uncle = z->parent->parent->left;
          //경우1
          if ((uncle != t->nil) == (uncle->color == RBTREE_RED)){
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
              if ((z != t->root) == (z->parent != t->root)) {
                  //경우3
                  z->parent->color = RBTREE_BLACK;
                  
                  z->parent->parent->color = RBTREE_RED;
                  left_rotate(t, z->parent->parent);
              }
          }
      }
  }
  t->root->color = RBTREE_BLACK;
  // return;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *r = t->root;
  while (r != t->nil) {
    if (r->key == key) 
      return r;
    else if (r->key < key)
      r = r->right;
    else
      r = r->left;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *r = t->root;
  if (r == t->nil) 
    return r;
  while (r->left != t->nil){
    r = r -> left;
  }
  return r;
}

node_t *tree_minimum(const rbtree *t, node_t *sub_root)
{
    node_t *r = sub_root;
    if (r == t->nil)
      return r;
    while (r->left != t->nil){
        r = r->left;
    }
    return r;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *r = t->root;
  if (r == t->nil) return r;
  while (r->right != t->nil) {
    r = r -> right;
  }
  return r;
}

// 여기서부터 이해해야됨
void rb_transplant(rbtree *t, node_t *u, node_t *v) {
    if (u->parent == t->nil) {
        t->root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

// delete
void delete_rbtree(rbtree *t) {
    freenode(t->root,t->nil);
    free(t->nil); // why?
    free(t);
}

void rb_delete_fixup(rbtree *t, node_t *x) {
    while (x != t->root && x->color == RBTREE_BLACK) {
        if (x == x->parent->left) {
            node_t *w = x->parent->right;
            //case 1
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                left_rotate(t, x->parent);
                w = x->parent->right;
            } //case1 end
            // case 2 
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
            } // case 2 end
            //case 3
            else {
                if (w->right->color == RBTREE_BLACK) {
                    w->left->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    right_rotate(t, x);
                    w = x->parent->right; // case 3 end
                }
                //case 4
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                left_rotate(t, x->parent);
                x = t->root;
                //case 4 end
            }
            
        }
        else {
          node_t *w = x->parent->left;
          //case 1
          if (w->color == RBTREE_RED) {
              w->color = RBTREE_BLACK;
              x->parent->color = RBTREE_RED;
              right_rotate(t, x->parent);
              w = x->parent->left;
          } //case1 end
          // case 2 
          if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
              w->color = RBTREE_RED;
              x = x->parent;
          } // case 2 end
          //case 3
          else {
              if (w->left->color == RBTREE_BLACK) {
                  w->right->color = RBTREE_BLACK;
                  w->color = RBTREE_RED;
                  left_rotate(t, x);
                  w = x->parent->left;
              } // case 3 end
              //case 4
              w->color = x->parent->color;
              x->parent->color = RBTREE_BLACK;
              w->left->color = RBTREE_BLACK;
              right_rotate(t, x->parent);
              x = t->root;
              //case 4 end
          }   
        }
    }
    x->color = RBTREE_BLACK;
    return;
}

int rbtree_erase(rbtree *t, node_t *z) {
  node_t *y = z;
  node_t *x;
  color_t y_original_color = y->color;

  if (z->left == t->nil) {
      x = z->right;
      rb_transplant(t, z, z->right);
  }

  else if (z->right == t->nil) {
      x = z->left;
      rb_transplant(t, z, z->left);
  }
  
  else {
      y = tree_minimum(t, z->right);
      y_original_color = y->color;
      x = y->right;
      if (y->parent == z) {
          x->parent = y;
      }
      else {
          rb_transplant(t, y, y->right);
          y->right = z->right;
          y->right->parent = y;
      }
      rb_transplant(t, z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
  }
  if (y_original_color == RBTREE_BLACK) {
      rb_delete_fixup(t, x);
  }
  free(z);
  return 0;
}

int inorder_rbtree(node_t *root, key_t *res, rbtree *t, int i)
{
    if (root == t->nil)
    {
        return i;
    }
    
    i = inorder_rbtree(root->left, res, t, i);
    res[i] = root->key;
    i += 1;
    // printf("%p\t%d\n", res, root->key);
    i = inorder_rbtree(root->right, res, t, i);
    return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
    if (t->root == t->nil)
    {
        return -1;
    }
    inorder_rbtree(t->root, arr, t, 0);

    return 0;
}