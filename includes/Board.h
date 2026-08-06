#pragma once

#include "Space.h"
#include "Heroes.h"

class Board
{
private:
    std ::vector<Space> spaces;
    // Heroes *hero;

public:
    Board();

    std::vector<Space> &get_spaces();
    bool is_Adjacent(Space *, Space *);
    void SwapHeroes(Heroes *, Heroes *);

 
    void ConnectPortal(Space *, Space *);
};
