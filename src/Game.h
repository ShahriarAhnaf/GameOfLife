#pragma once

#include <iostream>
#include <random>
#include <thread>
#include <chrono>
// class cell{
class Game
{
private:
    unsigned int** board; // array of pointers to arrays, basically works like a 2d array.
    int rows;
    int columns;
    int count_neigbours(unsigned int** board, int x, int y, int boundary_x_left, int boundary_y_top, int boundary_x_right, int boundary_y_bottom);
    void update_cells(unsigned int** board);
public:
    Game(int rows, int columns);
    ~Game();
    void init_random_device() { 
        std::random_device rd;
    std::mt19937::result_type seed = rd() ^ (
        (std::mt19937::result_type)
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
            ).count() +
        (std::mt19937::result_type)
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
            ).count());

    std::mt19937 gen(seed);
    std::uniform_int_distribution<unsigned> distrib(0, 1);
    }
    int determine_state(unsigned int(*board)[size], int x, int y);

};


const int size = 20;

int count_neigbours(unsigned int**board, int x, int y, int boundary_x_left, int boundary_y_top, int boundary_x_right, int boundary_y_bottom) {
    unsigned int sum = 0;
    for (int n = boundary_y_top; n < boundary_y_bottom; n++) {
        for (int m = boundary_x_left; m < boundary_x_right; m++) {
            // std::cout << board[x+m][y+n] << " " << m << n << " "; 
            sum += board[x + m][y + n];
        }
    }
    //std::cout << "--" << sum <<" -- " << x << y << std::endl;
    return sum;
}

int determine_state(unsigned int**board, int x, int y) {
    unsigned int sum = 0;
    // top left 
    if (x == 0 && y == 0) {
        sum += count_neigbours(board, x, y, 0, 0, 2, 2);
    }
    //bottom left 
    else if (x == 0 && y == size - 1) {
        sum += count_neigbours(board, x, y, 0, -1, 2, 1);
    }
    // top right 
    else if (x == size - 1 && y == 0) {
        sum += count_neigbours(board, x, y, -1, 0, 1, 2);
    }
    // bottom right
    else if (x == size - 1 && y == size - 1) {
        sum += count_neigbours(board, x, y, -1, -1, 1, 1);
    }
    else if (x == 0) { // left column
        sum += count_neigbours(board, x, y, 0, 1, 2, 2);
        for (int n = -1; n < 2; n++) {
            // right most column values
            sum += board[size - 1][y + n];
        }
    }
    else if (x == size - 1) {// right column
        sum += count_neigbours(board, x, y, -1, 1, 1, 2);
        for (int n = -1; n < 2; n++) {
            // left most column
            sum += board[0][y + n];
        }
    }
    else if (y == 0) { // top
        sum += count_neigbours(board, x, y, -1, 0, 2, 2);
        for (int n = -1; n < 2; n++) {
            // right most column values
            sum += board[x + n][size - 1];
        }
    }
    else if (y == size - 1) {// bottom 
        sum += count_neigbours(board, x, y, -1, -1, 2, 1);
        for (int n = -1; n < 2; n++) {
            // right most column values
            sum += board[x + n][0];
        }
    }
    else {
        sum += count_neigbours(board, x, y, -1, -1, 2, 2);
    }
    //std::cout << "--" << sum <<" -- " << x << y << std::endl;
    return sum;
}

// pass 2d array by reference moment
void update_cells(unsigned int**board) {
    unsigned int new_board[size][size] = { 0 };
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            unsigned int state = board[x][y];
            int neighbours = determine_state(board, x, y);
            // three rules of life
            if (neighbours == 3 || neighbours == 2) { new_board[x][y] = 1; }
            else {
                new_board[x][y] = 0;
            }
        }
    }
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            board[x][y] = new_board[x][y];
        }
    }
}

int main() {
    std::random_device rd;
    std::mt19937::result_type seed = rd() ^ (
        (std::mt19937::result_type)
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
            ).count() +
        (std::mt19937::result_type)
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
            ).count());

    std::mt19937 gen(seed);
    std::uniform_int_distribution<unsigned> distrib(0, 1);

    // array of pointers to array
    unsigned int** board = new unsigned int* [size];
    // populate board randomly!

    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            board[x][y] = distrib(gen);
        }
    }

    while (true) {
        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {

                if (board[x][y] == 1) {
                    // "\033[{FORMAT_ATTRIBUTE};{FORGROUND_COLOR};{BACKGROUND_COLOR}m{TEXT}\033[{RESET_FORMATE_ATTRIBUTE}m"
                    std::cout << "\033[1;31;49m" << board[x][y] << "\033[0m";
                }
                else {
                    std::cout << "\033[0;97;49m" << board[x][y] << "\033[0m";
                }
            }
            std::cout << std::endl;
        }// one second delay
        update_cells(board);

        std::cout << "-------------------------------" << std::endl;
    }
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    return 0;
}