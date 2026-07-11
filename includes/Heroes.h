#pragma once

#include <string>
#include <vector>
#include <random>

#include "Space.h"
#include "Card.h"

class Heroes
{

private:
    std::string name;
    std::string Attacktype;
    int Movement;
    int Action = 2;
    int Health;
    bool islive{true};

    Space *place = nullptr;

protected:
    std::vector<Card> deck;
    std::vector<Card> hand;
    std::vector<Card> discard;

public:
    Heroes(std::string, std::string, int, int);

    std::string get_name();
    std::string get_Attacktype();
    int get_Movement();
    int get_Health();

    void add_Action(int);
    int get_Action() const;

    void set_Health(int, int);

    bool get_islive();

    Space *get_place();
    void set_place(Space *);

    std::vector<Card> get_deck();
    std::vector<Card> get_hand();
    std::vector<Card> get_discard();

    void DrawnCard(); // یک کارت میکشد
    int discard_hand();

    void Damage(int);
    virtual void abiliti() = 0;
};