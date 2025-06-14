#include "board.hpp"
#include <iostream>

using namespace std;

Board::~Board() = default;

//Konstruktor kopiujący
Board::Board(const Board &b) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = b.board[i][j];
        }
    }

    // Skopiuj licznik pionów/dam
    for (int i = 0; i < 4; ++i) {
        Pieces_count[i] = b.Pieces_count[i];
    }
}

//Konstruktor tworzenie bazowej planszy
Board::Board() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1) {
                if (row < 3) {
                    board[row][col] = Piece(Black, Pawn);
                    Pieces_count[0] += 1;
                } else if (row > 4) {
                    board[row][col] = Piece(White, Pawn);
                    Pieces_count[1] += 1;
                } else {
                    board[row][col] = Piece();
                }
            } else {
                board[row][col] = Piece();
            }
        }
    }
}

//Sprawdzenie czy figura jest w zakresie planszy
bool Board::isInside(int row, int col) {
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        return true;
    } else {
        return false;
    }
}

//Robienie ruchu
void Board::makeMove(Move &move) {
    //Ruch piona
    Piece &moving = board[move.from.row][move.from.col];
    board[move.to.row][move.to.col] = moving;
    board[move.from.row][move.from.col] = Piece();
    //Sprawdzenie czy Biały pion może być damką
    if (board[move.to.row][move.to.col].owner == White && move.to.row == 0&&board[move.to.row][move.to.col].type == Pawn) {
        board[move.to.row][move.to.col].type = Queen;
        Pieces_count[3] += 1;

        //Sprawdzenie czy Czarny pion może być damką
    } else if (board[move.to.row][move.to.col].owner == Black && move.to.row == 7&&board[move.to.row][move.to.col].type == Pawn) {
        board[move.to.row][move.to.col].type = Queen;
        Pieces_count[2] += 1;
    }
    draw_counter+=1;

    // Zbijanie (jeśli są pozycje do zbicia)
    for (Position pos: move.capture) {
        draw_counter=0;
        //Odjecie czarnego piona z licznika
        if (board[pos.row][pos.col].owner == Black && board[pos.row][pos.col].type == Pawn) {
            Pieces_count[0] -= 1;
        }
        //Odjecie białego piona z licznika
        else if (board[pos.row][pos.col].owner == White && board[pos.row][pos.col].type == Pawn) {
            Pieces_count[1] -= 1;
        }
        //Odjecie czarnej damy z licznika
        else if (board[pos.row][pos.col].owner == Black && board[pos.row][pos.col].type == Queen) {
            Pieces_count[0] -= 1;
            Pieces_count[2] -= 1;
        }
        //Odjecie białej damy z licznika
        else if (board[pos.row][pos.col].owner == White && board[pos.row][pos.col].type == Queen) {
            Pieces_count[1] -= 1;
            Pieces_count[3] -= 1;
        }
        //Usuniecie pionka z planszy i zastąpienie go pustym pionem
        board[pos.row][pos.col] = Piece();
    }
}

//Sprawdzenie czy gra się skończyła
bool Board::isGameOver(bool Finalend) {
    vector<Move> whiteMoves = genMoves(White);
    vector<Move> blackMoves = genMoves(Black);
    
    // Gra się kończy gdy jeden z graczy nie ma ruchów
    if (whiteMoves.empty() && blackMoves.empty()) {
        if(Finalend){
        cout << "TIE" << endl;
        }
        return true;
    } else if (whiteMoves.empty()) {
        if(Finalend){
        cout << "Black wins" << endl;
        }
        return true;
    } else if (blackMoves.empty()) {
        if(Finalend){
        cout << "White wis" << endl;
        }
        return true;
    }
    
    // Sprawdzenie remisu przez draw_counter (np. 50 ruchów bez bicia)
    if (draw_counter >= 100) { // 50 ruchów każdego gracza
        cout << "DRAW - 50 move rule" << endl;
        return true;
    }
    
    return false; // Gra trwa dalej
}


