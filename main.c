#include <stdio.h>
#include <conio2.h>
#include <windows.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "main.h"
#include "utils.h"

int init();
int load_resources();
void on_event();
void on_update();
void on_render();
int cleanup();
int finalize();

#define BOARD_WIDTH     5
#define BOARD_HEIGHT    5

#define TILE_WIDTH      24
#define TILE_HEIGHT     24

int papan[BOARD_HEIGHT][BOARD_WIDTH];

SDL_Texture* boardTexture[2];

void board_draw(int board[BOARD_HEIGHT][BOARD_WIDTH]);

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

int main(int argc, char* argv[]) {
	// int pilih;
 //    int papan[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
 //    gotoxy(33,8);printf("TIC TAC TOE\n");
 //    gotoxy(32,9);printf("=====================\n");
	// gotoxy(33,10);printf("1. Mulai Permainan\n");
 //    gotoxy(33,11);printf("2. Keluar\n");
 //    gotoxy(32,12);printf("------------------\n");
 //    gotoxy(33,13);printf("Pilih : ");scanf("%i",&pilih);
 //    if (pilih == 1)
 //    {
 //    	clrscr();
 //    	//Simbol komputer ditandai dengan O dan Kamu dengan X
	// 	gotoxy(65,1);printf("Komputer : O");
	// 	gotoxy(65,2);printf("Kamu     : X");
	// 	printf("\nBermain ke (1) atau ke (2)? ");
 //    	int player=0;
 //    	scanf("%d",&player);
 //    	printf("\n");
 //    	unsigned turn;
 //    	for(turn = 0; turn < 25 && menang(papan) == 0; ++turn) 
	// 	{
 //        if((turn+player) % 2 == 0)
 //            gerak_komputer(papan);
 //        else {
 //            cetak(papan);
 //            gerak_player(papan);
 //        	}
 //    	}
 //    switch(menang(papan)) 
	// 	{
 //        case 0:
 //            textcolor(LIGHTBLUE);printf("\nPERMAINAN SERI\n");textcolor(WHITE);
 //            break;
 //        case 1:
 //            cetak(papan);
 //            textcolor(LIGHTRED);printf("\nKAMU KALAH\n");textcolor(WHITE);
 //            break;
 //        case -1:
 //            textcolor(LIGHTGREEN);printf("\nSELAMAT KAMU MENANG\n");textcolor(WHITE);
 //            break;
 //    	}
	// }
    
    /* init phase */ 
    if (init()) {
        fprintf(stderr, "Initialization failed!\n");
    }

    load_resources();

    /* game loop */
    printf("Starting game...\n");
    while(Game.flags.isRunning) {
        on_event();
        on_update();
        on_render();
        SDL_Delay(1000 / MAX_FPS);
    }

    /* shutdown phase */
    cleanup();
    finalize();

	return 0;
}

int init() {
    printf("Initializing...\n");

    printf("Initializing SDL.\n");
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    TTF_Init();

    printf("Creating windows.\n");
    Game.window = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (Game.window == NULL) {
        printf("SDL Create Window Error: %s\n", SDL_GetError());
        return 1;
    }
    
    printf("Creating renderer.\n");
    Game.renderer = SDL_CreateRenderer(Game.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (Game.renderer == NULL) {
        printf("SDL Renderer Error: %s\n", SDL_GetError());
        return 1;
    }

    Game.keystate = SDL_GetKeyboardState(NULL);

    Game.flags.isRunning = TRUE;
    Game.flags.isPaused  = FALSE;

    /* initialize game board */
    printf("Initializing game props.\n");
    for (int i = 0; i < 5; ++i) 
        for (int j = 0; j < 5; ++j) 
            papan[i][j] = 0;

    /* testing board --REMOVE ME-- */
    papan[3][2] = 1;
    papan[3][3] = 1;
    /* end */

    printf("Done.\n\n");
    return 0;
}

int load_resources() {
    printf("Loading resources...\n");

    boardTexture[0] = loadTexture("assets\\x.png", Game.renderer);
    boardTexture[1] = loadTexture("assets\\o.png", Game.renderer);

    printf("Done.\n\n");
    return 0;
}

void on_event() {
    SDL_PumpEvents();
    while (SDL_PollEvent(&Game.event)){
        if (!Game.flags.isPaused) {
            /* Specific keydown events */
            if (Game.event.type == SDL_KEYDOWN) {
                if(Game.keystate[SDL_SCANCODE_ESCAPE])
                    Game.flags.isRunning = FALSE;

            }

            /* Object events here! */

            /* End object events */


            /* Mouse events */
            if (Game.event.type == SDL_MOUSEBUTTONDOWN) {
                //Game.flags.isRunning = 0;
            }
        }

        /* If user clicked X button*/
        if (Game.event.type == SDL_QUIT)
            Game.flags.isRunning = FALSE;
    }
}

void on_update() {
    if (!Game.flags.isPaused) {

    }
    else {
        /* show isPaused control update */
    }
}

void on_render() {
    SDL_RenderClear(Game.renderer);

    SDL_SetRenderDrawColor(Game.renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(Game.renderer, NULL);

    board_draw(papan);
    if (Game.flags.isPaused) {
        /* show isPaused screen */
    }
    SDL_RenderPresent(Game.renderer);
}

int cleanup() {
    SDL_DestroyRenderer(Game.renderer);
    SDL_DestroyWindow(Game.window);

    return 0;
}

int finalize() {
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}

void board_draw(int board[BOARD_HEIGHT][BOARD_WIDTH]) {
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if (board[i][j] == 0) {
                renderTexture2(boardTexture[0], Game.renderer, i * TILE_HEIGHT, j * TILE_WIDTH, TILE_WIDTH, TILE_HEIGHT);
            }
            else if (board[i][j] == 1) {
                renderTexture2(boardTexture[1], Game.renderer, i * TILE_HEIGHT, j * TILE_WIDTH, TILE_WIDTH, TILE_HEIGHT);
            }
        }
    }
}