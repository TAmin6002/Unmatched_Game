
#include "Heroes.h"
using namespace std;

Heroes::Heroes(std::string name, std::string Attacktype, int Health, int Movement)
    : name(name), Attacktype(Attacktype), Health(Health), Movement(Movement) {}

string Heroes::get_name()
{
    return name;
    ;
}
string Heroes::get_Attacktype()
{
    return Attacktype;
}
int Heroes::get_Movement()
{
    return Movement;
}

void Heroes::add_Action(int amount)
{
    Action += amount;
}

int Heroes::get_Action() const
{
    return Action;
}

void Heroes::set_Health(int amount, int maxHealth)
{
    if (Health + amount > maxHealth)
        Health = maxHealth;
    else
        this->Health += amount;
}

bool Heroes::get_islive()
{
    return islive;
}

Space *Heroes::get_place()
{
    return place;
}
int Heroes::get_Health()
{
    return Health;
}

void Heroes::set_place(Space *p)
{
    place = p;
}

vector<Card>& Heroes::get_deck()
{
    return deck;
}

vector<Card>& Heroes::get_hand()
{
    return hand;
}

vector<Card>& Heroes::get_discard()
{
    return discard;
}

void Heroes::DrawnCard()
{
    if (deck.empty())
        return;

    hand.push_back(deck.back());
    deck.pop_back();
}

void Heroes::Damage(int amount)
{
    if (Health - amount <= 0)
        islive = false;

    else
        Health -= amount;
}

int Heroes::discard_hand()
{
    // باید چک شود که از بین پنج کارت توی دست هیچکدام اشتباه یا نال یا استفاده شده نباشند ...!!!!!!!!!!!!!!!!!!!!!!!!!!!

    int Boost;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 4); // 0 - 4
    int randomIndex = dist(rng);

    Boost = hand[randomIndex].get_Boost();

    discard.push_back(hand[randomIndex]);

    hand.erase(hand.begin() + randomIndex);

    return Boost;
}