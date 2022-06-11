#ifndef LAB1_BIT2CHAR_H
#define LAB1_BIT2CHAR_H
typedef union bit2char {
    unsigned char symb;
    struct bit {
        unsigned b1: 1;
        unsigned b2: 1;
        unsigned b3: 1;
        unsigned b4: 1;
        unsigned b5: 1;
        unsigned b6: 1;
        unsigned b7: 1;
        unsigned b8: 1;
    } mbit;
} bit2char;
#endif
