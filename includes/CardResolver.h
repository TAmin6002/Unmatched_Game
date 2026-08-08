#pragma once

#include "Card.h"
#include "Board.h"

#include <string>

#include "Ftxui_Front.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

class CardResolver
{
private:
    Ftxui_Front FF;

public:
    void TryDisableCard(Card *, Player *, Player *); 
    
    void excute(Card *, Player *, Player *, Heroes *, Heroes *, Board *, int &, int &, bool &, bool &);
};