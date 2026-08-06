#include "Board.h"
#include "Space.h"

#include <iostream>

using namespace std;

Board ::Board()
{
    spaces.resize(32);

    for (int i = 0; i < 32; i++)
        spaces[i] = Space();

    spaces[0] = Space(1, {&spaces[2], &spaces[1], &spaces[3], &spaces[4], &spaces[5]}, {&spaces[1], &spaces[5]});

    spaces[1] = Space(2, {&spaces[0], &spaces[2], &spaces[4], &spaces[5], &spaces[3]}, {&spaces[0], &spaces[2]});

    spaces[2] = Space(3, {&spaces[0], &spaces[1], &spaces[3], &spaces[4], &spaces[5], &spaces[31], &spaces[30], &spaces[29], &spaces[28]}, {&spaces[1], &spaces[3], &spaces[31]});

    spaces[3] = Space(4, {&spaces[0], &spaces[1], &spaces[2], &spaces[4], &spaces[5], &spaces[6], &spaces[7], &spaces[8], &spaces[9]}, {&spaces[2], &spaces[4], &spaces[6]});

    spaces[4] = Space(5, {&spaces[0], &spaces[1], &spaces[2], &spaces[3], &spaces[5]}, {&spaces[3], &spaces[5]});

    spaces[5] = Space(6, {&spaces[0], &spaces[1], &spaces[2], &spaces[3], &spaces[4]}, {&spaces[0], &spaces[4], &spaces[7]});

    spaces[6] = Space(7, {&spaces[3], &spaces[7], &spaces[8], &spaces[9]}, {&spaces[3], &spaces[7]});

    spaces[7] = Space(8, {&spaces[3], &spaces[6], &spaces[8], &spaces[9]}, {&spaces[5], &spaces[6], &spaces[8]});

    spaces[8] = Space(9, {&spaces[3], &spaces[6], &spaces[7], &spaces[9], &spaces[14], &spaces[15], &spaces[16], &spaces[13], &spaces[17], &spaces[18], &spaces[19], &spaces[20], &spaces[21]}, {&spaces[7], &spaces[9], &spaces[14]});

    spaces[9] = Space(10, {&spaces[3], &spaces[6], &spaces[7], &spaces[8], &spaces[10], &spaces[11], &spaces[12], &spaces[13], &spaces[28]}, {&spaces[8], &spaces[10]});

    spaces[10] = Space(11, {&spaces[9], &spaces[11], &spaces[12], &spaces[13], &spaces[28]}, {&spaces[9], &spaces[11], &spaces[13]});

    spaces[11] = Space(12, {&spaces[9], &spaces[10], &spaces[12], &spaces[13], &spaces[28]}, {&spaces[10], &spaces[12]});

    spaces[12] = Space(13, {&spaces[9], &spaces[10], &spaces[11], &spaces[13], &spaces[28], &spaces[21], &spaces[22], &spaces[23], &spaces[24]}, {&spaces[11], &spaces[28], &spaces[24], &spaces[23], &spaces[18], &spaces[19]});

    spaces[13] = Space(14, {&spaces[9], &spaces[10], &spaces[11], &spaces[12], &spaces[28], &spaces[8], &spaces[14], &spaces[15], &spaces[16], &spaces[17], &spaces[18], &spaces[19], &spaces[20], &spaces[21]}, {&spaces[10], &spaces[16], &spaces[17], &spaces[18]});

    spaces[14] = Space(15, {&spaces[8], &spaces[15], &spaces[16], &spaces[13], &spaces[17], &spaces[18], &spaces[19], &spaces[20], &spaces[21]}, {&spaces[8], &spaces[15]});

    spaces[15] = Space(16, {&spaces[8], &spaces[14], &spaces[16], &spaces[13], &spaces[17], &spaces[18], &spaces[19], &spaces[20], &spaces[21]}, {&spaces[14], &spaces[16]});

    spaces[16] = Space(17, {&spaces[8], &spaces[14], &spaces[15], &spaces[13], &spaces[17], &spaces[18], &spaces[19], &spaces[20], &spaces[21]}, {&spaces[15], &spaces[13], &spaces[17]});

    spaces[17] = Space(18, {&spaces[8], &spaces[14], &spaces[15], &spaces[16], &spaces[13], &spaces[18], &spaces[19], &spaces[20], &spaces[21]}, {&spaces[16], &spaces[13], &spaces[18]});

    spaces[18] = Space(19, {&spaces[8], &spaces[14], &spaces[15], &spaces[16], &spaces[13], &spaces[17], &spaces[19], &spaces[20], &spaces[21]}, {&spaces[13], &spaces[17], &spaces[12], &spaces[19]});

    spaces[19] = Space(20, {&spaces[8], &spaces[14], &spaces[15], &spaces[16], &spaces[13], &spaces[17], &spaces[18], &spaces[20], &spaces[21]}, {&spaces[18], &spaces[12], &spaces[20], &spaces[21]});

    spaces[20] = Space(21, {&spaces[8], &spaces[14], &spaces[15], &spaces[16], &spaces[13], &spaces[17], &spaces[18], &spaces[19], &spaces[21]}, {&spaces[19], &spaces[21]});

    spaces[21] = Space(22, {&spaces[8], &spaces[14], &spaces[15], &spaces[16], &spaces[13], &spaces[17], &spaces[18], &spaces[19], &spaces[20], &spaces[12], &spaces[23], &spaces[22], &spaces[24]}, {&spaces[19], &spaces[20], &spaces[22]});

    spaces[22] = Space(23, {&spaces[12], &spaces[23], &spaces[24], &spaces[21]}, {&spaces[21], &spaces[23], &spaces[24]});

    spaces[23] = Space(24, {&spaces[12], &spaces[24], &spaces[22], &spaces[21]}, {&spaces[12], &spaces[22]});

    spaces[24] = Space(25, {&spaces[12], &spaces[23], &spaces[22], &spaces[21]}, {&spaces[12], &spaces[22]});

    spaces[25] = Space(26, {&spaces[26], &spaces[27], &spaces[28]}, {&spaces[24], &spaces[27]});

    spaces[26] = Space(27, {&spaces[25], &spaces[27], &spaces[28]}, {&spaces[27]});

    spaces[27] = Space(28, {&spaces[25], &spaces[26], &spaces[28]}, {&spaces[25], &spaces[26], &spaces[28]});

    spaces[28] = Space(29, {&spaces[25], &spaces[26], &spaces[27], &spaces[29], &spaces[30], &spaces[31], &spaces[2], &spaces[12], &spaces[11], &spaces[10], &spaces[9], &spaces[13]}, {&spaces[25], &spaces[27], &spaces[29], &spaces[30], &spaces[12]});

    spaces[29] = Space(30, {&spaces[2], &spaces[31], &spaces[30], &spaces[28]}, {&spaces[31], &spaces[30], &spaces[28]});

    spaces[30] = Space(31, {&spaces[2], &spaces[31], &spaces[29], &spaces[28]}, {&spaces[31], &spaces[29], &spaces[28]});

    spaces[31] = Space(32, {&spaces[2], &spaces[30], &spaces[29], &spaces[28]}, {&spaces[2], &spaces[30], &spaces[29]});


    ConnectPortal(&spaces[0], &spaces[11]);  // House 1  <-> House 12
    ConnectPortal(&spaces[0], &spaces[14]);  // House 1  <-> House 15
    ConnectPortal(&spaces[0], &spaces[26]);  // House 1  <-> House 27
    ConnectPortal(&spaces[11], &spaces[14]); // House 12 <-> House 15
    ConnectPortal(&spaces[11], &spaces[26]); // House 12 <-> House 27
    ConnectPortal(&spaces[14], &spaces[26]); // House 15 <-> House 27
}

bool Board::is_Adjacent(Space *s1, Space *s2)
{
    for (auto const &p : s1->get_neighbor())
    {
        if (p == s2)
            return true;
    }
    return false;
}

std::vector<Space> &Board::get_spaces()
{
    return spaces;
}

void Board::ConnectPortal(Space *s1, Space *s2)
{
    if (s1 == nullptr || s2 == nullptr)
        throw std::runtime_error("Cannot connect a portal to a null space.");

  
    s1->add_portal(s2);
    s2->add_portal(s1);
}

void Board::SwapHeroes(Heroes *hero1, Heroes *hero2)
{
    if (hero1 == nullptr || hero2 == nullptr)
        throw std::runtime_error("Hero is nullptr.");

    Space *place1 = hero1->get_place();
    Space *place2 = hero2->get_place();

    if (place1 == nullptr || place2 == nullptr)
        throw std::runtime_error("Hero has no place.");

    place1->set_hero(hero2);
    place2->set_hero(hero1);

    hero1->set_place(place2);
    hero2->set_place(place1);
}