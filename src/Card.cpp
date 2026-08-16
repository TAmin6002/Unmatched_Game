#include "Card.h"
#include "Heroes.h"


using namespace std;

Card::Card(CardType name, CardTiming timing, std::string typeAttack, int amount, int Boost, string owner)
    : name(name), timing(timing), typeAttack(typeAttack), amount(amount), Boost(Boost), owner(owner) {}

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

std::string Card::get_owner()
{
    return owner;
}

std::string Card::get_Attacktype()
{
    return typeAttack;
}

void Card::set_user_card(Heroes *hero)
{
    user = hero;
}

Heroes *Card::get_user_card()
{
    return user;
}


// This method serializes a Card object's current state user e.t.c into a standard JSON structure for saving or transmission.
Json Card::toJson() const
{
    Json j = Json::makeObject();

    j.set("type", Json(static_cast<int>(name)));
    j.set("timing", Json(static_cast<int>(timing)));
    j.set("typeAttack", Json(typeAttack));
    j.set("amount", Json(amount));
    j.set("boost", Json(Boost));
    j.set("owner", Json(owner));
    j.set("applyEffects", Json(ApplyEffects));

    if (user != nullptr)
    {
        j.set("userName", Json(user->get_name()));
        j.set("userNumber", Json(user->get_number()));
    }
    else
    {
        j.set("userName", Json(std::string("")));
        j.set("userNumber", Json(-1));
    }

    return j;
}

//  This function reads the card information from the json file and creates and returns the card object.
Card Card::fromJson(const Json &j, const std::function<Heroes *(const std::string &)> &resolveHero)
{
    Card c(
        static_cast<CardType>(j["type"].asInt()),
        static_cast<CardTiming>(j["timing"].asInt()),
        j["typeAttack"].asString(),
        j["amount"].asInt(),
        j["boost"].asInt(),
        j["owner"].asString());

    c.set_ApplyEffects(j["applyEffects"].asBool(true));

    std::string userName = j["userName"].asString("");
    int userNumber = j["userNumber"].asInt(-1);

    if (!userName.empty() && resolveHero)
        c.set_user_card(resolveHero(userName + "#" + std::to_string(userNumber)));

    return c;
}