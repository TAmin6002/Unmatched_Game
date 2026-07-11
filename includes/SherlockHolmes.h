#pragma once

#include "Heroes.h"
#include "Dr_Watson.h"
#include "Card.h"

#include <vector>

class SherlockHolmes : public Heroes
{
private:
    //... متعلقات و کارت ها

public:
    SherlockHolmes();
    virtual void abiliti() override;
};
