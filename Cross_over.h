#include <iostream>
#include <vector>
#include "Genome.h"
using namespace std;
#include "System.h"
#include "types.h"
#ifndef NEAT_CROSS_OVER_H
#define NEAT_CROSS_OVER_H
Genome Cross_Over(Genome &g1, Genome &g2, float &start, int species_hint){
    // g1 will have a higher fitness than g2

    // inits some variables that we'll use
    std::vector<ConnectionGene> g1_connections = g1.connections;
    std::vector<ConnectionGene> g2_connections = g2.connections;

    // if one of them has no connections it will not do crossover
    if (g1_connections.size() == 0){
        return g2;
    } else if (g2_connections.size() == 0){
        return g1;
    }
    //disjoint genes of both genomes
    std::vector<ConnectionGene> g1_disjoint;

    std::vector<ConnectionGene> g2_disjoint;

    //excess genes for genome 1
    std::vector<ConnectionGene> g1_excess;

    // offspring connections
    std::vector<ConnectionGene> offspring_connections;

    // the current innovation for each genome
    Innovation num_gene1 = 0;
    Innovation num_gene2 = 0;


    // loop to go through the connections
    while ((num_gene1 < g1_connections.size()) && (num_gene2 < g2_connections.size())){

        // checks if the innovation numbers are the same
        if (g1_connections[num_gene1].innovation == g2_connections[num_gene2].innovation){
            //gets a random number to chose which genome the offspring will take from
            int rand_num = randomint(0,1, start);

            // if its 0 it will take from g1 and 1 will take from g2
            if (rand_num == 0){
                offspring_connections.push_back(g1_connections[num_gene1]);
            }else{
                offspring_connections.push_back(g2_connections[num_gene2]);
            }

            //adds one to both g1 and g2 innovation number
            num_gene1 += 1;
            num_gene2 += 1;
        }
        // if the gene for genome1's innovation is larger than the genome 2 gene's innovation
        else if(g1_connections[num_gene1].innovation > g2_connections[num_gene2].innovation){
            // adds the disjoint gene to a list and adds one to num_gene2
            g2_disjoint.push_back(g2_connections[num_gene2]);
            num_gene2 += 1;
        }
        else if(g2_connections[num_gene2].innovation > g1_connections[num_gene1].innovation){

            // adds the disjoint gene to a list and adds one to num_gene1
            g1_disjoint.push_back(g1_connections[num_gene1]);
            num_gene1 += 1;
        }
    }
    // creates the excess genes
    // if genome2 has more genes than genome1 there will be no excess for genome1
    // therefore this while loop will not even go off
    while (num_gene1 < g1_connections.size()){
        // adds the excess genes for genome1
        g1_excess.push_back(g1_connections[num_gene1]);
        num_gene1 += 1;
    }
    // finished the offspring with disjoint and excess genes of genome1
    // disjoint
    for (int i = 0; i < g1_disjoint.size(); i++){
        offspring_connections.push_back(g1_disjoint[i]);
    }

    // excess
    for (int i = 0; i < g1_excess.size(); i++){
        offspring_connections.push_back(g1_excess[i]);
    }

    // creates the offspring
    Genome offspring = Genome(g1);
    offspring.genes = g1.genes;
    offspring.connections = offspring_connections;
    offspring.age = 0;

    //sets its species_hint
    offspring.species_hint = species_hint;
    return offspring;
}

#endif //NEAT_CROSS_OVER_H
