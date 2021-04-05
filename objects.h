#include <vector>
#include <string>
#include "random.h"
#include "types.h"
#ifndef OPENGLTEST_OBJECTS_H
#define OPENGLTEST_OBJECTS_H

//VARIABLES
int HEIGHT = 100;
int WIDTH = 100;

//apple class
class Apple{
public:
    // doesnt do anything i just need it
    float start = 3;


    //randomly sets x and y
    int x = (randomint(0, 10, start) * 100);
    int y = (randomint(0, 10, start) * 100);

//    void draw(SDL_Surface *surface){
//        //draws a red square to represent the apple
//        SDL_Rect rect{x,y, WIDTH, HEIGHT};
//        SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format,255, 0, 0));
//
//
//    }

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
    float fitness = 0;

    // bool if the snake is dead
    bool dead = false;

    // direction the snake is currently going
    int direction = 0;
//
//    // surface
//    SDL_Surface *surface = nullptr;
//
//    // window
//    SDL_Window *window = nullptr;

    Snake(int x, int y){
        this->x= x;
        this->y= y;
//        this->surface = surface;
//        this->window = window;
    }
//    void draw(){
//        // black screen to clear screen
//        SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format,0, 0, 0));
//
//        // draws the rect the will represent the snake head
//        SDL_Rect rect1{x, y, width, height};
//        SDL_FillRect(surface, &rect1, SDL_MapRGB(surface->format,0, 255, 0));
//
//        // draws the rest of the body
//        for (int i = 0; i < snake_parts.size(); i ++){
//            // sets x and y
//            int x = snake_parts[i][0];
//            int y = snake_parts[i][1];
//            // draws the parts
//            SDL_Rect rect{x, y, width, height};
//            SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format,0, 255, 0));
//
//        }
//    }
    void move(){
        if (size > 1){

            // vector of the x and y cords
            std::vector<int> cords{this->x, this->y};

            //adds the first part of the snake
            this->snake_parts.insert(snake_parts.begin(), cords);
        }

        // removes the old snake head
        this->snake_head.clear();

        //checks to see what direction the snake is moving
        // if its going up
        if (direction == 0){
            // updates y cord
            this->y += this->height;
        }
        // going left
        else if (direction == -1){
            // updates y cord
            this->x -= this->height;

        }
        //going right
        else if (direction == 1) {
            // updates x cord
            this->x += height;
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
//        this->draw();
    }
    // kills the snake
    void die(){
        // clears snake_parts
        snake_parts.clear();

        // resets x and y
        this->x = 500;
        this->y = 500;
        this->dead = true;

        // resets size and direction
        direction = 0;
        size = 1;

        // adds the new head
        std::vector<int> new_cords{this->x, this->y};

        this->snake_head.push_back(new_cords);

    }
    void collision(std::vector<Apple> &apples){

        //sets the cords
        int x_cord = snake_head[0][0];
        int y_cord = snake_head[0][1];
        // checks to see if the snake collides with the apple
        if (x_cord == apples[0].x && y_cord == apples[0].y){
            // clears the apples vector
            apples.clear();

            // re adds another apple
            apples.push_back(Apple());

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
#endif //OPENGLTEST_OBJECTS_H
