#include "Card.h"

Card::Card(CardType name, CardTiming timing, std::string typeAttack, int amount, int Boost)
    : name(name), timing(timing), typeAttack(typeAttack), amount(amount), Boost(Boost) {}

CardType Card::get_CardType() const
{
    return this->name;
}
CardTiming Card::get_CardTiming() const
{
    return this->timing;
}

int Card::get_Boost() const
{
    return Boost;
}

void Card::set_Boost(int amount)
{
    this->Boost += amount;
}

int Card::get_amount() const
{
    return amount;
}

void Card::set_amount(int a)
{
    amount = a;
}

void Card::add_amount(int a)
{
    amount += a;
}

void Card::set_ApplyEffects(bool b)
{
    ApplyEffects = b;
}

bool Card::get_ApplyEffects()
{
    return ApplyEffects;
}
