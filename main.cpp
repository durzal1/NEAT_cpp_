#include <iostream>
#include <list>
#include <vector>
#include "Genome.h"
#include <stdlib.h> /* srand(), rand() */
#include <time.h>   /* time() */
#include "ctime"
using namespace std;
#include "mutate.h"
#include "System.h"
#include "random.h"
#include "Cross_over.h"
#include "Distance.h"
#include "Species.h"
#include "Specization.h"
#include "calculator.h"
#include <fstream>
#include "snake.h"



// INPUTS AND OUTPUTS FOR XOR
const int inputs__[4][2] = {{0,0}, {1,0}, {0,1}, {1,1}};
const int outputscorrect[4] = {0, 1, 1, 0};
float start = time(0);
class Neat{
public:

    // generation number
    int generation = 0;

    // list of species
    std::vector<Species> species;

    // list of genomes

    std::vector<Genome> genomes;
    //inputs, outputs, and pop size
    int inputs;
    int outputs;
    int pop_size;

    // tests each genome
    void test(){
        //GETS FITS AND SORTS FOR TESTING PURPOSES ONLY
        vector<float> fit;

        for (Genome& genome:this->genomes){
            genome.age += 1;
            Fitness fitness = snake_main(genome);
            genome.fitness = fitness;
            fit.push_back(fitness);
        }
        //sorts
        std::sort(fit.begin(), fit.end());

        //max fitness
        auto fit_max = *max_element(fit.begin(), fit.end());

        // sees which genome has the highest fitness
        auto it = find_if(this->genomes.begin(), this->genomes.end(), [&fit_max](const Genome& genome){return genome.fitness == fit_max;});
        // gets index
        auto index = std::distance(this->genomes.begin(), it);

        // average
        auto n = fit.size();
        float average = 0.0f;
        average = accumulate( fit.begin(), fit.end(), 0.0) / n;

        cout << this->generation << endl;
        cout << "=============" << endl;

        cout << average << endl;

        cout << fit_max << endl;


        cout << this->species.size() << endl;
        cout << "--------------------" << endl;




    }
    //constructor definition
    Neat(int inputs, int outputs, int pop_size){
        this->inputs = inputs;
        this->outputs = outputs;
        this->pop_size = pop_size;

        main_();

    }

    [[noreturn]] void main_(){
        System system;
        // this is for the 0th generation
        // creates a genome for the population size

        for (int i =0; i < this->pop_size; i++){
            Genome genome = Genome(this->inputs, this->outputs);

            this->genomes.push_back(genome);

        }

        // mutates each genome and tests it in the testing environment
        for (int i = 0; i < this->genomes.size(); i++){
            for (int a = 0; a < 1; a++){
                Mutate(this->genomes[i], system,start);
            }
            //TESTING PURPOSES
            Fitness fitness = snake_main(genomes[i]);
            genomes[i].fitness = fitness;
            // todo test genome here
        }
        // evolves
        while (true){
            std::vector<Genome> g = this->genomes;
            std::vector<Species> newspecies = selection(this->species, g);
            this->species = newspecies;
            std::vector<Species> newspecies_ = Sort(this->species);
            this->species = newspecies_;
            return_ r = Reproduce(this->pop_size, this->species, this->genomes, system,  start);
            // reinits species and gneomes
            this->species = r.species__;
            this->genomes = r.genomes__;
            this->generation += 1;
            test();
            cout << "";


        }

    }

};
//void evolve( System &system){
//    int f = 0;
//    while (f == 0){
//        std::vector<Species> newspecies = selection(neat.species, neat.genomes);
//        neat.species = newspecies;
//        std::vector<Species> newspecies_ = Sort(neat.species, neat);
//        neat.species = newspecies_;
//        f += 1;
//        Reproduce(neat, system,  start);
//
//
//    }

//}
int main(int argc, char* argv[]) {



//    FILE *f = fopen("data.bin", "rb" );   // r,w for read, write respectively, b for binary
//
//
//    uint64_t count_g = 0;
//    uint64_t count_c = 0;
//
//    int inputs = 0;
//    int outputs = 0;
//
//    fread(&inputs, sizeof(uint64_t), 1, f);
//    fread(&outputs, sizeof(uint64_t), 1, f);
//
//    Genome g(inputs,outputs);
//
//    fread(&count_g, sizeof(uint64_t), 1, f);
//    fread(&count_c, sizeof(uint64_t), 1, f);
//
//    // reserve the genes inside the vectors
//    g.genes.resize(count_g);
//    g.connections.resize(count_c);
//
//    fread(&g.genes.at(0), sizeof(NodeGene), g.genes.size(), f);
//    fread(&g.connections.at(0), sizeof(ConnectionGene), g.connections.size(), f);
//
//    fclose(f);

    Neat neat = Neat(4,3,500);
    return 0;
}
