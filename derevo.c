#include "derevo.h"
#include "stdlib.h"

void add2list(NODE **head, unsigned long long value, unsigned char symb) {
    while (*head) {
        if ((*head)->freq > value)
            break;
        head = &((*head)->next);
    }
    NODE *new = (NODE *) calloc(1, sizeof(NODE));
    new->freq = value;
    new->isSymb = 1;
    new->symb = symb;
    new->next = *head;
    *head = new;
}

void add2List(NODE **head, unsigned char value, unsigned int sort, const unsigned char *co, int j) {
    while (*head) {
        if ((*head)->freq < sort)
            break;
        head = &((*head)->next);
    }
    NODE *new = (NODE *) calloc(1, sizeof(NODE));
    new->symb = value;
    new->freq = sort;
    new->level = j;
    for (int i = 0; i <= j; ++i)
        new->code[i] = co[i];
    new->next = *head;
    *head = new;
}

void Add2List(NODE **head, NODE *value) {
    if (!(*head)) {
        (*head) = value;
        return;
    }
    NODE *new = (*head);
    NODE *old = (*head);
    if ((*head) && new->freq > value->freq)
        new = new->next;
    else
        while (new) {
            if (new->freq > value->freq)
                break;
            old = new;
            new = new->next;
        }
    value->next = new;
    old->next = value;
}

NODE *MakeNodeFromNode(NODE *left, NODE *right) {
    NODE *res = (NODE *) calloc(1, sizeof(NODE));
    res->freq = left->freq + right->freq;
    res->left = left;
    res->right = right;
    res->level = left->level > right->level ? left->level + 1 : right->level + 1;
    return res;
}

void MakeTreeFromList(NODE **head) {
    while ((*head) && (*head)->next) {
        NODE *left = (*head);
        NODE *right = (*head)->next;
        Add2List(&((*head)->next->next), MakeNodeFromNode(left, right));
        (*head) = (*head)->next->next;
    }
}


void MakeTreeFromTree(NODE **head, NODE *tmp, int i) {
    if (tmp->level == i) {
        (*head)->symb = tmp->symb;
        (*head)->isSymb = 1;
        return;
    }
    if (tmp->code[i] == '0') {
        if (!(*head)->left)
            (*head)->left = (NODE *) calloc(1, sizeof(NODE));
        MakeTreeFromTree(&(*head)->left, tmp, i + 1);
    } else {
        if (!(*head)->right)
            (*head)->right = (NODE *) calloc(1, sizeof(NODE));
        MakeTreeFromTree(&(*head)->right, tmp, i + 1);
    }
}

void NumerateTree(NODE **head, int fl, const unsigned char *co, int k, NODE **headic) {
    if (*head) {
        if (k != -1) {
            (*head)->level = k;
            for (int i = 0; i < 256; ++i)
                (*head)->code[i] = co[i];
            (*head)->code[k] = fl;
        }
        if ((*head)->isSymb == 1)
            add2List(headic, (*head)->symb, (*head)->freq, (*head)->code, (*head)->level);
        NumerateTree(&(*head)->left, 0, (*head)->code, 1 + k, headic);
        NumerateTree(&(*head)->right, 1, (*head)->code, 1 + k, headic);
    }
}

NODE *deltree(NODE *root) {
    if (root) {
        root->left = deltree(root->left);
        root->right = deltree(root->right);
        free(root);
    }
    return NULL;
}

NODE *DeleteList(NODE *phead) {
    if (phead) {
        DeleteList(phead->next);
        free(phead);
    }
    return NULL;
}