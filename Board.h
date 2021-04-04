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
        FORWARD = 0,
        LEFT    = -1,
        RIGHT   = 1
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

    public:

        const Size width, height;



        Board(const Size width, const Size height) : width(width), height(height) {

            assert(width > 3);
            assert(height > 3);

            this->reset();
        }

        int snakeSize(){
            return snake.size();
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


            res[0] = pow(0.1, this->rangeToObstacle(snake.front(), FORWARD));
            res[1] = pow(0.1, this->rangeToObstacle(snake.front(), LEFT));
            res[2] = pow(0.1, this->rangeToObstacle(snake.front(), RIGHT));

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

        void move(Control control){
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
                newFoodTarget();
                movesWithoutApple = 0;
                removeTailNextTime = false;
            }else{
                movesWithoutApple ++;
            }

            if(isObstacle(newHead)){
                gameOver = true;
            }

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
