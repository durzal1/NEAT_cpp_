
#ifndef NEAT_SYSTEM_H
#define NEAT_SYSTEM_H

#include <unordered_map>
#include "Genome.h"
#include <iostream>
#include <string>

class System{
    public:
        // the innovation number is the index for connections
        // inits global genes list
        // this list has the 2 genes of a connection and the last number is index for the new node
        std::vector<Innovation_string> GlobalGenes;

        //inits global connections vector
        std::vector<Innovation_string> GlobalConnectionGenes;

        // vector of actual connections
        std::vector<ConnectionGene> connections_;
        // checks to see if a connection has been made
        // if the connection has been made it returns the innovation number of it
        // if not it creates a new innovation number and sets said connection to it
        Innovation Connection_Check(Innovation con1, Innovation con2){
            //converts both innovations into string
            std::string con1_ = std::to_string(con1);
            std::string con2_ = std::to_string(con2);

            // combines both innovation numbers
            string fromto = con1_ + "_" + con2_;

            // checks if connection has been made
            bool Con_made = false;
            for (int i = 0; i < GlobalConnectionGenes.size(); i++){
                string connection = GlobalConnectionGenes[i];
                if (fromto == connection){
                    return i;
                }
            }
            if (Con_made == false){
                Innovation innovation = GlobalConnectionGenes.size();
                GlobalConnectionGenes.insert(GlobalConnectionGenes.end(), 1, fromto);
//                this->connections_.push_back()
                return innovation;

            }
            return -1;
        }
    Innovation Connection_Check_node(Innovation node1, Innovation node2, int inputs, int outputs){
        //converts both innovations into string
        std::string con1_ = std::to_string(node1);
        std::string con2_ = std::to_string(node2);

        // combines both innovation numbers
        string fromto = con1_ + con2_;

        // checks if node already has an inno
        bool Con_made = false;
        for (int i = 0; i < GlobalGenes.size(); i++){
            string connection = GlobalGenes[i];
            if (fromto == connection){
                return i+ inputs+outputs + 1;
            }
        }
        if (Con_made == false){
            Innovation innovation = GlobalGenes.size();
            GlobalGenes.insert(GlobalGenes.end(), 1, fromto);
            return innovation + inputs+outputs + 1;

        }
        return -1;
    }

};


#endif //NEAT_SYSTEM_H
