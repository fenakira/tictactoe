#include <stdio.h>
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

#define TILE_SIZE      64
#define SPACING         2

#define UNIT_TYPE_NONE  -1
#define UNIT_TYPE_X     0
#define UNIT_TYPE_O     1

typedef struct {
    int x, y;
    int tiles[BOARD_HEIGHT][BOARD_WIDTH];
    SDL_Texture* texture;
} Board;


typedef struct {
    int x;
    int y;
} Point;

Board papan;
SDL_Texture *boardUnit[2];

void init_board(Board *board);
void copy_board(Board *target, Board source);
void board_draw(Board board);
int  mouse_is_onboard(Board board, int mouseX, int mouseY);
void place_unit(int mouseX, int mouseY, Board *board, int type);
int is_validmove(Board board, int mouseX, int mouseY);
int is_win(Board board);

int turn;
int unitType;
int hasWinner;

void get_availablemoves(Board board, Point moves[BOARD_HEIGHT*BOARD_WIDTH], int *count) {
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if (board.tiles[i][j] == -1) {
                moves[(*count)].x = j;
                moves[(*count)].y = i;
                (*count)++;
            }        
        }
    }
}

int score(Board board, int depth) {
    if(is_win(board) == 1) 
        return 26 - depth;
    else if(is_win(board) == 2) 
        return depth - 26;

    return 0;
}

int minimax(Board board, int depth, int player) {
    if (is_win(board) || (depth == 1))
        return score(board, depth);
    
    depth++;

    int tileCount = BOARD_HEIGHT*BOARD_WIDTH;
    Point moves[tileCount];
    
    for (int i = 0; i < tileCount; ++i) {
        moves[i].x = 0;
        moves[i].y = 0;
    }

    int movesCount = 0;

    //get available moves
    get_availablemoves(board, moves, &movesCount);

    Point bestMove = { -1, -1 };
    int bestScore = -26;

    //loop each available moves
    for(int i = 0; i < movesCount; ++i) {
        //test current tile
        board.tiles[moves[i].y][moves[i].x] = player;

        //prepare temporary board for testing
        Board tmpBoard;
        copy_board(&tmpBoard, board);

        //get score
        int score = minimax(tmpBoard, depth, !player);

        //reset after test
        board.tiles[moves[i].y][moves[i].x] = -1;

        //compare with previous move and get the worst
        if (score > bestScore) {
            //store move and score
            bestScore = score;
            bestMove.x = moves[i].x;
            bestMove.y = moves[i].y;
        }
    }

   if ((bestMove.x == -1) && (bestMove.y == -1))
        return 0;

    return bestScore;
}

void computer_move(Board *board) {
    int tileCount = BOARD_HEIGHT*BOARD_WIDTH;
    Point moves[tileCount];
    
    for (int i = 0; i < tileCount; ++i) {
        moves[i].x = 0;
        moves[i].y = 0;
    }

    int movesCount = 0;

    //get available moves
    get_availablemoves(*board, moves, &movesCount);

    Point bestMove = { -1, -1 };
    int bestScore = -26;

    //loop each available moves
    for(int i = 0; i < movesCount; ++i) {
        //test current tile
        board->tiles[moves[i].y][moves[i].x] = 1;

        //prepare temporary board for testing
        Board tmpBoard;
        copy_board(&tmpBoard, *board);

        //get score
        int score = minimax(tmpBoard, 0, 0);

        //reset after test
        board->tiles[moves[i].y][moves[i].x] = -1;

        //compare with previous move and get the worst
        if (score > bestScore) {
            //store move and score
            bestScore = score;
            bestMove.x = moves[i].x;
            bestMove.y = moves[i].y;
        }
    }

    //move at the best tile
    board->tiles[bestMove.y][bestMove.x] = 1;
}

int main(int argc, char* argv[]) {
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
    printf("Cleaning up...\n");
    cleanup();
    finalize();
    printf("Done.\n\n");

    printf("Exiting...\n");
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
    init_board(&papan);

    papan.x = 30;
    papan.y = 30;

    /* initialize game statistics */
    turn      = 1;
    unitType  = UNIT_TYPE_X;
    hasWinner = 0;

    printf("Done.\n\n");
    return 0;
}

int load_resources() {
    printf("Loading resources...\n");

    papan.texture = loadTexture("..\\assets\\board.png", Game.renderer);
    boardUnit[0]  = loadTexture("..\\assets\\x.png", Game.renderer);
    boardUnit[1]  = loadTexture("..\\assets\\o.png", Game.renderer);

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
                if (Game.event.button.button == SDL_BUTTON_LEFT) {
                    if (!hasWinner) {
                        if (mouse_is_onboard(papan, Game.event.button.x, Game.event.button.y)) {
                            if (is_validmove(papan, Game.event.button.x, Game.event.button.y)) {
                                place_unit(Game.event.button.x, Game.event.button.y, &papan, unitType);
                                hasWinner = is_win(papan);
                                turn++;
                                unitType = !unitType;
                            }
                        }
                    }
                }
            }
        }

        /* If user clicked X button*/
        if (Game.event.type == SDL_QUIT)
            Game.flags.isRunning = FALSE;
    }
}

void on_update() {
    if (!Game.flags.isPaused) {
        /* Move AI */
        int maxTurn = BOARD_WIDTH * BOARD_HEIGHT;

        if ((turn <= maxTurn) && !is_win(papan)) {
            if ((turn % 2) == 0) {
                unitType = !unitType;
                computer_move(&papan);
                hasWinner = is_win(papan);
                turn++;
            }
        }
    }
    else {
        /* show isPaused control update */
    }
}

