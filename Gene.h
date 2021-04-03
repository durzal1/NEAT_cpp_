#include <iostream> 
#include <list> 
#include <vector>
#include <iterator> 
#include <ostream>
using namespace std; 
#include "types.h"
#ifndef NEAT_GENE_H
#define NEAT_GENE_H
struct NodeGene{
    //type of gene
    int type_gene;
    //coordinate for where it is placed on the grid
    Coordinate x;

    // innvoation number of the nodegene
    Innovation innovation;

    // value of the nodegene after calculation without sigmoid added
    Value value = 0;

    // value of the nodegene after being sigmoided
    Value sigmoid_value = 0;

    // dont remember what i used this for lol
    bool done = false;


    // constructor definition
    NodeGene(Coordinate x, Innovation innovation, NodeType nodeType){
        this->x= x;
        this->innovation = innovation;
        this->type_gene = nodeType;
    }
    // empty constructor
    NodeGene(){

    }
    friend std::ostream &operator<<(std::ostream &os, const NodeGene &gene){
        os << gene.innovation;
        return os;
    }
};

struct ConnectionGene{

    // the first gene in the connection 
    Innovation gene_from;

    // the second gene in the connection 
    Innovation gene_to;

    // if the connection is active or disabled
    bool is_enabled = true; 

    // innvoation number of the nodegene
    Innovation innovation;

    // weight of the connection
    Weight weight;

    // place used in calculator
    int place;

    // x value
    float x;

    // constructor definition
    ConnectionGene(Innovation innovation, Innovation gene_from, Innovation gene_to, bool is_enabled, Weight weight){
        this->innovation = innovation;
        this->gene_from = gene_from;
        this->gene_to = gene_to;
        this->is_enabled = is_enabled;
        this->weight = weight;
    }
    // empty constructor
    ConnectionGene(){

    }
};


#endif //NEAT_GENE_H