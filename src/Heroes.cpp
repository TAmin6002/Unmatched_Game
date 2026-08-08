
#include "Heroes.h"
#include "Ftxui_Front.h"

using namespace std;

Heroes::Heroes(std::string name, std::string Attacktype, int Health, int Movement, int number)
    : name(name), Attacktype(Attacktype), Health(Health), Movement(Movement), number(number) {}

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

void Heroes::set_Movement(int amount)
{
    Movement = amount;
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

void Heroes::set_islive(bool b)
{
    islive = b;
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

void Heroes::set_StartedTurnOnFog(bool b) {
     StartedTurnOnFog = b; 
}

bool Heroes::get_StartedTurnOnFog() { 
    return StartedTurnOnFog; 
}

vector<Card> &Heroes::get_deck()
{
    return deck;
}

vector<Card> &Heroes::get_hand()
{
    return hand;
}

vector<Card> &Heroes::get_discard()
{
    return discard;
}

int Heroes::DrawnCard()
{
    if (deck.empty())
        return 0;

    hand.push_back(deck.back());
    deck.pop_back();

    return 1;
}

void Heroes::Damage(int amount)
{
    if (Health - amount <= 0)
    {
        islive = false;
        Health = 0;

        if (place != nullptr){
            place->set_hero(nullptr);
            place = nullptr;
        } 
            
    }

    else
        Health -= amount;
}

int Heroes::discard_hand()
{

    if (hand.empty())
        return 0;

    int Boost;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, hand.size() - 1);
    int randomIndex = dist(rng);

    Boost = hand[randomIndex].get_Boost();

    discard.push_back(hand[randomIndex]);

    hand.erase(hand.begin() + randomIndex);

    return Boost;
}

void Heroes::DiscardCard(int selected)
{
    this->discard.push_back(hand[selected]);
    this->hand.erase(hand.begin() + selected);
}

void Heroes::Discard_Card(Card *card)
{
    auto it = std::find_if(hand.begin(), hand.end(),
                           [&](const Card &c)
                           {
                               return &c == card;
                           });

    if (it == hand.end())
    {
        Ftxui_Front FF;
        throw std::runtime_error("Card not found.");
        FF.get_msg().push_back("Card not found.");
    }

    discard.push_back(*it);

    hand.erase(it);
}

int Heroes::get_number()
{
    return number;
}
