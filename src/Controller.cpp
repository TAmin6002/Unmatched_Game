#include "Controller.h"
#include "Ftxui_Front.h"

#include <iostream>
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

void Controller::set_players_character(int choise)
{
    if (choise == 0)
    {
        if (p1.get_age() <= p2.get_age())
        {
            p1.set_character(&dracula);

            p1.set_comrade(&s1);
            p1.set_comrade(&s2);
            p1.set_comrade(&s3);

            p2.set_character(&sherlock);
            p2.set_comrade(&Watson);
        }

        else
        {
            p2.set_character(&dracula);

            p2.set_comrade(&s1);
            p2.set_comrade(&s2);
            p2.set_comrade(&s3);

            p1.set_character(&sherlock);
            p1.set_comrade(&Watson);
        }
    }

    else
    {
        if (p1.get_age() <= p2.get_age())
        {
            p1.set_character(&sherlock);
            p1.set_comrade(&Watson);

            p2.set_character(&dracula);
            p2.set_comrade(&s1);
            p2.set_comrade(&s2);
            p2.set_comrade(&s3);
        }

        else
        {
            p2.set_character(&sherlock);
            p2.set_comrade(&Watson);

            p1.set_character(&dracula);

            p1.set_comrade(&s1);
            p1.set_comrade(&s2);
            p1.set_comrade(&s3);
        }
    }
}

// void Controller::Initial_characters_places()
// {
//     get_younger_player()->get_character()->set_place(&(board.get_spaces()[23])); // space 1
//     board.get_spaces()[23].set_hero(get_younger_player()->get_character());

//     get_older_player()->get_character()->set_place(&(board.get_spaces()[2])); // space 2
//     board.get_spaces()[24].set_hero(get_older_player()->get_character());
// }

Player *Controller::get_younger_player()
{
    if (p1.get_age() <= p2.get_age())
        return &p1;

    return &p2;
}

Player *Controller::get_older_player()
{
    if (p1.get_age() <= p2.get_age())
        return &p2;

    return &p1;
}

// bool Controller::can_Attack(Player *turn)
// {
//     if (!turn->get_character()->get_islive())
//         return false;
// }

void Controller::Initial_turn()
{
    if (p1.get_age() <= p2.get_age())
    {
        turn = &p1;
        not_turn = &p2;
    }

    else
    {
        turn = &p2;
        not_turn = &p1;
    }
}

void Controller::chane_turn()
{
    if (turn == &p1)
    {
        turn = &p2;
        not_turn = &p1;
    }

    else
    {
        turn = &p1;
        not_turn = &p2;
    }
}

void Controller::run()
{

    bool Exit = false;

    while (true)
    {

        switch (FF.Menu_())
        {

        case e_Menu::Play:
        {
            board = Board();

            p1 = Player();
            p2 = Player();

            dracula = Dracula();
            s1 = Sisters{"1"};
            s2 = Sisters{"2"};
            s3 = Sisters{"3"};

            sherlock = SherlockHolmes();
            Watson = Dr_Watson();

            FF.Players_Info_List(&p1, &p2);

            if (round == 1)
                Initial_turn();

            set_players_character(FF.Det_characters(&p1, &p2));

            dracula.set_place(&board.get_spaces()[2]);
            s1.set_place(&board.get_spaces()[0]);
            s1.set_place(&board.get_spaces()[1]);
            s1.set_place(&board.get_spaces()[3]);

            board.get_spaces()[2].set_hero(&dracula);
            board.get_spaces()[0].set_hero(&s1);
            board.get_spaces()[1].set_hero(&s2);
            board.get_spaces()[3].set_hero(&s3);

            sherlock.set_place(&board.get_spaces()[8]);
            Watson.set_place(&board.get_spaces()[31]);

            board.get_spaces()[8].set_hero(&sherlock);
            board.get_spaces()[31].set_hero(&Watson);

            // FF.catch_place(&p1, &p2, &board);

            // FF.chose_comrad_place(&p1, &p2, &board);

            while (true)
            {
                FF.main_map(&p1, &p2, &board, turn);

                switch (FF.get_number_of_choose())
                {

                case 0: // Attack
                    if (turn->get_count() < 2)
                    {

                        try
                        {
                            FF.Attakcer_Heroes_Menu(turn, &board, Attacker);               // Attacker selection
                            FF.Defender_Heroes_Menu(not_turn, &board, Defender, Attacker); // Defender selection

                            // FF.Attacker_selected_card();
                            // FF.Defender_selected_card();
                        }
                        catch (const std::exception &e)
                        {
                            FF.get_msg().push_back(e.what());
                            cout << e.what() << endl;
                        }
                    }

                    // exeption ...

                    break;

                case 1: // Maneuver
                    if (turn->get_count() < 2)
                    {
                    }

                    // exeption ...
                    break;

                case 2: // Scheme
                    if (turn->get_count() < 2)
                    {
                    }

                    // exeption ...
                    break;

                case 3: // Back
                    Exit = true;
                    break;
                }
                if (Exit)
                {
                    Exit = false;
                    break;
                }

                turn->add_count();
                if (turn->get_count() % 2 == 0)
                    chane_turn();

                round++;
            }
        }
        break;

        case e_Menu::Help:
        {
        }
        break;

        case e_Menu::Exit:
        {
            Exit = true;
        }
        break;
        }

        if (Exit)
        {
            Exit = false;
            break;
        }
    }
}
