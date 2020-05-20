#ifndef NUMBERS_H
#define NUMBERS_H

#define PLAYER_WIDTH 45
#define PLAYER_HEIGHT 100
#define SCREEN_HEIGHT 700
#define MAX_HEIGHT 335
#define PLATFORM_HEIGHT 10
#define PLATFORM_WIDTH 100
#define MONSTER_HEIGHT 150
#define SPACING 70.0
#define ERIC_SIZE 80.0
#define FPS 48

#define INITIAL_JUMP_SPEED 20.0
#define STEP_SIZE 8.0
#define ROCKET_SPEED 20.0
#define PROPS_DURATION 5000
#define JUMP_TIME_SECS 1.7

#define WING_PROB 50
#define WEED_PROB 50
#define ELASTIC_PROB 50
#define MONSTER_PROB 50
#define ERIC_PROB 30
#define KENNY_PROB 20

#define BACKGROUND_LAYER 0
#define MOUNTAIN_LAYER 1
#define TREE_LAYER 2
#define PLATFORM_LAYER 3
#define MEMBERBERRY_LAYER 4
#define CHARACTER_LAYER 5

enum directions {left = -1, right = 1};
enum sides {leftSide, rightSide};

#endif // NUMBERS_H
