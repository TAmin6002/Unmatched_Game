#pragma once

#include "Card.h"
#include "Controller.h"
#include "board.h"

#include <string>

class CardResolver
{
private:
public:
    NeedInput excute(Card *, Player *, Player *, Heroes *, Heroes *, Board *);
};