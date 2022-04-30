#include "BinaryTreeArray.h"

void main(){
    Node n1 = makeRoot('-');
    Node n2 = makeLeftChild(n1, '*');
    Node n3 = makeRightChild(n1, '/');

    Node n4 = makeLeftChild(n2, 'A');
    Node n5 = makeRightChild(n2, 'B');

    Node n6 = makeLeftChild(n3, 'C');
    Node n7 = makeRightChild(n3, 'D');
}