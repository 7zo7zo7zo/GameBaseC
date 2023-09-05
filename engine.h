#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>

extern int width;
extern int height;

extern SDL_Window *window;
extern SDL_Renderer *renderer;

int initEngine();
int gameLoop();
int initRender();

#endif
