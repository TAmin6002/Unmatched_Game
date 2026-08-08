#include "Controller.h"
#include "Ftxui_Front.h"

#include <iostream>
using namespace std;

// int Controller::get_DraculaAction() const
// {
//     return DraculaAction;
// }

// void Controller::set_DraculaAction()
// {
//     DraculaAction++;
// }

// int Controller::get_SherlockAction() const
// {
//     return SherlockAction;
// }

// void Controller::set_SherlockAction()
// {
//     SherlockAction++;
// }

Dracula *Controller::get_Dracula()
{
    return &dracula;
}

SherlockHolmes *Controller::get_SherlockHolmes()
{
    return &sherlock;
}

Card *Controller::get_Attacker_selected_card()
{
    return Attacker_selected_card;
}

Card *Controller::get_Defender_selected_card(){

    return Defender_selected_card;
}

void Controller::set_players_character(int choise){

    Player * Yanger_Player = (p1.get_age() <= p2.get_age()? &p1 : &p2);
    Player * Older_Player = (p1.get_age() <= p2.get_age()? &p2 : &p1);

    if(choise == 0){
        Yanger_Player->set_character(&dracula);
        Yanger_Player->set_comrade(&s1);
        Yanger_Player->set_comrade(&s2);
        Yanger_Player->set_comrade(&s3);
        
        Older_Player->set_character(&sherlock);
        Older_Player->set_comrade(&Watson);
    }
    
    else if(choise == 1){
        Yanger_Player->set_character(&sherlock);
        Yanger_Player->set_comrade(&Watson);

        Older_Player->set_character(&dracula);
        Older_Player->set_comrade(&s1);
        Older_Player->set_comrade(&s2);
        Older_Player->set_comrade(&s3);
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

bool check_winner(Heroes *hero, Player *p1, Player *p2)
{
    Player *hero_player = (p1->get_character() == hero) ? p1 : p2;
    Player *opponent_player = (hero_player == p1) ? p2 : p1;

    if (opponent_player->get_character()->get_islive())
        return false;

    for (Heroes *comrade : opponent_player->get_comrade())
    {
        if (comrade->get_islive())
            return false;
    }

    return true;
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
            s1 = Sisters{1};
            s2 = Sisters{2};
            s3 = Sisters{3};
            
            sherlock = SherlockHolmes();
            Watson = Dr_Watson();
            

            FF.Players_Info_List(&p1, &p2);
            
            // FF.catch_place(&p1, &p2, &board);
            
            if(round == 1)
            Initial_turn();
            
            set_players_character(FF.Det_characters(&p1, &p2));

            
            dracula.set_place(&board.get_spaces()[2]);
            s1.set_place(&board.get_spaces()[0]);
            s2.set_place(&board.get_spaces()[1]);
            s3.set_place(&board.get_spaces()[3]);
            
            board.get_spaces()[2].set_hero(&dracula);
            board.get_spaces()[0].set_hero(&s1);
            board.get_spaces()[1].set_hero(&s2);
            board.get_spaces()[3].set_hero(&s3);
            
            sherlock.set_place(&board.get_spaces()[5]);
            Watson.set_place(&board.get_spaces()[31]);
            
            board.get_spaces()[5].set_hero(&sherlock);
            board.get_spaces()[31].set_hero(&Watson);
            
            // FF.catch_place(&p1, &p2, &board);
            
            // FF.chose_comrad_place(&p1, &p2, &board);
     



            while (true)
            {
                if(check_winner(&dracula, &p1, &p2))
                {
                    FF.DeclareWinner(&dracula);
                    Exit = true;
                    break;
                }
                else if(check_winner(&sherlock, &p1, &p2))
                {
                    FF.DeclareWinner(&sherlock);
                    Exit = true;
                    break;
                }

                  if (turn->get_count() == 0 && turn->get_character()->get_name() == "DRACULA")
                    {
                        try
                        {
                            dracula.abiliti(&board);
                        }
                        catch (const std::exception &e)
                        {
                            FF.get_msg().push_back(e.what());
                        }
                    }

                    if (turn->get_count() == 0 && turn->get_character()->get_name() == "InvisibleMan")
                        {
                            bool onFog = (turn->get_character()->get_place()->get_Fog() != nullptr);
                            turn->get_character()->set_StartedTurnOnFog(onFog);
                        }
                    
                FF.main_map(&p1, &p2, &board, turn);
                

                switch (FF.get_number_of_choose())
                {

                    case 0: // Attack
                    {
                        if (turn->get_count() < 2)
                        {
                            try
                            {

                                FF.Attakcer_Heroes_Menu(turn, &board, Attacker);
                                cout << "1\n";
                                
                                FF.Defender_Heroes_Menu(not_turn, &board, Defender, Attacker);
                                cout << "2\n";
                                
                                FF.Attacker_selected_card(Attacker, Defender, &p1, &p2, &board, Attacker_selected_card);
                                cout << "3\n";

                                FF.Defender_selected_card(Attacker, Defender, &p1, &p2, &board, Defender_selected_card);
                                cout << "4\n";
                                
                                FF.Reveal_Combat(Attacker, Defender, Attacker_selected_card, Defender_selected_card); // show tow v&s cards
                                cout << "5\n";
                                
        
                                Attack_Value = Attacker_selected_card->get_amount();
                                
                                if(Defender_selected_card == nullptr) {Defense_Value = 0;}
                                else
                                    Defense_Value = Defender_selected_card->get_amount();
                            }
                            catch (const std::exception &e)
                            {
                                FF.get_msg().push_back(e.what());
                                // cout << e.what() << endl;
                            }

                            // ------------------------ start combat -----------------------------------.

                            try
                            {

                                // -------------------------- Befor Calculation --------------------------.
                                cout << "6\n";
                                
                                if (Defender_selected_card != nullptr && Defender_selected_card->get_CardTiming() == CardTiming::Before)
                                {
                                    card_resolver.excute(Defender_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                                }

                                if (Attacker_selected_card->get_CardTiming() == CardTiming::Before)
                                {
                                    card_resolver.excute(Attacker_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                                }
                                
                                
                                // -------------------------- During Calculation --------------------------.
                                cout << "7\n";
                                
                                if (Defender_selected_card != nullptr && Defender_selected_card->get_CardTiming() == CardTiming::During)
                                {
                                    card_resolver.excute(Defender_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                                }

                                if (Attacker_selected_card->get_CardTiming() == CardTiming::During)
                                {
                                    card_resolver.excute(Attacker_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                                }
                                
                                
                                // --------------------------- Damage Calculation ------------------------.
                                
                                cout << "8\n";
                                
                                int Damage = Attack_Value - Defense_Value;
                                
                                if (Damage >= 1)
                                Defender->Damage(Damage);
                                
                                // Heroes *Winner = (Attack_Value > Defense_Value) ? Attacker : Defender;
                                
                                //---------------------------- After Calculation ---------------------------.
                                cout << "9\n";
                                
                                if (Defender_selected_card != nullptr && Defender_selected_card->get_CardTiming() == CardTiming::After)
                                {
                                    card_resolver.excute(Defender_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                                }

                                if (Attacker_selected_card->get_CardTiming() == CardTiming::After)
                                {
                                    card_resolver.excute(Attacker_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                                }
                                
                                
                                // ------------------------- Transfer Cards to discard --------------------------.
                                cout << "10\n";
                                
                                Attacker->Discard_Card(Attacker_selected_card);
                                Attacker_selected_card->set_user_card(nullptr);

                                if (Defender_selected_card != nullptr)
                                {
                                    Defender->Discard_Card(Defender_selected_card);
                                    Defender_selected_card->set_user_card(nullptr);
                                }

                                Attacker_selected_card = nullptr;
                                Defender_selected_card = nullptr;
                                
                                Player *Attacker_Player = (p1.get_character()->get_name() == Attacker->get_name() ? &p1 : &p2);
                                Player *Defender_Player = (p1.get_character()->get_name() == Defender->get_name() ? &p1 : &p2);
                                
                                Attacker_Player->set_selected_card(nullptr);
                                Defender_Player->set_selected_card(nullptr);
                                
                                Attack_Value = 0;
                                Defense_Value = 0;

                                Attack_Locked = false;
                                Defense_Locked = false;
                            }
                            
                            catch (const std::exception &e)
                            {
                                FF.get_msg().push_back(e.what());
                                // std::cerr << e.what() << '\n';
                            }
                            cout << "11\n";

                            turn->add_count();

                        } 
                        Attacker->set_Movement((Attacker->get_Movement() - 1 >= 0 ) ? Attacker->get_Movement() - 1 : 0);
                    }

                        // exeption ...

                        break;

                    case 1: // Maneuver
                        if (turn->get_count() < 2)
                        {
                              Heroes* selected = nullptr;
                            FF.Attakcer_Heroes_Menu(turn, &board, selected);

                            if (selected != nullptr)
                            {
                                Heroes *cardHolder = turn->get_character(); 

                                if (cardHolder->DrawnCard() == 0)
                                    cardHolder->Damage(2);

                                int extraMove = 0;

                                if (!cardHolder->get_hand().empty() && FF.AskBurnCardForMove(selected, &board))
                                {
                                    Card *burned = FF.ChooseCardFromHand(turn, &p1, &p2, &board);
                                    extraMove = burned->get_Boost();
                                    cardHolder->Discard_Card(burned);
                                }

                                FF.MoveHero(selected, &board, selected->get_Movement() + extraMove);

                                selected->set_Movement((selected->get_Movement() - 1 >= 0) ? selected->get_Movement() - 1 : 0);
                            }
                            turn->add_count();
                        }

                        // exeption ...
                        break;

                    case 2: // Event
                    {
                        if (turn->get_count() < 2)
                        {
                            int temp1, temp2 ;

                            try{
                                    Card *selected_Card = nullptr;
                                    Heroes * selectedHero = nullptr;
        
                                    FF.Attakcer_Heroes_Menu(turn, &board, selectedHero);
        
                                    if(selectedHero != nullptr)
                                    {
                                        FF.Event_Selected_Card(selectedHero, turn, &p1, &p2, &board, selected_Card);

                                        if(selected_Card != nullptr)
                                        {
                                            card_resolver.excute(selected_Card, &p1, &p2, selectedHero, nullptr, &board, temp1, temp2, Attack_Locked, Defense_Locked);

                                            selectedHero->Discard_Card(selected_Card);
                                            selected_Card->set_user_card(nullptr);
                                        }
                                    }

                                    turn->set_selected_card(nullptr);

                                }
                                catch (const std::exception &e)
                                {
                                    FF.get_msg().push_back(e.what());
                                    // std::cerr << e.what() << '\n';
                                }

                            turn->add_count();
                        }

                        // exeption ...
                        break;
                    }

                    case 3: // Back
                    {
                        Exit = true;
                        break;
                    }

                }



                    if (Exit)
                    {
                        Exit = false;
                        break;
                    }

                    if (turn->get_count() % 2 == 0)
                    {
                        turn->set_count(0);
                        chane_turn();
                        round++;
                    }
                    
                    round++;
                    
                }
        }
        break;

        case e_Menu::Help:
        {
            FF.Show_Help();
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
