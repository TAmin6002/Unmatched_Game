#pragma once

#include "Heroes.h"
#include "Sisters.h"
#include "Card.h"

#include <vector>

class Dracula : public Heroes
{

private:
    //... متعلقات و کارت ها

    std::vector<Sisters *> sisters;

public:
    Dracula();

    void set_sisters(Sisters &, Sisters &, Sisters &);
    std::vector<Sisters *> get_sisters();
    virtual void abiliti() override;
};