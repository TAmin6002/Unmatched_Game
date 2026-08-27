#include "CardResolver.h"
#include "Card.h"

#include <vector>
#include <iostream>

using namespace std;

void CardResolver::excute(Card *selectedcard, Player *p1, Player *p2, Heroes *Attacker, Heroes *Defender, Board *board, int &Attack_Value, int &Defence_Value, bool &Attack_Locked, bool &Defense_Locked)
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
        if (selectedcard->get_ApplyEffects() && !Attack_Locked)
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
            RF.put_in_any_space(Dracula_Player->get_character(), board);
            Dracula_Player->add_count();
        }
        break;
    }

    case CardType::Ambush:
    {
        if (selectedcard->get_ApplyEffects() && !Attack_Locked)
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

                RF.Revive_Sister(sister, Dracula_Player->get_character(), board);
            }
        }
        break;
    }

    case CardType::Beastform:
    {
        if (selectedcard->get_ApplyEffects() && !Attack_Locked)
        {
            Attack_Value += RF.DiscardCards(Attacker);
        }
        break;
    }

    case CardType::Dash:
    {
        if (selectedcard->get_ApplyEffects())
        {
            RF.MoveHero(selectedcard->get_user_card(), board, 3, p1, p2);
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
            Heroes *target = RF.SelectHero(board);

            if (target == nullptr)
                break;

                RF.MoveHero(target, board, 2, p1, p2);

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
                RF.PlaceHeroAdjacent(Dracula_Player->get_character(), Defender, board);
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
                    RF.PlaceHeroAdjacent(p1->get_character(), p1->get_comrade()[0], board);

                p1->get_character()->set_Health(1, 16);
                if (p1->get_character()->DrawnCard() == 0)
                {
                    p1->get_character()->Damage(2);
                }
            }

            else if (p2->get_character()->get_name() == "SHERLOCKHOLMES" or (!p2->get_comrade().empty() and p2->get_comrade()[0]->get_name() == "Dr_Watson"))
            {
                if (!p2->get_comrade().empty())
                    RF.PlaceHeroAdjacent(p2->get_character(), p2->get_comrade()[0], board);

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
            Dracula_Player->get_character()->Discard_Card(RF.ChooseCardFromHand(Dracula_Player, p1, p2, board));
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
            RF.MoveHero(Sherlock_Player->get_character(), board, 3, p1, p2);
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
                    RF.ShowHand(Defender, p1, p2, board);
            }
            else
            {
                if (Attack_Value - Defence_Value <= 0)
                    RF.ShowHand(Attacker, p1, p2, board);
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

            RF.ChooseCardsToTopOfDeck(Defender, 2, p1, p2, board);

        }
        break;
        
    }

    case CardType::Confound :
    {
        if (selectedcard->get_ApplyEffects()){

              if (p1->get_character()->get_name() == "InvisibleMan"){
                if(!RF.ChooseCardToDiscardOrSkip(p2))
                    RF.MoveFogToken(RF.SelectFogToken(board), board);
            }

            else if (p2->get_character()->get_name() == "InvisibleMan"){
                if(!RF.ChooseCardToDiscardOrSkip(p1))
                    RF.MoveFogToken(RF.SelectFogToken(board), board);

            }
        }
        break;

    }

     case CardType::Covert_Preparation :
    {
        if (selectedcard->get_ApplyEffects()){

            Player * InvisibleManPlayer = (Attacker->get_name() == "InvisibleMan") ? 
                ((p1->get_character() == Attacker || (!p1->get_comrade().empty() && p1->get_comrade()[0] == Attacker)) ? p1 : p2)
                : ((p1->get_character() == Defender || (!p1->get_comrade().empty() && p1->get_comrade()[0] == Defender)) ? p1 : p2);

            Player * opp = (InvisibleManPlayer == p1) ? p2 : p1;

            if (Attacker->get_name() == "InvisibleMan"){
                if (Attacker->DrawnCard() == 0)
                    Attacker->Damage(2);

                Heroes *oppComrade = RF.SelectComrade(opp, board);
                if (oppComrade != nullptr)
                    RF.MoveHero(oppComrade, board, 2, p1, p2);
            }

            else if (Defender->get_name() == "InvisibleMan"){
                if (Defender->DrawnCard() == 0)
                    Defender->Damage(2);

                Heroes *oppComrade = RF.SelectComrade(opp, board);
                if (oppComrade != nullptr)
                    RF.MoveHero(oppComrade, board, 2, p1, p2);
            }
        }
        break;
    }

    case CardType::Dreaming_of_Revenge :
    {
        if (selectedcard->get_ApplyEffects()){

            Heroes * InvisibleMan = (Attacker->get_name() == "InvisibleMan") ? Attacker : Defender;
            Player * InvisibleManPlayer = (p1->get_character()->get_name() == "InvisibleMan")? p1 : p2;
    
            if (InvisibleMan->get_place() != nullptr && InvisibleMan->get_place()->get_Fog() != nullptr){
    
                for (auto F : InvisibleManPlayer->get_comrade()){
    
                    if(F->get_place()->get_hero()->get_name() != "InvisibleMan"){
                        F->get_place()->get_hero()->Damage(1);   
                    }
                }
            }
        }
        break;
    }

    case CardType::Emerge_From_Mist:
    {
        if (selectedcard->get_ApplyEffects() && !Attack_Locked)
        {
            if (selectedcard->get_user_card()->get_StartedTurnOnFog())
            {
                Attack_Value = 5;
            }
        }
        break;
    }

    
      case CardType::Impossible_to_See:
    {
        if (selectedcard->get_ApplyEffects())
        {
            if (Attacker->get_name() == "InvisibleMan")
            {
                Defence_Value = 0;
                Defense_Locked = true;
            }
            else if (Defender->get_name() == "InvisibleMan")
            {
                Attack_Value = 0;
                Attack_Locked = true;
            }
        }
        break;


    }

   case CardType::Into_Thin_Air:
    {
        if (selectedcard->get_ApplyEffects())
        {
            Heroes *user = selectedcard->get_user_card();

            RF.MoveHero(user, board, 1, p1, p2);
            
            Space *fogSpace = RF.SelectFogToken(board);
            RF.MoveFogTokenDistance(fogSpace, board, 3);
        }

        break;

    }

    case CardType::Lurking:
    {
        if (selectedcard->get_ApplyEffects())
        {
            Heroes *user = selectedcard->get_user_card();

            user->DrawnCard();

            if (RF.ChooseBetweenTwoEffects("Move InvisibleMan to a Fog Token space", "Move a Fog Token up to 3 spaces"))
            {
                RF.MoveHeroToFogSpace(user, board);
            }
            else
            {
                Space *fogSpace = RF.SelectFogToken(board);
                RF.MoveFogTokenDistance(fogSpace, board, 3);
            }
        }
        break;
    }

    case CardType::Reign_Thrror:
    {
        if (selectedcard->get_ApplyEffects()){
            Player * InvisibleManPlayer = (p1->get_character()->get_name() == "InvisibleMan") ? p1: p2;
            Player * opp = (InvisibleManPlayer == p1) ? p2 : p1 ;


            if(InvisibleManPlayer->get_character()->get_place() != nullptr && InvisibleManPlayer->get_character()->get_place()->get_Fog() != nullptr){{
                for (auto o : opp->get_comrade()){

                    if (o != nullptr)
                        o->Damage(2);
                }

                opp->get_character()->Damage(2);
            }
        }
        break;
    }

    case CardType::Rolling_Fog:
    {
        if (selectedcard->get_ApplyEffects())
        {
            Space *fogSpace = RF.SelectFogToken(board);
            RF.MoveFogTokenAnywhere(fogSpace, board);

            Player *user_player =
                (p1->get_character() == Attacker ||
                (!p1->get_comrade().empty() && p1->get_comrade()[0] == Attacker))
                    ? p1
                    : p2;

            if (user_player->get_count() > 0)
                user_player->set_count(user_player->get_count() - 1);
        }
        break;
    }

    case CardType::Slip_Away:
    {
        if (selectedcard->get_ApplyEffects())
        {
            Heroes *user = selectedcard->get_user_card();

            Space *fogSpace = RF.SelectFogToken(board);
            Space *destination = RF.MoveFogTokenToEmptySpace(fogSpace, board);

           if (destination != nullptr)
            {
                if (user->get_place() != nullptr)
                    user->get_place()->set_hero(nullptr);

                destination->set_hero(user);
                user->set_place(destination);
            }
        }
        break;
    }

    case CardType::Step_Lightly:
    {
        if (selectedcard->get_ApplyEffects())
        {
            Heroes *user = selectedcard->get_user_card();

            Heroes *target = RF.SelectAdjacentHero(user, board);

           if (target != nullptr)
            {
                bool onFog = (user->get_place() != nullptr && user->get_place()->get_Fog() != nullptr);
                target->Damage(onFog ? 3 : 1);
            }

            Space *fogSpace = RF.SelectFogToken(board);
            RF.MoveFogTokenDistance(fogSpace, board, 2);
        }
        break;
    }

        case CardType::Vanish:
        {
            if (selectedcard->get_ApplyEffects())
            {
                Heroes *user = selectedcard->get_user_card();

                Player *user_player =
                    (p1->get_character() == Attacker ||
                    (!p1->get_comrade().empty() && p1->get_comrade()[0] == Attacker))
                        ? p1
                        : p2;

                user->set_Health(1, 15); 

                if (user->get_place() != nullptr)
                    user->get_place()->set_hero(nullptr);

                user->set_place(nullptr);
                user->set_PendingPlacement(true);

                if (user_player->get_count() == 0) 
                {
                    user_player->set_count(1); 
                }
            }
            break;
        }

    }
}}  

void CardResolver::TryDisableCard(Card *card, Player *p1, Player *p2)
{
    if (card == nullptr)
        return;

    if (p1 == nullptr || p2 == nullptr)
        return;

    std::string owner = card->get_owner();


    if (owner != "SHERLOCKHOLMES" && owner != "DR_WATSON")
    {
        card->set_ApplyEffects(false);
        return;
    }

    Player *Sherlock_Player = nullptr;

    if (p1->get_character() != nullptr && p1->get_character()->get_name() == "SHERLOCKHOLMES")
        Sherlock_Player = p1;
    else if (p2->get_character() != nullptr && p2->get_character()->get_name() == "SHERLOCKHOLMES")
        Sherlock_Player = p2;

    if (Sherlock_Player != nullptr)
    {
        Heroes *sherlock = Sherlock_Player->get_character();

        if (sherlock != nullptr && sherlock->get_islive())
        {
            return;
        }
    }

    card->set_ApplyEffects(false);
}


