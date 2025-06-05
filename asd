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
                board.makeMove(m);
                found = true;
                if(current==White)
                    {
                        current=Black;
                    }
                else
                    {
                        current=White;
                    }
                    

                break;
            }
        }
        if (!found) {
            std::cout << "Niepoprawny ruch!\n";
        }









        clear_borad(board);
    board.board[6][3]=Piece(White,Queen);
    board.board[5][4]=Piece(Black,Pawn);
    board.board[4][5]=Piece(Black,Pawn);
    board.drawBoard();
    std::vector<Move> moves = board.genMoves(White);
    board.printMoves(moves);
    cin>>game;