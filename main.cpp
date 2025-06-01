#include <iostream>
#include "board.hpp"

int main() {
    Board board;
    Player current = White;
    bool zbicie=false;
    bool game=true;
    while (game) {
        
        board.drawBoard();
        std::vector<Move> moves = board.genMoves(current);
        game=board.isGameOver();
        board.printMoves(moves);

        std::string input;
        std::cout << current;
        std::cout << "\nPodaj ruch (np. C3 D4): ";
        std::getline(std::cin, input);

        if (input.length() < 5)
        {
            std::cout << "Niepoprawny ruch!\n";
        } // Wyjście

        int fromCol = input[0] - 'A';
        int fromRow = 8 - (input[1] - '0');
        int toCol = input[3] - 'A';
        int toRow = 8 - (input[4] - '0');

        Move chosen({fromRow, fromCol}, {toRow, toCol});

        bool found = false;
        for ( Move& m : moves) {
            if (m.from.row == chosen.from.row && m.from.col == chosen.from.col &&
                m.to.row == chosen.to.row && m.to.col == chosen.to.col) {
                zbicie=board.makeMove(m);
                found = true;
                if(!zbicie){
                    if(current==White)
                    {
                        current=Black;
                    }
                    else
                    {
                        current=White;
                    }
                    
                }
                break;
            }
        }
        system("clear");
        if (!found) {
            std::cout << "Niepoprawny ruch!\n";
        }
    }

    return 0;
}


