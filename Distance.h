#ifndef NEAT_DISTANCE_H
#define NEAT_DISTANCE_H
#include <iostream>
#include <list>
#include <vector>
#include "Genome.h"
using namespace std;
#include "System.h"
#include "random.h"
#include "types.h"

// constants
const int c1 = 1;
const int c2 = 1;
const int c3 = 3; // 3 for large pop size 1000+ while 0.4 for small pop size
const int distance_threshold = 7; // 3 for small pop size and 4 for large pop size

bool Distance(Genome &g1, Genome &g2){
    // one of these genomes will be the rep of a species while the other will be a genome comparing against the rep
    // g1 is the genome with more connections

    // initialization of variables
    int index_g1 = 0;
    int index_g2 = 0;
    int disjoint = 0;
    float weight_diff = 0;
    int similar = 0;

    // while loop to go through each connection gene in the genomes to see how many disjoint,excess, and similar genes are present
    while (index_g1 < g1.connections.size() && index_g2 < g2.connections.size()){
        // sets the innovation numbers of each connection
        Innovation in1 = g1.connections[index_g1].innovation;
        Innovation in2 = g2.connections[index_g2].innovation;

        // sets the weight of the connection
        Weight c1_val = g1.connections[index_g1].weight;
        Weight c2_val = g2.connections[index_g2].weight;

        // if the innovation number of the connections are the same
        if (in1 == in2){
            // adds the weight difference of the connections
            weight_diff += abs(c1_val - c2_val);

            // adds one to the index number for each
            index_g2 += 1;
            index_g1 += 1;

            //adds one to the number of similar genes present
            similar += 1;
        }else if (in1 > in2){
            // innovation 1 is larger than innovation 2
            // aka a disjoint of genome 1
            disjoint += 1;
            index_g2 += 1;
        } else{
            // innovation 2 is larger than innovation 1
            // aka disjoint of genome 1
            index_g1 += 1;
            disjoint += 1;
        }
    }
    // makes sure the weight_diff will not divide by 0
    if (similar != 0){
        weight_diff /= similar;
    }
    // adds the extra excess genes
    int excess = g1.connections.size() - index_g1;
    int N = g1.connections.size();

    // if there is less than 20 connections the N is set to 1
    if (N < 20){
        N = 1;
    }
    // computes the distance between the two genomes
    float distance = ((c1 * excess) / N) + ((c2 * disjoint) / N) + (c3 * weight_diff);

    // returns true if it is compatible and false if it is not compatible
    if (distance <= distance_threshold){
        return true;
    } else{
        return false;
    }
}



#endif //NEAT_DISTANCE_H
