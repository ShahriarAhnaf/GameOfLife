#include "Game.h"

int Game::count_neigbours(unsigned int** board, int x, int y, int boundary_x_left, int boundary_y_top, int boundary_x_right, int boundary_y_bottom)
{
	return 0;
}

void Game::update_cells(unsigned int** board)
{
}

Game::Game(int rows, int columns)
{
    
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
        board = new unsigned int* [columns];
        // populate board randomly!
        for(int i = 0)
        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                board[x][y] = distrib(gen);
            }
        }
}

Game::~Game()
{
}

int Game::determine_state(unsigned int(*board)[size], int x, int y)
{
	return 0;
}
