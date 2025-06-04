#include "AI.hpp"
#include <algorithm>
#include <iostream>



using namespace std;

AI::AI()=default;
AI::~AI()=default;

Move AI::MakeBestMove(Player ai,int depth, Board board) {
    int bestscore_white = -999999;
    int bestscore_black = 999999;
    Move bestmove;

    vector<Move> moves=board.genMoves(ai);

    for (auto& move : moves) {

        Board newBoard = board;
        newBoard.makeMove(move);
        if (ai==White) {
            int score = minimax(newBoard,depth-1,-99999,99999,Black);

            if (score>bestscore_white) {
                bestscore_white = score;
                bestmove = move;
            }
        }
        else {
            int score = minimax(newBoard,depth-1,-99999,99999,White);

            if (score<bestscore_black) {
                bestscore_black = score;
                bestmove = move;
            }
        }



    }
    return bestmove;
}


int AI::minimax(Board board, int depth, int alpha, int beta, Player currentPlayer) {
    if (!board.isGameOver() || depth==0) {
        return board.evaulate(currentPlayer);
    }
    if (currentPlayer==Black) {
        vector<Move> moves = board.genMoves(currentPlayer);
        int value=999999;
            for (Move& move: moves) {
                Board newBoard = board;
                newBoard.makeMove(move);
                value=min(value,minimax(newBoard,depth-1,alpha,beta,White));
                beta=min(beta,value);
                if (beta<=alpha)
                {
                    break;
                }

            }

        return value;
    }
    else {
        vector<Move> moves = board.genMoves(currentPlayer);
        int value=-9999999;
        for (Move& move: moves) {
            Board newBoard = board;
            newBoard.makeMove(move);
            value=max(value,minimax(newBoard,depth-1,alpha,beta,Black));
            alpha=max(alpha,value);
            if (beta<=alpha)
            {
                break;
            }

        }
        return value;
    }

}
