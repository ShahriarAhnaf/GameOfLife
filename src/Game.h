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
    unsigned int rows;
    unsigned int columns;
    int count_neigbours(unsigned int* board, unsigned int x, unsigned int y, int boundary_x_left, int boundary_y_top, int boundary_x_right, int boundary_y_bottom);
    void update_cells(unsigned int* board);
    int determine_state(unsigned int* board, unsigned int x, unsigned int y);
    unsigned int index(unsigned int x, unsigned int y) {
        return y * rows + x;
    }
public:
    Game(unsigned int rows, unsigned int columns);
    ~Game();
    void run_frame();
    unsigned int GetRows() { return rows; }
    unsigned int GetColumns() { return columns; }
    int board_value(unsigned int x, unsigned int y) { return board[index(x, y)]; }
};
