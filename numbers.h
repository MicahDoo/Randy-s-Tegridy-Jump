#ifndef NUMBERS_H
#define NUMBERS_H

#define PLAYER_WIDTH 45
#define PLAYER_HEIGHT 100
#define SCREEN_HEIGHT 780.0
#define SCREEN_WIDTH 500
#define MAX_HEIGHT 435
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
#define JUMP_TIME_SECS 0.7
#define SPRING_TIME_SECS 1.2
#define FLIP_DISTANCE 1500
#define WELL_LENGTH 3000.0
#define WELL_INTERVAL 4000.0

#define WING_PROB 40
#define WEED_PROB 30
#define ELASTIC_PROB 30
#define MONSTER_PROB 30
#define ERIC_PROB 30
#define KENNY_PROB 30
#define HORI_PROB 10

#define BACKGROUND_LAYER 0
#define MOUNTAIN_LAYER 1
#define TREE_LAYER 2
#define PLATFORM_LAYER 3
#define MEMBERBERRY_LAYER 4
#define CHARACTER_LAYER 5
#define DEFAULT_GRAVITY 0.48

namespace myDirections{
enum directions {left = -1, right = 1};
}
enum sides {leftSide, rightSide};

#endif // NUMBERS_H
