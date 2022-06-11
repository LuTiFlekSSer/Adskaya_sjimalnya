#ifndef LAB1_KEYIO_H
#define LAB1_KEYIO_H

#include "stdio.h"
#include "derevo.h"

typedef union meta {
    unsigned char symb;
    struct shakal {
        unsigned ost: 3;
        unsigned size: 5;
    } s;
} meta;

void writekey(FILE *fw, NODE *headic, int k);

int readkey(FILE *fri, NODE **tmp, int *size);

#endif
