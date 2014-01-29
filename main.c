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
    printf(" %c | %c | %c | %c | %c\n",isi_kotak(b[0]),isi_kotak(b[1]),isi_kotak(b[2]),isi_kotak(b[3]),isi_kotak(b[4]));
    printf("---+---+---+---+---\n");
    printf(" %c | %c | %c | %c | %c\n",isi_kotak(b[5]),isi_kotak(b[6]),isi_kotak(b[7]),isi_kotak(b[8]),isi_kotak(b[9]));
    printf("---+---+---+---+---\n");
    printf(" %c | %c | %c | %c | %c\n",isi_kotak(b[10]),isi_kotak(b[11]),isi_kotak(b[12]),isi_kotak(b[13]),isi_kotak(b[14]));
    printf("---+---+---+---+---\n");
    printf(" %c | %c | %c | %c | %c\n",isi_kotak(b[15]),isi_kotak(b[16]),isi_kotak(b[17]),isi_kotak(b[18]),isi_kotak(b[19]));
    printf("---+---+---+---+---\n");
    printf(" %c | %c | %c | %c | %c\n",isi_kotak(b[20]),isi_kotak(b[21]),isi_kotak(b[22]),isi_kotak(b[23]),isi_kotak(b[24]));
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

int minimax(int papan[25], int player) {
    int pemenang = menang(papan);
    if(pemenang != 0) return pemenang*player;

    int pindah = -1;
    int skor = -2;
    int i;
    for(i = 0; i < 9; i++) {
        if(papan[i] == 0) {
            papan[i] = player;
            int thisSkor = -minimax(papan, player*-1);
            if(thisSkor > skor) {
                skor = thisSkor;
                pindah = i;
            }
            papan[i] = 0;
        }
    }
    if(pindah == -1) return 0;
    return skor;
}

void gerak_komputer(int papan[25]) {
    int pindah = -1;
    int skor = -2;
    int i;
    for(i = 0; i < 25; ++i) {
        if(papan[i] == 0) {
            papan[i] = 1;
            int tempSkor = -minimax(papan, -1);
            papan[i] = 0;
            if(tempSkor > skor) {
                skor = tempSkor;
                pindah = i;
            }
        }
    }

    papan[pindah] = 1;
}

void gerak_player(int papan[25]) {
    int pindah = 0;
    do {
        printf("Pindah ke kotak [0..24]: ");
        scanf("%d", &pindah);
        printf("\n");
    } while (pindah >= 25 || pindah < 0 && papan[pindah] == 0);
    papan[pindah] = -1;
}

int main() {
	int pilih;
    int papan[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    gotoxy(33,8);printf("TIC TAC TOE\n");
    gotoxy(32,9);printf("=====================\n");
	gotoxy(33,10);printf("1. Mulai Permainan\n");
    gotoxy(33,11);printf("2. Keluar\n");
    gotoxy(32,12);printf("------------------\n");
    gotoxy(33,13);printf("Pilih : ");scanf("%i",&pilih);
    if (pilih == 1)
    {
    	clrscr();
    	//Simbol komputer ditandai dengan O dan Kamu dengan X
		gotoxy(65,1);printf("Komputer : O");
		gotoxy(65,2);printf("Kamu     : X");
		printf("\nBermain ke (1) atau ke (2)? ");
    	int player=0;
    	scanf("%d",&player);
    	printf("\n");
    	unsigned turn;
    	for(turn = 0; turn < 25 && menang(papan) == 0; ++turn) 
		{
        if((turn+player) % 2 == 0)
            gerak_komputer(papan);
        else {
            cetak(papan);
            gerak_player(papan);
        	}
    	}
    switch(menang(papan)) 
		{
        case 0:
            textcolor(LIGHTBLUE);printf("\nPERMAINAN SERI\n");textcolor(WHITE);
            break;
        case 1:
            cetak(papan);
            textcolor(LIGHTRED);printf("\nKAMU KALAH\n");textcolor(WHITE);
            break;
        case -1:
            textcolor(LIGHTGREEN);printf("\nSELAMAT KAMU MENANG\n");textcolor(WHITE);
            break;
    	}
	}else
	return(0);
}