void on_render() {
    SDL_RenderClear(Game.renderer);

    /* Set white backround color */
    SDL_SetRenderDrawColor(Game.renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(Game.renderer, NULL);

    /* Draw board */
    board_draw(papan);

    /* Check for winning */
    int maxTurn = BOARD_WIDTH * BOARD_HEIGHT;
    SDL_Texture* winMessage = NULL;
    SDL_Color fontColor = {0, 115, 255};

    if (hasWinner == 1) {
        winMessage = renderText(Game.renderer, "X Win!", "..\\assets\\sf_atarian_system.ttf", fontColor, 72);
    }
    else if(hasWinner == 2) {
        winMessage = renderText(Game.renderer, "O Win!", "..\\assets\\sf_atarian_system.ttf", fontColor, 72);
    }
    else if(turn > maxTurn) {
        winMessage = renderText(Game.renderer, "Draw!", "..\\assets\\sf_atarian_system.ttf", fontColor, 72);
    }

    // /* Draw Win Message */
    if (winMessage != NULL) {
        renderTexture(winMessage, Game.renderer, 215, 200);
        SDL_DestroyTexture(winMessage);
    }

    if (Game.flags.isPaused) {
        /* show isPaused screen */
    }

    SDL_RenderPresent(Game.renderer);
}

int cleanup() {
    /* Unload game textures from memory */
    SDL_DestroyTexture(papan.texture);
    SDL_DestroyTexture(boardUnit[0]);
    SDL_DestroyTexture(boardUnit[1]);

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

void init_board(Board *board) {
    for (int i = 0; i < BOARD_HEIGHT; ++i) 
        for (int j = 0; j < BOARD_WIDTH; ++j) 
            board->tiles[i][j] = -1;

}

void copy_board(Board *target, Board source) {
    for (int i = 0; i < BOARD_HEIGHT; ++i)
        for (int j = 0; j < BOARD_WIDTH; ++j)
            target->tiles[i][j] = source.tiles[i][j];

}

void board_draw(Board board) {
    renderTexture(board.texture, Game.renderer, board.x, board.y);

    /* draw units */
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if (board.tiles[i][j] == UNIT_TYPE_X) {
                renderTexture2(boardUnit[0], Game.renderer, board.x + (j * TILE_SIZE) + (SPACING * (j+1)), board.y + (i * TILE_SIZE) + (SPACING * (i+1)), TILE_SIZE, TILE_SIZE);
            }
            else if (board.tiles[i][j] == UNIT_TYPE_O) {
                renderTexture2(boardUnit[1], Game.renderer, board.x + (j * TILE_SIZE) + (SPACING * (j+1)), board.y + (i * TILE_SIZE) + (SPACING * (i+1)), TILE_SIZE, TILE_SIZE);
            }
        }
    }
}

int mouse_is_onboard(Board board, int mouseX, int mouseY) {
    if ((mouseX >= board.x) && 
        (mouseX <= (TILE_SIZE * 5) + (SPACING * 5) + board.x) && 
        (mouseY >= board.y) && 
        (mouseY <= (TILE_SIZE * 5) + (SPACING * 5) + board.y))
        return 1;

    return 0;
}

void place_unit(int mouseX, int mouseY, Board *board, int type) {
    int x = (mouseX - board->x - SPACING)  / (TILE_SIZE + SPACING);
    int y = (mouseY - board->y - SPACING)  / (TILE_SIZE + SPACING);

    board->tiles[y][x] = type;
}

int is_win(Board board) {
    int win = 0;
    int type = UNIT_TYPE_NONE;

    /* horizontal checking */
    for(int i = 0; i < BOARD_WIDTH; ++i) {
        if(board.tiles[i][0] == board.tiles[i][1] &&
           board.tiles[i][0] == board.tiles[i][2] &&
           board.tiles[i][0] == board.tiles[i][3] &&
           board.tiles[i][0] == board.tiles[i][4] &&
           board.tiles[i][0] != -1) {
            win = 1;
            type = board.tiles[i][0];
        }
    }

    /* vertical checking */
    if (!win) {
        for(int i = 0; i < BOARD_HEIGHT; ++i) {
            if(board.tiles[0][i] == board.tiles[1][i] &&
               board.tiles[0][i] == board.tiles[2][i] &&
               board.tiles[0][i] == board.tiles[3][i] &&
               board.tiles[0][i] == board.tiles[4][i] &&
               board.tiles[0][i] != -1) {
                win = 1;
                type = board.tiles[0][i];
            }
        }
    }


    /* diagonal checking */
    if (!win) {
        if(board.tiles[0][0] == board.tiles[1][1] &&
            board.tiles[0][0] == board.tiles[2][2] &&
            board.tiles[0][0] == board.tiles[3][3] &&
            board.tiles[0][0] == board.tiles[4][4] &&
            board.tiles[0][0] != -1) {
            win = 1;
            type = board.tiles[0][0];

        }
        else if(board.tiles[0][4] == board.tiles[1][3] &&
            board.tiles[0][4] == board.tiles[2][2] &&
            board.tiles[0][4] == board.tiles[3][1] &&
            board.tiles[0][4] == board.tiles[4][0] &&
            board.tiles[0][4] != -1) {
            win = 1;
            type = board.tiles[0][4];
        }
    }

    if (win) {
        if(type == UNIT_TYPE_X)
            return 1; //X Win
        else if(type == UNIT_TYPE_O)
            return 2; //O Win    
    }

    return 0;
}

int is_validmove(Board board, int mouseX, int mouseY) {
    int x = (mouseX - board.x - SPACING)  / (TILE_SIZE + SPACING);
    int y = (mouseY - board.y - SPACING)  / (TILE_SIZE + SPACING);

    if (board.tiles[y][x] == UNIT_TYPE_NONE) {
        return 1;
    }

    return 0;
}