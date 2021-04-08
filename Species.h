#ifndef NEAT_SPECIES_H
#define NEAT_SPECIES_H
#include <vector>
#include "Genome.h"
using namespace std;
#include "types.h"
#include "math.h"

const double kill_precent = 0.7;
class Species{
public:
    // rep of the species
    Genome rep = Genome(2,1);

    // fitness of the species
    Fitness fitness = 0;

    // the species fitness
    std::vector<Genome> genomes;

    // old fitness
    Fitness old_fitness = 0;

    // number of generations without improvement
    int gens_without_improvement = 0;

    //champion of the species
    Genome Champ = Genome(1,1);

    // if its the most fit species in the network
    bool fit_best = false;


    // constructor definition
    Species(Genome rep){
        this->rep = rep;
        this->fitness = 0;
    }

    void fitness_func(){
        // score will be the new fitness of the species
        double score = 0;

        // loop that will add all the fitness scores into score and fit
        for (int i = 0; i < genomes.size();i++){
            score += genomes[i].fitness;
        }
        Fitness old_fit = fitness;
        this->old_fitness = old_fit;
        // makes sure fitness will not divide by 0
        if (score != 0){
            this->fitness = score / genomes.size();
        }
        else if (genomes.size() >= 1){
            this->fitness = 0.01;
        }
        else{
            this->fitness = 0.01;
        }
    }
    bool kill(){
        //resets place
        for(int i = 0; i < genomes.size(); i++){
            genomes[i].place = 0;
        }
        // list of all the fitnesses of each genome
        std::vector<Fitness> fitness_list;

        // array used for checking
        vector<vector<int>> c1;
        c1.resize(genomes.size());

        // array of the new genomes
        vector<Genome> genomes_new;
        for (int i=0; i < genomes.size(); i++) genomes_new.push_back(genomes[i]);
        //adds the fitnesses to the fitness list
        for (int i = 0; i < genomes.size(); i++){
            fitness_list.push_back(genomes[i].fitness);
        }
        //sorts list
        std::sort(fitness_list.begin(), fitness_list.end());

        for (int a = 0; a < genomes.size(); a++){
            auto it = std::find(fitness_list.begin(), fitness_list.end(), genomes[a].fitness);
            int index = it - fitness_list.begin();
            genomes[a].place = index;
        }
        for (int i = 0; i < genomes.size(); i++){
            genomes_new[genomes[i].place] = genomes[i];
        }
        // kills a precentage of them
        int amount = abs((floor(genomes_new.size() * kill_precent)) - genomes_new.size());
        for (int i = 0; i < amount ; i++){
            genomes_new.erase(genomes_new.begin() + 1);
        }

        // replaces species genome list with the new one
        this->genomes = genomes_new;
        // kills the species if theres no genomes remaining
        if (genomes_new.size() == 0 || this->gens_without_improvement >= 10){
            return false;
        }else{
            return true;
        }
    }
    std::vector<Genome> reproduce_(int amount_,  System &system, float &start, int species_hint){
        // list of offsprings without mutations
        std::vector<Genome> offsprings;

        // list of offsprings with mutations
        std::vector<Genome> offsprings_new;
        double sum_fitness = 0;

        // checks if there is more than one genome
        if (genomes.size() != 1){
            // adds the fitness to the sum fitness
            for (int i = 0; i < genomes.size(); i ++){
                sum_fitness += genomes[i].fitness;
            }
            for (int i = 0; i < genomes.size(); i ++){
                // if the fitness is negative it does not use it
                if (genomes[i].fitness <= 0){
                    continue;
                }
                // gets percent chance that the genome becomes a parent
                // the higher the fitness the larger the chance
                genomes[i].percent = genomes[i].fitness / sum_fitness;
            }
            // goes through loop until there are enough offspring
            while (offsprings.size() < genomes.size() / amount_){
                for (int i = 0; i < genomes.size(); i++){
                    if (randomfloat(0,1 ,start) <= genomes[i].percent){
                        // creates the new offspring using a random genome for the second parent
                        float ran = randomint(0, genomes.size() - 1, start);
                        Genome genome1 = genomes[ran];
                        Genome offspring = Genome(2,1);
                        if (genome1.fitness > genomes[i].fitness){
                            offspring = Cross_Over(genome1,genomes[i] , start, species_hint);
                        }else{
                            offspring = Cross_Over(genomes[i], genome1, start, species_hint);
                        }
                        //adds the offspring to offsprings vector
                        offsprings.push_back(offspring);
                    }
                }
            }
        }
        if (offsprings.size() <= 1){
            for (int i = 0; i < amount_; i++){
                // creates deep copy of the genome to be mutated
                Genome mutation = Genome(genomes[0]);
                mutation.species_hint = species_hint;
                Mutate(mutation,system, start);
                offsprings_new.push_back(mutation);
            }
        }else{
            // creates offspring for the amount designated
            for (int i = 0; i < amount_;i ++){
                // gets a random offspring, mutates it, and adds it the offsprings_new vector
                float ran = randomint(0, offsprings.size() - 1, start);
                Genome copy1 = Genome(offsprings[ran]);
                copy1.species_hint = species_hint;
                Mutate(copy1, system, start);
                offsprings_new.push_back(copy1);
            }
        }

        return offsprings_new;
    }
};

#endif //NEAT_SPECIES_H
