#include <random>
#include <iostream>
#include "time.h"
#ifndef NEAT_RANDOM_H
#define NEAT_RANDOM_H


double randomfloat(float min, float max, float start){
    float a = static_cast<float>(rand()) / RAND_MAX * (max - min) + min;
    return a;

}
int randomint(int min, int max, float &start){
    // sets another time to add more rng
    int b = rand()%(max-min+1)+min;
    return b;

}



#endif //NEAT_RANDOM_H
