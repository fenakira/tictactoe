#include "utils.h"

#include <stdio.h>
#include <windows.h>

SDL_Texture* loadTexture(const char* file, SDL_Renderer* renderer) {
	SDL_Texture* texture     = NULL;
	SDL_Surface* loadedImage = IMG_Load(file);

	if (loadedImage != NULL) {
		texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
		SDL_FreeSurface(loadedImage);

		if (texture == NULL) {
			fprintf(stderr, "Create texture error: %s\n", SDL_GetError());
		}
	}
	else {
		MessageBoxA(NULL, IMG_GetError(), "Error", MB_OK | MB_ICONEXCLAMATION);
		return NULL;
	}

	return texture;
}

void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y) {
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;

	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void renderTexture2(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int w, int h) {
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;

	SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void renderTexture3(SDL_Texture* texture, SDL_Renderer* renderer, int srcX, int srcY, int srcW, int srcH, int destX, int destY) {
	SDL_Rect src, dest;

	src.x = srcX;
	src.y = srcY;
	src.w = srcW;
	src.h = srcH;

	dest.x = destX;
	dest.y = destY;

	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(renderer, texture, &src, &dest);
}

void renderTexture4(SDL_Texture* texture, SDL_Renderer* renderer, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH) {
	SDL_Rect src, dest;

	src.x = srcX;
	src.y = srcY;
	src.w = srcW;
	src.h = srcH;

	dest.x = destX;
	dest.y = destY;
	dest.w = destW;
	dest.h = destH;

	SDL_RenderCopy(renderer, texture, &src, &dest);
}

void renderTexture5(SDL_Texture* texture, SDL_Renderer* renderer, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH, double angle) {
	SDL_Rect src, dest;

	src.x = srcX;
	src.y = srcY;
	src.w = srcW;
	src.h = srcH;

	dest.x = destX;
	dest.y = destY;
	dest.w = destW;
	dest.h = destH;

	SDL_RenderCopyEx(renderer, texture, &src, &dest, angle, NULL, SDL_FLIP_NONE);
}

SDL_Texture* renderText(SDL_Renderer* renderer, const char* message, const char* fontFile, SDL_Color color, int fontSize) {
	TTF_Font* font = TTF_OpenFont(fontFile, fontSize);
	if (font == NULL) {
		MessageBoxA(NULL, TTF_GetError(), "Error", MB_OK | MB_ICONEXCLAMATION);
		return NULL;
	}

	SDL_Surface* surface = TTF_RenderText_Blended(font, message, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);

	return texture;
}

SDL_Texture* renderText2(SDL_Renderer* renderer, const char* message, TTF_Font* font, SDL_Color color) {
	if (font == NULL) {
		MessageBoxA(NULL, TTF_GetError(), "Error", MB_OK | MB_ICONEXCLAMATION);
		return NULL;
	}

	SDL_Surface* surface = TTF_RenderText_Blended(font, message, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	
	SDL_FreeSurface(surface);

	return texture;
}