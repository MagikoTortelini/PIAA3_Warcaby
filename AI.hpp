#ifndef AI_HPP
#define AI_HPP
#include "board.hpp"

using namespace std;

class AI
{
    public:
    AI();
    ~AI();
    Move MakeBestMove(Player ai,int depth, Board board);
    int minimax(Board board, int depth, int alpha, int beta, Player currentPlayer);
};


#endif