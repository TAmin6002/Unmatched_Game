#pragma once

#include <vector>

class Heroes;

class Space
{
private:
    std::vector<Space *> zone;
    std::vector<Space *> neighbor = {};
    std::vector<Space *> Hidden_way = {};

    Heroes *hero = nullptr;
    Heroes *Fog = nullptr;

    int number;
public:
    Space(int, std::vector<Space *>, std ::vector<Space *>);
    Space() = default;

    std::vector<Space *> get_zone();
    std::vector<Space *> get_neighbor();

    void set_hero(Heroes *);
    Heroes *get_hero();

    void set_Fog(Heroes *);
    Heroes * get_Fog();

    int get_number();


    void add_portal(Space *destination);
    std::vector<Space *> get_portal();
    bool has_portal();
};

