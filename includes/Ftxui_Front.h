#pragma once

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include "Player.h"
#include "Enums.h"
#include "Board.h"

using namespace ftxui;

class Ftxui_Front
{
private:
public:
    enum ::e_Menu Menu_();

    void Players_Info_List(Player *, Player *);

    int Det_characters(Player *, Player *);

    void main_map(Player *, Player *, Board *board);

    void chose_comrad_place(Player *, Player *, Board *);
};