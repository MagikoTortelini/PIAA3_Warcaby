#ifndef BOARD_HPP
#define BOARD_HPP
#include "piece.hpp"
#include "move.hpp"
#include <vector>

using namespace std;

class Board
{

public:
    Piece board[8][8];

    Board();
    ~Board();
    bool makeMove(Move& move);
    bool isInside(int row,int col);
    bool isGameOver();
    vector<Move> genMoves(Player player);
    void drawBoard();
    void printMoves(vector<Move>& moves);
    void promotion(Piece p);


};




#endif