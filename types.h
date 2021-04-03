#ifndef NEAT_TYPES_H
#define NEAT_TYPES_H

#include <cstdint>

typedef uint64_t Innovation;
typedef float    Coordinate;
typedef double    Weight;
typedef float    Output;
typedef float    Input;
typedef float    Value;
typedef float    Fitness;
typedef string   Innovation_string;
enum NodeType{
    INPUT_NODE,
    OUTPUT_NODE,
    HIDDEN_NODE,
    BIAS_NODE
};


#endif //NEAT_TYPES_H