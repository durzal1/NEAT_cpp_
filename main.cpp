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
#include "Board.h"
#include <cmath>
#include "snake.h"
#include <string>
using namespace snake;
// INPUTS AND OUTPUTS FOR XOR
//const int inputs__[4][2] = {{0,0}, {1,0}, {0,1}, {1,1}};
//const int outputscorrect[4] = {0, 1, 1, 0};
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

    // snake_main function
    void snake_main1(Genome &g){
        SDL_Window *window = nullptr;
        SDL_Surface *windowSurface = nullptr;

        // window and surface
        window = SDL_CreateWindow("fff", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, SDL_WINDOW_SHOWN);
        windowSurface = SDL_GetWindowSurface(window);

        double TOTAL = 0;
        int size1 = 0;

        for (int i = 0; i < 10; i++){
            //creates board
            Board b{20,20, windowSurface, window};
            int moves = 0;
            Fitness fitness_ = 0;
            // while the snake is still alive

            while(!b.IsGameOver() && b.GetMovesWithoutApple() < 150){
                moves ++;

                // gets inputs and calculates the output
                std::vector<float> inputs1 = b.getNetworkInput();
                std::vector<float> outputs = calculate(g, inputs1);

                //best output value
                auto fit_max = *max_element(outputs.begin(), outputs.end());

                auto it = std::find(outputs.begin(), outputs.end(), fit_max);

                // gets index
                int index = it - outputs.begin();

                // moves snake based off of the index
                switch (index) {
                    case 0:
                        b.move(LEFT_);
                        break;
                    case 1:
                        b.move(FORWARD_);
                        break;
                    case 2:
                        b.move(RIGHT_);
                        break;
                }

            }
            size1 += b.snakeSize();
            // complex fitness func that i split into mulitple steps
            double secondPart = (pow(2, b.snakeSize() - 2) + pow(b.snakeSize()-2, 2.1) *500);
            double lastPart = (pow(b.snakeSize() - 2, 1.2) * pow((0.25 * b.totalSteps), 1.3));
            double fitness__ = b.totalSteps + secondPart - lastPart;
            TOTAL += fitness__;
        }
        g.size_snake = size1 / 10;
        g.fitness = TOTAL / 10;
        cout << "";

    }
    // tests each genome
    void test(){
        //GETS FITS AND SORTS FOR TESTING PURPOSES ONLY
        vector<double> fit;

        for (Genome& genome:this->genomes){
            genome.age += 1;
            snake_main1(genome);
            fit.push_back(genome.fitness);

            if (genome.size_snake >= 30){
                FILE *f = fopen("best.bin", "wb" );

                uint64_t count_g = 0;
                uint64_t count_c = 0;

                count_g += genome.genes.size();
                count_c += genome.connections.size();

                fwrite(&genome.inputs, sizeof(int),1, f);
                fwrite(&genome.outputs, sizeof(int),1, f);
                fwrite(&genome.fitness, sizeof(double), 1, f);

                fwrite(&count_g, sizeof(uint64_t), 1, f);
                fwrite(&count_c, sizeof(uint64_t), 1, f);

                fwrite(&genome.genes.at(0), sizeof(NodeGene), genome.genes.size(), f);
                fwrite(&genome.connections.at(0), sizeof(ConnectionGene), genome.connections.size(), f);
                fclose(f);

            }
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

        cout << genomes[index].connections.size() << endl;

        cout << genomes[index].size_snake << endl;

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
        FILE *t = fopen("best.bin", "rb" );   // r,w for read, write respectively, b for binary

        uint64_t count_g2 = 0;
        uint64_t count_c2 = 0;

        int inputs2 = 0;
        int outputs2 = 0;
        double fitness = 0;

        fread(&inputs2, sizeof(int), 1, t);
        fread(&outputs2, sizeof(int), 1, t);


        Genome g(inputs2,outputs2);

        fread(&fitness, sizeof(double), 1, t);
        fread(&count_g2, sizeof(uint64_t), 1, t);
        fread(&count_c2, sizeof(uint64_t), 1, t);

        // reserve the genes inside the vectors
        g.genes.resize(count_g2);
        g.connections.resize(count_c2);

        // sets fitness
        g.fitness = fitness;

        fread(&g.genes.at(0), sizeof(NodeGene), g.genes.size(),t);
        fread(&g.connections.at(0), sizeof(ConnectionGene), g.connections.size(), t);

        fclose(t);
        snake_main1(g);

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
//            Fitness fitness = snake_main(genomes[i]);
//            genomes[i].fitness = fitness;

            snake_main1(genomes[i]);

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
//        FILE *t = fopen("data1.bin", "rb" );   // r,w for read, write respectively, b for binary
//
//        uint64_t count_g2 = 0;
//        uint64_t count_c2 = 0;
//
//        int inputs2 = 0;
//        int outputs2 = 0;
//        float fitness = 0;
//
//        fread(&inputs2, sizeof(int), 1, t);
//        fread(&outputs2, sizeof(int), 1, t);
//
//
//        Genome g(inputs2,outputs2);
//
//        fread(&fitness, sizeof(float), 1, t);
//        fread(&count_g2, sizeof(uint64_t), 1, t);
//        fread(&count_c2, sizeof(uint64_t), 1, t);
//
//        // reserve the genes inside the vectors
//        g.genes.resize(count_g2);
//        g.connections.resize(count_c2);
//
//        // sets fitness
//        g.fitness = fitness;
//
//        fread(&g.genes.at(0), sizeof(NodeGene), g.genes.size(),t);
//        fread(&g.connections.at(0), sizeof(ConnectionGene), g.connections.size(), t);
//
//        fclose(t);
//        snake_main1(g);
    Neat neat = Neat(4,3,1000);
    return 0;
}
