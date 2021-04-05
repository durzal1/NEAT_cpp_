#include <iostream>
#include <vector>
#include "objects.h"
#include "Genome.h"
#include "types.h"
#include "calculator.h"
#include <string>
#include <cmath>
#include <SDL2/SDL.h>
struct Vector2f{

    float x,y;

    Vector2f operator-(Vector2f B){
        return {x - B.x, y - B.y};
    }

    Vector2f& operator*=(float scalar){
        x *= scalar;
        y *= scalar;
        return *this;
    }

    float operator~(){
        return sqrt(x * x + y * y);
    }

    void normalise(){
        (*this) *= 1.0 / ~(*this);
    }


};

bool checkForObstacle(Snake s, int x, int y){
    // returns true if there is an obstacle
    /// checks borders
    if (x >= 1000){
        return true;
    }else if (x < 0){
        return true;
    } else if (y >= 1000){
        return true;
    }else if (y < 0){
        return true;
    }
    /// checks to see if its about to collide with itself
    for (int i = 0; i < s.snake_parts.size(); i++){
        int x_body = s.snake_parts[i][0];
        int y_body = s.snake_parts[i][1];

        // changes them both to strings
        string part = to_string(x_body) + to_string(y_body);
        string newXY = to_string(x) + to_string(y);

        // checks to see if the body equals the x and y
        if (part == newXY){
            return true;
        }

    }

}

// returns range to the obstacle
int rangeToObstacle(int direction, Snake snake){
    int count = 0;
    bool run = true;
    int y = snake.y;
    int x = snake.x;
    while(run){
        switch (direction){
            case -1:
                x -= snake.height;

                // checks for obstacle with the new y
                if (checkForObstacle(snake, x, snake.y)){
                    run = false;
                }
                break;
            case 0:
                y += snake.height;

                if (checkForObstacle(snake, snake.x, y)){
                    run = false;
                }
                break;
            case 1:
                x += snake.height;

                if (checkForObstacle(snake, x, snake.y)){
                    run = false;
                }
                break;
        }
        count ++;
    }

    return count - 1;
}
// sets the fitness of the genome
//Genome &genome
void snake_main(){

    SDL_Window *window = nullptr;
    SDL_Surface *windowSurface = nullptr;

    // window and surface
    window = SDL_CreateWindow("fff", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, SDL_WINDOW_SHOWN);
    windowSurface = SDL_GetWindowSurface(window);

    // snake and apple
    Snake snake = Snake(500, 500, windowSurface, window);
    Apple apple;


    // vectors for each
    std::vector<Snake> snakes;
    std::vector<Apple> apples;

    // adds the snake and apple respectivly
    snakes.push_back(snake);
    apples.push_back(apple);

    bool run_ = true;
    while (run_){
        // main loop
        for (Snake &s:snakes){
            s.moves -= 1;
            /// gets inputs for NEAT

            // inits variables
            float space_left = 0;
            float space_right = 0;
            float space_up = 0;
            float angle = 0;

            // checks left right and above
            space_left = pow(0.1, rangeToObstacle(LEFT, s));
            space_up = pow(0.1, rangeToObstacle(FORWARD, s));
            space_right = pow(0.1, rangeToObstacle(RIGHT, s));


            Vector2f nex {(float)s.x, (float)s.y + s.height};
            Vector2f fod {(float)apples[0].x, (float)apples[0].y};
            Vector2f cur {(float)s.x, (float)s.y};

            Vector2f dir = nex - cur;
            Vector2f tar = fod - cur;

            dir.normalise();
            tar.normalise();

            angle =  atan2(dir.y, dir.x) - atan2(tar.y, tar.x);

            if(angle > M_PI) angle -= 2 * M_PI;
            else if(angle < -M_PI) angle += 2 * M_PI;
            angle /= M_PI;

            // puts the inputs into an array
            vector<float> inputs = {space_left, space_up, space_right, angle};
            //todo try only adding one per food

            // calculates the move based off the inputs
//            std::vector<float> outputs = calculate(genome, inputs);

//            /// finds the best output (one with highest value)
//            //best output value
//            auto fit_max = *max_element(outputs.begin(), outputs.end());
//
//            auto it = std::find(outputs.begin(), outputs.end(), fit_max);
//
//            // gets index
//            int index = it - outputs.begin();
//
//            // sets direction of the snake based off of the output that was calculated
//            if (index == 0){
//                s.direction = LEFT;
//            }
//            else if(index == 1){
//                s.direction = FORWARD;
//            }
//            else if(index == 2){
//                s.direction = RIGHT;
//            }

            // moves the snake and checks for collision with apple
            s.move();
            s.collision(apples);

            // draws the apple
            apples[0].draw(windowSurface);


            // if the snake runs out of moves it kills it
            if (s.moves <= 0){
                s.die();
            }

            // if the snake is dead
            if (s.dead){
//                // changes size of the genome
//                genome.size_snake = s.size;
//
//                // returns fitness of the snake
//                genome.fitness = s.fitness;
                run_ = false;
            }
        }

        SDL_UpdateWindowSurface(window);
        cout << 'f';
    }

}
