#include "keyio.h"
#include "bit2char.h"
#include "stdlib.h"

void writekey(FILE *fw, NODE *head, int k) {
    fprintf(fw, "%d", k < 8 ? k : 0);
    bit2char symb;
    meta m;
    while (head) {
        m.s.size = (unsigned) (head->level + 1) % 8 == 0 ? (unsigned) (head->level + 1) / 8 - 1 :
                   (unsigned) (head->level + 1) / 8;
        m.s.ost = (unsigned) (head->level + 1) % 8;
        fprintf(fw, "%c", m.symb);
        for (unsigned int i = 0; i < (unsigned) m.s.size + 1; ++i) {
            symb.mbit.b8 = (unsigned) head->code[i * 8];
            symb.mbit.b7 = (unsigned) head->code[i * 8 + 1];
            symb.mbit.b6 = (unsigned) head->code[i * 8 + 2];
            symb.mbit.b5 = (unsigned) head->code[i * 8 + 3];
            symb.mbit.b4 = (unsigned) head->code[i * 8 + 4];
            symb.mbit.b3 = (unsigned) head->code[i * 8 + 5];
            symb.mbit.b2 = (unsigned) head->code[i * 8 + 6];
            symb.mbit.b1 = (unsigned) head->code[i * 8 + 7];
            fprintf(fw, "%c", symb.symb);
        }
        fprintf(fw, "%c", head->symb);
        head = head->next;
    }
}

int readkey(FILE *fri, NODE **tmp, int *size) {
    int k = fgetc(fri) - '0';
    meta m;
    bit2char symb;
    while (!feof(fri)) {
        m.symb = fgetc(fri);
        if (feof(fri))
            break;
        tmp[*size] = (NODE *) calloc(1, sizeof(NODE));
        unsigned int lvl;
        if (m.s.ost == (unsigned) 0)
            lvl = ((unsigned int) m.s.size + 1) * 8;
        else
            lvl = (unsigned int) m.s.size * 8 + (unsigned int) m.s.ost;
        for (unsigned int i = 0; i < (unsigned int) m.s.size + 1; ++i) {
            symb.symb = fgetc(fri);
            tmp[(*size)]->code[i * 8] = (unsigned) symb.mbit.b8 + (unsigned) '0';
            tmp[(*size)]->code[i * 8 + 1] = (unsigned) symb.mbit.b7 + (unsigned) '0';
            tmp[(*size)]->code[i * 8 + 2] = (unsigned) symb.mbit.b6 + (unsigned) '0';
            tmp[(*size)]->code[i * 8 + 3] = (unsigned) symb.mbit.b5 + (unsigned) '0';
            tmp[(*size)]->code[i * 8 + 4] = (unsigned) symb.mbit.b4 + (unsigned) '0';
            tmp[(*size)]->code[i * 8 + 5] = (unsigned) symb.mbit.b3 + (unsigned) '0';
            tmp[(*size)]->code[i * 8 + 6] = (unsigned) symb.mbit.b2 + (unsigned) '0';
            tmp[(*size)]->code[i * 8 + 7] = (unsigned) symb.mbit.b1 + (unsigned) '0';
        }
        tmp[(*size)]->level = lvl;
        tmp[(*size)]->symb = fgetc(fri);
        ++(*size);
    }
    return k;
}