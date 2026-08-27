#pragma once
#include <stdexcept>

#include "raylib.h"

#include "Enums.h"
#include "Player.h"
#include "Board.h"

#include <queue>
#include <set>


class Raylib
{
private:
    int number_of_choose = -1;

    std::vector<std::string> msg;
    Font gothicFont{};
    bool fontLoaded = false;    

public:

    Font &GetGameFont();
    void UnloadGameFont();

    std::vector<std::string> &get_msg();

    int get_number_of_choose();
    void set_number_of_choose(int);

    enum ::e_Menu Menu_();

    void Players_Info_List(Player *, Player *);

    std::vector<int> Det_characters(Player *, Player *);
    void catch_place(Player *, Player *, Board *);
    void ChooseAction(Player *, Player *);
    void main_map(Player *, Player *, Board *, Player *);

    void DrawHeroBox(Player *, Rectangle);
    void DrawBoardMap(Board *, Rectangle);


    bool AskUseSpecialAbility(Heroes *, Board *);
    bool AskBurnCardForMove(Heroes *, Board *);
    void choose_comrad_place(Player *, Player *, Board *);

    bool Attakcer_Heroes_Menu(Player *, Board *, Heroes *&);
    bool Defender_Heroes_Menu(Player *, Board *, Heroes *&, Heroes *&);

    bool Attacker_selected_card(Heroes *, Heroes *, Player *, Player *, Board *, Card *&);
    bool Defender_selected_card(Heroes *, Heroes *, Player *, Player *, Board *, Card *&);
    void Reveal_Combat(Heroes *, Heroes *, Card *, Card *);
    void put_in_any_space(Heroes *, Board *);        // Gives every fighter to every empty house.
    void Revive_Sister(Heroes *, Heroes *, Board *); // He revives the defeated sister and places her in every house.
    int DiscardCards(Heroes *);                      // Shows the fighter's hand so that the player can draw cards from it without restriction.
    void MoveHero(Heroes *, Board *, int, Player *, Player *); // Moves the fighter up to a specified number of spaces.
    Heroes *SelectHero(Board *, Heroes * = nullptr);
    void PlaceHeroAdjacent(Heroes *, Heroes *, Board *);
    void ShowHand(Heroes *, Player *, Player *, Board *);
    Card *ChooseCardFromHand(Player *, Player *, Player *, Board *); // The opponent's fighter is shown and the player chooses and burns one card from among them.
    bool Event_Selected_Card(Heroes *, Player *, Player * p1, Player * p2, Board *, Card *&);

    void DeclareWinner(Heroes *);
    void Show_Help();

    void ChooseCardsToTopOfDeck(Heroes *, int, Player *, Player *, Board *);
    bool ChooseCardToDiscardOrSkip(Player *);
    void MoveFogToken(Space *, Board *);
    Heroes *SelectComrade(Player *, Board *); // Shows the player's living comrades in a menu and returns the one chosen.

    Space *SelectFogToken(Board *);
    void MoveFogTokenDistance(Space *, Board *, int);

    void MoveHeroToFogSpace(Heroes *, Board *); // for LURKING effect card
    bool ChooseBetweenTwoEffects(std::string, std::string); // for LURKING effect card

    void MoveFogTokenAnywhere(Space *, Board *); // for ROLLING FOG effect card

    Space *MoveFogTokenToEmptySpace(Space *, Board *); // for SLIP AWAY effect card

    Heroes *SelectAdjacentHero(Heroes *, Board *); // for STEP LIGHTLY effect card

    void PlaceHeroOnBoard(Heroes *, Board *); // for VANISH effect card

    int SlotMenu(const std::vector<std::string> &, const std::string &);
};