#ifndef _UTILS_H_
#define _UTILS_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

/* SDL Section */
SDL_Texture* loadTexture(const char* file, SDL_Renderer* renderer);

void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y);
void renderTexture2(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int w, int h);
void renderTexture3(SDL_Texture* texture, SDL_Renderer* renderer, int srcX, int srcY, int srcW, int srcH, int destX, int destY);
void renderTexture4(SDL_Texture* texture, SDL_Renderer* renderer, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH);
void renderTexture5(SDL_Texture* texture, SDL_Renderer* renderer, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH, double angle);

SDL_Texture* renderText(SDL_Renderer* renderer, const char* message, const char* fontFile, SDL_Color color, int fontSize);
SDL_Texture* renderText2(SDL_Renderer* renderer, const char* message, TTF_Font* font, SDL_Color color);
/* End SDL Section */

#endif
/* _UTILS_H_ */
