#ifndef MOVE_HPP
#define MOVE_HPP
#include<vector>

using namespace std;

struct Position
{
    int row;
    int col;

    Position(int r=0,int c=0)
    {
        row=r;
        col=c;
    }
};

struct Move
{
    Position from;
    Position to;
    vector<Position> capture;

    Move(Position f={},Position t={},Position c={})
    {
        from=f;
        to=t;
        if (c.row >= 0 && c.col >= 0 && c.row < 8 && c.col < 8) {
            capture.push_back(c);
        }
    }
};




#endif