#ifndef BOARD_H
#define BOARD_H

#include "space.h"
#include "Heroes.h"

class Board
{
private:
    std ::vector<Space> spaces;
    Heroes *hero;

public:
    Board();

    bool is_Adjacent(Space *, Space *);    
};

#endif