#include "Ftxui_Front.h"
#include "Controller.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
using namespace ftxui;

int Ftxui_Front::get_number_of_choose()
{
    return number_of_choose;
}

void Ftxui_Front::set_number_of_choose(int amount)
{
    number_of_choose = amount;
}

enum ::e_Menu Ftxui_Front::Menu_()
{
    auto screen = ScreenInteractive::Fullscreen();

    std::vector<std::string> Menu = {
        "Play",
        "Help",
        "Exit"};

    int selected = 0;

    auto menu = ftxui::Menu(&Menu, &selected);

    auto component = CatchEvent(menu, [&](Event event)
                                {
            if (event == Event::Return) {
                if (selected == 0) {
                    screen.ExitLoopClosure()();
                    return true;
                }
                
                else if (selected == 1) {
                    screen.ExitLoopClosure()();
                    return true;
                }
                
                else if (selected == 2) {
                    screen.ExitLoopClosure()();
                    screen.Exit();
                    return true;
                }
            }
            return false; });

    auto renderer = Renderer(component, [&]
                             { return vbox({text("UNMATCHED") | bold | center,
                                            separator(),
                                            component->Render()}) |
                                      border; });

    screen.Loop(renderer);

    switch (selected)
    {
    case 0:
        return e_Menu::Play;
        break;

    case 1:
        return e_Menu::Help;
        break;

    case 2:
        return e_Menu::Exit;
        break;
    }
}

void Ftxui_Front::Players_Info_List(Player *p1, Player *p2)
{
    
    string name_p1;
    string age_p1;
    
    string name_p2;
    string age_p2;

    auto screen = ScreenInteractive::Fullscreen();

    auto complet_button = Button("Completed", [&]
                                 { screen.Exit(); }, ButtonOption::Simple()) |
                                 color(Color::Green);

    auto Input_name_p1 = Input(&name_p1, "Name the player 1 : ");
    auto Input_age_p1 = Input(&age_p1, "age the player 1 : ");
    
    auto Input_name_p2 = Input(&name_p2, "Name the player 2 : ");
    auto Input_age_p2 = Input(&age_p2, "age the player 2 : ");

    auto main_container = Container::Vertical({
        Input_name_p1,
        Input_age_p1,
        Input_name_p2,
        Input_age_p2,
        complet_button,
    });

    auto renderer = Renderer(main_container, [&]
                             { return vbox({hbox({vbox({text("Player 1") | bold | center, separator(),
                                                        Input_name_p1->Render(), Input_age_p1->Render()}) |
                                                      flex,
                                                  separator(),
                                                  vbox({text("Player 2") | bold | center, separator(),
                                                    Input_name_p2->Render(), Input_age_p2->Render()}) |
                                                    flex}) |
                                                    flex,
                                                    separator(),
                                            complet_button->Render() | center}) |
                                            borderRounded; });
                                            
                                            auto component = CatchEvent(renderer, [&](Event event)
                                            { return complet_button->OnEvent(event); });
                                        
    
    screen.Loop(component);

    p1->set_name(name_p1), p1->set_age(stoi(age_p1));
    p2->set_name(name_p2), p2->set_age(stoi(age_p2));

}

int Ftxui_Front::Det_characters(Player *p1, Player *p2)
{

    Player* younger = (p1->get_age() <= p2->get_age()) ? p1 : p2;

    std::vector<std::string> entries = {
        "DRACULA",
        "SHERLOCK HOLMES"
    };

    int selected = 0;

    auto screen = ScreenInteractive::Fullscreen();

    auto menu = Menu(&entries, &selected);

    auto confirm = Button("Confirm", [&] {
        screen.Exit();
    });

    auto container = Container::Vertical({
        menu,
        confirm,
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
            text(younger->get_name() + " choose your hero") | bold | center,
            separator(),
            menu->Render(),
            separator(),
            confirm->Render() | center,
        }) | border;
    });

    screen.Loop(renderer);

    return selected;
}

void Ftxui_Front::catch_place(Player *p1, Player *p2, Board *board)
{
    int selected = 0;
    auto screen = ScreenInteractive::Fullscreen();

    vector<string> places = {"space 3", "space 24"};

    auto menu = ftxui::Menu(&places, &selected);

    auto component = CatchEvent(menu, [&](Event event)
                                {
    if (event == Event::Return) {
        screen.ExitLoopClosure()(); 
        return true;
    }
    return false; });

    auto container = Container::Vertical({
        component,
    });

    auto renderer = Renderer(container, [&]
                             { return vbox({
                                          text((p1->get_age() <= p2->get_age() ? p1->get_name() : p2->get_name()) + " Choose your Hero place") | border | center,
                                          container->Render(),
                                      }) |
                                      border; });
    screen.Loop(renderer);

    if (selected == 0)
    {
        (p1->get_age() <= p2->get_age() ? p1 : p2)->get_character()->set_place(&board->get_spaces()[2]);
        board->get_spaces()[2].set_hero((p1->get_age() <= p2->get_age() ? p1 : p2)->get_character());

        (p1->get_age() > p2->get_age() ? p1 : p2)->get_character()->set_place(&board->get_spaces()[23]);
        board->get_spaces()[23].set_hero((p1->get_age() > p2->get_age() ? p1 : p2)->get_character());
    }

    else if (selected == 1)
    {
        (p1->get_age() <= p2->get_age() ? p1 : p2)->get_character()->set_place(&board->get_spaces()[23]);
        board->get_spaces()[23].set_hero((p1->get_age() <= p2->get_age() ? p1 : p2)->get_character());

        (p1->get_age() > p2->get_age() ? p1 : p2)->get_character()->set_place(&board->get_spaces()[2]);
        board->get_spaces()[2].set_hero((p1->get_age() > p2->get_age() ? p1 : p2)->get_character());
    }
}


Element Dracula_Box(Player *p1, Player *p2)
{

    Player *dracula_player = (p1->get_character()->get_name() == "DRACULA") ? p1 : p2;
    return vbox({
               text("DRACULA\n") | bold | color(Color::Red1),
               text("Health : " + to_string(dracula_player->get_character()->get_Health()) + " / 13"),
               text("Move : " + to_string(dracula_player->get_character()->get_Movement())),
               text("card in hand : " + to_string(dracula_player->get_character()->get_hand().size())),
               text("card in deck : " + to_string(dracula_player->get_character()->get_deck().size())),
               text("card in discard : " + to_string(dracula_player->get_character()->get_discard().size())),
           }) |
           border;
}

