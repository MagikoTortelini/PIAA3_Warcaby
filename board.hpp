#ifndef BOARD_HPP
#define BOARD_HPP
#include "piece.hpp"
#include "move.hpp"
#include <vector>

using namespace std;




class Board
{

public:
    int draw_counter=0;
    Piece board[8][8];
    int Pieces_count[4]={0,0,0,0};
    Board();
    ~Board();
    Board(const Board& b);
    int evaulate(Player currentplayer);
    void makeMove(Move& move);
    bool isInside(int row,int col);
    bool isGameOver(bool Finalend);
    vector<Move> genMoves(Player player);
    void drawBoard();
    void printMoves(vector<Move>& moves);
    void znajdz_bicia(Position from, Position current, Player player,
                         std::vector<Position> zbite, std::vector<Move> &moves,
                          Position dir, bool ispawn);

};




#endif