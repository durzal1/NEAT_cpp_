#ifndef NEAT_CALCULATOR_H
#define NEAT_CALCULATOR_H

#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include "algorithm"
#include <cmath>
using namespace std;
#include "Genome.h"
#include "Gene.h"
#include <ostream>
#include "types.h"
#include "random.h"

// define sigmoid function
float sigmoid(float x){
    double s = 1 / (1 + exp(-x));
    return s;
}

std::vector<float> calculate(Genome &genome, const vector<float> inputs) {
    //resets all prev values for genes
    for (int i = 0; i < genome.genes.size(); i++) {
        genome.genes[i].value = 0;

        //sets value of inputs
        if (genome.genes[i].type_gene == 0) {
            genome.genes[i].value = inputs[i];
        }
        // sets static val of 1 for bias
        if (genome.genes[i].type_gene == 3) {
            genome.genes[i].value = 1;
        }
    }
    // todo give the connections x values then sort
    //gives each connection the x value of its fromgene aka gene1

    for (ConnectionGene &connection:genome.connections) {
        // x val of the its gene_from
        int index = genome.get_index(connection.gene_from);
        float x = genome.genes[index].x;
        connection.x = x;
    }

    // resets place
    for (ConnectionGene &connection:genome.connections) {
        connection.place = 0;
    }
    // inits vectors
    std::vector<float> x;
    std::vector<ConnectionGene> newcons;

    // makes the newcons as large as genome.connections
    for (int i = 0; i < genome.connections.size(); i++) {
        newcons.push_back(genome.connections[i]);
    }
    for (ConnectionGene &connection:genome.connections) {
        // adds x values to the vector
        x.push_back(connection.x);
    }
    // sorts vector
    std::sort(x.begin(), x.end());

    // gets the place of the value based on where it is in the x list
    for (ConnectionGene& con:genome.connections) {
        auto it = std::find(x.begin(), x.end(), con.x);
        int index = it - x.begin();
        con.place = index;

        // resets value of vector index so it cannot be used again
        x[index] = 99999;

    }
    // sets the connection in newcons in order
    for (ConnectionGene &connection:genome.connections) {
        newcons[connection.place] = connection;
    }

    // sets the value of every gene
    for (ConnectionGene &connection:newcons) {
        bool is_enabled = connection.is_enabled;
        if (is_enabled) {
            // sets variables
            Innovation gene_from = connection.gene_from;
            Innovation gene_to = connection.gene_to;

            //sets indexes
            int index = genome.get_index(gene_from);
            int index1 = genome.get_index(gene_to);

            float value_gene;

            for (int i = 0; i < genome.genes.size(); i++) {
                // if its the genefrom
                if (i == index) {
                    // if its an input it will not use sigmoid
                    if ((genome.genes[i].type_gene == 0) || (genome.genes[i].type_gene == 3) ) {
                        value_gene = genome.genes[i].value;
                    } else { //else it will use sigmoid
                        value_gene = sigmoid(genome.genes[i].value);
                    }
                }
            }
            Weight weight = connection.weight;
            float val = value_gene * weight;

            for (int i = 0; i < genome.genes.size(); i++) {
                // if its the gene_to
                if (i == index1) {
                    // it adds the value to the value of geneto
                    genome.genes[i].value += val;
                }
            }
        }
    }
    std::vector<float> outputs_;

    for (NodeGene &node: genome.genes) {
        if (node.type_gene == 1) {
            // finds the outputs and adds their sigmoided value into outputs_
            outputs_.push_back(sigmoid(node.value));
        }
    }
    return outputs_;
}


#endif //NEAT_CALCULATOR_H
