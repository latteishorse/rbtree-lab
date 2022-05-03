#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

// 추가한 함수
// .h 수정 없이 .c만 수정해서 구현해야하므로 상단에 추가
node_t *find_key(node_t *root, node_t *nil, const key_t key);
void free_rbtree(node_t *root, node_t *nil);
void left_rotate(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *x);
void rbtree_insert_fixup(rbtree *t, node_t *z);
void rbtree_erase_fixup(rbtree *t, node_t *x);
void rb_transplant(rbtree *t, node_t *u, node_t *v);
void make_array(node_t *root, node_t *nil, key_t *arr, const size_t n);

// init
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  
  p->root = p->nil;
  p->nil->color = RBTREE_BLACK;

  return p;
}

void delete_rbtree(rbtree *t) {
  free_rbtree(t->root, t->nil);
  free(t->nil);
  free(t);
  t = NULL;
}

void free_rbtree(node_t *root, node_t *nil) {
    if (root == nil)
      return;

  free_rbtree(root->left, nil);
  free_rbtree(root->right, nil);
  free(root);
  root = NULL;
}

// ITAR pseudo code 바탕으로 작성
void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;

  // y.left = T.nil이 될 때까지
  if (y->left != t->nil) 
      y->left->parent = x;

  y->parent = x->parent; // x의 부모를 y root로
  if (x->parent == t->nil) 
      t->root = y; // x가 root였다면 y를 root로
  
  else if (x == x->parent->left) 
      x->parent->left = y;    

  else 
      x->parent->right = y;
  
  y->left = x;
  x->parent = y;
}

// ITAR pseudo code 바탕으로 작성
void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;

  if (y->right != t->nil) 
      y->right->parent = x;

  y->parent = x->parent;
  if (x->parent == t->nil) 
      t->root = y;
  else if (x == x->parent->right) 
      x->parent->right = y;
  else 
      x->parent->left = y;

  y->right = x;
  x->parent = y;
}

/*
1. 삽입 전 RBT 속성 만족 상태
2. 삽입 방식은 BST와 동일
3. 삽입 후 RBT 위반 여부 확인
    - RBT 속성 위반 → 재조정
4. RBT 속성 만족
*/

node_t *rbtree_insert(rbtree *t, const key_t key) {

  // part1. 새 노드의 컬러를 RED로 삽입
  node_t *z = calloc(1, sizeof(node_t));
  node_t *y = t->nil;   
  node_t *x = t->root;

  z->key = key;

  // 삽입 위치 찾기
  while (x != t->nil) {
    y = x;
    if (key < x->key)
      x = x->left;
    else
      x = x->right;   
  }
  z->parent = y;

  // 빈 트리인 경우
  if (y == t->nil)
    t->root = z;  // 루트를 z로 설정

  // 키 삽입할 위치 찾기
  else if (key < y->key)
    y->left = z;
  else  
    y->right = z;

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED; // 삽입은 항상 red
  rbtree_insert_fixup(t, z);

  // return t->root;
  return z; // 입력 받은 값을 return -> assert로 find된 값과 비교하기 때문에
}

