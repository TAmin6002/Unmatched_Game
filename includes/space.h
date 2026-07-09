#ifndef SPACE_H
#define SPACE_H

#include <vector>

class Heroes;

class Space
{
private:
    std::vector<Space *> zone;
    std::vector<Space *> neighbor = {};
    std::vector<Space *> Hidden_way = {};

    Heroes *hero = nullptr;

    int number;
public:
    Space(int, std::vector<Space *>, std ::vector<Space *>);
    Space() = default;

    std::vector<Space *> get_zone();
    std::vector<Space *> get_neighbor();

    void set_hero(Heroes *);
    Heroes *get_hero();

    int get_number();
};

#endif