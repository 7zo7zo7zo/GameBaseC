#include <SDL2/SDL.h>
#include <stdbool.h>

#include "engine.h"
#include "tick.h"
#include "render.h"
#include "input.h"

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
		//| SDL_RENDERER_PRESENTVSYNC
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
	
	int tps = 20;
	int maxFPS = 100;
	float frameDelta = 1000.0f / maxFPS;
	float tickDelta = 1000.0f / tps;
	float timer = 0;
	int ticks = 0;
	int frames = 0;
	running = true;
	//Uint32 currTick;
	float elapsedT = 0;
	float elapsedF = 0;
	float diffT;
	float diffF;
	bool fpsUncapped = true;
	bool tpsUncapped = false;

	while(running) {
		// input
		while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					input(event.key.keysym.sym);
					//fprintf(stdout, "%d\n", event.key.keysym.sym);
					break;
			}
		}
		// tick
		if(tpsUncapped || elapsedT >= tickDelta) {
			tick(elapsedT);
			ticks++;
			elapsedT = 0;
		}
		diffT = tickDelta - elapsedT;
		//if(diffT < 0) diffT = 0;
		if(diffT < 1) diffT = 1;
		// render
		if(fpsUncapped || elapsedF >= frameDelta) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			render(elapsedF);
			frames++;
			SDL_RenderPresent(renderer);
			elapsedF = 0;
		}
		diffF = frameDelta - elapsedF;
		//if(diffF < 0) diffF = 0;
		if(diffF < 1) diffF = 1;
		// delay
		if(fpsUncapped || tpsUncapped) {
			SDL_Delay(1);
		}
		else {
			if(diffF <= diffT) {
				SDL_Delay(diffF);
			}
			else {
				SDL_Delay(diffT);
			}
		}

		timer += SDL_GetTicks() - lastTick;
		if(timer >= 1000) {
			fprintf(stdout, "%d tps and %d fps\n", ticks, frames);
			ticks = 0	;
			frames = 0;
			timer -= 1000;
		}
		elapsedT += SDL_GetTicks() - lastTick;
		elapsedF += SDL_GetTicks() - lastTick;
		lastTick = SDL_GetTicks();
	}
	if(renderer) SDL_DestroyRenderer(renderer);
	if(window) SDL_DestroyWindow(window);
	SDL_Quit();
	fprintf(stdout, "Exiting...\n");
	return 0;
}
