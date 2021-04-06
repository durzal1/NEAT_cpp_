//
// Created by Luecx on 03.04.2021.
//

#ifndef SNAKE_SRC_BOARD_H_
#define SNAKE_SRC_BOARD_H_

#include <vector>
#include <algorithm>
#include <list>
#include <cassert>
#include <cmath>
#include <ostream>
#include <iostream>
#include <SDL2/SDL.h>
namespace snake{

// clock wise
    enum Direction {
        NORTH,
        EAST,
        SOUTH,
        WEST
    };

// clock wise orientation
    enum Control{
        FORWARD_ = 0,
        LEFT_    = -1,
        RIGHT_   = 1
    };

    typedef int Coordinate;
    typedef int Size;

    struct Field{
        Coordinate x,y;

        bool operator==(const Field &rhs) const {
            return x == rhs.x &&
                   y == rhs.y;
        }
        bool operator!=(const Field &rhs) const {
            return !(rhs == *this);
        }
    };

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


    class Board {

    private:
        Field               nextFood;
        std::list<Field>    snake;
        Direction           direction = NORTH;

        bool removeTailNextTime = false;
        bool gameOver           = false;
        int  movesWithoutApple  = 0;
        int size = 0;
    public:
        int totalSteps = 0;
        const Size width, height;

//        // surface
//        SDL_Surface *surface = nullptr;
//
//        // window
//        SDL_Window *window = nullptr;


    // add this SDL_Surface *surface, SDL_Window *window
        Board(const Size width, const Size height) : width(width), height(height) {

            assert(width > 3);
            assert(height > 3);

            this->reset();
//            this->surface = surface;
//            this->window = window;
        }

        int snakeSize(){
            return snake.size();
        }
        list<Field> getSnakes(){
            return snake;
        }

        void reset(){
            this->snake.clear();
            this->snake.push_back(Field{width / 2, height / 2});

            this->direction = NORTH;
            this->removeTailNextTime = false;
            this->gameOver = false;
            this->movesWithoutApple = 0;

            newFoodTarget();
        }

        void newFoodTarget(){
            Field next = {
                    static_cast<int>(static_cast<float>(rand()) / RAND_MAX * width),
                    static_cast<int>(static_cast<float>(rand()) / RAND_MAX * height)};
            while(std::find(snake.begin(), snake.end(), next) != snake.end()){
                next = {
                        static_cast<int>(static_cast<float>(rand()) / RAND_MAX * width),
                        static_cast<int>(static_cast<float>(rand()) / RAND_MAX * height)};
            }
            this->nextFood = next;
        }

        std::vector<float> getNetworkInput(){
            std::vector<float> res{};
            res.resize(4);

            res[0] = pow(0.1, this->rangeToObstacle(snake.front(), FORWARD_));
            res[1] = pow(0.1, this->rangeToObstacle(snake.front(), LEFT_));
            res[2] = pow(0.1, this->rangeToObstacle(snake.front(), RIGHT_));

            Field forwardNext = traverse(snake.front());

            Vector2f nex {(float)forwardNext.x, (float)forwardNext.y};
            Vector2f fod {(float)nextFood.x, (float)nextFood.y};
            Vector2f cur {(float)snake.front().x, (float)snake.front().y};

            Vector2f dir = nex - cur;
            Vector2f tar = fod - cur;

            dir.normalise();
            tar.normalise();

            double angle =  atan2(dir.y, dir.x) - atan2(tar.y, tar.x);

            if(angle > M_PI) angle -= 2 * M_PI;
            else if(angle < -M_PI) angle += 2 * M_PI;
            angle /= M_PI;

            res[3] = angle;

            return res;
        }
//        void draw(){
//            // black screen to clear screen
//            SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format,0, 0, 0));
//
//            std::vector<Field> result(snake.size());
//            std::copy(snake.begin(), snake.end(), result.begin());
//
//            for (int i = 0; i < result.size(); i++){
//                int x = result[i].x;
//                int y =result[i].y;
//
//                // rect for body
//                SDL_Rect rect2{x * 100, y * 100, 100, 100};
//                SDL_FillRect(surface, &rect2, SDL_MapRGB(surface->format,0, 255, 0));
//
//            }
//
//            // rect for food
//            SDL_Rect rect2{nextFood.x * 100, nextFood.y * 100, 100, 100};
//            SDL_FillRect(surface, &rect2, SDL_MapRGB(surface->format,255, 0, 0));
//            SDL_UpdateWindowSurface(window);
//            // draws body
//            SDL_Delay(200);
//        }
        void move(Control control){
            totalSteps++;
            if (gameOver) return;

            this->direction = transformDirection(this->direction, control);
            Field newHead = traverse(snake.front());

            snake.push_front(newHead);
            if(removeTailNextTime){
                snake.pop_back();
            }else{
                removeTailNextTime = true;
            };

            if(newHead == nextFood) {
                size ++;
                newFoodTarget();
                movesWithoutApple = 0;
                removeTailNextTime = false;
            }else{
                movesWithoutApple ++;
            }

            if(isObstacle(newHead)){
                gameOver = true;
            }
//            draw();

        }

        friend std::ostream &operator<<(std::ostream &os, Board &board) {

            for(int i = board.height - 1;i >= 0; i--){
                for(int n = 0; n < board.width; n++){
                    if(Field{n,i} == board.nextFood){
                        std::cout << "A";
                    }else if(board.isObstacle({n,i}) || Field{n,i} == board.snake.front()){
                        std::cout << "#";
                    }else{
                        std::cout << '.';
                    }
                }
                std::cout << "\n";
            }

            return os;
        }

        bool IsGameOver() const {
            return gameOver;
        }

        int GetMovesWithoutApple() const {
            return movesWithoutApple;
        }

    private:
        int rangeToObstacle(Field root, Control control){
            Field current = root;

            int count = 0;

            while(!isObstacle(current)){
                switch (transformDirection(this->direction, control)){
                    case 0: current = {current.x, current.y+1}; break;
                    case 1: current = {current.x-1, current.y}; break;
                    case 2: current = {current.x, current.y-1}; break;
                    case 3: current = {current.x+1, current.y}; break;
                }
                count ++;
            }
            return count-1;
        }

        Field traverse(Field root){
            switch (direction){
                case 0: return {root.x, root.y+1};
                case 1: return {root.x-1, root.y};
                case 2: return {root.x, root.y-1};
                case 3: return {root.x+1, root.y};
            }
            return {-1,-1};
        }

        Direction transformDirection(Direction direction, Control control){
            return Direction((direction + control + 4) % 4);
        }

        bool isObstacle(Field field){

            for(Field& f:snake){
                if(f != snake.front() && f == field){
                    return true;
                }
            }



            return field.x < 0 || field.x >= width || field.y < 0 || field.y >= height;
        }

    };


}

#endif //SNAKE_SRC_BOARD_H_
