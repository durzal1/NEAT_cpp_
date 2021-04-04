#include <vector>
#include <string>
#include "random.h"
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

    // current x and y
    int x;
    int y;

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
    // 0 = none | 1 = up | 2 = right | 3= down | 4 = left
    int direction = 0;

    Snake(int x, int y){
        this->x= x;
        this->y= y;

    }
    void move(){
        if (size > 1){

            // vector of the x and y cords
            std::vector<int> cords{this->x, this->y};

            //adds the first part of the snake //todo does not move
            this->snake_parts.insert(snake_parts.begin(), cords);
        }


        // removes the old snake head
        this->snake_head.clear();

        //checks to see what direction the snake is moving

        // if its going up
        if (this->direction == 1){
            // updates y cord
            this->y -= this->height;
        }else if (this->direction == 3){
            // updates y cord
            this->y += this->height;

        }else if (this->direction == 2){
            // updates x cord
            this->x += height;

        }else if (this->direction == 4){
            // updates x cord
            this->x -= height;
        }
        // adds the new head
        std::vector<int> new_cords{this->x, this->y};

        this->snake_head.push_back(new_cords);



        // checks to see if the head hits the body
        if (snake_parts.size() != 0){
            // deletes the first element of snake_parts (aka the old one)
            this->snake_parts.pop_back();

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
        int xh1 = snake_head[0][0];
        int yh1 = snake_head[0][1];

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

//        this->draw();
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
        size += 3;

        this->fitness += 1;

        // resets apple_move
        this->apple_moves = 50;

        // adds 100 to the moves
        moves += 150;

        // if has grown more than once
        if (size > 4){
            // last element index
            int index = snake_parts.size() - 1;
            // gets the last x and y coords from the vector(last part of the snake)
            int x_cord = this->snake_parts[index][0];
            int y_cord = this->snake_parts[index][1];

            // for loop to add 3 new parts
            for (int i = 0; i < 4; i ++){
                if (i != 0){
                    if (direction == 1){
                        // adds the new part
                        std::vector<int> new_part{this->x, this->y + (width * i)};
                        snake_parts.push_back(new_part);
                    }else if (direction == 3){
                        std::vector<int> new_part{this->x, this->y - (width * i)};
                        snake_parts.push_back(new_part);
                    } else if (direction == 2){
                        std::vector<int> new_part{this->x - (width * i), this->y};
                        snake_parts.push_back(new_part);
                    } else if (direction == 4){
                        std::vector<int> new_part{this->x + (width * i), this->y};
                        snake_parts.push_back(new_part);
                    }
                }
            }
        } else{
            // there are no parts
            //inits the x and y cords
            int x_cord = snake_head[0][0];
            int y_cord = snake_head[0][1];

            // for loop to add 3 new parts
            for (int i = 0; i < 4; i ++){
                if (i != 0) {
                    if (direction == 1) {
                        // adds the new part
                        std::vector<int> new_part{this->x, this->y + (width * i)};
                        snake_parts.push_back(new_part);
                    } else if (direction == 3) {
                        std::vector<int> new_part{this->x, this->y - (width * i)};
                        snake_parts.push_back(new_part);
                    } else if (direction == 2) {
                        std::vector<int> new_part{this->x - (width * i), this->y};
                        snake_parts.push_back(new_part);
                    } else if (direction == 4) {
                        std::vector<int> new_part{this->x + (width * i), this->y};
                        snake_parts.push_back(new_part);
                    }
                }
            }
        }

    }
};
#endif //OPENGLTEST_OBJECTS_H