//Generacja możliwych ruchów dla danego gracza
std::vector<Move> Board::genMoves(Player player) {
    //Przechowanie ruchów bijącyhc
    std::vector<Move> moves;
    //Kierunke w jakim można się poruszyć Góra/Dół
    int direction;
    //Czy jest dostępne bicie
    bool bicie = false;

    //Ustawienie kierunku
    if (player == White) {
        direction = -1;
    } else {
        direction = 1;
    }

    //Przejscie po wszystkich miejscach na planszy
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece p = board[row][col];

            //Pominięcie pionów poza własnymi
            if (p.owner != player)
                continue;

            //Wektor przechowujący zbicia i ruchy bijace dostepne dla piona
            std::vector<Position> zbite;
            std::vector<Move> bicia;

            //Znajdowanie dostępnych bic dla piona
            if (p.type == Pawn) {
                znajdz_bicia({row, col}, {row, col}, player, zbite, bicia,{0,0},true);
                if (!bicia.empty()) {
                    bicie = true;
                    moves.insert(moves.end(), bicia.begin(), bicia.end());
                }
            }
            //Generowanie bicia dla damy
            else if (p.type == Queen) {
                //przejscie po wszystkich kierunkach w jedneym kierunku X
                
                for (int dx = -1; dx <= 1; dx += 2) {
                    int newRow = row + direction;
                    int newCol = col + dx;
                    while (isInside(newRow, newCol)&&board[newRow][newCol].owner==Noone) {
                            znajdz_bicia({row, col}, {newRow, newCol}, player, zbite, bicia,{direction,dx},false);
                        
                        newRow += direction;
                        newCol += dx;
                    }
                }
                //przejscie po wszystkich kierunkach w jedneym kierunku Y
                for (int dx = -1; dx <= 1; dx += 2) {
                    int newRow = row - direction;
                    int newCol = col + dx;
                    while (isInside(newRow, newCol)&&board[newRow][newCol].owner==Noone) {
                            znajdz_bicia({row, col}, {newRow, newCol}, player, zbite, bicia,{-direction,dx},false);
                            
                        
                        newRow -= direction;
                        newCol += dx;
                    }
                }
                
                znajdz_bicia({row, col}, {row, col}, player, zbite, bicia, {0,0},true);
                if (!bicia.empty()) {
                bicie = true;
                moves.insert(moves.end(), bicia.begin(), bicia.end());
            }
            }
            
        }
    }
    //Jeżeli są dostępne bicia to pomijamy zwkyłe ruchy i zwracamy listę dostepnych ruchów
    if (bicie) {
       
        return moves;
    }

    //Wektor przechowujący normalne ruchu
    vector<Move> normlaMoves;

    //Podobnie jak w przypadku bicia
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece p = board[row][col];
            if (p.owner != player)
                continue;

            //Sprawdzenie czy jest pionem po czym sprawdzenie ruchów lewo/prawo
            if (p.type == Pawn) {
                for (int dx = -1; dx <= 1; dx += 2) {
                    int newRow = row + direction;
                    int newCol = col + dx;
                    //Sprawdzenie czy ruch miesci sie w planszy oraz czy pole jest puste
                    if (isInside(newRow, newCol) && board[newRow][newCol].owner == Noone) {
                        normlaMoves.push_back(Move({row, col}, {newRow, newCol}, {-1, -1}));
                    }
                }
            }
            //Poruszanie damki podobnie jak pion tylko jeszcze do tyłu
            else if(p.type==Queen) {
                for (int dx = -1; dx <= 1; dx += 2) {
                    int newRow = row + direction;
                    int newCol = col + dx;
                    while (isInside(newRow, newCol)) {
                        if (board[newRow][newCol].owner == Noone) {
                            normlaMoves.push_back(Move({row, col}, {newRow, newCol}, {-1, -1}));
                        } else {
                            break;
                        }
                        newRow += direction;
                        newCol += dx;
                    }
                }
                for (int dx = -1; dx <= 1; dx += 2) {
                    int newRow = row - direction;
                    int newCol = col + dx;
                    while (isInside(newRow, newCol)) {
                        if (board[newRow][newCol].owner == Noone) {
                            normlaMoves.push_back(Move({row, col}, {newRow, newCol}, {-1, -1}));
                        } else {
                            break;
                        }
                        newRow -= direction;
                        newCol += dx;
                    }
                }
            }
        }
    }
    //Zwrócenie normalnych ruchów
    return normlaMoves;
}

