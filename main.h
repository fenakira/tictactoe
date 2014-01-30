#ifndef _MAIN_H_
#define _MAIN_H_

struct gameFlags {
	int isRunning, isPaused;
};

struct _game {
	struct gameFlags flags;
	const Uint8* 	 keystate;
	SDL_Window* 	 window;
	SDL_Renderer* 	 renderer;
	SDL_Event 		 event;
} Game;

#define GAME_TITLE      "Tic Tac Toe"
#define	MAX_FPS			30

#endif
/* _MAIN_H_ */
