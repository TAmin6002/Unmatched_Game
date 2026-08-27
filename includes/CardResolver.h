#pragma once

#include "Card.h"
#include "Board.h"
#include "Raylib.h"

#include <string>


class CardResolver
{
private:
    
    Raylib RF;

public:
    void TryDisableCard(Card *, Player *, Player *); 
    
    void excute(Card *, Player *, Player *, Heroes *, Heroes *, Board *, int &, int &, bool &, bool &);
};