#ifndef NEAT_MUTATE_H
#define NEAT_MUTATE_H
#include <iostream>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
#include <set>
#include <cstring>
using namespace std;
#include "Genome.h"
#include <ostream>
#include <iostream>
#include "types.h"
#include "System.h"
#include "random.h"
 // .05 in smaller pop and 0.3 for large pop
const double PROBABILITY_MUTATE_LINK = 0.29;
const double PROBABILITY_MUTATE_NODE = 0.01;
const double PROBABILITY_MUTATE_WEIGHTS = 1;
const double PROBABILITY_MUTATE_WEIGHT_SHIFT = 0.9;
const double PROBABILITY_MUTATE_WEIGHT_RANDOM = 0.1;
const double PROBABILITY_MUTATE_TOGGLE_LINK = 0.0;
const double WEIGHT_SHIFT_STRENGTH = 0.30;
const double PROBABILITY_ADD_SENSOR_ALL = 1;

void mutate_link(Genome &genome, System &system, float &start){
    // per genome
    for (int i = 0; i < genome.genes.size() * 2; i++){

        // randomly sets node1 and node2 innos
        int gene1_inno = randomint(0,genome.genes.size() - 1,start);
        int gene2_inno = randomint(0,genome.genes.size() - 1,start );

        // gets index for g1 and g2
        int index1 = genome.get_index(gene1_inno);
        int index2 = genome.get_index(gene2_inno);

        //sets the genes
        NodeGene gene1 = genome.genes[index1];
        NodeGene gene2 = genome.genes[index2];

        // checks which one is first
        if (gene1.x > gene2.x) {
            gene1 = genome.genes[index1];
            gene2 = genome.genes[index2];
            gene1_inno = gene1.innovation;
            gene2_inno = gene2_inno;
        }
        // if they have the same inno go next
        if (gene1_inno == gene2_inno){
            continue;
        }

        // if gene1 is an output go next or gene2 is an bias/input
        if (gene1.type_gene == 1  || gene2.type_gene == 0 || gene2.type_gene == 3 ) {
            continue;
        }
        if (gene2.type_gene == 3){
            continue;
        }

        // gets the name of the connection (both innos in string format next to each other)
        string connection_name = std::to_string(gene1_inno) + std::to_string(gene2_inno);
        // gets innovation and weight of new connection
        Innovation innovation_number = system.Connection_Check(gene1_inno, gene2_inno);
        Weight weight = randomfloat(-2,2,start);
        // checks if the connection has not been made yet
        if (std::find(genome.connection_innos.begin(), genome.connection_innos.end(), innovation_number) == genome.connection_innos.end()){
            //creates gene and adds it to the connections list
            ConnectionGene connectionGene = ConnectionGene(innovation_number, gene1_inno, gene2_inno, true, weight);
            genome.connections.push_back(connectionGene);
            genome.connection_innos.push_back(innovation_number);
            break;
        }
    }
}
void mutate_link_gene(Genome &genome, System &system, float &start){
    // mutate link but for every gene instead of every genome
    //creates a set of every possible connection
    set<Innovation> cons;
    for (int i = 0; i < genome.genes.size(); i ++){
        for (int a= 0; a < genome.genes.size(); a++){
            // sets index for g1 and g2
            int index1 = i;
            int index2 = a;

            //sets the genes
            NodeGene gene1 = genome.genes[index1];
            NodeGene gene2 = genome.genes[index2];

            //sets innovation numbers for g1 and g2
            int inno1 = gene1.innovation;
            int inno2 = gene2.innovation;

            // checks which one is first
            if (gene1.x > gene2.x) {
                gene1 = genome.genes[index1];
                gene2 = genome.genes[index2];
            }
            // if they have the same index go next
            if (index1 == index2){
                continue;
            }

            // if gene1 is an output go next or gene2 is an bias/input
            if ((gene1.type_gene == 1) || (gene2.type_gene == 0) || (gene2.type_gene == 3 )) {
                continue;
            }
            if (gene2.type_gene == 3){
                continue;
            }
            // puts the connection in string form with the innos next to each other
            string connection_name = std::to_string(inno1) + std::to_string(inno2);

            // gets innovation and weight of new connection
            Innovation innovation_number = system.Connection_Check(inno1, inno2);

            // adds the innovation to cons
            cons.insert(innovation_number);


        }
    }
    // iterates through the possible connections
    for (Innovation it: cons){
        // gives each a probability
        float n1 = randomfloat(0, 1, start);
        if (PROBABILITY_MUTATE_LINK >= n1) {
        }else{
            continue;
        }
        string val = system.GlobalConnectionGenes[it];
        // gets the gene1 and gene2
        // does bool value to see if its g1 or g2
        string g1;
        string g2;
        bool before_underscore = true;
        for (int i = 0; i < val.length(); i++){
            string str = "_";
            char v = val[i];
            string value(1,v);

            if (value != str){
                if (before_underscore){
                    g1 += value;
                }else{
                    g2 += value;
                }
            }else if (value == str){
                before_underscore = false;
            }
        }
        // makes g1 and g2 innovation(ints)
        Innovation g1_int = stoi(g1);
        Innovation g2_int = stoi(g2);

        //gets weight
        Weight weight = randomfloat(-2,2,start);

//        creates gene and adds it to the connections list
        ConnectionGene connectionGene = ConnectionGene(it, g1_int, g2_int, true, weight);
        genome.connections.push_back(connectionGene);
        genome.connection_innos.push_back(it);
    }

}
void mutate_node_gene(Genome &genome, System &system, float &start) {
    // mutate_node but its per gene instead of per genome
    if (genome.connections.size() > 0){
        // iterates through every connection
        for (ConnectionGene& connection:genome.connections){
            // if it passes the probability
            float n1 = randomfloat(0, 1, start);
            if (PROBABILITY_MUTATE_NODE >= n1) {
            }else{
                continue;
            }

            // sets all the variables i will need from genome.genes and genome.connections respectivly
            bool active = connection.is_enabled;
            Innovation gene1 = connection.gene_from;
            Innovation gene2 = connection.gene_to;

            // gets index for g1 and g2
            int index1 = genome.get_index(gene1);
            int index2 = genome.get_index(gene2);

            Coordinate x1 = genome.genes[index1].x;
            Coordinate x2 = genome.genes[index2].x;
            // todo KEEP DOING THIS FOR EVERY TIME I NEED IT
            if (active && genome.genes[index1].type_gene != 3){
                // disables the connection
                connection.is_enabled = false;

                //sets the x value of the new connection
                Coordinate coordinate = randomfloat(x1, x2, start);

                //get innovation number of new node based off of the genes from the old connection
                Innovation inno = system.Connection_Check_node(gene1, gene2, genome.inputs, genome.outputs);

                //creates a new gene and adds it to genome.genes
                NodeGene gene = NodeGene(coordinate, inno, HIDDEN_NODE);
                genome.genes.push_back(gene);

                // sets the variables needed for the new connections
                Weight og_weight = connection.weight;

                // genefrom is the og gene1 while geneto is the new node we just created
                Innovation gene_from1 = gene1;
                Innovation gene_to1 = inno;

                // gene_from is the new node we creates and geneto is the og geneto
                Innovation gene_from2 = gene_to1;
                Innovation gene_to2 = connection.gene_to;

                // sets the new innovation number of the new connections
                Innovation innovation_number1 = system.Connection_Check(gene_from1, gene_to1);
                Innovation innovation_number2 = system.Connection_Check(gene_from2, gene_to2);

                //adds the innovation numbers to the list
                genome.connection_innos.push_back(innovation_number1);
                genome.connection_innos.push_back(innovation_number2);

                // adds them to the list
                ConnectionGene con_gene1 = ConnectionGene(innovation_number1, gene_from1, gene_to1, true, 1);
                ConnectionGene con_gene2 = ConnectionGene(innovation_number2, gene_from2, gene_to2, true, og_weight);
                genome.connections.push_back(con_gene1);
                genome.connections.push_back(con_gene2);
                break;


            }
        }
    }
}

