#include "CardResolver.h"
#include "Card.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace ftxui;

void CardResolver::excute(Card *selectedcard, Player *p1, Player *p2, Heroes *Attacker, Heroes *Defender, Board *board, int &Attack_Value, int &Defence_Value)
{
    cout << "Entered the excute\n";

    Player *Dracula_Player;
    Player *Sherlock_Player;

    Dracula_Player = (p1->get_character()->get_name() == "DRACULA" ? p1 : p2);
    Sherlock_Player = (p1->get_character()->get_name() == "SHERLOCKHOLMES" ? p1 : p2);

    switch (selectedcard->get_CardType())
    {
    case CardType::Feedingfrenzy:
    {
        if (selectedcard->get_ApplyEffects())
        {
            vector<Space *> opponent = Defender->get_place()->get_zone();
            for (auto const &o : opponent)
            {
                for (auto const &s : Dracula_Player->get_comrade())
                {
                    if (o == s->get_place())
                        Attack_Value++;
                }
            }
        }
        break;
    }

    case CardType::Mistform:
    {
        if (selectedcard->get_ApplyEffects())
        {
            FF.put_in_any_space(Dracula_Player->get_character(), board);
            Dracula_Player->add_count();
        }
        break;
    }

    case CardType::Ambush:
    {
        if (selectedcard->get_ApplyEffects())
        {
            Attack_Value += Sherlock_Player->get_character()->discard_hand();
        }
        break;
    }

    case CardType::Baptism_of_blood:
    {
        if (selectedcard->get_ApplyEffects())
        {
            Dracula_Player->get_character()->set_Health(2, 13);

            Heroes *sister = nullptr;

            for (auto &s : Dracula_Player->get_comrade()) // SISTERS
            {
                if (!s->get_islive())
                {
                    sister = s;
                }
            }

            if (sister != nullptr)
            {
                sister->set_islive(true);
                sister->set_Health(1, 1);

                FF.Revive_Sister(sister, Dracula_Player->get_character(), board);
            }
        }
        break;
    }

    case CardType::Beastform:
    {
        if (selectedcard->get_ApplyEffects())
        {
            Attack_Value += FF.DiscardCards(Attacker);
        }
        break;
    }

    case CardType::Dash:
    {
        if (selectedcard->get_ApplyEffects())
        {
            FF.MoveHero(selectedcard->get_user_card(), board, 3);
        }
        break;
    }

    case CardType::Exploit:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (p1->get_character()->get_name() == "DRACULA" or (!p1->get_comrade().empty() and p1->get_comrade()[0]->get_name() == "SISTERS"))
            {
                if (p1->get_character()->DrawnCard() == 0)
                    p1->get_character()->Damage(2);
            }

            else if (p2->get_character()->get_name() == "DRACULA" or (!p2->get_comrade().empty() and p2->get_comrade()[0]->get_name() == "SISTERS"))
            {
                if (p2->get_character()->DrawnCard() == 0)
                    p2->get_character()->Damage(2);
            }
        }
        break;
    }

    case CardType::Look_into_my_eyes:
    {
        if (selectedcard->get_ApplyEffects())
        {
            int Boost = 0;

            if (Sherlock_Player->get_selected_card() != nullptr)
                Boost = Sherlock_Player->get_selected_card()->get_Boost();

            selectedcard->add_amount(Boost);
        }
        break;
    }

    case CardType::Prey_upon:
    {
        if (selectedcard->get_ApplyEffects())
        {
            int count = 0;

            if (p1->get_character()->get_name() == "DRACULA")
            {
                count = 0;

                if (board->is_Adjacent(p1->get_character()->get_place(), p2->get_character()->get_place()))
                {
                    p2->get_character()->Damage(1);
                    count++;
                }

                if (!p2->get_comrade().empty() and board->is_Adjacent(p1->get_character()->get_place(), p2->get_comrade()[0]->get_place()))
                {
                    p2->get_comrade()[0]->Damage(1);
                    count++;
                }
                p1->get_character()->set_Health(count, 13);
            }

            else if (p2->get_character()->get_name() == "DRACULA")
            {
                count = 0;

                if (board->is_Adjacent(p2->get_character()->get_place(), p1->get_character()->get_place()))
                {
                    p1->get_character()->Damage(1);
                    count++;
                }

                if (!p1->get_comrade().empty() and board->is_Adjacent(p2->get_character()->get_place(), p1->get_comrade()[0]->get_place()))
                {
                    p1->get_comrade()[0]->Damage(1);
                    count++;
                }
                p2->get_character()->set_Health(count, 13);
            }
        }
        break;
    }

    case CardType::Ravening_Seduction:
    {
        if (selectedcard->get_ApplyEffects())
        {
            Heroes *target = FF.SelectHero(board);

            if (target == nullptr)
                break;

            FF.MoveHero(target, board, 2);

            int damage = 0;

            for (Space *space : target->get_place()->get_neighbor())
            {
                Heroes *hero = space->get_hero();

                if (hero == nullptr)
                    continue;

                if (hero->get_name() == "SISTERS")
                    damage++;
            }

            target->Damage(damage);
        }

        break;
    }

    case CardType::Thirst_for_sustenance:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (Attack_Value - Defence_Value >= 1)
            {
                FF.PlaceHeroAdjacent(Dracula_Player->get_character(), Defender, board);
            }
        }
        break;
    }

    case CardType::Feint:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (p1->get_character()->get_name() == "DRACULA" or (!p1->get_comrade().empty() and p1->get_comrade()[0]->get_name() == "SISTERS"))
            {
                if (p2->get_selected_card() != nullptr)
                    TryDisableCard(p2->get_selected_card(), p1, p2);
            }

            else if (p2->get_character()->get_name() == "DRACULA" or (!p2->get_comrade().empty() and p2->get_comrade()[0]->get_name() == "SISTERS"))
            {
                if (p1->get_selected_card() != nullptr)
                    TryDisableCard(p1->get_selected_card(), p1, p2);
            }
        }
        break;
    }

    case CardType::Administer_Aid:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (p1->get_character()->get_name() == "SHERLOCKHOLMES" or (!p1->get_comrade().empty() and p1->get_comrade()[0]->get_name() == "Dr_Watson"))
            {
                if (!p1->get_comrade().empty())
                    FF.PlaceHeroAdjacent(p1->get_character(), p1->get_comrade()[0], board);

                p1->get_character()->set_Health(1, 16);
                if (p1->get_character()->DrawnCard() == 0)
                {
                    p1->get_character()->Damage(2);
                }
            }

            else if (p2->get_character()->get_name() == "SHERLOCKHOLMES" or (!p2->get_comrade().empty() and p2->get_comrade()[0]->get_name() == "Dr_Watson"))
            {
                if (!p2->get_comrade().empty())
                    FF.PlaceHeroAdjacent(p2->get_character(), p2->get_comrade()[0], board);

                p2->get_character()->set_Health(1, 16);

                if (p2->get_character()->DrawnCard() == 0)
                {
                    p2->get_character()->Damage(2);
                }
            }
        }
        break;
    }

    case CardType::Counterpunch:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (Attacker->get_name() == "SHERLOCKHOLMES")
            {
                if (board->is_Adjacent(Attacker->get_place(), Defender->get_place()))
                    Defender->Damage(2);
            }

            else if (Defender->get_name() == "SHERLOCKHOLMES")
            {
                if (board->is_Adjacent(Defender->get_place(), Attacker->get_place()))
                    Attacker->Damage(2);
            }
        }
        break;
    }

    case CardType::Deduce_Strategy:
    {
        cout << "Dedece Card \n";
        if (selectedcard->get_ApplyEffects())
        {
            if (p1->get_character()->get_name() == "SHERLOCKHOLMES")
            {
                if (p2->get_selected_card() != nullptr)
                {
                    int Boost = p2->get_selected_card()->get_Boost();
                    p2->get_selected_card()->set_amount(Boost);
                }
            }

            else if (p2->get_character()->get_name() == "SHERLOCKHOLMES")
            {
                if (p1->get_selected_card() != nullptr)
                {
                    int Boost = p1->get_selected_card()->get_Boost();
                    p1->get_selected_card()->set_amount(Boost);
                }
            }
        }
        break;
    }

    case CardType::Education_Never_Ends:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (Attacker->get_name() == "SHERLOCKHOLMES" or Attacker->get_name() == "Dr_Watson")
            {
                if (Attack_Value - Defence_Value >= 1)
                {   
                    if (Defender->DrawnCard() == 0)
                        Defender->Damage(2);
                }

                else
                {
                    if (Attacker->DrawnCard() == 0)
                        Attacker->Damage(2);

                    if (Attacker->DrawnCard() == 0)
                        Attacker->Damage(2);
                }
            }
            else if (Defender->get_name() == "SHERLOCKHOLMES" or Defender->get_name() == "Dr_Watson")
            {
                if (Attack_Value - Defence_Value <= 0)
                {
                    if (Attacker->DrawnCard() == 0)
                        Attacker->Damage(2);
                }

                else
                {
                    if (Defender->DrawnCard() == 0)
                        Defender->Damage(2);

                    if (Defender->DrawnCard() == 0)
                        Defender->Damage(2);
                }
            }
        }
        break;
    }

    case CardType::Eliminate_The_Impossible:
    {
        if (selectedcard->get_ApplyEffects())
        {
            Dracula_Player->get_character()->Discard_Card(FF.ChooseCardFromHand(Dracula_Player, p1, p2, board));
        }
        break;
    }

    case CardType::Feint2:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (Dracula_Player->get_selected_card() != nullptr)
                TryDisableCard(Dracula_Player->get_selected_card(), p1, p2);
        }
        break;
    }

    case CardType ::Fixed_Point_in_a_Changing_Age:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (!Sherlock_Player->get_comrade().empty() and board->is_Adjacent(Sherlock_Player->get_character()->get_place(), Sherlock_Player->get_comrade()[0]->get_place()))
            {
                Sherlock_Player->get_character()->set_Health(1, 16);
                Sherlock_Player->get_comrade()[0]->set_Health(1, 8);
            }
        }
        break;
    }

    case CardType::Master_of_Disguise:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (Attacker->get_name() == "SHERLOCKHOLMES")
            {
                board->SwapHeroes(Attacker, Defender);
                Defender->Damage(1);
            }

            else
            {
                board->SwapHeroes(Defender, Attacker);
                Attacker->Damage(1);
            }
        }
        break;
    }

    case CardType::The_Game_Is_Afoot:
    {
        if (selectedcard->get_ApplyEffects())
        {
            FF.MoveHero(Sherlock_Player->get_character(), board, 3);
        }

        break;
    }

    case CardType::Service_Revolver:
    {
        break;
    }

    case CardType::Study_Methods:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (Attacker->get_name() == "SHERLOCKHOLMES" or Attacker->get_name() == "Dr_Watson")
            {
                if (Attack_Value - Defence_Value >= 1)
                    FF.ShowHand(Defender, p1, p2, board);
            }
            else
            {
                if (Attack_Value - Defence_Value <= 0)
                    FF.ShowHand(Attacker, p1, p2, board);
            }
        }

        break;
    }

    // ------------------- Invisible Man ------------------------

    case CardType::Coded_Notes:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (Defender->DrawnCard() == 0){
                Defender->Damage(2);
            }
    
             if (Defender->DrawnCard() == 0){
                Defender->Damage(2);
            }
    
             if (Defender->DrawnCard() == 0){
                Defender->Damage(2);
            }

            FF.ChooseCardsToTopOfDeck(Defender, 2, p1, p2, board);

        }
        
    }

    case CardType::Confound :
    {
        if (selectedcard->get_ApplyEffects()){

              if (p1->get_character()->get_name() == "InvisibleMan"){
                if(!FF.ChooseCardToDiscardOrSkip(p2))
                    FF.MoveFogToken(FF.SelectComrade(p1, board), board);
            }

            else if (p2->get_character()->get_name() == "InvisibleMan"){
                if(!FF.ChooseCardToDiscardOrSkip(p1))
                    FF.MoveFogToken(FF.SelectComrade(p2, board), board);


            }
        }

    }

     case CardType::Covert_Preparation :
    {
        if (selectedcard->get_ApplyEffects()){

            if (Attacker->get_name() == "InvisibleMan"){
                if (Attacker->DrawnCard() == 0)
                    Attacker->Damage(2);
            }

            else if (Defender->get_name() == "InvisibleMan"){
                if (Defender->DrawnCard() == 0)
                    Defender->Damage(2);
            }
        }

    }


    }
}

void CardResolver::TryDisableCard(Card *card, Player *p1, Player *p2)
{
    if (card == nullptr)
        return;

    Player *Sherlock_Player = (p1->get_character()->get_name() == "SHERLOCKHOLMES" ? p1 : p2);
    Heroes *sherlock = Sherlock_Player->get_character();

    std::string owner = card->get_owner();

    if (sherlock->get_islive() && (owner == "SHERLOCKHOLMES" || owner == "DR_WATSON")) {
        return; 
    }

    card->set_ApplyEffects(false);
}