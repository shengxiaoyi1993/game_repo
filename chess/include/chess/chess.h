#ifndef chess_h
#define chess_h

/// player
/// board
/// PlayerSelectRule
/// WinnerRule

#include <iostream>
#include <vector>


class Node
{
public:
    enum State{
        none=0,
        write,
        black
    };
    int setState(State v_state){__state=v_state;}

public:
    Node():__state(none){}

private:
    State __state;

};

class Board
{
public:
    Board(int v_width,int v_height):__width(v_width),__height(v_height){}

private:
    std::vector<std::vector<Node>> __board;
    int __width;
    int __height;

};



#endif
