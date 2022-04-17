#include "Game.h"

int Game::count_neigbours(unsigned int* board, unsigned int x,unsigned int y, int boundary_x_left, int boundary_y_top, int boundary_x_right, int boundary_y_bottom)
{
    unsigned int sum = 0;
    for (int n = boundary_y_top; n < boundary_y_bottom; n++) {
        for (int m = boundary_x_left; m < boundary_x_right; m++) {
            // std::cout << board[x+m][y+n] << " " << m << n << " "; 
            sum += board[index(x+m, y+n)];
        }
    }
    //std::cout << "--" << sum <<" -- " << x << y << std::endl;
    return sum;
}

void Game::update_cells(unsigned int* board)
{
    for (unsigned int y = 0; y < rows; y++) {
        for (unsigned int x = 0; x < columns; x++) {
            unsigned int state = board[index(x,y)];
            int neighbours = determine_state(board, x, y);
            // three rules of life
            if (neighbours == 3 || neighbours == 2) { new_board[index(x, y)] = 1; }
            else {
                new_board[index(x,y)] = 0;
            }
        }
    }
    for (unsigned int y = 0; y < rows; y++) {
        for (unsigned int x = 0; x < columns; x++) {
            board[index(x,y)] = new_board[index(x,y)];
        }
    }
}

Game::Game(unsigned int rows, unsigned int columns)
    :rows{ rows },
    columns{columns}
{
        // random engine code from stack overflow as always
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
        board = new unsigned int[(size_t)rows * columns]{};
        new_board = new unsigned int[(size_t)rows * columns]{};
        // populate board randomly!
        for (unsigned int y = 0; y < rows; y++) {
            for (unsigned int x = 0; x < columns; x++) {
                board[index(x,y)] = distrib(gen);
            }
        }
}

Game::~Game()
{
    // gaarbage clean up 
    delete[] board;
    board = nullptr;
    delete[] new_board;
    new_board = nullptr;
}

int Game::determine_state(unsigned int *board, unsigned int x, unsigned int y)
{
    unsigned int sum = 0;
    // top left 
    if (x == 0 && y == 0) {
        sum += count_neigbours(board, x, y, 0, 0, 2, 2);
    }
    //bottom left 
    else if (x == 0 && y == rows - 1) {
        sum += count_neigbours(board, x, y, 0, -1, 2, 1);
    }
    // top right 
    else if (x == columns - 1 && y == 0) {
        sum += count_neigbours(board, x, y, -1, 0, 1, 2);
    }
    // bottom right
    else if (x == columns - 1 && y == rows - 1) {
        sum += count_neigbours(board, x, y, -1, -1, 1, 1);
    }
    else if (x == 0) { // left column
        sum += count_neigbours(board, x, y, 0, 1, 2, 2);
        for (int n = -1; n < 2; n++) {
            // right most column values
            sum += board[index(columns - 1,y + n)];
        }
    }
    else if (x == columns - 1) {// right column
        sum += count_neigbours(board, x, y, -1, 1, 1, 2);
        for (int n = -1; n < 2; n++) {
            // left most column
            sum += board[index(0,y + n)];
        }
    }
    else if (y == 0) { // top
        sum += count_neigbours(board, x, y, -1, 0, 2, 2);
        for (int n = -1; n < 2; n++) {
            // right most column values
            sum += board[index(x + n, rows - 1)];
        }
    }
    else if (y == rows - 1) {// bottom 
        sum += count_neigbours(board, x, y, -1, -1, 2, 1);
        for (int n = -1; n < 2; n++) {
            // right most column values
            sum += board[index(x + n, 0)];
        }
    }
    else {
        sum += count_neigbours(board, x, y, -1, -1, 2, 2);
    }
    //std::cout << "--" << sum <<" -- " << x << y << std::endl;
    return sum;
}

void Game::run_frame()
{
        for (unsigned int y = 0; y < rows; y++) {
            for (unsigned int x = 0; x < columns; x++) {

                if (board[index(x,y)] == 1) {
                    // "\033[{FORMAT_ATTRIBUTE};{FORGROUND_COLOR};{BACKGROUND_COLOR}m{TEXT}\033[{RESET_FORMATE_ATTRIBUTE}m"
                    std::cout <<  board[index(x,y)] << " ";
                }
                else {
                    std::cout <<  board[index(x,y)] << " ";
                }
            }
            std::cout << std::endl;
        }// one second delay
        update_cells(board);

        std::cout << "-------------------------------" << std::endl;
}



/*GAME TEST MAIN
int main() {
    Game new_game(10,10);

    while (true) {
        new_game.run_frame();
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    return 0;
}
*/