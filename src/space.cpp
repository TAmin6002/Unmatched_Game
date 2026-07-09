#include "space.h"
using namespace std;

Space ::Space(int number, vector<Space *> z, vector<Space *> v) : number(number), zone(z), neighbor(v) {}

vector<Space *> Space::get_zone()
{
    return zone;
}

std ::vector<Space *> Space::get_neighbor()
{
    return neighbor;
}

Heroes *Space::get_hero()
{
    return hero;
}

void Space::set_hero(Heroes *hero)
{
    this->hero = hero;
}

int Space::get_number()
{
    return number;
}