Element Sherlock_Box(Player *p1, Player *p2)
{
    Player *sherlock_player = (p1->get_character()->get_name() == "SHERLOCKHOLMES") ? p1 : p2;

    return vbox({
               text("SHERLOCK\n") | bold | color(Color::Blue1),
               text("Health : " + to_string(sherlock_player->get_character()->get_Health()) + " / 16"),
               text("Move : " + to_string(sherlock_player->get_character()->get_Movement())),
               text("card in hand : " + to_string(sherlock_player->get_character()->get_hand().size())),
               text("card in deck : " + to_string(sherlock_player->get_character()->get_deck().size())),
               text("card in discard : " + to_string(sherlock_player->get_character()->get_discard().size())),
           }) |
           border;
}

Element Graph_Box(vector<Space> &spaces)
{
    auto c = Canvas(200, 65);

    c.DrawPointLine(9, 6, 31, 6, Color::White); // 1
    c.DrawPointLine(9, 6, 9, 19, Color::White); // 1

    c.DrawPointLine(31, 6, 50, 14, Color::White); // 2

    c.DrawPointLine(50, 14, 54, 25, Color::White); // 3
    c.DrawPointLine(50, 14, 73, 6, Color::White);  // 3

    c.DrawPointLine(54, 25, 31, 19, Color::White); // 4
    c.DrawPointLine(54, 25, 31, 20, Color::White); // 4

    c.DrawPointLine(31, 19, 9, 19, Color::White); // 5

    c.DrawPointLine(9, 19, 9, 31, Color::White); // 6

    c.DrawPointLine(31, 20, 9, 31, Color::White); // 7

    c.DrawPointLine(9, 31, 10, 38, Color::White); // 8

    c.DrawPointLine(10, 38, 51, 37, Color::White); // 9
    c.DrawPointLine(10, 38, 19, 48, Color::White); // 9

    c.DrawPointLine(51, 37, 75, 33, Color::White); // 10

    c.DrawPointLine(75, 33, 92, 29, Color::White); // 11
    c.DrawPointLine(75, 33, 81, 44, Color::White); // 11

    c.DrawPointLine(92, 29, 127, 33, Color::White); // 12

    c.DrawPointLine(127, 33, 107, 48, Color::White); // 13
    c.DrawPointLine(127, 33, 130, 14, Color::White); // 13
    c.DrawPointLine(127, 33, 157, 26, Color::White); // 13
    c.DrawPointLine(127, 33, 148, 33, Color::White); // 13
    c.DrawPointLine(127, 33, 130, 55, Color::White); // 13

    c.DrawPointLine(81, 44, 62, 49, Color::White);  // 14
    c.DrawPointLine(81, 44, 81, 56, Color::White);  // 14
    c.DrawPointLine(81, 44, 107, 48, Color::White); // 14

    c.DrawPointLine(19, 48, 41, 56, Color::White); // 15

    c.DrawPointLine(41, 56, 62, 49, Color::White); // 16

    c.DrawPointLine(62, 49, 81, 56, Color::White); // 17

    c.DrawPointLine(81, 56, 107, 48, Color::White); // 18

    c.DrawPointLine(107, 48, 131, 55, Color::White); // 19

    c.DrawPointLine(130, 55, 150, 66, Color::White); // 20

    c.DrawPointLine(150, 66, 164, 48, Color::White); // 21

    c.DrawPointLine(164, 48, 164, 33, Color::White); // 22

    c.DrawPointLine(164, 33, 148, 33, Color::White); // 23
    c.DrawPointLine(164, 33, 157, 26, Color::White); // 23

    c.DrawPointLine(157, 26, 157, 14, Color::White); // 25

    c.DrawPointLine(157, 14, 157, 6, Color::White); // 26

    c.DrawPointLine(179, 6, 157, 6, Color::White); // 27

    c.DrawPointLine(157, 6, 130, 14, Color::White); // 28

    c.DrawPointLine(130, 14, 107, 6, Color::White); // 29

    c.DrawPointLine(107, 6, 92, 14, Color::White); // 30
    c.DrawPointLine(107, 6, 73, 6, Color::White);  // 30

    c.DrawPointLine(92, 14, 73, 6, Color::White); // 31

    // ---- homes ----

    c.DrawPointCircle(9, 6, 3, Color::Cyan);
    c.DrawText(8, 5, (spaces[0].get_hero() == nullptr ? "1" : (spaces[0].get_hero()->get_number() == 0 ? "" : to_string(spaces[0].get_hero()->get_number())) + (spaces[0].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(31, 6, 3, Color::Cyan);
    c.DrawText(30, 5, (spaces[1].get_hero() == nullptr ? "2" : (spaces[1].get_hero()->get_number() == 0 ? "" : to_string(spaces[1].get_hero()->get_number())) + (spaces[1].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(50, 14, 3, Color::Cyan);
    c.DrawText(49, 13, (spaces[2].get_hero() == nullptr ? "3" : (spaces[2].get_hero()->get_number() == 0 ? "" : to_string(spaces[2].get_hero()->get_number())) + (spaces[2].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(54, 25, 3, Color::Cyan);
    c.DrawText(53, 24, (spaces[3].get_hero() == nullptr ? "4" : (spaces[3].get_hero()->get_number() == 0 ? "" : to_string(spaces[3].get_hero()->get_number())) + (spaces[3].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(31, 19, 3, Color::Cyan);
    c.DrawText(30, 18, (spaces[4].get_hero() == nullptr ? "5" : (spaces[4].get_hero()->get_number() == 0 ? "" : to_string(spaces[4].get_hero()->get_number())) + (spaces[4].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(9, 19, 3, Color::Cyan);
    c.DrawText(8, 18, (spaces[5].get_hero() == nullptr ? "6" : (spaces[5].get_hero()->get_number() == 0 ? "" : to_string(spaces[5].get_hero()->get_number())) + (spaces[5].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(31, 20, 3, Color::Cyan);
    c.DrawText(30, 19, (spaces[6].get_hero() == nullptr ? "7" : (spaces[6].get_hero()->get_number() == 0 ? "" : to_string(spaces[6].get_hero()->get_number())) + (spaces[6].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(9, 31, 3, Color::Cyan);
    c.DrawText(8, 30, (spaces[7].get_hero() == nullptr ? "8" : (spaces[7].get_hero()->get_number() == 0 ? "" : to_string(spaces[7].get_hero()->get_number())) + (spaces[7].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(10, 38, 3, Color::Cyan);
    c.DrawText(9, 37, (spaces[8].get_hero() == nullptr ? "9" : (spaces[8].get_hero()->get_number() == 0 ? "" : to_string(spaces[8].get_hero()->get_number())) + (spaces[8].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(51, 37, 3, Color::Cyan);
    c.DrawText(50, 36, (spaces[9].get_hero() == nullptr ? "10" : (spaces[9].get_hero()->get_number() == 0 ? "" : to_string(spaces[9].get_hero()->get_number())) + (spaces[9].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(75, 33, 3, Color::Cyan);
    c.DrawText(74, 32, (spaces[10].get_hero() == nullptr ? "11" : (spaces[10].get_hero()->get_number() == 0 ? "" : to_string(spaces[10].get_hero()->get_number())) + (spaces[10].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(92, 29, 3, Color::Cyan);
    c.DrawText(91, 28, (spaces[11].get_hero() == nullptr ? "12" : (spaces[11].get_hero()->get_number() == 0 ? "" : to_string(spaces[11].get_hero()->get_number())) + (spaces[11].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(127, 33, 3, Color::Cyan);
    c.DrawText(126, 32, (spaces[12].get_hero() == nullptr ? "13" : (spaces[12].get_hero()->get_number() == 0 ? "" : to_string(spaces[12].get_hero()->get_number())) + (spaces[12].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(81, 44, 3, Color::Cyan);
    c.DrawText(80, 43, (spaces[13].get_hero() == nullptr ? "14" : (spaces[13].get_hero()->get_number() == 0 ? "" : to_string(spaces[13].get_hero()->get_number())) + (spaces[13].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(19, 48, 3, Color::Cyan);
    c.DrawText(18, 47, (spaces[14].get_hero() == nullptr ? "15" : (spaces[14].get_hero()->get_number() == 0 ? "" : to_string(spaces[14].get_hero()->get_number())) + (spaces[14].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(41, 56, 3, Color::Cyan);
    c.DrawText(40, 55, (spaces[15].get_hero() == nullptr ? "16" : (spaces[15].get_hero()->get_number() == 0 ? "" : to_string(spaces[15].get_hero()->get_number())) + (spaces[15].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(62, 49, 3, Color::Cyan);
    c.DrawText(61, 48, (spaces[16].get_hero() == nullptr ? "17" : (spaces[16].get_hero()->get_number() == 0 ? "" : to_string(spaces[16].get_hero()->get_number())) + (spaces[16].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(81, 56, 3, Color::Cyan);
    c.DrawText(80, 55, (spaces[17].get_hero() == nullptr ? "18" : (spaces[17].get_hero()->get_number() == 0 ? "" : to_string(spaces[17].get_hero()->get_number())) + (spaces[17].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(107, 48, 3, Color::Cyan);
    c.DrawText(106, 47, (spaces[18].get_hero() == nullptr ? "19" : (spaces[18].get_hero()->get_number() == 0 ? "" : to_string(spaces[18].get_hero()->get_number())) + (spaces[18].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(130, 55, 3, Color::Cyan);
    c.DrawText(129, 54, (spaces[19].get_hero() == nullptr ? "20" : (spaces[19].get_hero()->get_number() == 0 ? "" : to_string(spaces[19].get_hero()->get_number())) + (spaces[19].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(150, 60, 3, Color::Cyan);
    c.DrawText(149, 59, (spaces[20].get_hero() == nullptr ? "21" : (spaces[20].get_hero()->get_number() == 0 ? "" : to_string(spaces[20].get_hero()->get_number())) + (spaces[20].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(164, 48, 3, Color::Cyan);
    c.DrawText(163, 47, (spaces[21].get_hero() == nullptr ? "22" : (spaces[21].get_hero()->get_number() == 0 ? "" : to_string(spaces[21].get_hero()->get_number())) + (spaces[21].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(164, 33, 3, Color::Cyan);
    c.DrawText(163, 32, (spaces[22].get_hero() == nullptr ? "23" : (spaces[22].get_hero()->get_number() == 0 ? "" : to_string(spaces[22].get_hero()->get_number())) + (spaces[22].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(148, 33, 3, Color::Cyan);
    c.DrawText(147, 32, (spaces[23].get_hero() == nullptr ? "24" : (spaces[23].get_hero()->get_number() == 0 ? "" : to_string(spaces[23].get_hero()->get_number())) + (spaces[23].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(157, 26, 3, Color::Cyan);
    c.DrawText(156, 25, (spaces[24].get_hero() == nullptr ? "25" : (spaces[24].get_hero()->get_number() == 0 ? "" : to_string(spaces[24].get_hero()->get_number())) + (spaces[24].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(157, 14, 3, Color::Cyan);
    c.DrawText(156, 13, (spaces[25].get_hero() == nullptr ? "26" : (spaces[25].get_hero()->get_number() == 0 ? "" : to_string(spaces[25].get_hero()->get_number())) + (spaces[25].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(179, 6, 3, Color::Cyan);
    c.DrawText(178, 5, (spaces[26].get_hero() == nullptr ? "27" : (spaces[26].get_hero()->get_number() == 0 ? "" : to_string(spaces[26].get_hero()->get_number())) + (spaces[26].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(157, 6, 3, Color::Cyan);
    c.DrawText(156, 5, (spaces[27].get_hero() == nullptr ? "28" : (spaces[27].get_hero()->get_number() == 0 ? "" : to_string(spaces[27].get_hero()->get_number())) + (spaces[27].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(130, 14, 3, Color::Cyan);
    c.DrawText(129, 13, (spaces[28].get_hero() == nullptr ? "29" : (spaces[28].get_hero()->get_number() == 0 ? "" : to_string(spaces[28].get_hero()->get_number())) + (spaces[28].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(107, 6, 3, Color::Cyan);
    c.DrawText(106, 5, (spaces[29].get_hero() == nullptr ? "30" : (spaces[29].get_hero()->get_number() == 0 ? "" : to_string(spaces[29].get_hero()->get_number())) + (spaces[29].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(92, 14, 3, Color::Cyan);
    c.DrawText(91, 13, (spaces[30].get_hero() == nullptr ? "31" : (spaces[30].get_hero()->get_number() == 0 ? "" : to_string(spaces[30].get_hero()->get_number())) + (spaces[30].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(73, 6, 3, Color::Cyan);
    c.DrawText(72, 5, (spaces[31].get_hero() == nullptr ? "32" : (spaces[31].get_hero()->get_number() == 0 ? "" : to_string(spaces[31].get_hero()->get_number())) + (spaces[31].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    return canvas(std::move(c));
}

bool Ftxui_Front::AskUseSpecialAbility(Heroes *hero, Board *board)
{
    std::vector<std::string> entries = {"Yes", "No"};
    int selected = 0;

    auto menu = Menu(&entries, &selected);

    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    auto component = CatchEvent(menu,
                                [&](Event event)
                                {
                                    if (event == Event::Return)
                                    {
                                        screen.Exit();
                                        return true;
                                    }

                                    return false;
                                });

    auto renderer = Renderer(component,
                             [&]
                             {
                                 return hbox({Graph_Box(board->get_spaces()),
                                              separator(),
                                              window(
                                                  text(hero->get_name() + " - Use Special Ability?"),
                                                  component->Render())});
                             });

    screen.Loop(renderer);

    return selected == 0; // 0 = Yes, 1 = No
}


std::string CardTypeToString(CardType card)
{
    switch (card)
    {
    case CardType::Feedingfrenzy:
        return "Feeding ";

    case CardType::Mistform:
        return "Mist";

    case CardType::Ambush:
        return "Ambush";

    case CardType::Baptism_of_blood:
        return "Baptism";

    case CardType::Beastform:
        return "Beast";

    case CardType::Dash:
        return "Dash";

    case CardType::Exploit:
        return "Exploit";

    case CardType::Look_into_my_eyes:
        return "Look_e";

    case CardType::Prey_upon:
        return "Prey    ";

    case CardType::Ravening_Seduction:
        return "Ravening";

    case CardType::Thirst_for_sustenance:
        return "Thirst";

    case CardType::Feint:
        return "Feint";

    case CardType::Administer_Aid:
        return "Aid";

    case CardType::Counterpunch:
        return "Counter";

    case CardType::Deduce_Strategy:
        return "Deduce";

    case CardType::Education_Never_Ends:
        return "Education";

    case CardType::Eliminate_The_Impossible:
        return "Eliminate";

    case CardType::Feint2:
        return "Feint";

    case CardType::Fixed_Point_in_a_Changing_Age:
        return "Fixed";

    case CardType::The_Game_Is_Afoot:
        return "T_Game";

    case CardType::Service_Revolver:
        return "Service";

    case CardType::Study_Methods:
        return "Study";
    }
}

std::string CardTimingToString(CardTiming card)
{
    switch (card)
    {
    case CardTiming::After:
        return "After";

    case CardTiming::Before:
        return "Before";

    case CardTiming::During:
        return "During";

    case CardTiming::Immediate:
        return "Immediate";
    }
}

Element Dracula_Hand(Player *p1, Player *p2)
{
    Player *dracula_player =
        (p1->get_character()->get_name() == "DRACULA") ? p1 : p2;

    vector<Card> Hand_Cards = dracula_player->get_character()->get_hand();

    Elements cards;

    for (int i = 0; i < Hand_Cards.size(); i++)
    {
        cards.push_back(
            vbox({
                text("[ " + Hand_Cards[i].get_Attacktype() + " ]") | center,
                separator(),
                text(CardTypeToString(Hand_Cards[i].get_CardType())) | center,
                text(Hand_Cards[i].get_owner()) | center,
                text("Boost: " + to_string(Hand_Cards[i].get_Boost())) | center,
                text(CardTimingToString(Hand_Cards[i].get_CardTiming())) | center,
            }) |
            border |
            color(Color::RGB(0, 100, 0)));
    }

    return vbox({
        text("Dracula - Hand") | bold | center,
        separator(),
        hbox(std::move(cards)),
    });
}

Component Ftxui_Front::ChooseAction(Player *p1, Player *p2, ScreenInteractive *screen)
{

    static std::vector<std::string> entries = {
        "Attack",
        "Maneuver",
        "Event",
        "Back"};

    static int selected = 0;

    auto menu = Menu(&entries, &selected);

    auto component = CatchEvent(menu, [this, screen](Event event)
                                {
            if (event == Event::Return) {
                number_of_choose = selected;

                    screen->ExitLoopClosure()();
                    return true;
            }
            return false; });

    // auto renderer = Renderer(component, [&]
    //                          { return vbox({
    //                                       text("Choose Action") | bold | center,
    //                                       separator(),
    //                                       component->Render(),
    //                                   }) |
    //                                   border; });

    return component;
}

Element Sherlock_Hand(Player *p1, Player *p2)
{
    Player *sherlock_player =
        (p1->get_character()->get_name() == "SHERLOCKHOLMES") ? p1 : p2;

    vector<Card> Hand_Cards = sherlock_player->get_character()->get_hand();

    Elements cards;

    for (int i = 0; i < Hand_Cards.size(); i++)
    {
        cards.push_back(
            vbox({
                text("[ " + Hand_Cards[i].get_Attacktype() + " ]") | center,
                separator(),
                text(CardTypeToString(Hand_Cards[i].get_CardType())) | center,
                text(Hand_Cards[i].get_owner()) | center,
                text("Boost: " + std::to_string(Hand_Cards[i].get_Boost())) | center,
                text(CardTimingToString(Hand_Cards[i].get_CardTiming())) | center,
            }) |
            border | color(Color::RGB(0, 100, 0)));
    }

    return vbox({
        text("Sherlock - Hand") | bold | center,
        separator(),
        hbox(std::move(cards)),
    });
}

void Ftxui_Front::choose_comrad_place(Player *p1, Player *p2, Board *board)
{
    Player *dracula_player = (p1->get_character()->get_name() == "DRACULA") ? p1 : p2;
    Player *sherlock_player = (p1->get_character()->get_name() == "SHERLOCKHOLMES") ? p1 : p2;

    {

        auto screen = ScreenInteractive::Fullscreen();

        vector<Space *> zone = dracula_player->get_character()->get_place()->get_zone();

        if (zone.empty())
        {
            throw std::runtime_error("AllowHand is empty");
            return;
        }

        vector<string> entries;

        for (auto s : zone)
            entries.push_back("Space " + to_string(s->get_number()));

        int selected = 0;
        int confirmed = 0;
        vector<int> choices;

        auto menu = ftxui::Menu(&entries, &selected);

        auto confirm_btn = Button("Confirm", [&]
                                  {
                bool duplicate = false;
                for(auto c : choices) if(c == selected) duplicate = true; // check duplicate
                
                if(!duplicate) {
                    choices.push_back(selected);
                    confirmed++;
                }

                if(confirmed == 3) {
                    for(int i = 0; i < 3; i++)
                    {
                        dracula_player->get_comrade()[i]->set_place(zone[choices[i]]);
                        zone[choices[i]]->set_hero(dracula_player->get_comrade()[i]);
                    }
                    screen.ExitLoopClosure()();
                } });

        auto container = Container::Vertical({menu, confirm_btn});

        auto renderer = Renderer(container, [&]
                                 { return vbox({
                                       hbox({
                                           vbox({
                                               text(dracula_player->get_name() + " - Choose 3 spaces for sisters") | bold | center,
                                               separator(),
                                               text("Selected: " + to_string(confirmed) + " / 3") | color(confirmed == 3 ? Color::Green : Color::Red) | center,
                                               separator(),
                                               menu->Render(),
                                               separator(),
                                               confirm_btn->Render() | center,
                                           }) | border |
                                               size(WIDTH, EQUAL, 30),
                                           Graph_Box(board->get_spaces()),
                                       }),
                                   }); });

        screen.Loop(renderer);
    }

    {
        auto screen = ScreenInteractive::Fullscreen();

        vector<Space *> zone = sherlock_player->get_character()->get_place()->get_zone();

        if (zone.empty())
        {
            throw std::runtime_error("AllowHand is empty");
            return;
        }

        vector<string> entries;
        for (auto s : zone)
            entries.push_back("Space " + to_string(s->get_number()));

        int selected = 0;
        auto menu = ftxui::Menu(&entries, &selected);
        auto confirm_btn = Button("Confirm", [&]
                                  {
                sherlock_player->get_comrade()[0]->set_place(zone[selected]);
                zone[selected]->set_hero(sherlock_player->get_comrade()[0]);

                screen.ExitLoopClosure()(); });

        auto container = Container::Vertical({menu, confirm_btn});

        auto renderer = Renderer(container, [&]
                                 { return vbox({
                                       //    text("turn : ") | bold | center | border,
                                       hbox({
                                           vbox({
                                               text(sherlock_player->get_name() + " - Choose 1 space for Watson") | bold | center,
                                               separator(),
                                               menu->Render(),
                                               separator(),
                                               confirm_btn->Render() | center,
                                           }) | border |
                                               size(WIDTH, EQUAL, 30),
                                           Graph_Box(board->get_spaces()),
                                       }),
                                   }); });

        screen.Loop(renderer);
    }
}

void Ftxui_Front::Attakcer_Heroes_Menu(Player *player, Board *board, Heroes *&hero)
{
    auto screen = ScreenInteractive::Fullscreen();

    vector<Heroes *> fighters;

    if (player->get_character() != nullptr and player->get_character()->get_islive())
        fighters.push_back(player->get_character());

    for (auto c : player->get_comrade())
    {
        if (c != nullptr and c->get_islive())
            fighters.push_back(c);
    }

    if (fighters.empty())
    {
        throw std::runtime_error("No valid target to attack.");
        return;
    }
    vector<string> entries;

    for (auto h : fighters)
        entries.push_back((h->get_number() == 0 ? "" : to_string(h->get_number())) + h->get_name());

    int selected = 0;

    auto menu = Menu(&entries, &selected);

    auto confirm = Button("Confirm", [&]
                          { 
                            hero = fighters[selected];
                            
                            screen.ExitLoopClosure()(); });

    auto container = Container::Vertical({
        menu,
        confirm,
    });

    auto renderer = Renderer(container, [&]
                             { return hbox({

                                   vbox({
                                       text(player->get_name() + " Choose Attacker") | bold | center,
                                       separator(),
                                       menu->Render(),
                                       separator(),
                                       confirm->Render() | center,
                                   }) | border |
                                       size(WIDTH, EQUAL, 30),

                                   Graph_Box(board->get_spaces()),

                               }); });

    screen.Loop(renderer);
}

bool Exist_path(vector<Space *> zone, Space *target)
{
    if (zone.empty())
    {
        throw "zone (path) is empty.";
    }

    else
    {
        for (auto const &z : zone)
        {
            if (z == target)
                return true;
        }
        return false;
    }
}

void Ftxui_Front::Defender_Heroes_Menu(Player *player, Board *board, Heroes *&defender, Heroes *&Attacker)
{
    cout << "enter the defender heroes emnu\n";

    if (Attacker == nullptr)
    {
        throw runtime_error("Attacker is nullptr");
    }

    else
    {
        auto screen = ScreenInteractive::Fullscreen();

        vector<Heroes *> defenders;

        try
        {
            if (Attacker->get_Attacktype() == "MELEE")
            {
                if (player->get_character() != nullptr and player->get_character()->get_islive() and Exist_path(Attacker->get_place()->get_neighbor(), player->get_character()->get_place()))
                    defenders.push_back(player->get_character());

                for (auto c : player->get_comrade())
                {
                    if (c != nullptr and c->get_islive() and Exist_path(Attacker->get_place()->get_neighbor(), c->get_place()))
                        defenders.push_back(c);
                }
            }
            else if (Attacker->get_Attacktype() == "RANGED")
            {

                if (player->get_character() != nullptr and player->get_character()->get_islive() and Exist_path(Attacker->get_place()->get_zone(), player->get_character()->get_place()))
                    defenders.push_back(player->get_character());

                for (auto c : player->get_comrade())
                {
                    if (c != nullptr and c->get_islive() and Exist_path(Attacker->get_place()->get_zone(), c->get_place()))
                        defenders.push_back(c);
                }
            }
        }
        catch (const char *m)
        {
            msg.push_back(m);
        }

        if (defenders.empty())
        {
            throw std::runtime_error("Attack is not possible.");
            return;
        }

        vector<string> entries;

        for (auto h : defenders)
            entries.push_back((h->get_number() == 0 ? "" : to_string(h->get_number())) + h->get_name());

        int selected = 0;

        auto menu = Menu(&entries, &selected);

        auto confirm = Button("Confirm", [&]
                              { 
                defender = defenders[selected];
                
                screen.ExitLoopClosure()(); });

        auto container = Container::Vertical({
            menu,
            confirm,
        });

        auto renderer = Renderer(container, [&]
                                 { return hbox({

                                       vbox({
                                           text(player->get_name() + " Choose Defender") | bold | center,
                                           separator(),
                                           menu->Render(),
                                           separator(),
                                           confirm->Render() | center,
                                       }) | border |
                                           size(WIDTH, EQUAL, 30),

                                       Graph_Box(board->get_spaces()),

                                   }); });

        screen.Loop(renderer);
    }
    cout << "exit the defender heroes emnu\n";
}

void Ftxui_Front::main_map(Player *p1, Player *p2, Board *board, Player *turn)
{
    cout << "enter amin map...\n";

    auto screen = ScreenInteractive::Fullscreen();

    auto container = Container::Vertical({
        ChooseAction(p1, p2, &screen),
    });


    auto renderer = Renderer(container, [&]
                             { return vbox({
                                   vbox({
                                       text("turn : " + turn->get_name()) | bold | center | border,
                                       hbox({
                                           Dracula_Box(p1, p2),
                                           Graph_Box(board->get_spaces()),
                                           Sherlock_Box(p1, p2),
                                       }) | center,
                                   }),

                                   vbox({
                                       hbox({
                                           Dracula_Hand(p1, p2),
                                           vbox({
                                               text("   Choose Action  ") | bold | color(Color::Khaki1),
                                               separatorHeavy(),

                                               container->Render() | center,

                                           }),
                                           Sherlock_Hand(p1, p2),
                                       }),

                                   }) | center,
                               }); });

    screen.Loop(renderer);

    cout << "exit amin map...\n";
}

std::vector<std::string> &Ftxui_Front::get_msg()
{
    return msg;
}

void Ftxui_Front::Attacker_selected_card(Heroes *Attacker, Heroes *Defender, Player *p1, Player *p2, Board *board, Card *&Attacker_Card)
{
    vector<Card *> AllowHand;

    for (auto &c : Attacker->get_hand())
    {
        if ((c.get_owner() == Attacker->get_name() or c.get_owner() == "ANY") and (c.get_Attacktype() == "Attack" or c.get_Attacktype() == "Both"))
        {
            AllowHand.push_back(&c);
        }
    }

    if (AllowHand.empty())
    {
        throw std::runtime_error("AllowHand is empty");
        msg.push_back("AllowHand is empty");

        return; // ................
    }

    vector<string> entries;
    static int selected = 0;

    for (auto *c : AllowHand)
        entries.push_back(CardTypeToString(c->get_CardType()));

    auto screen = ScreenInteractive::Fullscreen();

    auto menu = Menu(&entries, &selected);

    auto confirm = Button("Confirm", [&]
                          { screen.ExitLoopClosure()(); });

    auto left = Renderer(Container::Vertical({
                             menu,
                             confirm,
                         }),
                         [&]
                         {
                             return vbox({
                                        text(Attacker->get_name() + " Choose Card for Arrack") | bold | center,
                                        separator(),
                                        menu->Render() | border,
                                        filler(),
                                        confirm->Render() | center,
                                    }) |
                                    size(WIDTH, EQUAL, 30);
                         });

    auto graph = Graph_Box(board->get_spaces());

    Element hand;

    if (Attacker->get_name() == "DRACULA" or
        Attacker->get_name() == "SISTER" or
        Attacker->get_name() == "SISTER" or
        Attacker->get_name() == "SISTER")

        hand = Dracula_Hand(p1, p2);

    else if (Attacker->get_name() == "SHERLOCKHOLMES" or Attacker->get_name() == "Dr_Watson")
        hand = Sherlock_Hand(p1, p2);

    auto renderer = Renderer(Container::Horizontal({
                                 left,
                             }),
                             [&]
                             {
                                 return hbox({
                                     left->Render(),
                                     separator(),
                                     vbox({
                                         graph,
                                         hand,
                                     }),
                                 });
                             });

    screen.Loop(renderer);

    Attacker_Card = AllowHand[selected];

    if (p1->get_character()->get_name() == Attacker->get_name() or p1->get_comrade()[0]->get_name() == Attacker->get_name())
    {
        p1->set_selected_card(AllowHand.at(selected));
    }
    else
        p2->set_selected_card(AllowHand.at(selected));
}

void Ftxui_Front::Defender_selected_card(Heroes *Attacker, Heroes *Defender, Player *p1, Player *p2, Board *board, Card *&Defender_Card)
{
    Heroes *CardOwner = Defender;

    if (Defender->get_name() == "Dr_Watson")
        CardOwner = p1->get_character()->get_name() == "SHERLOCKHOLMES"
                        ? p1->get_character()
                        : p2->get_character();

    else if (Defender->get_name() == "SISTER" ||
             Defender->get_name() == "SISTER" ||
             Defender->get_name() == "SISTER")

        CardOwner = p1->get_character()->get_name() == "DRACULA"
                        ? p1->get_character()
                        : p2->get_character();

    vector<Card *> AllowHand;

    for (auto &c : CardOwner->get_hand()) // Defender hand
    {
        if ((c.get_owner() == Defender->get_name() or c.get_owner() == "ANY") and (c.get_Attacktype() == "Defense" or c.get_Attacktype() == "Both"))
        {
            AllowHand.push_back(&c);
        }
    }

    vector<string> entries;
    static int selected = 0;

    for (auto *c : AllowHand)
        entries.push_back(CardTypeToString(c->get_CardType()));

    entries.push_back("No Defense");

    auto screen = ScreenInteractive::Fullscreen();

    auto menu = Menu(&entries, &selected);

    auto confirm = Button("Confirm", [&]
                          { screen.ExitLoopClosure()(); });

    auto left = Renderer(Container::Vertical({
                             menu,
                             confirm,
                         }),
                         [&]
                         {
                             return vbox({
                                        text(Defender->get_name() + " Choose Card for Defense") | bold | center,
                                        separator(),
                                        menu->Render() | border,
                                        filler(),
                                        confirm->Render() | center,
                                    }) |
                                    size(WIDTH, EQUAL, 30);
                         });

    auto graph = Graph_Box(board->get_spaces());

    Element hand;

    if (Defender->get_name() == "DRACULA" or
        Defender->get_name() == "SISTER" or
        Defender->get_name() == "SISTER" or
        Defender->get_name() == "SISTER")

        hand = Dracula_Hand(p1, p2);

    else if (Defender->get_name() == "SHERLOCKHOLMES" or Defender->get_name() == "Dr_Watson")
        hand = Sherlock_Hand(p1, p2);

    auto renderer = Renderer(Container::Horizontal({
                                 left,
                             }),
                             [&]
                             {
                                 return hbox({
                                     left->Render(),
                                     separator(),
                                     vbox({
                                         graph,
                                         hand,
                                     }),
                                 });
                             });

    screen.Loop(renderer);

    if (entries[selected] == "No Defense")
    {
        Defender_Card = nullptr;

        if (p1->get_character()->get_name() == Defender->get_name() or p1->get_comrade()[0]->get_name() == Defender->get_name())
        {
            p1->set_selected_card(nullptr);
        }
        else
            p2->set_selected_card(nullptr);
    }

    else
    {
        Defender_Card = AllowHand.at(selected);

        if (p1->get_character()->get_name() == Defender->get_name() or p1->get_comrade()[0]->get_name() == Defender->get_name())
        {
            p1->set_selected_card(AllowHand.at(selected));
        }
        else
            p1->set_selected_card(AllowHand.at(selected));
    }
}

Element Card_Box(Card *card, string title)
{
    if (card == nullptr)
    {
        return vbox({
                   text(title) | bold | center,
                   separator(),
                   text("NO DEFENSE") | center | color(Color::Red),
               }) |
               borderRounded;
    }

    return vbox({
               text(title) | bold | center,
               separatorHeavy(),

               text(CardTypeToString(card->get_CardType())) | bold | center,
               separator(),

               hbox({
                   text("Amount : "),
                   text(to_string(card->get_amount())),
               }),

               hbox({
                   text("Type : "),
                   text(card->get_Attacktype()),
               }),

               hbox({
                   text("Boost: "),
                   text(to_string(card->get_Boost())),
               }),

               hbox({
                   text("Owner: "),
                   text(card->get_owner()),
               }),

               hbox({
                   text("Time : "),
                   text(CardTimingToString(card->get_CardTiming())),
               }),
           }) |
           borderRounded | size(WIDTH, EQUAL, 35);
}

void Ftxui_Front::Reveal_Combat(Heroes *attacker, Heroes *defender, Card *attack_card, Card *defense_card)
{

    auto screen = ScreenInteractive::Fullscreen();

    auto btn = Button("Resolve Combat", [&]
                      { screen.ExitLoopClosure()(); });

    auto renderer = Renderer(btn, [&]
                             { return vbox({

                                   text("COMBAT") | bold | center | color(Color::YellowLight),

                                   separatorDouble(),

                                   hbox({
                                       vbox({
                                           text(attacker->get_name()) | center,
                                           separator(),
                                           Card_Box(attack_card, "ATTACK"),
                                       }),

                                       text(" ⚔️  ") | bold | center,

                                       vbox({
                                           text(defender->get_name()) | center,
                                           separator(),
                                           Card_Box(defense_card, "DEFENSE"),
                                       }),
                                   }) | center,

                                   separatorDouble(),

                                   btn->Render() | center,

                               }); });

    screen.Loop(renderer);
}

void Ftxui_Front::put_in_any_space(Heroes *fighter, Board *board)
{
    auto screen = ScreenInteractive::Fullscreen();

    static int selected = 0;
    vector<Space *> AllowSpace;
    vector<string> entries;

    for (Space &b : board->get_spaces())
    {
        if (b.get_hero() == nullptr)
        {
            AllowSpace.push_back(&b);
            entries.push_back("Spase" + to_string(b.get_number()));
        }
    }

    auto menu = Menu(&entries, &selected);

    auto renderer = Renderer(menu, [&]
                             { return vbox({text("Choose a space:"),
                                            menu->Render()}); });

    auto component = CatchEvent(renderer, [&](Event event)
                                {

        if(event == Event::Return)
        {
            cout << "Selected: "
                 << entries[selected]
                 << endl;

            screen.ExitLoopClosure()();
            return true;
        }

        return false; });

    screen.Loop(component);

    if (AllowSpace.empty())
    {
        throw std::runtime_error("AllowHand is empty");
        return;
    }

    else
    {
        fighter->set_place(AllowSpace[selected]);
        AllowSpace[selected]->set_hero(fighter);
    }
}

void Ftxui_Front::Revive_Sister(Heroes *sister, Heroes *dracula, Board *board)
{
    std::vector<Space *> available_spaces;
    std::vector<std::string> entries;

    for (Space *space : dracula->get_place()->get_zone())
    {
        if (space->get_hero() == nullptr)
        {
            available_spaces.push_back(space);

            entries.push_back(
                "Space " + std::to_string(space->get_number()));
        }
    }

    if (available_spaces.empty())
    {
        // exeption ...
        throw std::runtime_error("No available adjacent spaces to move.");
    }

    int selected = 0;

    auto menu = Menu(&entries, &selected);

    auto renderer = Renderer(menu, [&]
                             { return hbox({Graph_Box(board->get_spaces()),
                                            separator(),
                                            window(
                                                text("Choose revive position"),
                                                menu->Render())}); });

    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    auto component = CatchEvent(renderer, [&](Event event)
                                {
        if (event == Event::Return)
        {
            sister->set_place(available_spaces[selected]);

            available_spaces[selected]->set_hero(sister);

            screen.Exit();
            return true;
        }

        return false; });

    screen.Loop(component);
}

int Ftxui_Front::DiscardCards(Heroes *dracula)
{
    int discard_count = 0;

    while (true)
    {
        std::vector<std::string> entries;

        for (auto &card : dracula->get_hand())
            entries.push_back(CardTypeToString(card.get_CardType()));

        entries.push_back("Done");

        int selected = 0;

        auto menu = Menu(&entries, &selected);

        ScreenInteractive screen = ScreenInteractive::Fullscreen();

        auto renderer = Renderer(menu, [&]
                                 { return window(
                                       text("Choose cards to discard"),
                                       menu->Render()); });

        auto component = CatchEvent(renderer, [&](Event event)
                                    {
            if (event == Event::Return)
            {
                screen.Exit();
                return true;
            }

            return false; });

        screen.Loop(component);

        if (selected == (int)entries.size() - 1)
            break;

        dracula->DiscardCard(selected);

        discard_count++;
    }

    return discard_count;
}

void Ftxui_Front::MoveHero(Heroes *hero, Board *board, int max_distance)
{
    std::vector<Space *> available_spaces;
    std::vector<std::string> entries;

    std::queue<std::pair<Space *, int>> q;
    std::set<Space *> visited;

    Space *start = hero->get_place();

    q.push({start, 0});
    visited.insert(start);

    while (!q.empty())
    {
        auto [current, distance] = q.front();
        q.pop();

        if (distance == max_distance)
            continue;

        for (Space *next : current->get_neighbor())
        {
            if (visited.count(next))
                continue;

            visited.insert(next);

            q.push({next, distance + 1});

            if (next->get_hero() == nullptr)
            {
                available_spaces.push_back(next);

                entries.push_back(
                    "Space " + std::to_string(next->get_number()));
            }
        }
    }

    if (available_spaces.empty())
        throw std::runtime_error("No available spaces.");

    int selected = 0;

    auto menu = Menu(&entries, &selected);

    auto renderer = Renderer(menu, [&]
                             { return hbox({Graph_Box(board->get_spaces()),
                                            separator(),
                                            window(
                                                text("Choose destination"),
                                                menu->Render())}); });

    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    auto component = CatchEvent(renderer,
                                [&](Event event)
                                {
                                    if (event == Event::Return)
                                    {
                                        Space *current = hero->get_place();
                                        Space *target = available_spaces[selected];

                                        current->set_hero(nullptr);

                                        target->set_hero(hero);

                                        hero->set_place(target);

                                        screen.Exit();
                                        return true;
                                    }

                                    return false;
                                });

    screen.Loop(component);
}

Heroes *Ftxui_Front::SelectHero(Board *board)
{
    std::vector<Heroes *> fighters;
    std::vector<std::string> entries;

    for (Space &space : board->get_spaces())
    {
        Heroes *hero = space.get_hero();

        if (hero == nullptr)
            continue;

        if (!hero->get_islive())
            continue;

        fighters.push_back(hero);

        entries.push_back(
            hero->get_name() +
            " (Space " + std::to_string(space.get_number()) + ")");
    }

    if (fighters.empty())
        throw std::runtime_error("No fighters available.");

    int selected = 0;

    auto menu = Menu(&entries, &selected);

    auto renderer = Renderer(menu, [&]
                             { return hbox({Graph_Box(board->get_spaces()),
                                            separator(),
                                            window(
                                                text("Choose Fighter"),
                                                menu->Render())}); });

    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    auto component = CatchEvent(renderer,
                                [&](Event event)
                                {
                                    if (event == Event::Return)
                                    {
                                        screen.Exit();
                                        return true;
                                    }

                                    return false;
                                });

    screen.Loop(component);

    return fighters[selected];
}

void Ftxui_Front::PlaceHeroAdjacent(
    Heroes *hero,
    Heroes *target,
    Board *board)
{
    if (hero == nullptr || target == nullptr)
        throw std::runtime_error("Hero is nullptr.");

    std::vector<Space *> available;

    for (Space *space : target->get_place()->get_zone())
    {
        if (space->get_hero() == nullptr)
            available.push_back(space);
    }

    if (available.empty())
        throw std::runtime_error("No adjacent empty spaces.");

    std::vector<std::string> entries;

    for (Space *space : available)
    {
        entries.push_back("Space " + std::to_string(space->get_number()));
    }

    int selected = 0;

    auto menu = Menu(&entries, &selected);

    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    auto component = CatchEvent(menu,
                                [&](Event event)
                                {
                                    if (event == Event::Return)
                                    {
                                        screen.Exit();
                                        return true;
                                    }

                                    return false;
                                });

    auto renderer = Renderer(component,
                             [&]
                             {
                                 return hbox({Graph_Box(board->get_spaces()),
                                              separator(),
                                              window(
                                                  text("Choose Adjacent Space"),
                                                  component->Render())});
                             });

    screen.Loop(renderer);

    Space *destination = available[selected];

    hero->get_place()->set_hero(nullptr);

    destination->set_hero(hero);

    hero->set_place(destination);
}

void Ftxui_Front::ShowHand(Heroes *owner, Player *p1, Player *p2, Board *board)
{
    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    std::vector<std::string> entries = {"Continue"};
    int selected = 0;

    auto menu = Menu(&entries, &selected);

    auto component = CatchEvent(menu,
                                [&](Event event)
                                {
                                    if (event == Event::Return)
                                    {
                                        screen.Exit();
                                        return true;
                                    }

                                    return false;
                                });

    auto renderer = Renderer(component,
                             [&]
                             {
                                 Element hand;

                                 if (owner->get_name() == "DRACULA")
                                     hand = Dracula_Hand(p1, p2);
                                 else
                                     hand = Sherlock_Hand(p1, p2);

                                 return hbox({
                                     Graph_Box(board->get_spaces()),
                                     separator(),
                                     vbox({
                                         hand,
                                         separator(),
                                         window(
                                             text("Continue"),
                                             component->Render()),
                                     }),
                                 });
                             });

    screen.Loop(renderer);
}

Card *Ftxui_Front::ChooseCardFromHand(Player *owner, Player *p1, Player *p2, Board *board)
{
    auto &hand = owner->get_character()->get_hand();

    if (hand.empty())
        throw std::runtime_error("Hand is empty.");

    std::vector<std::string> entries;

    for (int i = 0; i < hand.size(); i++)
        entries.push_back("Card " + std::to_string(i + 1));

    int selected = 0;

    auto menu = Menu(&entries, &selected);

    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    auto component = CatchEvent(menu,
                                [&](Event event)
                                {
                                    if (event == Event::Return)
                                    {
                                        screen.Exit();
                                        return true;
                                    }

                                    return false;
                                });

    auto renderer = Renderer(component,
                             [&]
                             {
                                 Element hand_element;

                                 if (owner->get_character()->get_name() == "DRACULA")
                                     hand_element = Dracula_Hand(p1, p2);
                                 else
                                     hand_element = Sherlock_Hand(p1, p2);

                                 return hbox({
                                     Graph_Box(board->get_spaces()),
                                     separator(),
                                     vbox({
                                         hand_element,
                                         separator(),
                                         window(
                                             text("Choose Card"),
                                             component->Render()),
                                     }),
                                 });
                             });

    screen.Loop(renderer);

    return &hand[selected];
}