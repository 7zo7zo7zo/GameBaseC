#include "engine.h"
#include "assets.h"

int main(int argc, char *argv[])
{
	initEngine();
	initAssets();
	return gameLoop();
}
