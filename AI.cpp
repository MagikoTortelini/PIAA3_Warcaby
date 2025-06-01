#include "AI.hpp"
#include <limits>

Move AI::getBestMove(Board& board, Player aiPlayer, int depth) {
    int bestScore = std::numeric_limits<int>::min();
    Move bestMove;
    auto moves = board.genMoves(aiPlayer);

    for (Move move : moves) {
        std::vector<Piece> captured;
        for (auto pos : move.capture)
            captured.push_back(board.board[pos.row][pos.col]);

        board.makeMove(move);
        int score = minimax(board, depth - 1, false, -10000, 10000, aiPlayer);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}

int AI::minimax(Board& board, int depth, bool isMax, int alpha, int beta, Player aiPlayer) {
    if (depth == 0 || board.isGameOver()) {
        return board.evaluate(aiPlayer);
    }

    Player current = isMax ? aiPlayer : (aiPlayer == White ? Black : Black);
    auto moves = board.genMoves(current);

    if (isMax) {
        int best = -10000;
        for (Move move : moves) {
            std::vector<Piece> captured;
            for (auto pos : move.capture)
                captured.push_back(board.board[pos.row][pos.col]);

            board.makeMove(move);
            int score = minimax(board, depth - 1, false, alpha, beta, aiPlayer);


            best = std::max(best, score);
            alpha = std::max(alpha, score);
            if (beta <= alpha) break;
        }
        return best;
    } else {
        int best = 10000;
        for (Move move : moves) {
            std::vector<Piece> captured;
            for (auto pos : move.capture)
                captured.push_back(board.board[pos.row][pos.col]);

            board.makeMove(move);
            int score = minimax(board, depth - 1, true, alpha, beta, aiPlayer);

            best = std::min(best, score);
            beta = std::min(beta, score);
            if (beta <= alpha) break;
        }
        return best;
    }
}
