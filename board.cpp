#include "board.hpp"
#include <iostream>

Board::~Board()=default;

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
                }
                else if(row>4)
                {
                    board[row][col]=Piece(White,Pawn);
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

bool Board::makeMove(Move& move)
{
    //Ruch piona
    Piece& moving = board[move.from.row][move.from.col];
    board[move.to.row][move.to.col] = moving;
    board[move.from.row][move.from.col] = Piece();
    if(board[move.to.row][move.to.col].owner==White && move.to.col==7)
    {
        board[move.to.row][move.to.col].type=Queen;
    }
    else if (board[move.to.row][move.to.col].owner==Black && move.to.col==0)
    {
        board[move.to.row][move.to.col].type=Queen;
    }
    
    // Zbijanie (jeśli są pozycje do zbicia)
    for (Position pos : move.capture) {
        board[pos.row][pos.col] = Piece();
        return true;
    }
    return false;
}

bool Board::isGameOver() {
    if(!genMoves(White).empty() || !genMoves(Black).empty())
    {
        return true;
    }
    else if (genMoves(White).empty())
    {
        cout<<"BLACK WINS";
        return false;
    }
    else
    {
        cout<<"WHITE WINS";
        return false;
    }
}
std::vector<Move> Board::genMoves(Player player) {
    std::vector<Move> moves;
    int direction;
    bool bicie=false;
    bool after_bicie=false;


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

            // Ruchy do przodu (lewo/prawo)
            for (int dx = -1; dx <= 1; dx += 2) {
                int newRow = row + direction;
                int newCol = col + dx;

                //Zwykle ruch pionkow
                if (isInside(newRow, newCol) && board[newRow][newCol].owner == Noone&& !bicie) {
                    
                    moves.push_back(Move({ row, col }, { newRow, newCol},{-1,-1}));
                }

                //Sprawdzenia bicia
                else if((board[newRow][newCol].owner != Noone&&board[newRow][newCol].owner != player )&& isInside(newRow + direction, newCol+dx) && board[newRow + direction][newCol+dx].owner == Noone)
                {   
                    if(!after_bicie)
                    {
                        moves.clear();
                        after_bicie=true;
                    }
                    if(!bicie)
                    {
                        bicie=true;
                    }
                    moves.push_back(Move({ row, col }, { newRow+direction, newCol+dx},{newRow,newCol} ));
                }
                
            }
        }
    }

    return moves;
}

void Board::drawBoard()  {
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