#pragma once

#include "Card.h"
#include "board.h"

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
    void excute(Card *, Player *, Player *, Heroes *, Heroes *, Board *, int &, int &);
};