#pragma once

#include "Card.h"
#include "Dracula.h"
#include "SherlockHolmes.h"
#include "Player.h"
#include "Sisters.h"
#include "Dr_Watson.h"
#include "board.h"
#include "Ftxui_Front.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

#include <string>

class Controller
{
private:
    Board board;

    Player p1;
    Player p2;

    Dracula dracula;
    Sisters s1, s2, s3;

    SherlockHolmes sherlock;
    Dr_Watson Watson;

    Heroes *Attacker;
    Heroes *Defender; // این دوتا هنگام ی ست میشوند که بازیکن از بین گزینه های اکشن اتک زدن را اتخاب کرده باشد

    Card Attacker_selected_card;
    Card Defender_selected_card; // وقتی دو مبارز وارد جنگ میشوند ابتدا از اتکر و سپس از دفاع کننده کارت های انتخابی شان گرفته میشود

    int DraculaAction = 2;
    int SherlockAction = 2;

    Ftxui_Front FF;

public:
    void run();

    Dracula *get_Dracula();
    SherlockHolmes *get_SherlockHolmes();

    int get_DraculaAction() const;
    void set_DraculaAction();

    int get_SherlockAction() const;
    void set_SherlockAction();

    Card get_Attacker_selected_card();
    Card get_Defender_selected_card();

    void set_players_character(int);

    void Initial_characters_places();

    int dis_sumcards(); // Removes as many cards as the player wants
};