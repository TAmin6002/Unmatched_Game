#include "CardResolver.h"
#include "Card.h"

#include <vector>
using namespace std;

NeedInput CardResolver::excute(Card *selectedcard, Player *p1, Player *p2, Heroes *Attacker, Heroes *Defender, Board *board)
{

    switch (selectedcard->get_CardType())
    {

    case CardType::Feedingfrenzy:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (p1->get_character()->get_name() == "DRACULA")
            {
                vector<Space *> opponent = p2->get_character()->get_place()->get_zone();

                for (auto &s : p1->get_comrade())
                {
                    for (auto const &o : opponent)
                    {
                        if (s->get_place() == o)
                            selectedcard->add_amount(1);
                    }
                }
            }

            else if (p2->get_character()->get_name() == "DRACULA")
            {
                vector<Space *> opponent = p1->get_character()->get_place()->get_zone();

                for (auto &s : p2->get_comrade())
                {
                    for (auto const &o : opponent)
                    {
                        if (s->get_place() == o)
                            selectedcard->add_amount(1);
                    }
                }
            }
        }
        return NeedInput::None;
        break;
    }

    case CardType::Mistform:
    {
        if (selectedcard->get_ApplyEffects())
        {
            // put Dracula in any space ...

            if (p1->get_character()->get_name() == "DRACULA")
            {
                p1->get_character()->add_Action(1);
                return NeedInput::PlaceDraculaAnywhere;
            }

            else if (p2->get_character()->get_name() == "DRACULA")
            {
                p2->get_character()->add_Action(1);
                return NeedInput::PlaceDraculaAnywhere;
            }
        }
        return NeedInput::None;
        break;
    }

    case CardType::Ambush:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (p1->get_character()->get_name() == "DRACULA" or p1->get_comrade()[0]->get_name() == "SISTERS")
            {
                selectedcard->add_amount(p2->get_character()->discard_hand());
            }

            else if (p2->get_character()->get_name() == "DRACULA" or p2->get_comrade()[0]->get_name() == "SISTERS")
            {
                selectedcard->add_amount(p1->get_character()->discard_hand());
            }

            // بعد از محاسبه این مقدار افزایش یافه برمیگردد
        }
        return NeedInput::None;
        break;
    }

    case CardType::Baptism_of_blood:
    {
        if (selectedcard->get_ApplyEffects())
        {

            if (p1->get_character()->get_name() == "DRACULA")
            {
                p1->get_character()->set_Health(2, 13);

                for (auto &s : p1->get_comrade())
                {
                    if (s->get_islive() == false)
                        return NeedInput::PlaceSisterInDraculaZone;
                }
            }
            return NeedInput::None;
        }

        else if (p2->get_character()->get_name() == "DRACULA")
        {
            p2->get_character()->set_Health(2, 13);

            for (auto &s : p2->get_comrade())
            {
                if (s->get_islive() == false)
                    return NeedInput::PlaceSisterInDraculaZone;
            }

            return NeedInput::None;
        }
    }

    case CardType::Beastform:
    {
        if (selectedcard->get_ApplyEffects())
        {
            return NeedInput::SelectOwnCardToDiscard;
        }

        break;
    }

    case CardType::Dash: // uncomplate ......??????
    {
        if (selectedcard->get_ApplyEffects())
        {
            return NeedInput::MoveOwnFighter3;
        }
        return NeedInput::None;
        break;
    }

    case CardType::Exploit:
    {
        if (selectedcard->get_ApplyEffects())
        {

            if (p1->get_character()->get_name() == "DRACULA" or p1->get_comrade()[0]->get_name() == "SISTERS")
            {
                p1->get_character()->DrawnCard();
            }

            else if (p2->get_character()->get_name() == "DRACULA" or p2->get_comrade()[0]->get_name() == "SISTERS")
            {
                p2->get_character()->DrawnCard();
            }
        }
        return NeedInput::None;
        break;
    }

    case CardType::Look_into_my_eyes:
    {
        if (selectedcard->get_ApplyEffects())
        {

            if (p1->get_character()->get_name() == "DRACULA")
            {
                int Boost = p2->get_selected_card().get_Boost(); // Opponent's card boost amount
                p1->get_selected_card().set_amount(Boost);
            }

            else if (p2->get_character()->get_name() == "DRACULA")
            {
                int Boost = p1->get_selected_card().get_Boost(); // Opponent's card boost amount
                p2->get_selected_card().set_amount(Boost);
            }
        }
        return NeedInput::None;
        break;
    }

    case CardType::Prey_upon:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (p1->get_character()->get_name() == "DRACULA")
            {

                if (board->is_Adjacent(p1->get_character()->get_place(), p2->get_character()->get_place()))
                {
                    p2->get_character()->Damage(1);
                }

                if (board->is_Adjacent(p1->get_character()->get_place(), p2->get_comrade()[0]->get_place()))
                {
                    p2->get_comrade()[0]->Damage(1);
                }
            }

            else if (p2->get_character()->get_name() == "DRACULA")
            {

                if (board->is_Adjacent(p2->get_character()->get_place(), p1->get_character()->get_place()))
                {
                    p1->get_character()->Damage(1);
                }

                if (board->is_Adjacent(p2->get_character()->get_place(), p1->get_comrade()[0]->get_place()))
                {
                    p1->get_comrade()[0]->Damage(1);
                }
            }
        }
        return NeedInput::None;
        break;
    }

    case CardType::Ravening_Seduction:
    {
        if (selectedcard->get_ApplyEffects())
        {
            return NeedInput::MoveOpponentFighter;
        }

        break;
    }

    case CardType::Thirst_for_sustenance:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (p1->get_comrade()[0]->get_name() == "SISTERS")
            {
                if (p1->get_isAttacker())
                {
                    if (p1->get_selected_card().get_amount() - p2->get_selected_card().get_amount() >= 1)
                    {
                        return NeedInput::PlaceDraculaAdjacentToOpponent;
                    }
                }

                else
                {
                    if (p1->get_selected_card().get_amount() - p2->get_selected_card().get_amount() < 1)
                    {
                        return NeedInput::PlaceDraculaAdjacentToOpponent;
                    }
                }
            }

            else if (p2->get_comrade()[0]->get_name() == "SISTERS")
            {
                if (p2->get_isAttacker())
                {
                    if (p2->get_selected_card().get_amount() - p1->get_selected_card().get_amount() >= 1)
                    {
                        return NeedInput::PlaceDraculaAdjacentToOpponent;
                    }
                }

                else
                {
                    if (p2->get_selected_card().get_amount() - p1->get_selected_card().get_amount() < 1)
                    {
                        return NeedInput::PlaceDraculaAdjacentToOpponent;
                    }
                }
            }
        }
        return NeedInput::None;
        break;
    }
    case CardType::Feint:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (p1->get_character()->get_name() == "DRACULA" or p1->get_comrade()[0]->get_name() == "SISTERS")
            {
                p2->get_selected_card().set_ApplyEffects(false); // Cancel the effect opponent's card.
            }

            else if (p2->get_character()->get_name() == "DRACULA" or p2->get_comrade()[0]->get_name() == "SISTERS")
            {
                p1->get_selected_card().set_ApplyEffects(false); // Cancel the effect opponent's card.
            }
            return NeedInput::None;
            break;
        }
    }

        // if (p1->get_character()->get_name() == "DRACULA" or p1->get_comrade()->get_name() == "SISTERS")
        // {
        // }

        // else if (p2->get_character()->get_name() == "DRACULA" or p2->get_comrade()->get_name() == "SISTERS")
        // {
        // }

    case CardType::Administer_Aid:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (p1->get_comrade()[0]->get_name() == "Dr_Watson")
            { // ..........................................................................
                for (auto &s : p1->get_character()->get_place()->get_neighbor())
                {
                    if (s->get_hero() == nullptr)
                        p1->get_comrade()[0]->set_place(s);
                }

                p1->get_character()->set_Health(1, 16); // adding a Health to sherlock
            }

            else if (p2->get_comrade()[0]->get_name() == "Dr_Watson")
            {
                for (auto &s : p2->get_character()->get_place()->get_neighbor())
                {
                    if (s->get_hero() == nullptr)
                        p2->get_comrade()[0]->set_place(s);
                }
                p2->get_character()->set_Health(1, 16); // adding a Health to sherlock
            }
            return NeedInput::None;
            break;
        }
    }

        // if (p1->get_character()->get_name() == "SHERLOCKHOLMES" or p1->get_comrade()->get_name() == "Dr_Watson")
        //     else if (p2->get_character()->get_name() == "SHERLOCKHOLMES" or p2->get_comrade()->get_name() == "Dr_Watson")

        // case CardType::Confirm_Suspicion:
        // {
        //     if (selectedcard->get_ApplyEffects())
        //     {
        //         // pointing
        //         break;
        //     }
        // }

    case CardType::Counterpunch:
    {
        if (selectedcard->get_ApplyEffects())
        {

            if (p1->get_character()->get_name() == "SHERLOCKHOLMES")
            {
                if (p1->get_isAttacker())
                {
                    if (board->is_Adjacent(p1->get_character()->get_place(), Defender->get_place()))
                    {
                        Defender->Damage(2);
                    }
                }

                else
                {
                    if (board->is_Adjacent(p1->get_character()->get_place(), Attacker->get_place()))
                    {
                        Attacker->Damage(2);
                    }
                }
            }
        }

        else if (p2->get_character()->get_name() == "SHERLOCKHOLMES")
        {
            if (p2->get_isAttacker())
            {
                if (board->is_Adjacent(p2->get_character()->get_place(), Defender->get_place()))
                {
                    Defender->Damage(2);
                }
            }

            else
            {
                if (board->is_Adjacent(p2->get_character()->get_place(), Attacker->get_place()))
                {
                    Attacker->Damage(2);
                }
            }
        }
        return NeedInput::None;
        break;
    }

    case CardType::Deduce_Strategy:
    {
        if (selectedcard->get_ApplyEffects())
        {

            if (p1->get_character()->get_name() == "SHERLOCKHOLMES")
            {
                int Boost = p2->get_selected_card().get_Boost();
                p2->get_selected_card().set_amount(Boost);
            }

            else if (p2->get_character()->get_name() == "SHERLOCKHOLMES")
            {
                int Boost = p1->get_selected_card().get_Boost();
                p1->get_selected_card().set_amount(Boost);
            }
            return NeedInput::None;
            break;
        }
    }

    case CardType::Education_Never_Ends:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (p1->get_character()->get_name() == "SHERLOCKHOLMES" or p1->get_comrade()[0]->get_name() == "Dr_Watson")
            {
                if (p1->get_isAttacker())
                {
                    // if we were Attakcer :
                    if ((p1->get_selected_card().get_amount() - p2->get_selected_card().get_amount()) >= 1)
                    {
                        // if we had won :
                        p2->get_character()->DrawnCard();
                    }
                    else
                    {
                        p1->get_character()->DrawnCard();
                        p1->get_character()->DrawnCard();
                    }
                }
            }
            else if (p2->get_character()->get_name() == "SHERLOCKHOLMES" or p2->get_comrade()[0]->get_name() == "Dr_Watson")
            {
                if (p2->get_isAttacker())
                {

                    // if we were Attakcer :
                    if ((p2->get_selected_card().get_amount() - p1->get_selected_card().get_amount()) >= 1)
                    {
                        // if we had won :
                        p1->get_character()->DrawnCard();
                    }

                    else
                    {
                        p2->get_character()->DrawnCard();
                        p2->get_character()->DrawnCard();
                    }
                }
            }
        }
        return NeedInput::None;
        break;
    }

        // case CardType::Elementary:
        // {
        //     // After ... (bonus)
        //     break;
        // }

    case CardType::Eliminate_The_Impossible:
    {
        return NeedInput::SelectCardToDiscard; // در کنترلر انجام میشود ...
        break;
    }

    case CardType::Feint2:
    {

        if (p1->get_character()->get_name() == "SHERLOCKHOLMES" or p1->get_comrade()[0]->get_name() == "Dr_Watson")
        {
            p2->get_selected_card().set_ApplyEffects(false); // p2 dis carding
        }

        else if (p2->get_character()->get_name() == "SHERLOCKHOLMES" or p2->get_comrade()[0]->get_name() == "Dr_Watson")
        {
            p1->get_selected_card().set_ApplyEffects(false); // p1 dis carding
        }
        return NeedInput::None;
        break;
    }

    case CardType ::Fixed_Point_in_a_Changing_Age:
    {

        if (p1->get_comrade()[0]->get_name() == "Dr_Watson")
        {
            if (board->is_Adjacent(p1->get_character()->get_place(), p1->get_comrade()[0]->get_place()))
            {
                p1->get_character()->set_Health(1, 16);
                p1->get_comrade()[0]->set_Health(1, 8);
            }
        }

        else if (p2->get_comrade()[0]->get_name() == "Dr_Watson")
        {
            if (board->is_Adjacent(p2->get_character()->get_place(), p2->get_comrade()[0]->get_place()))
            {
                p2->get_character()->set_Health(1, 16);
                p2->get_comrade()[0]->set_Health(1, 8);
            }
        }
        return NeedInput::None;
        break;
    }

    case CardType::Master_of_Disguise:
    {
        if (p1->get_character()->get_name() == "SHERLOCKHOLMES")
        {
            if (Attacker->get_name() == "SHERLOCKHOLMES")
            {
                p1->get_character()->set_place(Defender->get_place());

                Defender->Damage(1);
            }

            if (Defender->get_name() == "SHERLOCKHOLMES")
            {
                p1->get_character()->set_place(Attacker->get_place());
                Attacker->Damage(1);
            }
            if (Attacker->get_name() == "SHERLOCKHOLMES")
            {
                p1->get_character()->set_place(Defender->get_place());

                Defender->Damage(1);
            }

            if (Defender->get_name() == "SHERLOCKHOLMES")
            {
                p1->get_character()->set_place(Attacker->get_place());
                Attacker->Damage(1);
            }
        }

        else if (p2->get_character()->get_name() == "SHERLOCKHOLMES")
        {
            if (Attacker->get_name() == "SHERLOCKHOLMES")
            {
                p2->get_character()->set_place(Defender->get_place());

                Defender->Damage(1);
            }

            if (Defender->get_name() == "SHERLOCKHOLMES")
            {
                p2->get_character()->set_place(Attacker->get_place());
                Attacker->Damage(1);
            }
        }
        return NeedInput::None;
        break;
    }

    case CardType::The_Game_Is_Afoot:
    {
        if (p1->get_character()->get_name() == "SHERLOCKHOLMES" or p2->get_character()->get_name() == "SHERLOCKHOLMES")
            return NeedInput::MoveSherlock3;
        break;
    }

    case CardType::Service_Revolver:
    {
        // hase't .
        return NeedInput::None;
        break;
    }

    case CardType::Study_Methods:
    {
        return NeedInput::ShowOpponentHand;
        break;
    }
    }
}
