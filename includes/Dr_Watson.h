#pragma once

#include "Heroes.h"

class Dr_Watson : public Heroes
{
private:
public:
    Dr_Watson();
    virtual void abiliti(Board *) override;
};