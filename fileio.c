#include "stdio.h"
#include "fileio.h"
#include "stdlib.h"
#include "bit2char.h"
#include "time.h"
#include "math.h"
#include "string.h"

double ch = 0; //неловко вышло

void open(unsigned long long *freq, char *from) {
    FILE *fr = fopen64(from, "rb");
    if (!fr) {
        printf("File not found\n");
        exit(1);
    }
    double z = 0;
    _fseeki64(fr, 0, SEEK_END);
    unsigned long long length = _ftelli64(fr);
    //unsigned long long length = ftell(fr);
    _fseeki64(fr, 0, SEEK_SET);
    unsigned long long blocks = length / (256 * 1024), ost = length - blocks * 256 * 1024;
    unsigned char *buf = malloc(256 * 1024 * sizeof(unsigned char));
    double count = 0;
    clock_t t = clock();
    for (unsigned long long i = 0; i < blocks; ++i) {
        fread(buf, sizeof(char), 256 * 1024, fr);
        for (int s = 0; s < 256 * 1024; ++s) {
            if (((double) (i * s) / 2 / (double) length) * 100 >= z) {
                double q = count / (1024 * 1024) / ((double) (clock() - t) / CLOCKS_PER_SEC);
                printf("\r%d%% %.2lf MB/S     ", (int) z, isnan(q) ? 0 : q);
                ch = q;
                count = 0;
                t = clock();
                z += 5;
            }
            unsigned char symb = buf[s];
            freq[symb]++;
            ++count;
        }
    }
    if (ost != 0) {
        fread(buf, sizeof(char), ost, fr);
        for (int s = 0; s < ost; ++s)
            freq[buf[s]]++;
    }
    fclose(fr);
    free(buf);
}

int writear(char *from, NODE **headic, char *in) {
    printf("\r50%%");
    unsigned char str[8];
    FILE *fr = fopen64(from, "rb");
    bit2char sym;
    FILE *fw = fopen64(in, "wb");
    setvbuf(fw, NULL, _IOFBF, 256 * 1024);
    if (!fr)
        exit(1);
    if (!fw) {
        printf("Can't create file\n");
        exit(1);
    }
    _fseeki64(fr, 0, SEEK_END);
    unsigned long long length = _ftelli64(fr);
    //unsigned long long length = ftell(fr);
    _fseeki64(fr, 0, SEEK_SET);
    int k = 0;
    NODE *temp[256] = {0};
    NODE *head = *headic;
    while (head) {
        temp[(int) head->symb] = head;
        head = head->next;
    }
    unsigned char *res = malloc(256 * 1024 * sizeof(unsigned char)), *buf = malloc(256 * 1024 * sizeof(unsigned char));
    unsigned long long blocks = length / (256 * 1024), ost = length - blocks * 256 * 1024;
    unsigned long long ll = 0;
    double e = 0;
    clock_t t = clock(), tempt;
    double count = 0, q;
    for (unsigned long long i = 0; i < blocks; ++i) {
        fread(buf, sizeof(char), 256 * 1024, fr);
        for (int s = 0; s < 256 * 1024; ++s) {
            if (((double) (i * s) / (double) length) * 50 >= e) {
                tempt = clock();
                q = count / 4 / ((double) (tempt - t) / CLOCKS_PER_SEC);
                printf("\r%d%% %.2lf MB/S     ", (int) e + 50, isnan(q) ? ch / 2 : q);
                count = 0;
                t = tempt;
                e += 5;
            }
            for (int j = 0; j <= temp[buf[s]]->level; ++j) {
                str[k++] = temp[buf[s]]->code[j];
                if (k == 8) {
                    sym.mbit.b8 = (unsigned) str[0];
                    sym.mbit.b7 = (unsigned) str[1];
                    sym.mbit.b6 = (unsigned) str[2];
                    sym.mbit.b5 = (unsigned) str[3];
                    sym.mbit.b4 = (unsigned) str[4];
                    sym.mbit.b3 = (unsigned) str[5];
                    sym.mbit.b2 = (unsigned) str[6];
                    sym.mbit.b1 = (unsigned) str[7];
                    k = 0;
                    res[ll++] = sym.symb;
                    if (ll == 256 * 1024) {
                        fwrite(res, sizeof(char), 256 * 1024, fw);
                        ++count;
                        ll = 0;
                    }
                }
            }
        }
    }
    if (ost != 0) {
        fread(buf, sizeof(char), ost, fr);
        for (int s = 0; s < ost; ++s) {
            unsigned char symb = buf[s];
            for (int j = 0; j <= temp[(int) symb]->level; ++j) {
                str[k++] = temp[(int) symb]->code[j];
                if (k == 8) {
                    sym.mbit.b8 = (unsigned) str[0];
                    sym.mbit.b7 = (unsigned) str[1];
                    sym.mbit.b6 = (unsigned) str[2];
                    sym.mbit.b5 = (unsigned) str[3];
                    sym.mbit.b4 = (unsigned) str[4];
                    sym.mbit.b3 = (unsigned) str[5];
                    sym.mbit.b2 = (unsigned) str[6];
                    sym.mbit.b1 = (unsigned) str[7];
                    k = 0;
                    res[ll++] = sym.symb;
                    if (ll == 256 * 1024) {
                        fwrite(res, sizeof(char), 256 * 1024, fw);
                        ++count;
                        ll = 0;
                    }
                }
            }
        }
    }
    q = count / 4 / ((double) (clock() - t) / CLOCKS_PER_SEC);
    printf("\r100%% %.2f MB/S     \n", isnan(q) ? 0 : q);
    if (ll != 0) {
        fwrite(res, sizeof(char), ll, fw);
    }
    if (k != 0) {
        for (int i = k; i < 8; ++i)
            str[i] = '0';
        sym.mbit.b8 = (unsigned) str[0];
        sym.mbit.b7 = (unsigned) str[1];
        sym.mbit.b6 = (unsigned) str[2];
        sym.mbit.b5 = (unsigned) str[3];
        sym.mbit.b4 = (unsigned) str[4];
        sym.mbit.b3 = (unsigned) str[5];
        sym.mbit.b2 = (unsigned) str[6];
        sym.mbit.b1 = (unsigned) str[7];
        fprintf(fw, "%c", sym.symb);
    }
    fclose(fw);
    fclose(fr);
    free(res);
    free(buf);
    return 8 - k;
}