// insert한 다음 검증하기 위한 부분
void rbtree_insert_fixup(rbtree *t, node_t *z) {
  node_t *y;  // 삽입한 z의 삼촌 노드
  
  while (z->parent->color == RBTREE_RED) {

    if (z->parent == z->parent->parent->left) // 부모노드가 root->left
    {
      y = z->parent->parent->right; // right node 삼촌 노드
      // 삽입 노드 red, 부모 red, 부모 형제 red인 경우
      if (y->color == RBTREE_RED) 
      {
        z->parent->color = RBTREE_BLACK;  // 부모 red -> black
        y->color = RBTREE_BLACK;          // 삼촌 red -> black
        z->parent->parent->color = RBTREE_RED;  // 부모의 부모 -> red
        z = z->parent->parent;            // z의 위치를 할아버지로 변경. 할아버지랑 부모노드랑 같은 빨강일 수도 있으므로.
      }
      else{   // 삼촌 노드가 검정색인 경우
        if (z == z->parent->right) {  // z가 오른쪽 자식인 경우
          z = z->parent;              // left_rotate를 위한 사전 작업
          left_rotate(t, z);
        }

        z->parent->color = RBTREE_BLACK;        // right_rotate를 위한 사전 작업 1
        z->parent->parent->color = RBTREE_RED;  // right_rotate를 위한 사전 작업 2
        right_rotate(t, z->parent->parent);
      }
    }
    // 대칭 - 부모
    else {    
      y = z->parent->parent->left;   

      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else{
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

// RBT내에 해당 key가 있는지 탐색, 존재하면 해당 node pointer 반환
node_t *rbtree_find(const rbtree *t, const key_t key) {
  return find_key(t->root, t->nil, key);
}

node_t *find_key(node_t *root, node_t *nil, const key_t key) {
  if (root == nil)
    return NULL;  
  if (root->key == key)
    return root;
  else if (root->key > key)
    return find_key(root->left, nil, key);
  else
    return find_key(root->right, nil, key);
}

// 왼쪽 최하단 노드
// OK
node_t *rbtree_min(const rbtree *t) {

  node_t *p = t->root;

  while (p->left != t->nil)
    p = p->left;
  
  return p;
}

// 오른쪽 최하단 노드
// OK
node_t *rbtree_max(const rbtree *t) {

  node_t *p = t->root;

  while (p->right != t->nil)
    p = p->right;

  return p;
}

// RB tree 내부의 ptr로 지정된 node를 삭제하고 메모리 반환
int rbtree_erase(rbtree *t, node_t *z) {
  node_t *y = z;    // 삭제 후 자리를 대신할 노드 y
  node_t *x;        // z의 자식 노드 x (없을 수도 있음)

  color_t y_ori_color = y->color;
  // 자식 x or only child -> right 
  if (z->left == t->nil) {
    x = z->right;
    rb_transplant(t, z, z->right);
  }
  // only child left
  else if (z->right == t->nil) {
    x = z->left;
    rb_transplant(t, z, z->left);
  }
  // two child (left, right)
  else{
    // successor -> z의 right sub-tree 중 min
    y = z->right;
    while (y->left != t->nil){
      y = y->left;
    }

    y_ori_color = y->color;
    x = y->right;

    if (y->parent == z)
      x->parent = y;
    else{
      rb_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }

    rb_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  // 삭제한 부분 free
  free(z);
  z = NULL;
  if (y_ori_color == RBTREE_BLACK) // black이 삭제되면 재조정 필요
    rbtree_erase_fixup(t, x);
  
  return 0;
}

void rbtree_erase_fixup(rbtree *t, node_t *x) {
  node_t *w; // 형제 node
  while (x != t->root && x->color == RBTREE_BLACK) { 

    if (x == x->parent->left){
      w = x->parent->right;

      // case 1: x의 형제 w가 적색인 경우
      /*
        1. 부모와 형제 색을 바꾸고
        2. 부모 기준으로 왼쪽으로 회전
        3. doubly black을 기준으로 case 2,3,4 중에 하나로 해결
      */
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent); 
        w = x->parent->right;
      }

      // case 2: x의 형제 w는 흑색이고, w의 두 자식은 모두 흑색인 경우
      /*
        doubly black과 그 형제의 black을 모아서
        부모에게 전달 -> 부모가 extra black을 해결하도록 위임
      */

      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }

      else {
      // case 3: x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자식은 흑색인 경우
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
      // case 4: x의 형제 w는 흑색, w의 오른쪽 자식은 적색
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }

    // 대칭 case
    else {
      w = x->parent->left;
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }

        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

// erase할 때 필요
// 두 번째 매개 변수의 부모를 가져와서 세 번째 매개 변수를 가리키고,
// 세 번째 매개 변수의 부모를 두 번째 매개 변수의 부모로 가진다
void rb_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil) 
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  
  v->parent = u->parent;
}

// 중위순회하면 정렬된 배열을 얻을 수 있다
// -> key 순서대로 주어진 array 변환
int idx = 0;
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  make_array(t->root, t->nil, arr, n);
  idx = 0;
  return 0;
}

void make_array(node_t *root, node_t *nil, key_t *arr, const size_t n) {  
  if (root == nil)
    return;
    
  if (idx == n)
    return;

  make_array(root->left, nil, arr, n);
  arr[idx] = root->key; idx++;
  make_array(root->right, nil, arr, n);
}
