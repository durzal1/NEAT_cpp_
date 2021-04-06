
#ifndef NEAT_GENOME_H
#define NEAT_GENOME_H
#include <iostream> 
#include <list> 
#include <vector>
#include <iterator>
#include <fstream>
#include "algorithm"
using namespace std; 
#include "Gene.h"
#include <ostream>
#include "types.h"
#include "random.h"


struct Genome{

    // number of inputs the genome will have 
    int inputs;
    // number of outputs the genome will have
    int outputs;

    // generational age of the gene
    int age = 0;

    // what species the genome already came from as an index
    int species_hint;

    // size used for snake
    int size_snake = 0;

    // constructor definition
    Genome(int num_inputs, int num_outputs){
        this->inputs= num_inputs;
        this->outputs = num_outputs;
        this->fitness = 0;
        encode();

    }
    // copy constructor
    Genome(const Genome& genome){
        this->inputs= genome.inputs;
        this->outputs = genome.outputs;
        this->fitness = genome.fitness;
        this->species_hint = genome.species_hint;
        this->place = genome.place;
        this->age = genome.age;
        for(NodeGene g:genome.genes) this->genes.push_back(g);
        for(ConnectionGene g_:genome.connections) this->connections.push_back(g_);
        for(Innovation g_:genome.connection_innos) this->connection_innos.push_back(g_);
    }
    // othercopy constructor which i wont use
    Genome& operator=(const Genome& genome){
        this->species_hint = genome.species_hint;
        this->genes.clear();
        this->connections.clear();
        this->connection_innos.clear();
        this->inputs= genome.inputs;
        this->outputs = genome.outputs;
        this->fitness = genome.fitness;
        this->place = genome.place;
        this->age = genome.age;
        for(NodeGene g:genome.genes) this->genes.push_back(g);
        for(ConnectionGene g_:genome.connections) this->connections.push_back(g_);
        for(Innovation g_:genome.connection_innos) this->connection_innos.push_back(g_);
        return *this;
    }

    // used later on for the place in the list not that important 
    int place = 0;

    // used to add all of the sensors once
    bool addSensors = true;

    // list of genes in the genome
    std::vector<NodeGene> genes;

    //list of connections 
    std::vector<ConnectionGene> connections;

    //list of connections innos
    std::vector<Innovation> connection_innos;
    
    // fitness of a genome
    Fitness fitness = 0;

    // percent used in reproduction
    float percent = 0;

    // used for XOR
    int right = 0;

    // creates the input and output genes and bias
    void encode(){
        // adds the inputs to genes list

        for (Innovation i = 0; i < inputs; i++) {
            NodeGene gene{0,i, INPUT_NODE};
            // adds the new input gene into the genes list
            genes.insert(genes.end(), 1, gene);
        }

        //adds the outputs to the genes list
        for (Innovation i = 0; i < outputs; i++) {
            NodeGene gene{1,inputs+i, OUTPUT_NODE};
            // adds the new otuput gene into the genes list
            genes.insert(genes.end(), 1, gene);
        }
        // adds the bias
        Innovation n = inputs + outputs;
        NodeGene gene{0,n, BIAS_NODE};
        genes.insert(genes.end(), 1, gene );

    }

    void fix_inno(){
        // resets place
        for (ConnectionGene &connection:this->connections){
            connection.place = 0;
        }
        // inits vectors
        std::vector<int> innos;
        std::vector<ConnectionGene> newcons;

        // makes the newcons as large as this->connections
        for (int i=0; i < connections.size(); i++) newcons.push_back(connections[i]);

        for (ConnectionGene &connection:this->connections){
            // adds innos to the vector
            innos.push_back(connection.innovation);
        }
        // sorts vector
        std::sort(innos.begin(), innos.end());

        // gets the place of the value based on where it is in the innos vector
        for (int i = 0; i < connections.size(); i ++){
            auto it = std::find(innos.begin(), innos.end(), connections[i].innovation);
            int index=  it - innos.begin();
            connections[i].place = index;

        }
        // sets each index with the corresponding connection in order
        for (ConnectionGene &connection:this->connections){
            newcons[connection.place] = connection;
        }
        // resets this->connections
        this->connections = newcons;
    }
    //gets index of gene based off of its innovation number
    int get_index(int innovation){
        auto it = find_if(this->genes.begin(), this->genes.end(), [&innovation](const NodeGene& gene){return gene.innovation == innovation;});
        // gets index
        auto index = std::distance(this->genes.begin(), it);
        return index;
    }

};


#endif //NEAT_GENOME_H


