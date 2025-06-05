#include <iostream>

#include "AI.hpp"
#include "board.hpp"


void clear_borad(Board& b)
{
    for(int i=0;i<8;i++)
    {
        for(int z=0;z<8;z++)
        {
            b.board[i][z]=Piece();
        }
    }
}


int main() {
    Board board;
    Player current = Black;
    bool state=true;
    AI ai;

    while (state) {
        board.drawBoard();
        std::vector<Move> moves = board.genMoves(current);
        if(board.isGameOver(true))
        {
            state=false;
            break;
        }
        if (current==Black) {
            Move ai_move= ai.MakeBestMove(Black,5,board);
            board.makeMove(ai_move);
            current=White;
            cout<<"AI Black"<<endl;
            board.printMoves(moves);  
            continue;
        }
         if (current==White) {
            Move ai_move= ai.MakeBestMove(White,9,board);
            board.makeMove(ai_move);
            current=Black;
            cout<<"AI White"<<endl;
            board.printMoves(moves);  
            continue;
        }
        
        board.printMoves(moves);  
        
    }

    return 0;
}



