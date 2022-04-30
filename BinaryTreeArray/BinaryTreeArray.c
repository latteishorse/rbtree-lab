#include "BinaryTreeArray.h"

// tree
#define NODE_MAXCOUNT 100
DATA tree[NODE_MAXCOUNT] = {0};
 
Node makeRoot(DATA data){
    tree[1] = data;
    return 1;
}

Node makeLeftChild(Node cur, DATA data){
    tree[cur * 2] = data;
    return cur * 2;
}

Node makeRightChild(Node cur, DATA data){
    tree[cur * 2 + 1] = data;
    return cur * 2 + 1;
}

DATA getCurData(Node cur){
    return tree[cur];
}

Node getLeftChild(Node cur){
    return cur * 2;
}

DATA getLeftChildData(Node cur){
    return tree[cur * 2];
}

Node getRightChild(Node cur){
    return cur * 2 + 1;
}

DATA getRightChildData(Node cur){
    return tree[cur * 2 + 1];
}

int isTreeEmpty(Node root){
    if (tree[root] == 0)
        return 1;
    else
        return 0;
} 