void mutate_node(Genome &genome, System &system, float &start){
    // per genome
    //checks if there is at least one connection
    if (genome.connections.size() > 0){
        // loops enough times to find a connection
        for (int i = 0; i < genome.connections.size() * 3; i++){
            int ran_num;
            // if there is only one connection that connection will get mutated otherwise a random connection will be mutated
            if (genome.connections.size() == 1){
                ran_num = 0;
            } else{
                ran_num = randomint(0,genome.connections.size() - 1, start);
            }
            // sets all the variables i will need from genome.genes and genome.connections respectivly
            ConnectionGene con = genome.connections[ran_num];
            bool active = genome.connections[ran_num].is_enabled;
            Innovation gene1 = genome.connections[ran_num].gene_from;
            Innovation gene2 = genome.connections[ran_num].gene_to;

            // gets index for g1 and g2
            int index1 = genome.get_index(gene1);
            int index2 = genome.get_index(gene2);

            Coordinate x1 = genome.genes[index1].x;
            Coordinate x2 = genome.genes[index2].x;
            // todo KEEP DOING THIS FOR EVERY TIME I NEED IT
            if (active && genome.genes[index1].type_gene != 3){
                // disables the connection
                genome.connections[ran_num].is_enabled = false;

                //sets the x value of the new connection
                Coordinate coordinate = randomfloat(x1, x2, start);

                //get innovation number of new node based off of the genes from the old connection
                Innovation inno = system.Connection_Check_node(gene1, gene2, genome.inputs, genome.outputs);

                //creates a new gene and adds it to genome.genes
                NodeGene gene = NodeGene(coordinate, inno, HIDDEN_NODE);
                genome.genes.push_back(gene);

                // sets the variables needed for the new connections
                Weight og_weight = con.weight;

                // genefrom is the og gene1 while geneto is the new node we just created
                Innovation gene_from1 = gene1;
                Innovation gene_to1 = inno;

                // gene_from is the new node we creates and geneto is the og geneto
                Innovation gene_from2 = gene_to1;
                Innovation gene_to2 = con.gene_to;

                // sets the new innovation number of the new connections
                Innovation innovation_number1 = system.Connection_Check(gene_from1, gene_to1);
                Innovation innovation_number2 = system.Connection_Check(gene_from2, gene_to2);

                //adds the innovation numbers to the list
                genome.connection_innos.push_back(innovation_number1);
                genome.connection_innos.push_back(innovation_number2);

                // adds them to the list
                ConnectionGene con_gene1 = ConnectionGene(innovation_number1, gene_from1, gene_to1, true, 1);
                ConnectionGene con_gene2 = ConnectionGene(innovation_number2, gene_from2, gene_to2, true, og_weight);
                genome.connections.push_back(con_gene1);
                genome.connections.push_back(con_gene2);
                break;


            }
        }
    }
}
void mutate_weight_shift(Genome &genome, System &system, float &start){
    // goes through each connection
    for (int i = 0; i < genome.connections.size(); i++){
        float n = randomfloat(0,1, start);

        // does an rng problem to see if it should mutate it
        if (PROBABILITY_MUTATE_WEIGHT_SHIFT >= n){
            ;
        }else{
            continue;
        }
        // creates a new number between -2,2 and multiply it by the strength
        float mult_num = randomfloat(-2, 2, start * i + PROBABILITY_MUTATE_WEIGHTS);
        float strength = pow(0.9, genome.age);
        float num = mult_num * strength;
        float add_ = genome.connections[i].weight + num;

        // checks if the new total would be over 8 or less than -8
        // this number acts as a cap
        if (add_ >= 8 || add_ <= -8){
            ;
        } else{
            genome.connections[i].weight = add_;
        }

    }
}
void mutate_add_sensor(Genome &genome, System &system, float &start){
    for (int i = 0; i < genome.inputs; i++){
        // randomly sets node1 and node2 innos
        int gene1_inno = i;
        for (int a = 0; a < genome.outputs; a++){
            int gene2_inno = genome.inputs + a;

            // gets the name of the connection (both innos in string format next to each other)
            string connection_name = std::to_string(gene1_inno) + std::to_string(gene2_inno);

            // gets innovation and weight of new connection
            Innovation innovation_number = system.Connection_Check(gene1_inno, gene2_inno);
            Weight weight = randomfloat(-2,2,start);

            //creates gene and adds it to the connections list
            ConnectionGene connectionGene = ConnectionGene(innovation_number, gene1_inno, gene2_inno, true, weight);
            genome.connections.push_back(connectionGene);
            genome.connection_innos.push_back(innovation_number);

        }

    }
}
void mutate_weight_random(Genome &genome, System &system, float &start){
    //again goes through each connection
    for (int i = 0; i < genome.connections.size(); i++){
        float n = randomfloat(0,1, start * i + PROBABILITY_MUTATE_WEIGHTS + i);
        if (PROBABILITY_MUTATE_WEIGHT_RANDOM >= n){
            ;
        }else{
            continue;
        }
        // chooses a random number between 2 and -2 and makes that the new weight
        float mult_num = randomfloat(-2, 2, start);
        genome.connections[i].weight = mult_num;


    }
}
void toggle_link(Genome &genome, System &system, float &start){
     // todo ill do this some other time since i wont use this
}
void Mutate(Genome &genome, System &system, float &start) {
    vector<ConnectionGene> prev = genome.connections;
    vector<NodeGene> prev_ = genome.genes;
    //gets 3 different probabilities to see what mutations should occur
//    if (genome.connections.size() == 0){
//        mutate_add_sensor(genome, system, start);
//    }

    // weight mutations, mutate node, and mutate link
    float n2 = randomfloat(0, 1, start);
    if (PROBABILITY_MUTATE_LINK >= n2) {
        mutate_link(genome, system, start);
    }
//    float n1 = randomfloat(0, 1, start);
//    if (PROBABILITY_MUTATE_NODE >= n1) {
    mutate_node_gene(genome, system, start);
//    }
    float n3 = randomfloat(0, 1, start);
    if (PROBABILITY_MUTATE_WEIGHTS >= n3) {
        mutate_weight_random(genome, system, start);
        mutate_weight_shift(genome, system, start);
    }


    genome.fix_inno();
}
#endif //NEAT_MUTATE_H
