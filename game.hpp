#ifndef GAME_HPP
#define GAME_HPP
#include "board.hpp";
#include "AI.hpp";

using namespace std;

class game
{
private:
    Board board;
    AI ai;

    Player currentPlayer;
    bool vsComputer;
    void handleInput();
public:
    game(/* args */);
    ~game();
};

game::game(/* args */)
{
}

game::~game()
{
}


#endif