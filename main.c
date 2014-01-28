#include <stdio.h>

char cetak(int i) {
    switch(i) {
        case -1:
            return 'X';
        case 0:
            return ' ';
        case 1:
            return 'O';
    }
}

void papan(int b[25]) {
    printf(" %c | %c | %c | %c | %c \n",cetak(b[0]),cetak(b[1]),cetak(b[2]),cetak(b[3]),cetak(b[4]));
    printf("---+---+---+---+---\n");
    printf(" %c | %c | %c | %c | %c \n",cetak(b[5]),cetak(b[6]),cetak(b[7]),cetak(b[8]),cetak(b[9]));
    printf("---+---+---+---+---\n");
    printf(" %c | %c | %c | %c | %c \n",cetak(b[10]),cetak(b[11]),cetak(b[12]),cetak(b[13]),cetak(b[14]));
    printf("---+---+---+---+---\n");
    printf(" %c | %c | %c | %c | %c \n",cetak(b[15]),cetak(b[16]),cetak(b[17]),cetak(b[18]),cetak(b[19]));
    printf("---+---+---+---+---\n");
    printf(" %c | %c | %c | %c | %c \n",cetak(b[20]),cetak(b[21]),cetak(b[22]),cetak(b[23]),cetak(b[24]));
}


main()
{
    printf("Komputer : O dan Kamu : X\n");
    printf("Mau bermain ke (1) atau ke (2)? ");
    int player=0;
    scanf("%d",&player);
    printf("\n");

    return(0);
}