#include <stdio.h>
#include <conio2.h>

char isi_kotak(int i) {
    switch(i) {
        case -1:
            return 'X';
        case 0:
            return ' ';
        case 1:
            return 'O';
    }
}

void cetak(int b[25]) {
    printf(" %c | %c | %c | %c | %c \n",isi_kotak(b[0]),isi_kotak(b[1]),isi_kotak(b[2]),isi_kotak(b[3]),isi_kotak(b[4]));
    printf("---+---+---+---+---\n");
    printf(" %c | %c | %c | %c | %c \n",isi_kotak(b[5]),isi_kotak(b[6]),isi_kotak(b[7]),isi_kotak(b[8]),isi_kotak(b[9]));
    printf("---+---+---+---+---\n");
    printf(" %c | %c | %c | %c | %c \n",isi_kotak(b[10]),isi_kotak(b[11]),isi_kotak(b[12]),isi_kotak(b[13]),isi_kotak(b[14]));
    printf("---+---+---+---+---\n");
    printf(" %c | %c | %c | %c | %c \n",isi_kotak(b[15]),isi_kotak(b[16]),isi_kotak(b[17]),isi_kotak(b[18]),isi_kotak(b[19]));
    printf("---+---+---+---+---\n");
    printf(" %c | %c | %c | %c | %c \n",isi_kotak(b[20]),isi_kotak(b[21]),isi_kotak(b[22]),isi_kotak(b[23]),isi_kotak(b[24]));
}

int menang(const int papan[25]) {
    unsigned menang[24][5] = {{0,1,2,3,4},{5,6,7,8,9},{10,11,12,13,14},{15,16,17,18,19},{20,21,22,23,24},
							{0,5,10,15,20},{1,6,11,16,21},{2,7,12,17,22,},{3,8,13,18,23},{4,9,14,19,24},
							{0,6,12,18,24},{4,8,12,16,20}};
	int i;
    for(i = 0; i < 8; ++i) {
        if(papan[menang[i][0]] != 0 &&
           papan[menang[i][0]] == papan[menang[i][1]] &&
           papan[menang[i][0]] == papan[menang[i][2]] &&
           papan[menang[i][0]] == papan[menang[i][3]] &&
           papan[menang[i][0]] == papan[menang[i][4]])
            return papan[menang[i][4]];
    }
    return 0;
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