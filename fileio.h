#ifndef LAB1_FILEIO_H
#define LAB1_FILEIO_H

#include "derevo.h"

int writear(char *from, NODE **headic, char *in);

void open(unsigned long long *freq, char *from);

void writeunar(FILE *frb, char *from, NODE **tree, int k, int size, NODE **tmp);

#endif