//Funkcja badająca wszystkie dostępne bicia
void Board::znajdz_bicia(Position from, Position current, Player player,
                         std::vector<Position> zbite, std::vector<Move> &moves,
                        Position dir, bool ispawn) {


    bool do_bicia = false;
    //Kierunki w jakich można bić
    int direction[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    //Sprawdzenie czy jest pionem
    if (ispawn) {
        for (auto &[dr, dc]: direction) {


            int midrow = current.row + dr;
            int midcol = current.col + dc;
            int endrow = current.row + 2 * dr;
            int endcol = current.col + 2 * dc;

            if (!isInside(endrow, endcol) || !isInside(midrow, midcol))
                continue;

            Piece enemy = board[midrow][midcol];
            Piece target = board[endrow][endcol];

            if (enemy.owner != Noone && enemy.owner != player &&
                target.owner == Noone) {
                // Symulacja ruchu
                Piece undo_piec=board[midrow][midcol];
                Position undo_position={midrow,midcol};

                board[midrow][midcol]=Piece();

                zbite.push_back({midrow, midcol});

                // Rekurencja – sprawdzanie dalszych bić
                znajdz_bicia(from, {endrow, endcol}, player, zbite, moves,{0,0},true);
                

                // Cofnięcie stanu
                board[undo_position.row][undo_position.col]=undo_piec;
                zbite.pop_back();

                do_bicia = true;
            }
        }
    } 
    else {
        int midrow = current.row + dir.row;
        int midcol = current.col + dir.col;
        int endrow = current.row + 2 * dir.row;
        int endcol = current.col + 2 * dir.col;
        if (!isInside(endrow, endcol) || !isInside(midrow, midcol)) {
            
            return;
        }
        Piece enemy = board[midrow][midcol];
        Piece target = board[endrow][endcol];

        if (enemy.owner != Noone && enemy.owner != player &&
            target.owner == Noone) {
            // Symulacja ruchu
            Piece undo_piec=board[midrow][midcol];
            Position undo_position={midrow,midcol};
            board[midrow][midcol]=Piece();
            zbite.push_back({midrow, midcol});

            // Rekurencja – sprawdzanie dalszych bić
            znajdz_bicia(from, {endrow, endcol}, player, zbite, moves,{0,0},true);

            // Cofnięcie stanu
            board[undo_position.row][undo_position.col]=undo_piec;
            zbite.pop_back();

            do_bicia = true;
        }
    }


    if (!do_bicia && !zbite.empty()) {
        moves.push_back(Move(from, current));
        moves.back().capture = zbite;

    };
}

void Board::drawBoard() {
    cout << "Czarne Piony: " << Pieces_count[0] << " Biale Piony: " << Pieces_count[1] << " Czarne Damy: " <<
            Pieces_count[2] << " Biale Damy: " << Pieces_count[3] <<"draw_counter: "<<draw_counter << endl;
    cout << "   A B C D E F G H\n";
    for (int row = 0; row < 8; ++row) {
        cout << 8 - row << "  ";
        for (int col = 0; col < 8; ++col) {
            const Piece &p = board[row][col];
            char symbol = '.';
            if ((row + col) % 2 == 0) {
                symbol = ' ';
            } else if (p.owner == White) {
                if (p.type == Queen) {
                    symbol = 'W';
                } else {
                    symbol = 'w';
                }
            } else if (p.owner == Black) {
                if (p.type == Queen) {
                    symbol = 'B';
                } else {
                    symbol = 'b';
                }
            }
            cout << symbol << " ";
        }
        cout << " " << 8 - row << "\n";
    }
    cout << "   A B C D E F G H\n";
}

void Board::printMoves(std::vector<Move> &moves) {
    std::cout << "\nMożliwe ruchy:\n";
    for (const Move &move: moves) {
        char fromCol = 'A' + move.from.col;
        char toCol = 'A' + move.to.col;
        int fromRow = 8 - move.from.row;
        int toRow = 8 - move.to.row;
        std::cout << fromCol << fromRow << " -> " << toCol << toRow << " " << move.capture.size();
        std::cout << "\n";
    }
}

int Board::evaulate(Player currentplayer) {
    //cout<<(Pieces_count[1] - Pieces_count[0] + 2 * (Pieces_count[3] - Pieces_count[2]))<<endl;
    int pawn_v=100;
    int queen_v=300;
    int draw_penalty=2;
    if(currentplayer==White){
    return (pawn_v*(Pieces_count[1] - Pieces_count[0]) + 2 *queen_v *(Pieces_count[3] - Pieces_count[2])-(draw_counter*draw_penalty));
    }
    else{
      return (pawn_v*(Pieces_count[1] - Pieces_count[0]) + 2 *queen_v *(Pieces_count[3] - Pieces_count[2])+(draw_counter*draw_penalty));  
    }
}
