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
#define MAX_LADDERS 7
#define MAX_FLOORS 7

#define HIGHEST_FLOOR floor((GAME_END_Y - GAME_BEG_Y * 1.0) / (FLOOR_HEIGHT))

// values used as parameter to switch between modes
#define FLOOR 1
#define LADDER 2

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define JUMP 4

#define X_AXIS 0
#define Y_AXIS 1