// screen size in pixels 
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

// border & game items size
#define BORDER_SIZE 16
#define INFO_BAR_SIZE 48
#define FLOOR_HEIGHT 112

// game view position
#define GAME_BEG_X BORDER_SIZE
#define GAME_BEG_Y INFO_BAR_SIZE
#define GAME_END_X SCREEN_WIDTH - BORDER_SIZE
#define GAME_END_Y SCREEN_HEIGHT - BORDER_SIZE

// 
#define MAX_ELEMENTS 16

// collider floor extension
#define END_MODE 1
#define BEG_MODE 0

#define SAND 0
#define PLANKS 1
#define ROTTEN 2
#define FOOTBRIDGE 3

#define HIGHEST_FLOOR floor((GAME_END_Y - GAME_BEG_Y * 1.0) / (FLOOR_HEIGHT))

// values used as parameter to switch between modes
#define FLOOR 1
#define LADDER 2

// identify directions / actions
#define UNDEF -1
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define JUMP 4

// barrel config
#define ALWAYS_DOWN 1
#define ALLOW_CLIMB_DOWN 1
#define ALLOW_FALL 0
#define FORCE_FINISH 1
#define FINISH_DIRECTION LEFT
#define SPAWN_DELAY 2500 // in ms

// identify axis
#define X_AXIS 0
#define Y_AXIS 1

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

// control keys
#define KEY_LEFT SDLK_LEFT
#define KEY_RIGHT SDLK_RIGHT
#define KEY_UP SDLK_UP
#define KEY_DOWN SDLK_DOWN
#define KEY_JUMP SDLK_SPACE
#define KEY_ESCAPE SDLK_ESCAPE
#define KEY_RESET SDLK_n
#define KEY_CONTINUE SDLK_p