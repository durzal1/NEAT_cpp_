#include <iostream>
#include <vector>
#include "objects.h"
#include "Genome.h"
#include "types.h"
#include "calculator.h"

// returns the fitness of the genome
Fitness snake_main(Genome &genome){

    // snake and apple
    Snake snake = Snake(500, 500);
    Apple apple;

    // vectors for each
    std::vector<Snake> snakes;
    std::vector<Apple> apples;

    // adds the snake and apple respectivly
    snakes.push_back(snake);
    apples.push_back(apple);
    while (true){
        // main loop
        for (Snake &s:snakes){
            s.moves -= 1;
            /// todo GET INPUTS FOR NEAT

            // inits variables
            int space_left = 0;
            int space_right = 0;
            int space_down = 0;
            int space_up = 0;
            int angle = 0;

            // 0 means that the space is clear and 1 means there is an obstacle there either snake body or border

            /// CHECKS FOR BORDERS
            // if border is one space above
            if (s.y - s.height < 0){
                space_up = 1;
            }

            // if border is one space right
            if (s.x + s.height >= 1000){
                space_right = 1;
            }

            // if border is one space below
            if (s.y + s.height >= 1000){
                space_down = 1;
            }

            // if border is one space left
            if (s.x - s.height < 0){
                space_down = 1;
            }
            /// checks to see if its about to collide with itself
            for (int i = 0; i < s.snake_parts.size(); i++){
                int x_body = s.snake_parts[i][0];
                int y_body = s.snake_parts[i][1];

                // checks every direction to see if there is a body part there
                if (s.x == x_body && (s.y - s.height) == y_body){
                    space_up = 1;
                }
                if ((s.x + s.height) == x_body && s.y == y_body){
                    space_right = 1;
                }
                if (s.x == x_body && (s.y + s.height) == y_body){
                    space_down = 1;
                }
                if ((s.x - s.height) == x_body && s.y == y_body){
                    space_left = 1;
                }
            }
            /// angle to the apple
            // -1 left, 0, same y cord, 1 right

            // left
            if (apples[0].x < s.x){
                angle = -1;
            }
            // up or below
            else if(apples[0].x == s.x){
                angle = 0;
            }
            //right
            else{
                angle = 1;
            }
            // puts the inputs into an array
            int inputs [] = {space_left, space_up, space_right, angle};

            // calculates the move based off the inputs
            std::vector<float> outputs = calculate(genome, inputs);

            /// finds the best output (one with highest value)
            //best output value
            auto fit_max = *max_element(outputs.begin(), outputs.end());

            auto it = std::find(outputs.begin(), outputs.end(), fit_max);

            // gets index
            int index = it - outputs.begin();

            // sets direction of the snake based off of the output that was calculated
            if (index == 0){
                s.direction = 4;
            }
            else if(index == 1){
                s.direction = 1;
            }
            else if(index == 2){
                s.direction = 2;
            }


            // moves the snake and checks for collision with apple
            s.move();
            s.collision(apples);

//            // draws the apple
//            apples[0].draw(windowSurface);

            // if apple has been eaten recently
            if (s.apple_moves > 0){
                s.fitness ++;
            }
            // if the snake runs out of moves it kills it
            if (s.moves <= 0){
                s.die();
            }

            // if the snake is dead
            if (s.dead){
                // changes size of the genome
                genome.size_snake = s.size;

                // returns fitness of the snake
                return s.fitness;
            }
        }



    }

}
