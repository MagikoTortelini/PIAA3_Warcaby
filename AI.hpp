#ifndef AI_HPP
#define AI_HPP
#include "board.hpp"

using namespace std;

class AI
{
    Move getBestMove(Board& board, Player aiPlayer, int depth);

    int minimax(Board& board, int depth, bool isMax,int alpha, int beta,Player currentPlayer);
};


#endif