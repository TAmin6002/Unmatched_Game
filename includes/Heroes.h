#pragma once

#include <string>
#include <vector>
#include <random>
#include <stdexcept>
#include <functional>

#include "Space.h"
#include "Card.h"
#include "Json.h"

class Board;

class Heroes
{

private:
    std::string name;
    std::string Attacktype;
    int Movement = 2;
    int Action = 2;
    int Health;
    int number;
    bool islive{true};

    Space *place = nullptr;

    bool StartedTurnOnFog = false;

    bool PendingPlacement = false;

protected:
    std::vector<Card> deck;
    std::vector<Card> hand;
    std::vector<Card> discard;

public:
    Heroes(std::string, std::string, int, int, int);

    int get_number();
    std::string get_name();
    std::string get_Attacktype();
    int get_Health();

    void set_Movement(int);
    int get_Movement();

    void add_Action(int);
    int get_Action() const;

    void set_Health(int, int);

    bool get_islive();
    void set_islive(bool);

    Space *get_place();
    void set_place(Space *);

    void set_StartedTurnOnFog(bool);
    bool get_StartedTurnOnFog();    

    void set_PendingPlacement(bool);
    bool get_PendingPlacement();

    std::vector<Card> &get_deck();
    std::vector<Card> &get_hand();
    std::vector<Card> &get_discard();

    int DrawnCard(); 
    int discard_hand(); 
    void DiscardCard(int);
    void Discard_Card(Card* );

    void Damage(int);
    virtual void abiliti(Board *) = 0;


    void set_Action(int);          // absolute setter, unlike add_Action()
    void set_HealthValue(int);     // absolute setter, unlike set_Health(amount, maxHealth)

    Json toJson() const;
    
    // <const std::function<Heroes *(const std::string &)> &> 
    //This parameter is a callback function that takes a string identifier as input and returns a pointer to a Heroes object, used to find and link heroes during loading.
    void loadFromJson(const Json &, Board &, const std::function<Heroes *(const std::string &)> &);
};