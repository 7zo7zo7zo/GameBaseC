#include <SDL2/SDL.h>
#include <stdbool.h>

#include "engine.h"
#include "tick.h"
#include "render.h"

int width = 800;
int height = 600;
bool running = false;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int initEngine() {
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow(
		"Game", 
		SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
		SDL_WINDOW_SHOWN
	);

	if(!window) {
		fprintf(stderr, "Failed to initialize window: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(
		window,
		-1, 
		SDL_RENDERER_ACCELERATED
	);

	if(!renderer) {
		fprintf(stderr, "Failed to initialize renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		return 1;
	}

	return 0;
}

int gameLoop() {
	SDL_Event event;
	Uint32 lastTick = SDL_GetTicks();
	
	float tps = 60.0f;
	float frameDelta = 1000.0f / tps;
	float timer = 0;
	int ticks = 0;
	int frames = 0;
	running = true;
	Uint32 currTick;

	while(running) {
		while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					fprintf(stdout, "%d\n", event.key.keysym.sym);
					break;
			}
		}
		currTick = SDL_GetTicks();
		float elapsed = currTick - lastTick;
		if(frameDelta > elapsed) {
			//fprintf(stdout, "%f\n", frameDelta - elapsed);
			SDL_Delay(frameDelta - elapsed);
		}
		currTick = SDL_GetTicks();

		// tick
		timer += currTick - lastTick;
		tick();
		ticks++;

		// render
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		render();
		frames++;
		SDL_RenderPresent(renderer);

		lastTick = currTick;
		if(timer >= 1000) {
			fprintf(stdout, "%d tps and %d fps\n", ticks, frames);
			ticks = 0	;
			frames = 0;
			timer -= 1000;
		}
	}
	if(renderer) SDL_DestroyRenderer(renderer);
	if(window) SDL_DestroyWindow(window);
	SDL_Quit();
	fprintf(stdout, "Exiting...\n");
	return 0;
}
