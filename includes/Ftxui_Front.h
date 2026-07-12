#pragma once
#include <stdexcept>

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
    int number_of_choose = -1;

    std::vector<std::string> msg;

public:
    int get_number_of_choose();
    void set_number_of_choose(int);

    enum ::e_Menu Menu_();

    void Players_Info_List(Player *, Player *);

    int Det_characters(Player *, Player *);

    void catch_place(Player *, Player *, Board *);

    Component ChooseAction(Player *, Player *, ScreenInteractive *);

    void main_map(Player *, Player *, Board *, Player *);

    void chose_comrad_place(Player *, Player *, Board *);

    void Attakcer_Heroes_Menu(Player *, Board *, Heroes *&);
    void Defender_Heroes_Menu(Player *, Board *, Heroes *&, Heroes *&);

    std::vector<std::string> &get_msg();
};