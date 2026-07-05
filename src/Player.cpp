#include "Player.h"
using namespace std;

void Player::set_age(int amount)
{
    age = amount;
}

int Player::get_age()
{
    return age;
}

void Player::set_name(std::string name)
{
    this->name = name;
}

std::string Player::get_name()
{
    return name;
}

void Player::set_character(Heroes *character)
{
    this->character = character;
}

Heroes *Player::get_character()
{
    return character;
}

void Player::set_comrade(Heroes *c)
{
    comrade.push_back(c);
}

vector<Heroes *> Player::get_comrade()
{
    return comrade;
}

void Player::set_isAttacker(bool b)
{
    is_Attacker = b;
}

bool Player::get_isAttacker()
{
    return is_Attacker;
}

Card Player::get_selected_card()
{
    return selected_card;
}

void Player::set_selected_card(Card card)
{
    selected_card = card;
}