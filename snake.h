#include <iostream>
#include <vector>
#include "objects.h"
#include "Genome.h"
#include "types.h"
#include "calculator.h"
#include <string>
#include <cmath>
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
    if (s.snake_parts.size() > 0){
        for (int i = 0; i < s.snake_parts.size(); i++){
            int x_body = s.snake_parts[i][0];
            int y_body = s.snake_parts[i][1];

            // checks to see if the body equals the x and y
            if (x_body == x && y_body == y){
                return true;
            }

        }
    }


}

// returns range to the obstacle
int rangeToObstacle(int direction, Snake snake){
    int count = 0;
    bool run = true;
    int y = snake.y;
    int x = snake.x;

    // gets the real direction its facing
    int go = (snake.CardinalDirection + direction + 4) % 4;
    int real_direction = go;

    while(run){
        switch (real_direction){
            case LEFT1:
                x -= snake.height;

                // checks for obstacle with the new y
                if (checkForObstacle(snake, x, snake.y)){
                    run = false;
                }
                break;
            case UP1:
                y += snake.height;

                if (checkForObstacle(snake, snake.x, y)){
                    run = false;
                }
                break;
            case RIGHT1:
                x += snake.height;

                if (checkForObstacle(snake, x, snake.y)){
                    run = false;
                }
                break;
            case DOWN1:
                y -= snake.height;

                if (checkForObstacle(snake, snake.x, y)){
                    run = false;
                }
                break;
        }
        count ++;
    }

    return count - 1;
}
// sets the fitness of the genome
void snake_main(Genome &genome){
    // does this multiple times to make sure its accurate
    double FITNESS = 0;
    double SIZE = 0;

    for (int i = 0; i < 20; i++){
            SDL_Window *window = nullptr;
    SDL_Surface *windowSurface = nullptr;

    // window and surface
    window = SDL_CreateWindow("fff", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, SDL_WINDOW_SHOWN);
    windowSurface = SDL_GetWindowSurface(window);

        // snake and apple
        // ,windowSurface, window
        Snake snake = Snake(500, 500,windowSurface, window);
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
                s.totalSteps++;

                /// gets inputs for NEAT
                // inits variables
                float space_left = 0;
                float space_right = 0;
                float space_up = 0;
                float angle = 0;

                int forNexX;
                int forNexY;


                // checks left right and above
                space_left = pow(0.1, rangeToObstacle(LEFT, s));
                space_up = pow(0.1, rangeToObstacle(FORWARD, s));
                space_right = pow(0.1, rangeToObstacle(RIGHT, s));

                // gets the cords of next move
                switch (s.CardinalDirection){
                    case 0:
                        forNexX = s.x;
                        forNexY = s.y + s.height;
                        break;
                    case 1:
                        forNexX = s.x + s.height;
                        forNexY = s.y;
                        break;
                    case 2:
                        forNexX = s.x;
                        forNexY = s.y - s.height;
                        break;
                    case 3:
                        forNexX = s.x - s.height;
                        forNexY = s.y;
                        break;
                }


                Vector2f nex {(float)forNexX, (float)forNexY};
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
                vector<float> inputs = {space_up, space_left, space_right, angle};

//             calculates the move based off the inputs
                std::vector<float> outputs = calculate(genome, inputs);

                /// finds the best output (one with highest value)
                //best output value
                auto fit_max = *max_element(outputs.begin(), outputs.end());

                auto it = std::find(outputs.begin(), outputs.end(), fit_max);

                // gets index
                int index = it - outputs.begin();

                // sets direction of the snake based off of the output that was calculated
                if (index == 0){
                    s.direction = LEFT;
                }
                else if(index == 1){
                    s.direction = FORWARD;
                }
                else if(index == 2){
                    s.direction = RIGHT;
                }

                // moves the snake and checks for collision with apple
                s.move();
                s.collision(apples);

////             draws the apple
                apples[0].draw(windowSurface);
                SDL_UpdateWindowSurface(window);
                SDL_Delay(100);

                // if the snake runs out of moves it kills it
                if (s.moves <= 0){
                    s.die();
                }

                // if the snake is dead
                if (s.dead){
//                // changes size of the genome
                    SIZE += s.size;


                    // complex fitness func that i split into mulitple steps
                    double secondPart = (pow(2, s.size - 1) + pow(s.size-1, 2.1) *500);
                    double lastPart = (pow(s.size-1, 1.2) * pow((0.25 * s.totalSteps), 1.3));
                    double fitness = s.totalSteps + secondPart - lastPart;
                    FITNESS += fitness;
                    run_ = false;
                    if (FITNESS < 0){
                        fitness = 10;
                    }
                }
            }


        }
    }

    // sets the average fitness and size
    FITNESS /= 20;
    SIZE /= 20;
    if (FITNESS < 0){
        cout << "???";
    }
    genome.fitness = FITNESS;
    genome.size_snake = SIZE;

}
