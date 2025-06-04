#include "board.hpp"
#include <iostream>

using namespace std;

Board::~Board()=default;

Board::Board(const Board& b) {
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

Board::Board()
{
    for(int row=0;row<8;row++)
    {
        for(int col=0;col<8;col++)
        {
            if((row+col)%2==1)
            {
                if(row<3)
                {
                    board[row][col]=Piece(Black,Pawn);
                    Pieces_count[0]+=1;
                }
                else if(row>4)
                {
                    board[row][col]=Piece(White,Pawn);
                    Pieces_count[1]+=1;
                }
                else
                {
                    board[row][col]=Piece();
                }
            }
            else
            {
                board[row][col]=Piece();
            }
        }
    }
}

bool Board::isInside(int row,int col)
{
    if(row>=0&&row<8&&col>=0&&col<8)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Board::makeMove(Move& move)
{
    //Ruch piona
    Piece& moving = board[move.from.row][move.from.col];
    board[move.to.row][move.to.col] = moving;
    board[move.from.row][move.from.col] = Piece();
    if(board[move.to.row][move.to.col].owner==White && move.to.row==0)
    {
        board[move.to.row][move.to.col].type=Queen;
        Pieces_count[3]+=1;
    }
    else if (board[move.to.row][move.to.col].owner==Black && move.to.row==7)
    {
        board[move.to.row][move.to.col].type=Queen;
        Pieces_count[2]+=1;
    }
    
    // Zbijanie (jeśli są pozycje do zbicia)
    for (Position pos : move.capture) {
        if(board[pos.row][pos.col].owner==Black && board[pos.row][pos.col].type==Pawn) {
            Pieces_count[0]-=1;
        }
        else if (board[pos.row][pos.col].owner==White && board[pos.row][pos.col].type==Pawn) {
            Pieces_count[1]-=1;
        }
        else if (board[pos.row][pos.col].owner==Black && board[pos.row][pos.col].type==Queen) {
            Pieces_count[0]-=1;
            Pieces_count[2]-=1;
        }
        else if (board[pos.row][pos.col].owner==White && board[pos.row][pos.col].type==Queen) {
            Pieces_count[1]-=1;
            Pieces_count[3]-=1;
        }
        board[pos.row][pos.col] = Piece();


    }

}

bool Board::isGameOver() {
    if(!genMoves(White).empty() || !genMoves(Black).empty())
    {
        return true;
    }
    else if (genMoves(White).empty() && genMoves(Black).empty())
    {
        cout<<"TIE";
        return false;
    }
    else if (genMoves(White).empty())
    {
        cout<<"BLACK WINS";
        return false;
    }
    else if (genMoves(Black).empty())
    {
        cout<<"WHITE WINS";
        return false;
    }
}
std::vector<Move> Board::genMoves(Player player) {
    std::vector<Move> moves;
    int direction;
    bool bicie=false;

    if(player==White)
    {
        direction=-1;
    }
    else
    {
        direction=1;
    }

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece p = board[row][col];
            if (p.owner != player)
                continue;
            std::vector<Position> zbite;
            std::vector<Move> bicia;
            if(p.type==Pawn){
            std::vector<std::vector<bool>> visited(8, std::vector<bool>(8, false));
            
            znajdz_bicia({row, col}, {row, col}, player, zbite, bicia, visited);
            if (!bicia.empty()) {
                bicie = true;
                moves.insert(moves.end(), bicia.begin(), bicia.end());
            }
            }
            else if(p.type==Queen)
            {
                std::vector<Move> Queen_move;
                for (int dx = -1; dx <= 1; dx += 2) {
                    int newRow = row + direction;
                    int newCol = col + dx;
                    while (isInside(newRow, newCol)) {
                        if (board[newRow][newCol].owner == Noone) {
                            Queen_move.push_back(Move({ row, col }, { newRow, newCol},{-1,-1}));
                        }
                        else{
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
                            Queen_move.push_back(Move({ row, col }, { newRow, newCol},{-1,-1}));
                        }
                        else{
                            break;
                        }
                        newRow -= direction;
                        newCol += dx;                
                    }
                }
                if(!Queen_move.empty()){
                for(auto& move : Queen_move){
                        std::vector<std::vector<bool>> visited(8, std::vector<bool>(8, false));

                        znajdz_bicia({move.from.row, move.from.col}, {move.to.row, move.to.col}, player, zbite, bicia, visited);
                    
                } 
                }
                else{
                    std::vector<std::vector<bool>> visited(8, std::vector<bool>(8, false));
                    znajdz_bicia({row, col}, {row, col}, player, zbite, bicia, visited);
                }
                }
                if (!bicia.empty()) {
                        bicie = true;
                        moves.insert(moves.end(), bicia.begin(), bicia.end());
                }
            
        
            
            
            
        }
    }


    if (bicie) {
        return moves;
    }

    vector<Move> normlaMoves;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece p = board[row][col];
            if (p.owner != player)
                continue;

            // Ruchy do przodu (lewo/prawo)
            if (p.type==Pawn) {
                for (int dx = -1; dx <= 1; dx += 2) {
                    int newRow = row + direction;
                    int newCol = col + dx;

                    //Zwykle ruch pionkow
                    if (isInside(newRow, newCol) && board[newRow][newCol].owner == Noone) {
                        normlaMoves.push_back(Move({ row, col }, { newRow, newCol},{-1,-1}));
                    }
                }
            }
            else {
                for (int dx = -1; dx <= 1; dx += 2) {
                    int newRow = row + direction;
                    int newCol = col + dx;
                    while (isInside(newRow, newCol)) {
                        if (board[newRow][newCol].owner == Noone) {
                            normlaMoves.push_back(Move({ row, col }, { newRow, newCol},{-1,-1}));
                        }
                        else{
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
                            normlaMoves.push_back(Move({ row, col }, { newRow, newCol},{-1,-1}));
                        }
                        else{
                            break;
                        }
                        newRow -= direction;
                        newCol += dx;
                        
                    }
                }
            }
        }
    }

    return normlaMoves;
}
void Board::znajdz_bicia(Position from, Position current, Player player,
                               std::vector<Position> zbite, std::vector<Move>& moves,
                               std::vector<std::vector<bool>>& visited) {
    bool do_bicia = false;
    int direction[4][2] = {{-1,-1}, {-1,1}, {1,-1}, {1,1}};


    for (auto& [dr, dc] : direction) {
            int midR = current.row + dr;
            int midC = current.col + dc;
            int landR = current.row + 2 * dr;
            int landC = current.col + 2 * dc;

            if (!isInside(landR,landC) || !isInside(midR,midC))
                continue;

            Piece enemy = board[midR][midC];
            Piece target = board[landR][landC];

            if (enemy.owner != Noone && enemy.owner != player &&
                target.owner == Noone && !visited[midR][midC]) {

                // Symulacja ruchu
                visited[midR][midC] = true;
                zbite.push_back({midR, midC});

                // Rekurencja – sprawdzanie dalszych bić
                znajdz_bicia(from, {landR, landC}, player, zbite, moves, visited);

                // Cofnięcie stanu
                visited[midR][midC] = false;
                zbite.pop_back();

                do_bicia = true;
                }
        }
    
    
            
        
    
    

    if (!do_bicia && !zbite.empty()) {
        moves.push_back(Move(from, current));
        moves.back().capture = zbite;
    }
}
void Board::drawBoard()  {
    cout << "Czarne Piony: "<<Pieces_count[0]<<" Biale Piony: "<<Pieces_count[1]<<" Czarne Damy: "<<Pieces_count[2]<<" Biale Damy: "<<Pieces_count[3] << endl;
    cout << "   A B C D E F G H\n";
    for (int row = 0; row < 8; ++row) {
        cout << 8 - row << "  ";
        for (int col = 0; col < 8; ++col) {
            const Piece& p = board[row][col];
            char symbol = '.';
            if ((row + col) % 2 == 0) {
                symbol = ' ';
            } else if (p.owner == White) {
                if(p.type==Queen)
                {
                    symbol =  'W';
                }
                else
                {
                    symbol =  'w';
                }
                
            } else if (p.owner == Black) {
                if(p.type==Queen)
                {
                    symbol =  'B';
                }
                else
                {
                    symbol =  'b';
                }
            }
            cout << symbol << " ";
        }
        cout << " " << 8 - row << "\n";
    }
    cout << "   A B C D E F G H\n";
}

void Board::printMoves( std::vector<Move>& moves)  {
    std::cout << "\nMożliwe ruchy:\n";
    for (const Move& move : moves) {
        char fromCol = 'A' + move.from.col;
        char toCol = 'A' + move.to.col;
        int fromRow = 8 - move.from.row;
        int toRow = 8 - move.to.row;
        std::cout << fromCol << fromRow << " -> " << toCol << toRow<<" "<<move.capture.size();
        std::cout << "\n";
    }
}
int Board::evaulate(Player currentplayer) {
    if (currentplayer==Black) {
        return (Pieces_count[0]-Pieces_count[1]+2*(Pieces_count[2]-Pieces_count[3]));
    }
    else {
        return (Pieces_count[1]-Pieces_count[2]+2*(Pieces_count[3]-Pieces_count[2]));
    }
}
