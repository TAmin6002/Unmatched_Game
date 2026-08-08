#pragma once

#include "Card.h"
#include "Dracula.h"
#include "SherlockHolmes.h"
#include "Player.h"
#include "Sisters.h"
#include "Dr_Watson.h"
#include "Board.h"
#include "CardResolver.h"
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
    Sisters s1{1}, s2{2}, s3{3};

    SherlockHolmes sherlock;
    Dr_Watson Watson;

    Heroes *Attacker = nullptr;
    Heroes *Defender = nullptr; // این دوتا هنگام ی ست میشوند که بازیکن از بین گزینه های اکشن اتک زدن را اتخاب کرده باشد

    Card *Attacker_selected_card = nullptr;
    Card *Defender_selected_card = nullptr; // وقتی دو مبارز وارد جنگ میشوند ابتدا از اتکر و سپس از دفاع کننده کارت های انتخابی شان گرفته میشود

    int Attack_Value = 0;
    int Defense_Value = 0;

    bool Attack_Locked = false;
    bool Defense_Locked = false;

    CardResolver card_resolver;

    // int DraculaAction = 0;
    // int SherlockAction = 0;

    Player *turn = nullptr;
    Player *not_turn = nullptr;

    int round = 1;

    Ftxui_Front FF;

public:
    void run();

    Dracula *get_Dracula();
    SherlockHolmes *get_SherlockHolmes();

    int get_DraculaAction() const;
    void set_DraculaAction();

    int get_SherlockAction() const;
    void set_SherlockAction();

    Card *get_Attacker_selected_card();
    Card *get_Defender_selected_card();

    void set_players_character(int);

    void Initial_characters_places();
    Player *get_younger_player();
    Player *get_older_player();

    // bool can_Attack(Player *);

    void Initial_turn();
    void chane_turn();

    int dis_sumcards(); // Removes as many cards as the player wants
};