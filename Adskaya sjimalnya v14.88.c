#include <stdio.h>
#include "stdlib.h"
#include "derevo.h"
#include "string.h"
#include "keyio.h"
#include "fileio.h"

void ar() {
    char from[1000] = {0}, inn[1000], in[1000];
    gets(from);
    from[strlen(from)] = '\0';
    printf("Okaaaaaaaaaaay, let's goooooooo\n");
    strcpy(in, from);
    strcpy(inn, from);
    int qq = (int) strlen(from);
    in[qq] = '.';
    in[qq + 1] = 'b';
    in[qq + 2] = 'i';
    in[qq + 3] = 'b';
    in[qq + 4] = 'a';
    in[qq + 5] = '\0';
    NODE *list = NULL, *headic = NULL;
    printf("Status:\n");
    unsigned long long freq[256] = {0};
    open(freq, from);
    for (int i = 0; i < 256; ++i) {
        if (freq[i] == 0)
            continue;
        add2list(&list, freq[i], (unsigned char) i);
    }
    MakeTreeFromList(&list);
    strcat(from, ".key");
    unsigned char co[256] = {0};
    NumerateTree(&list, 0, co, -1, &headic);
    list = deltree(list);
    int k = writear(inn, &headic, in);
    FILE *fw = fopen64(from, "wb");
    if (!fw) {
        printf("Can't create file\n");
        exit(1);
    }
    writekey(fw, headic, k);
    printf("Mission complete\n");
    fclose(fw);
    headic = DeleteList(headic);
}

void unar() {
    char from[1000] = {0}, fromb[1000], fromi[1000];
    gets(from);
    from[strlen(from)] = '\0';
    printf("Okaaaaaaaaaaay, let's goooooooo\n");
    printf("Status:\n");
    strcpy(fromb, from);
    strcpy(fromi, from);
    strcat(fromb, ".biba");
    strcat(fromi, ".key");
    FILE *fri = fopen64(fromi, "rb");
    if (!fri) {
        printf("File '%s' not found\n", fromi);
        exit(1);
    }
    NODE *tmp[256] = {0};
    int size = 0, k = readkey(fri, tmp, &size);
    NODE *tree = (NODE *) calloc(1, sizeof(NODE));
    for (int i = 0; i < size; ++i)
        MakeTreeFromTree(&tree, tmp[i], 0);
    FILE *frb = fopen64(fromb, "rb");
    if (!frb) {
        printf("File '%s' not found\n", fromb);
        exit(1);
    }
    writeunar(frb, from, &tree, k, size, tmp);
    fclose(fri);
    deltree(tree);
    printf("Mission complete\n");
}

int main() {
    system("cls");
    printf("------------------------------------------\n");
    printf("| ADSKAYA SJIMALNYA V14.88 WELCOMES YOU! |\n");
    printf("------------------------------------------\n");
    printf("What do you want to do?\n\n[1] - Adsky sjat'\n[2] - Nechelovechesky razjat'\n> ");
    char q[1000];
    scanf("%s", q);
    if (!strcmp(q, "1")) {
        getchar();
        printf("Enter file name:\n> ");
        ar();
        return 0;
    } else if (!strcmp(q, "2")) {
        getchar();
        printf("Enter name of source file:\n> ");
        unar();
        return 0;
    }
    printf("Wrong input\n");
    return 2;
}
