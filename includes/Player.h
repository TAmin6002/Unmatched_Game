#pragma once

#include "Heroes.h"

class Player
{

private:
    int age = 0;
    std::string name;
    int count = 0; // count Action

    Heroes *character;
    std::vector<Heroes *> comrade;

    bool is_Attacker{};

    Card *selected_card = nullptr; // The card chosen by the player

public:
    void set_age(int);
    int get_age();

    void set_name(std::string);
    std::string get_name();

    int get_count();
    void add_count();

    void set_character(Heroes *);
    Heroes *get_character();

    void set_comrade(Heroes *);
    std::vector<Heroes *> get_comrade();

    void set_isAttacker(bool);
    bool get_isAttacker();

    Card *get_selected_card();
    void set_selected_card(Card *);

    void set_count(int);
};