#ifndef _CONSTANTS_H
#define _CONSTANTS_H

// Screen Dimesions
#define SCREEN_WIDTH					512
#define SCREEN_HEIGHT					410

// Map Related Dimensions
#define TILE_WIDTH						32
#define TILE_HEIGHT						32
#define MAP_HEIGHT						13
#define MAP_WIDTH						17

// Character Movement Related
#define MOVEMENT_SPEED					0.03
#define MOVEMENT_SPEED_INCREASE_RIGHT	200.0f
#define MOVEMENT_SPEED_INCREASE_LEFT	-200.0f
#define MAX_MOVEMENT_SPEED				80.0f

// Character Jump Related
#define PLAYER_GRAVITY					250
#define JUMP_FORCE_DECREMENT			380.0f
#define INITIAL_JUMP_FORCE				530
#define COLLISION_FORCE					2.5f

// Enemy Related
#define VULNERABLE_TIME					2.5f
#define INITIAL_JUMP_FORCE_SMALL		250
#define ENEMY_TIMER						20000.0f
#define PLAYER_ANIMATION_DELAY			0.05
#define ANIMATION_DELAY					0.15

// POW Block Related
#define SCREENSHAKE_DURATION			0.25f
#define POWBLOCK_HIT_TIMER				1000.0f

// Miscellaneous Constants
#define START_TIMER						1002.0f
#define MUSIC_TIMER						30000.0f

#endif // !_CONSTANTS_H