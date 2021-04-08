#ifndef SNAKE_OBJECTS_H
#define SNAKE_OBJECTS_H

#include <vector>
#include <string>
#include "random.h"
#include "types.h"
#include <SDL2/SDL.h>
//VARIABLES
int HEIGHT = 20;
int WIDTH = 20;

//apple class
class Apple{
public:
    // doesnt do anything i just need it
    float start = 3;

    int x;
    int y;


    //randomly sets x and y
    Apple(){
        this->init();
    }

    void init(){

        x = (randomint(0, 9, start) * 100);
        y = (randomint(0, 9, start) * 100);

    }


    void draw(SDL_Surface *surface) {

        //draws a red square to represent the apple
        SDL_Rect rect{x, 900 - y, WIDTH, HEIGHT};
        SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 0, 0));
    }



};

// snake class
class Snake{
public:

    // the x and y cords of the head
    std::vector <std::vector<int>> snake_head ;

    // the x and y cords of the body parts
    std::vector <std::vector<int>> snake_parts ;

    // current size of the snake
    int size = 1;

    //total steps taken
    int totalSteps = 0;

    // current x and y
    int x;
    int y;

    // if it should remove the last part
    bool removeLast = true;

    // width and height of the snake
    int height = HEIGHT;
    int width = WIDTH;

    // moves the snake has left before it dies
    int moves = 50;

    // moves since an apple has been eaten
    int apple_moves = 0;

    // fitness of the snake
    Fitness fitness = 0;

    // bool if the snake is dead
    bool dead = false;

    // direction used for calculating the real direction
    int direction = 0;

    // the cardinal direction it is facing
    int CardinalDirection = NORTH;

    // the real direction the snake is going (DEPENDENT ON CARDINAL DIRECTION)
    int real_direction = UP1;

    // surface
    SDL_Surface *surface = nullptr;

    // window
    SDL_Window *window = nullptr;

//, SDL_Surface *surface, SDL_Window *window
    Snake(int x, int y, SDL_Surface *surface, SDL_Window *window){
        this->x= x;
        this->y= y;
        this->surface = surface;
        this->window = window;
    }
    void draw(){
        // black screen to clear screen
        SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format,0, 0, 0));

        // draws the rect the will represent the snake head
        SDL_Rect rect1{x,900- y, width, height};
        SDL_FillRect(surface, &rect1, SDL_MapRGB(surface->format,0, 0, 255));

        // draws the rest of the body
        for (int i = 0; i < snake_parts.size(); i ++){
            // sets x and y
            int x = snake_parts[i][0];
            int y = snake_parts[i][1];
            // draws the parts
            SDL_Rect rect{x, 900-y, width, height};
            SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format,0, 255, 0));

        }
    }

    //checks to see what direction the snake is moving
    // then moves it that direction
    void getDirection(){
        // calculates which direction it will move
        int go = (this->CardinalDirection + direction + 4) % 4;
        this->real_direction = go;

        // resets cardinal direction
        this->CardinalDirection = go;
    }
    void move(){
        if (size > 1){

            // vector of the x and y cords
            std::vector<int> cords{this->x, this->y};

            //adds the first part of the snake
            this->snake_parts.insert(snake_parts.begin(), cords);
        }

        // removes the old snake head
        this->snake_head.clear();

        this->getDirection();

        /// moves the snake based off of the real direction
        // if its going up
        if (real_direction == UP1){
            // updates y cord
            this->y += this->height;
        }
            // going left
        else if (real_direction == LEFT1){
            // updates x cord
            this->x -= this->height;

        }
            //going right
        else if (real_direction == RIGHT1) {
            // updates x cord
            this->x += height;
        }
        // going down
        else if (real_direction == DOWN1){
            //updates  y
            this->y -= height;
        }

        // adds the new head
        std::vector<int> new_cords{this->x, this->y};

        this->snake_head.push_back(new_cords);

        // checks to see if the head hits the body
        if (snake_parts.size() != 0){
            // if it should remove the last part
            if (removeLast){
                // deletes the first element of snake_parts (aka the old one)
                this->snake_parts.pop_back();
            }else{
                removeLast = true;
            }
            //changes the x and y of the head to strings
            std::string x1 = std::to_string(snake_head[0][0]);
            std::string y1 = std::to_string(snake_head[0][1]);
            for (int i = 0; i < snake_parts.size(); i++){
                // changes the x and y of the body parts to strings
                std::string xp1 = std::to_string(snake_parts[i][0]);
                std::string yp1 = std::to_string(snake_parts[i][1]);

                // checks if the snake collided with itself
                if (xp1+yp1 == x1+y1){
                    this->die();
                }
            }
        }
        // checks to see if the snake goes out of bounds
        if (this->x < 0 || this->y < 0 || this->x >= 1000 || this->y >= 1000){
            this->die();
        }
        this->draw();
    }
    // kills the snake
    void die(){
        dead = true;
    }
    void collision(std::vector<Apple> &apples){

        //sets the cords
        int x_cord = snake_head[0][0];
        int y_cord = snake_head[0][1];
        // checks to see if the snake collides with the apple
        if (x_cord == apples[0].x && y_cord == apples[0].y){
            // clears the apples vector
            apples.clear();

            // creates an apple
            Apple apple = Apple();

            bool d = false;
            while (!d){
                // makes sure the apple is not in the head
                if (apple.x == x && apple.y == y){
                    // creates a new apple
                    apple = Apple();
                    continue;
                }
                // makes sure the apple is not in the body
                for (int i = 0; i < snake_parts.size(); i++){
                    if (snake_parts[i][0] == apple.x && snake_parts[i][1] == apple.y){
                        apple = Apple();
                        continue;
                    }
                }
                d = true;
            }
            // re adds the apple
            apples.push_back(apple);

            this->grow();
        }
    }
    void grow(){
        size += 1;

        // resets apple_move
        this->apple_moves = 50;

        // adds 100 to the moves
        moves += 100;

        // makes sure the program does not remove the last body next move
        removeLast = false;

    }
};
#endif //SNAKE_OBJECTS_H
