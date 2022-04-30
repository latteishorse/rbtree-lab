#ifndef _BINARYTREE_H_
#define _BINARYTREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

// 노드의 데이터 타입
typedef char DATA;
// 노드: tree[]의 index로 노드를 가리킴
typedef int Node;

// 구성
// root node 생성
Node makeRoot(DATA data);
// root 이하의 모든 node
Node makeLeftChild(Node cur, DATA data);
Node makeRightChild(Node cur, DATA data);

// 데이터 가져오는 get함수
DATA getCurData(Node cur);
Node getLeftChild(Node cur);
DATA getLeftChildData(Node cur);

Node getRightChild(Node cur);
DATA getRightChildData(Node cur);

int isTreeEmpty(Node root); // 인자의 이름이 root일뿐, 아무 node나 주면 된다


#endif // _BINARYTREE_H_
