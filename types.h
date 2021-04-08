#ifndef NEAT_TYPES_H
#define NEAT_TYPES_H

#include <cstdint>

typedef uint64_t Innovation;
typedef float    Coordinate;
typedef double    Weight;
typedef float    Output;
typedef float    Input;
typedef float    Value;
typedef double    Fitness;
typedef string   Innovation_string;
enum NodeType{
    INPUT_NODE,
    OUTPUT_NODE,
    HIDDEN_NODE,
    BIAS_NODE
};
// a constant value to help calculate direction
enum direction{
    FORWARD = 0,
    LEFT = -1,
    RIGHT = 1,
};
// which direction the head is facing
enum cardinal_Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

// this is the direction that the snake will actually move
// so left will subtract from the x
enum real_direction{
    UP1 = 0,
    RIGHT1 = 1,
    DOWN1 = 2,
    LEFT1 = 3,
};
#endif //NEAT_TYPES_H