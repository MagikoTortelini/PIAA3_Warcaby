#include "AI.hpp"
#include <algorithm>


using namespace std;

AI::AI()=default;
AI::~AI()=default;

Move AI::MakeBestMove(Player ai,int depth, Board board) {
    int bestscore = -999999;
    Move bestmove;

    vector<Move> moves=board.genMoves(ai);

    for (auto& move : moves) {
        Board newBoard = board;
        newBoard.makeMove(move);
        if (ai==White) {
            int score = minimax(board,depth-1,-99999,99999,Black);
            if (score>bestscore) {
                bestscore = score;
            }
        }
        else {
            int score = minimax(board,depth-1,-99999,99999,White);
            if (score>bestscore) {
                bestscore = score;
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
        int value=-999999;
        for (Move& move: moves) {
            Board newBoard = board;
            newBoard.makeMove(move);
            value=max(value,minimax(board,depth-1,alpha,beta,White));
            if (value>beta)
            {
                break;
            }
            alpha=max(alpha,value);
        }
        return value;
    }
    else {
        vector<Move> moves = board.genMoves(currentPlayer);
        int value=9999999;
        for (Move& move: moves) {
            Board newBoard = board;
            newBoard.makeMove(move);
            value=max(value,minimax(board,depth-1,alpha,beta,Black));
            if (value<=alpha)
            {
                break;
            }
            beta=min(beta,value);
        }
        return value;
    }

}
