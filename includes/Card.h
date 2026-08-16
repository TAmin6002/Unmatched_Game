#pragma once

#include <string>
#include <functional>
#include "Enums.h"
#include "Json.h"

class Heroes;

class Card
{

private:
    CardType name;
    CardTiming timing;      // "DURING" , "AFTER" , "BEFORE", "IMMEDIATE"
    std::string typeAttack; // "ATTACK" , "DEFENSE" , "EVENT", "BOTH"

    int amount = 0;
    int Boost = 0;

    std::string owner;
    Heroes *user = nullptr;

    bool ApplyEffects{true};

public:
    Card() = default;
    Card(CardType, CardTiming, std::string, int, int, std::string);

    CardType get_CardType() const;
    CardTiming get_CardTiming() const;

    int get_Boost() const;
    void set_Boost(int);

    int get_amount() const;
    void set_amount(int);
    void add_amount(int);

    bool get_ApplyEffects();
    void set_ApplyEffects(bool);

    std::string get_owner();
    std::string get_Attacktype();

    void set_user_card(Heroes *);
    Heroes *get_user_card();

    Json toJson() const;
    static Card fromJson(const Json &, const std::function<Heroes *(const std::string &)> &);
};