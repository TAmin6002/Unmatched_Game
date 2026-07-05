#include "Controller.h"

using namespace std;

int Controller::get_DraculaAction() const
{
    return DraculaAction;
}

void Controller::set_DraculaAction()
{
    DraculaAction++;
}

int Controller::get_SherlockAction() const
{
    return SherlockAction;
}

void Controller::set_SherlockAction()
{
    SherlockAction++;
}

Dracula *Controller::get_Dracula()
{
    return &dracula;
}

SherlockHolmes *Controller::get_SherlockHolmes()
{
    return &sherlock;
}

Card Controller::get_Attacker_selected_card()
{
    return Attacker_selected_card;
}

Card Controller::get_Defender_selected_card()
{
    return Defender_selected_card;
}


void Controller::run()
{
    while(true)
    {

    }
}
