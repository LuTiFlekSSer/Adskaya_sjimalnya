#ifndef LAB1_DEREVO_H
#define LAB1_DEREVO_H

typedef struct node {
    unsigned char symb;
    unsigned char isSymb;
    unsigned long long freq;
    unsigned char code[256];
    unsigned char level;
    struct node *left, *right, *next;
} NODE;

void MakeTreeFromList(NODE **head);

void add2list(NODE **head, unsigned long long value, unsigned char symb);

void NumerateTree(NODE **head, int fl, const unsigned char *co, int k, NODE **headic);

void MakeTreeFromTree(NODE **head, NODE *tmp, int i);

NODE *deltree(NODE *root);

NODE *DeleteList(NODE *phead);

#endif
