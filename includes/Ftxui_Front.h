#pragma once
#include <stdexcept>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include "Enums.h"
#include "Player.h"
#include "Board.h"

#include <queue>
#include <set>


using namespace ftxui;

class Ftxui_Front
{
private:
    int number_of_choose = -1;

    std::vector<std::string> msg;

public:
    std::vector<std::string> &get_msg();

    int get_number_of_choose();
    void set_number_of_choose(int);

    enum ::e_Menu Menu_();

    void Players_Info_List(Player *, Player *);

    int Det_characters(Player *, Player *);

    void catch_place(Player *, Player *, Board *);

    
    Component ChooseAction(Player *, Player *, ScreenInteractive *);
    
    void main_map(Player *, Player *, Board *, Player *);

    bool AskUseSpecialAbility(Heroes *, Board *);
    
    bool AskBurnCardForMove(Heroes *, Board *);
    
    void choose_comrad_place(Player *, Player *, Board *);

    void Attakcer_Heroes_Menu(Player *, Board *, Heroes *&);
    void Defender_Heroes_Menu(Player *, Board *, Heroes *&, Heroes *&);

    void Attacker_selected_card(Heroes *, Heroes *, Player *, Player *, Board *, Card *&);
    void Defender_selected_card(Heroes *, Heroes *, Player *, Player *, Board *, Card *&);
    
    void Reveal_Combat(Heroes *, Heroes *, Card *, Card *);
    
    void put_in_any_space(Heroes *, Board *);        // Gives every fighter to every empty house.
    void Revive_Sister(Heroes *, Heroes *, Board *); // He revives the defeated sister and places her in every house.
    int DiscardCards(Heroes *);                      // Shows the fighter's hand so that the player can draw cards from it without restriction.
    void MoveHero(Heroes *, Board *, int);           // Moves the fighter up to a specified number of spaces.
    Heroes *SelectHero(Board *, Heroes *exclude = nullptr);
    void PlaceHeroAdjacent(Heroes *, Heroes *, Board *);
    void ShowHand(Heroes *, Player *, Player *, Board *);
    Card *ChooseCardFromHand(Player *, Player *, Player *, Board *); // The opponent's fighter is shown and the player chooses and burns one card from among them.
    
    void Event_Selected_Card(Heroes *, Player *, Player * p1, Player * p2, Board *, Card *&);

    void DeclareWinner(Heroes *);
    void Show_Help();

    void ChooseCardsToTopOfDeck(Heroes *, int, Player *, Player *, Board *);


};