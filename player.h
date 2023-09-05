#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
	float x;
	float y;
} Player;

void initPlayer(Player* player, float x, float y);

#endif
