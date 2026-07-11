#pragma once

#include "Heroes.h"

class Sisters : public Heroes
{

private:
public:
    Sisters(std::string);
    virtual void abiliti() override;
};