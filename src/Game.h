#pragma once

#include <iostream>
#include <random>
#include <thread>
#include <chrono>
// class cell{
class Game
{
private:
    unsigned int* board; // array of pointers to arrays, basically works like a 2d array.
    unsigned int* new_board;
    size_t rows;
    size_t columns;
    int count_neigbours(unsigned int* board, size_t x, size_t y, int boundary_x_left, int boundary_y_top, int boundary_x_right, int boundary_y_bottom);
    void update_cells(unsigned int* board);
    int determine_state(unsigned int* board, size_t x, size_t y);
    size_t index(size_t x, size_t y) {
        return y * rows + x;
    }
public:
    Game(size_t rows, size_t columns);
    ~Game();
    void run_frame();
};
