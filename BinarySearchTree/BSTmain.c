#include <stdio.h>
#include <stdlib.h>

typedef char data;
typedef struct _Node {
    char key;
    struct _Node* left;
    struct _Node* right;
} Node;

Node* searchBST(Node* root, char x){
    Node* p = root;
    while (p != NULL) {
        if (p->key == x)
            return p;
        else if (p-> key < x)
            p = p -> right;
        else
            p = p -> left;
    }
    return NULL;
}

Node* insertBST(Node* root, char x){
    Node* p = root;
    Node* parent = NULL;

    while (p != NULL) {
        if (p->key == x) {
            printf("같은 키가 있습니다.\n");
            return p;
        }
        else if (p-> key < x)
            p = p -> right;
        else
            p = p -> left;
    }
    // 새 노드 할당
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode -> key = x;
    newNode -> left = newNode -> right = NULL;

    // parent의 자식으로 새 노드 붙이기
    if (parent != NULL) {
        if (parent -> key < newNode -> key)
            parent -> right = newNode;
        else
            parent-> left = newNode;
    }
    return NULL;
}

Node* deleteBST(Node* root, char x){
    Node* p = root;
    Node* parent = NULL;

    while ((p != NULL) && (p->key != x)) {
        if (p->key == x) {
            printf("같은 키가 있습니다.\n");
            return p;
        }
        else if (p-> key < x)
            p = p -> right;
        else
            p = p -> left;
    }
    if (p == NULL) {
        printf("찾는 노드가 없습니다.\n");
        return root;
    }
    // 차수가 0인 경우
    // 단말 node 삭제
    if (p->left == NULL && p-> right ==NULL) {
        if(parent == NULL)
            root = NULL;
        else{
            if (parent->left == p)
                parent->left = NULL;
            else
                parent->right = NULL;
        }
    }
    // 차수가 1인 경우
    else if (p->left == NULL || p-> right ==NULL) {
        Node *child = (p->left != NULL) ? p->left : p->right;
        if (parent == NULL)
            root = child;
        else{
            if (parent->left == p)
                parent->left = child;
            else
                parent->right = child;            
        }

    }
    // 차수 2
    else {
        Node* succ_parent = p;
        Node* succ = p->right;
        while (succ->left !=NULL) {
            succ_parent = succ;
            succ = succ -> left;
        }
        p -> key = succ -> key;
        if (succ_parent->left == succ)
            succ_parent->left = succ -> right;
        else
            succ_parent->right = succ-> right;
        p = succ;
    }
    free(p);
    return root;
}

int main(){
    

    return 0;
};