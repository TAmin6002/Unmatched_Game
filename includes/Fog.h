# pragma once

#include "Heroes.h"

class Fog : public  Heroes{
    private:
    public:
    Fog(int number);
    virtual void abiliti(Board *) override;

};