void writeunar(FILE *frb, char *from, NODE **tree, int k, int size, NODE **tmp) {
    bit2char symb;
    _fseeki64(frb, 0, SEEK_END);
    unsigned long long length = _ftelli64(frb);
    //unsigned long long length = ftell(frb);
    _fseeki64(frb, 0, SEEK_SET);
    unsigned long long blocks = length / (256 * 1024), ost = length - blocks * 256 * 1024, ll = 0;
    int le;
    double z = 0;
    NODE *w = *tree;
    FILE *fw = fopen64(from, "rb");
    char tt[1000] = {0};
    tt[0] = '1';
    tt[1] = '_';
    strcat(tt, from);
    if (fw) {
        fclose(fw);
        fw = fopen64(tt, "wb");
    } else
        fw = fopen64(from, "wb");
    if (!fw) {
        printf("Can't create file\n");
        exit(1);
    }
    setvbuf(fw, NULL, _IOFBF, 256 * 1024);
    clock_t t = clock(), temp;
    double count = 0, qq;
    unsigned char *res = malloc(256 * 1024 * sizeof(unsigned char)), *buf = malloc(
            256 * 1024 * sizeof(unsigned char)), *code = malloc(8 * 256 * 1024 * sizeof(unsigned char));
    for (unsigned long long q = 0; q < blocks; ++q) {
        fread(buf, sizeof(char), 256 * 1024, frb);
        le = 256 * 1024;
        for (int i = 0; i < le; ++i) {
            symb.symb = buf[i];
            code[i * 8] = symb.mbit.b8 + (unsigned) '0';
            code[i * 8 + 1] = symb.mbit.b7 + (unsigned) '0';
            code[i * 8 + 2] = symb.mbit.b6 + (unsigned) '0';
            code[i * 8 + 3] = symb.mbit.b5 + (unsigned) '0';
            code[i * 8 + 4] = symb.mbit.b4 + (unsigned) '0';
            code[i * 8 + 5] = symb.mbit.b3 + (unsigned) '0';
            code[i * 8 + 6] = symb.mbit.b2 + (unsigned) '0';
            code[i * 8 + 7] = symb.mbit.b1 + (unsigned) '0';
        }
        if (ost == 0 && q == blocks - 1)
            le = 256 * 1024 * 8 - k + 1;
        else
            le = 256 * 1024 * 8;
        for (int i = 0; i < le; ++i) {
            if (w->isSymb == 1) {
                res[ll++] = w->symb;
                if (ll == 256 * 1024) {
                    fwrite(res, sizeof(char), ll, fw);
                    ++count;
                    ll = 0;
                }
                w = *tree;
                if (((double) (i * q) / (double) length) * 12.5 >= z) {
                    temp = clock();
                    qq = count / 4 / ((double) (temp - t) / CLOCKS_PER_SEC);
                    printf("\r%d%% %.2lf MB/S     ", (int) z, isnan(qq) ? 0 : qq);
                    count = 0;
                    t = temp;
                    z += 5;
                }
            }
            if (code[i] == '0')
                w = w->left;
            else
                w = w->right;
        }
    }
    if (ost != 0) {
        fread(buf, sizeof(char), ost, frb);
        for (int i = 0; i < ost; ++i) {
            symb.symb = buf[i];
            code[i * 8] = symb.mbit.b8 + (unsigned int) '0';
            code[i * 8 + 1] = symb.mbit.b7 + (unsigned int) '0';
            code[i * 8 + 2] = symb.mbit.b6 + (unsigned int) '0';
            code[i * 8 + 3] = symb.mbit.b5 + (unsigned int) '0';
            code[i * 8 + 4] = symb.mbit.b4 + (unsigned int) '0';
            code[i * 8 + 5] = symb.mbit.b3 + (unsigned int) '0';
            code[i * 8 + 6] = symb.mbit.b2 + (unsigned int) '0';
            code[i * 8 + 7] = symb.mbit.b1 + (unsigned int) '0';
        }
        for (int i = 0; i < ost * 8 - k + 1; ++i) {
            if (w->isSymb == 1) {
                res[ll++] = w->symb;
                if (ll == 256 * 1024) {
                    fwrite(res, sizeof(char), ll, fw);
                    ++count;
                    ll = 0;
                }
                w = *tree;
            }
            if (code[i] == '0')
                w = w->left;
            else
                w = w->right;
        }
    }
    if (ll != 0)
        fwrite(res, sizeof(char), ll, fw);
    double q = count / 4 / ((double) (clock() - t) / CLOCKS_PER_SEC);
    printf("\r100%% %.2f MB/S     \n", isnan(q) ? 0 : q);
    for (int i = 0; i < size; ++i)
        free(tmp[i]);
    fclose(fw);
    free(res);
    free(buf);
    free(code);
}