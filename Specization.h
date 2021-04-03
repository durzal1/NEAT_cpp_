#include <iostream>
#include <list>
#include <vector>
#include "Genome.h"
using namespace std;
#include "System.h"
#include "random.h"
#include "types.h"
#include "Species.h"
#include "Neat.h"

#ifndef NEAT_SPECIZATION_H
#define NEAT_SPECIZATION_H
std::vector<Species> selection(std::vector<Species> species, std::vector<Genome> genomes){
    // puts genomes into species and if there isnt one that is good for it, it will create a new one
    if (species.size() != 0){
        for (int i = 0; i < genomes.size();i++){
            // bool val to check if the genome found a species
            bool in_one = false;

            // if it is compatible with its species
            bool val;

            // first it checks with its species_hint
            int s_hint = genomes[i].species_hint;
            Genome r = species[s_hint].rep;
            if (genomes[i].connections.size() > r.connections.size()){
                val = Distance(genomes[i], r);
            } else{
                val = Distance(r, genomes[i]);
            }
            // compatible
            if (val == true){
                species[s_hint].genomes.push_back(genomes[i]);
                continue;
            }
            for (int a = 0; a < species.size();a++){
                // rep of the species
                Genome rep = species[a].rep;

                // finds the distance between the genome and the rep of the species
                if (genomes[i].connections.size() > rep.connections.size()){
                    val = Distance(genomes[i], rep);
                } else{
                    val = Distance(rep, genomes[i]);
                }
                // if val is true then it is compatible
                if (val == true){
                    species[a].genomes.push_back(genomes[i]);
                    in_one = true;
                    break;
                }
            }
            // if it goes through every species without finding a match it will create its own
            if (in_one == false){
                species.push_back(Species(genomes[i]));
            }
        }
    }else{
        // creates a new species
        Species s = Species(genomes[0]);
        s.Champ = genomes[0];
        // adds the first species to the species vector
        species.push_back(s);

        for (int i = 0; i < genomes.size(); i++){
            // makes sure it does not do the first genome since that is already in the first species
            if (i != 0){
                // bool val to check if the genome found a species
                bool in_one = false;

                // if it is compatible with its species
                bool val;

                for (int a = 0; a < species.size();a++){
                    // rep of the species
                    Genome rep = species[a].rep;
                    species[a].Champ = rep;
                    // finds the distance between the genome and the rep of the species
                    if (genomes[i].connections.size() > rep.connections.size()){
                        val = Distance(genomes[i], rep);
                    } else{
                        val = Distance(rep, genomes[i]);
                    }
                    // if val is true then it is compatible
                    if (val == true){
                        species[a].genomes.push_back(genomes[i]);
                        in_one = true;
                        break;
                    }
                }
                // if it goes through every species without finding a match it will create its own
                if (in_one == false){
                    species.push_back(Species(genomes[i]));
                }
            }
        }
    }
    return species;
}
std::vector<Species> Sort(std::vector<Species> s_){
    // gets fitness and kills a precentage of the genomes | kills species that are extinct
    // species
    // list of dead genomes
    std::vector<Species> dead_species;

    // list of fitnesses
    std::vector<float> fit;

//     loop that gives each species a fitness and kills a precentage of its population
//     if the species dies it adds it to dead_species
    for (int i = 0; i < s_.size(); i++){
        Species s = s_[i];
        s_[i].fitness_func();

        //adds fitness to fit vector
        fit.push_back(s_[i].fitness);

        // if the species dies it removes it
        if (s_[i].kill() == false){
            // puts a copy of the species in the back then removes the one at the index
            // after it removes the back species
            s_.push_back(s_[i]);
            s_.erase(s_.begin() + i);
            s_.pop_back();

            // substracts one to index because it still has to check the new species at that element
            i -= 1;
        }
    }
    // finds the max fitness in all the species
    auto fit_max = *max_element(fit.begin(), fit.end());

    // gives the species that is the most fit the fit_best bool value
    for (int i = 0; i < s_.size(); i++){
        if (s_[i].fitness == fit_max){
            s_[i].fit_best = true;
        }
    }
    return s_;
}

// struct so i can return the species and genomes
struct return_{
    std::vector<Species> species__;
    std::vector<Genome> genomes__;

    //constructor
    return_(std::vector<Species> sp,std::vector<Genome> ge){
        this->genomes__ = ge;
        this->species__ = sp;
    }
};

return_ Reproduce(int pop, std::vector<Species> species,std::vector<Genome> genomes,  System &system, float &start){
    // reproduces in each species
    float sum_fitness = 0;
    // deletes all of genomes
    genomes.clear();

    // adds fitness
    for (int i = 0; i < species.size(); i++){
        sum_fitness += species[i].fitness;
    }

    // creates the genomes
    for (int i = 0; i < species.size(); i ++){
        // gets the amount of genomes each species will get
        float share = species[i].fitness / sum_fitness;
        int amount = ceil(share * pop);
        int reproduce_amount = amount * 0.75;

        // species reproduces to get its offsprings
        std::vector<Genome> offsprings = species[i].reproduce_(reproduce_amount, system, start, i);

        // appends each offspring to genomes vector
        for (Genome offspring:offsprings) genomes.push_back(offspring);

//      gets offspring that will come from mutation and not crossover
        int mutate_share = amount;
        int amount_share = mutate_share * 0.25;
        for (int a = 0; a < amount_share; a++) {
            // takes champion of the species and mutates it to get 25% of the species genomes
            Genome new_ = Genome(species[i].Champ);
            new_.species_hint = i;
            Mutate(new_, system, start);
            genomes.push_back(new_);

        }

    }


    // gets rep and best genome for each species
    for (int c = 0;c < species.size(); c ++){
        Species species_ = species[c];
        // vector for fitness
        std::vector<Fitness> fit;

        // gets a random genome in the species that will become the rep
        int num = randomint(0, species[c].genomes.size() - 1, start);

        for (int i = 0; i < species[c].genomes.size(); i ++){
            if (i == num){
                // sets random genome to the rep
                species[c].rep = species[c].genomes[i];
            }
            fit.push_back(species[c].genomes[i].fitness);
        }
        // finds the max fitness in the species
        auto fit_max = *max_element(fit.begin(), fit.end());

        for (int i = 0; i < species[c].genomes.size(); i ++){
            // if its the most fit genome in the species
            if (species[c].genomes[i].fitness == fit_max){
                //sets the genome
                Genome best = Genome(species[c].genomes[i]);

                // if the genome is more fit than the species champ
                if (best.fitness > species[c].Champ.fitness){
                    species[c].Champ = best;
                }
                break;
            }
        }
        // sees if the species improved
        bool improved = false;
        // if the fitness of the species is higher than the fitness from the previous generation
        if (species[c].fitness > species[c].old_fitness){
            improved = true;
        }
        // if it improved it resets gens without improvement
        if (improved == true){
            species[c].gens_without_improvement = 0;
        } else{
            species[c].gens_without_improvement += 1;
        }
//todo c++ doesnt have -1 so if i want this to work redo it
        //reset everything in the species
        species[c].genomes.clear();
        species[c].genomes.push_back(species_.Champ);
        species[c].fit_best = false;
    }
    // reset the genomes to a fitness of 0
    for (int i = 0; i < genomes.size(); i ++){
        genomes[i].fitness = 0;
        // add age
    }


    return_ r = return_(species, genomes);
    return r;
}



#endif //NEAT_SPECIZATION_H
