#ifndef BOARD_H
#define BOARD_H

#include "space.h"
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

    // Creates a bidirectional, movement-only portal between two spaces.
    // Does not touch `neighbor`, so it never affects is_Adjacent()/attacks.
    void ConnectPortal(Space *, Space *);
};

#endif