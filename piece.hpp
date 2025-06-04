#ifndef PIECE_HPP
#define PIECE_HPP
#include <string>

using namespace std;


//TYPY POMOCNICZNE DO USTALENIE WARTOSCI PIONKA
enum PieceType {None,Pawn,Queen};
enum Player {Noone,White,Black};

struct Piece
{
    Player owner;
    PieceType type;

    Piece()
    {
        type=None;
        owner=Noone;
    }

    Piece(Player o,PieceType t)
    {
        type=t;
        owner=o;
    }
};




#endif