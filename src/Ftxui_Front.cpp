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

std::vector<int> Ftxui_Front::Det_characters(Player *p1, Player *p2)
{
    Player *younger = (p1->get_age() <= p2->get_age()) ? p1 : p2;
    Player *older   = (p1->get_age() <= p2->get_age()) ? p2 : p1;

    std::vector<std::string> master_list = {
        "DRACULA",
        "SHERLOCK HOLMES",
        "INVISIBLE MAN"
    };

    std::vector<std::string> entries_1 = master_list;
    int selected_1 = 0;

    {
        auto screen = ScreenInteractive::Fullscreen();
        auto menu = Menu(&entries_1, &selected_1);
        auto confirm = Button("Confirm", [&] { screen.Exit(); });
        auto container = Container::Vertical({menu, confirm});

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
    }

    int younger_choice = selected_1; 

    std::vector<std::string> entries_2;
    std::vector<int> remaining_idx; 

    for (int i = 0; i < (int)master_list.size(); i++)
    {
        if (i != younger_choice)
        {
            entries_2.push_back(master_list[i]);
            remaining_idx.push_back(i);
        }
    }

    int selected_2 = 0;

    {
        auto screen = ScreenInteractive::Fullscreen();
        auto menu = Menu(&entries_2, &selected_2);
        auto confirm = Button("Confirm", [&] { screen.Exit(); });
        auto container = Container::Vertical({menu, confirm});

        auto renderer = Renderer(container, [&] {
            return vbox({
                text(older->get_name() + " choose your hero") | bold | center,
                separator(),
                menu->Render(),
                separator(),
                confirm->Render() | center,
            }) | border;
        });

        screen.Loop(renderer);
    }

    int older_choice = remaining_idx[selected_2]; 

    return {younger_choice, older_choice};
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
               text("Action : " + to_string( 2 - dracula_player->get_count())),
            //    text("Move : " + to_string(dracula_player->get_character()->get_Movement())),
               text("card in hand : " + to_string(dracula_player->get_character()->get_hand().size())),
               text("card in deck : " + to_string(dracula_player->get_character()->get_deck().size())),
               text("card in discard : " + to_string(dracula_player->get_character()->get_discard().size())),
               separator(),
               text("SISTERS") |bold |color(Color::RGB(180, 120, 255)),
               text(dracula_player->get_comrade()[0]->get_name() + to_string(dracula_player->get_comrade()[0]->get_number()) + ": " + to_string(dracula_player->get_comrade()[0]->get_Health())),
               text(dracula_player->get_comrade()[1]->get_name() + to_string(dracula_player->get_comrade()[1]->get_number()) + ": " + to_string(dracula_player->get_comrade()[1]->get_Health())),
               text(dracula_player->get_comrade()[2]->get_name() + to_string(dracula_player->get_comrade()[2]->get_number()) + ": " + to_string(dracula_player->get_comrade()[2]->get_Health())),
            }) |
           border;
}

Element Sherlock_Box(Player *p1, Player *p2)
{
    Player *sherlock_player = (p1->get_character()->get_name() == "SHERLOCKHOLMES") ? p1 : p2;
    
    return vbox({
                text("SHERLOCK\n") | bold | color(Color::Blue1),
                text("Health : " + to_string(sherlock_player->get_character()->get_Health()) + " / 16"),
                text("Action : " + to_string(2 -sherlock_player->get_count())),
            //    text("Move : " + to_string(sherlock_player->get_character()->get_Movement())),
               text("card in hand : " + to_string(sherlock_player->get_character()->get_hand().size())),
               text("card in deck : " + to_string(sherlock_player->get_character()->get_deck().size())),
               text("card in discard : " + to_string(sherlock_player->get_character()->get_discard().size())),
               separator(),
               text("Dr_Watsone")|bold|color(Color::RGB(245, 205, 85)),
               text(sherlock_player->get_comrade()[0]->get_name() + ": " + to_string(sherlock_player->get_comrade()[0]->get_Health())),
           }) |
           border;
}


Element Hero_Box(Player *player)
{
    Heroes *hero = player->get_character();
    std::string hero_name = hero->get_name();

    std::string comrade_title;
    Color titleColor = Color::White;
    int maxHealth = hero->get_Health();

    if (hero_name == "DRACULA")
    {
        comrade_title = "SISTERS";
        titleColor = Color::Red1;
        maxHealth = 13;
    }
    else if (hero_name == "SHERLOCKHOLMES")
    {
        comrade_title = "Dr_Watsone";
        titleColor = Color::Blue1;
        maxHealth = 16;
    }
    else if (hero_name == "InvisibleMan")
    {
        comrade_title = "FOG";
        titleColor = Color::RGB(150, 150, 150);
        maxHealth = 15;
    }

    Elements content = {
        text(hero_name + "\n") | bold | color(titleColor),
        text("Health : " + to_string(hero->get_Health()) + " / " + to_string(maxHealth)),
        text("Action : " + to_string(2 - player->get_count())),
        text("card in hand : " + to_string(hero->get_hand().size())),
        text("card in deck : " + to_string(hero->get_deck().size())),
        text("card in discard : " + to_string(hero->get_discard().size())),
    };

    if (!player->get_comrade().empty())
    {
        content.push_back(separator());
        content.push_back(text(comrade_title) | bold | color(titleColor));

        for (auto *c : player->get_comrade())
        {
            std::string label = (c->get_number() == 0)
                                     ? c->get_name()
                                     : c->get_name() + to_string(c->get_number());

            content.push_back(text(label + ": " + to_string(c->get_Health())));
        }
    }

    return vbox(std::move(content)) | border;
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
    c.DrawPointLine(54, 25, 31, 28, Color::White); // 4

    c.DrawPointLine(31, 19, 9, 19, Color::White); // 5

    c.DrawPointLine(9, 19, 9, 31, Color::White); // 6

    c.DrawPointLine(31, 28, 9, 31, Color::White); // 8

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

    c.DrawPointCircle(31, 28, 3, Color::Cyan);
    c.DrawText(30, 27, (spaces[6].get_hero() == nullptr ? "7" : (spaces[6].get_hero()->get_number() == 0 ? "" : to_string(spaces[6].get_hero()->get_number())) + (spaces[6].get_hero()->get_name().substr(0, 2))), Color::Yellow);

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


bool Ftxui_Front::AskBurnCardForMove(Heroes *hero, Board *board)
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
                                                  text(hero->get_name() + " - Burn a card to move further?"),
                                                  component->Render())});
                             });

    screen.Loop(renderer);

    return selected == 0;
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

    case CardType::Coded_Notes:
        return "Coded";

    case CardType::Confound:
        return "Confound";

    case CardType::Covert_Preparation:
        return "Covert";

    case CardType::Dreaming_of_Revenge:
        return "Dreaming";

    case CardType::Emerge_From_Mist:
        return "Emerge";

    case CardType::Impossible_to_See:
        return "Impossible";

    case CardType::Into_Thin_Air:
        return "Into_Thin";

      case CardType::Lurking:
        return "Lurking";

      case CardType::Reign_Thrror:
        return "Reign";

      case CardType::Rolling_Fog:
        return "Rolling";

      case CardType::Slip_Away:
        return "Slip";

      case CardType::Step_Lightly:
        return "Step";

      case CardType::Vanish:
        return "Vanish";

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

Element InvisibleMan_Hand(Player *p1, Player *p2)
{
    Player *invisibleMan_player =
        (p1->get_character()->get_name() == "InvisibleMan") ? p1 : p2;

    vector<Card> Hand_Cards = invisibleMan_player->get_character()->get_hand();

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
        text("InvisibleMan - Hand") | bold | center,
        separator(),
        hbox(std::move(cards)),
    });
}

Element Hero_Hand(Player *player)
{
    Heroes *hero = player->get_character();
    vector<Card> Hand_Cards = hero->get_hand();

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
        text(hero->get_name() + " - Hand") | bold | center,
        separator(),
        hbox(std::move(cards)),
    });
}

void Ftxui_Front::choose_comrad_place(Player *p1, Player *p2, Board *board)
{
    auto place_comrades = [&](Player *player, const std::string &comrade_label)
    {
        int needed = (int)player->get_comrade().size(); 

        if (needed == 0)
            return;

        auto screen = ScreenInteractive::Fullscreen();

        vector<Space *> zone = player->get_character()->get_place()->get_zone();

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
                for (auto c : choices) if (c == selected) duplicate = true;

                if (!duplicate)
                {
                    choices.push_back(selected);
                    confirmed++;
                }

                if (confirmed == needed)
                {
                    for (int i = 0; i < needed; i++)
                    {
                        player->get_comrade()[i]->set_place(zone[choices[i]]);
                        zone[choices[i]]->set_hero(player->get_comrade()[i]);
                    }
                    screen.ExitLoopClosure()();
                } });

        auto container = Container::Vertical({menu, confirm_btn});

        auto renderer = Renderer(container, [&]
                                 { return vbox({
                                       hbox({
                                           vbox({
                                               text(player->get_name() + " - Choose " + to_string(needed) + " space(s) for " + comrade_label) | bold | center,
                                               separator(),
                                               text("Selected: " + to_string(confirmed) + " / " + to_string(needed)) | color(confirmed == needed ? Color::Green : Color::Red) | center,
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
    };

    for (Player *player : {p1, p2})
    {
        std::string hero_name = player->get_character()->get_name();

        if (hero_name == "DRACULA")
            place_comrades(player, "sisters");

        else if (hero_name == "SHERLOCKHOLMES")
            place_comrades(player, "Watson");

        else if (hero_name == "InvisibleMan")
            place_comrades(player, "Fog tokens");
    }
}

void Ftxui_Front::Attakcer_Heroes_Menu(Player *player, Board *board, Heroes *&hero)
{
    if (player == nullptr or hero == nullptr)
    return ;

    auto screen = ScreenInteractive::Fullscreen();

    vector<Heroes *> fighters;

   if (player->get_character() != nullptr and player->get_character()->get_islive()
    and player->get_character()->get_place() != nullptr
    and !player->get_character()->get_PendingPlacement())
    fighters.push_back(player->get_character());

        for (auto c : player->get_comrade())
        {
            if (c != nullptr and c->get_islive()
                and c->get_place() != nullptr
                and !c->get_PendingPlacement())
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

        if(entries.size() == 0)
        return;

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
    if(player != nullptr or defender != nullptr or Attacker != nullptr)
    return ;

    cout << "enter the defender heroes emnu\n";

    if (Attacker == nullptr)
    {
        throw runtime_error("Attacker is nullptr");
    }
    
    else if (Attacker->get_place() == nullptr)
    {
        throw runtime_error("Attacker has no place.");
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

             if(entries.size() == 0)
                return;

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
                                           Hero_Box(p1),
                                           Graph_Box(board->get_spaces()),
                                           Hero_Box(p2),
                                       }) | center,
                                   }),

                                   vbox({
                                       hbox({
                                           Hero_Hand(p1),
                                           vbox({
                                               text("   Choose Action  ") | bold | color(Color::Khaki1),
                                               separatorHeavy(),

                                               container->Render() | center,

                                           }),
                                           Hero_Hand(p2),
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

    Player * Attacker_Player = (p1->get_character() == Attacker or p1->get_comrade()[0]->get_name() == Attacker->get_name() ? p1 : p2);

    for (auto &c : Attacker_Player->get_character()->get_hand())
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
    int selected = 0;

    for (auto *c : AllowHand)
        entries.push_back(CardTypeToString(c->get_CardType()));

         if(entries.size() == 0)
        return;

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
            Attacker->get_name() == "SISTERS" or
            Attacker->get_name() == "SISTERS" or
            Attacker->get_name() == "SISTERS")

        hand = Dracula_Hand(p1, p2);

    else if (Attacker->get_name() == "SHERLOCKHOLMES" or Attacker->get_name() == "Dr_Watson")
        hand = Sherlock_Hand(p1, p2);

    else if (Attacker->get_name() == "InvisibleMan" or Attacker->get_name() == "FOG")
        hand = InvisibleMan_Hand(p1, p2);

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
        AllowHand[selected]->set_user_card(Attacker);
    }
    else
    {
        p2->set_selected_card(AllowHand.at(selected));
        AllowHand[selected]->set_user_card(Attacker);
    }
}

void Ftxui_Front::Defender_selected_card(Heroes *Attacker, Heroes *Defender, Player *p1, Player *p2, Board *board, Card *&Defender_Card)
{
    Heroes *CardOwner = Defender;

    if (Defender->get_name() == "Dr_Watson")
        CardOwner = p1->get_character()->get_name() == "SHERLOCKHOLMES"
                        ? p1->get_character()
                        : p2->get_character();

    else if (Defender->get_name() == "SISTERS" ||
                Defender->get_name() == "SISTERS" ||
                Defender->get_name() == "SISTERS")

            CardOwner = p1->get_character()->get_name() == "DRACULA"
                            ? p1->get_character()
                            : p2->get_character();

        else if (Defender->get_name() == "FOG")
            CardOwner = p1->get_character()->get_name() == "InvisibleMan"
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
    int selected = 0;

    for (auto *c : AllowHand)
        entries.push_back(CardTypeToString(c->get_CardType()));

     if(entries.size() == 0)
        return;

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
            Defender->get_name() == "SISTERS" or
            Defender->get_name() == "SISTERS" or
            Defender->get_name() == "SISTERS")

            hand = Dracula_Hand(p1, p2);

        else if (Defender->get_name() == "SHERLOCKHOLMES" or Defender->get_name() == "Dr_Watson")
            hand = Sherlock_Hand(p1, p2);

        else if (Defender->get_name() == "InvisibleMan" or Defender->get_name() == "FOG")
            hand = InvisibleMan_Hand(p1, p2);

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
            AllowHand[selected]->set_user_card(Defender);
        }
        else
        {
            p2->set_selected_card(AllowHand.at(selected));
            AllowHand[selected]->set_user_card(Defender);
        }
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

    int selected = 0;
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
    }

    else
    {
        fighter->get_place()->set_hero(nullptr);

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

        entries.push_back("Done !!!!");

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

void Ftxui_Front::MoveHero(Heroes *hero, Board *board, int max_distance, Player *p1, Player *p2)
{
    if (hero == nullptr || hero->get_place() == nullptr)
        return;

    auto belongs_to = [](Player *pl, Heroes *h)
    {
        if (pl == nullptr || h == nullptr)
            return false;

        if (pl->get_character() == h)
            return true;

        for (Heroes *c : pl->get_comrade())
            if (c == h)
                return true;

        return false;
    };

    Player *hero_owner = belongs_to(p1, hero) ? p1 : p2;

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

        std::vector<Space *> movable_from_current = current->get_neighbor();
        std::vector<Space *> portals_from_current = current->get_portal();
        movable_from_current.insert(movable_from_current.end(),
                                     portals_from_current.begin(),
                                     portals_from_current.end());

        if (hero->get_name() == "InvisibleMan" && current->get_Fog() != nullptr)
        {
            for (Space &s : board->get_spaces())
            {
                if (&s == current || s.get_Fog() == nullptr)
                    continue;

                Heroes *occupant = s.get_hero();

                if (occupant != nullptr && !belongs_to(hero_owner, occupant))
                    continue;

                movable_from_current.push_back(&s);
            }
        }
        // ---------------------------------------------------------------

        for (Space *next : movable_from_current)
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

Heroes *Ftxui_Front::SelectHero(Board *board, Heroes *exclude)
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

        if (hero == exclude)
            continue;

        fighters.push_back(hero);

        entries.push_back(
            hero->get_name() +
            " (Space " + std::to_string(space.get_number()) + ")");
    }

 
    if (fighters.empty())
        return nullptr;

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

void Ftxui_Front::PlaceHeroAdjacent(Heroes *hero, Heroes *target, Board *board)
{
    if (hero == nullptr || target == nullptr)
        throw std::runtime_error("Hero is nullptr.");

    std::vector<Space *> available;

    if(target == nullptr)
    {
        msg.push_back("target is nullptr (PlaceHeroAdjacent)");
        return;
    }

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
                                 else if (owner->get_name() == "InvisibleMan")
                                     hand = InvisibleMan_Hand(p1, p2);
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
                                 else if (owner->get_character()->get_name() == "InvisibleMan")
                                     hand_element = InvisibleMan_Hand(p1, p2);
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

void Ftxui_Front::Event_Selected_Card(Heroes * SelectedHero, Player *turn, Player * p1, Player * p2, Board *board, Card *&selected_Card)
{
    vector<Card *> AllowHand;

    for (auto &c : turn->get_character()->get_hand())
    {
        if ((c.get_owner() == SelectedHero->get_name() or c.get_owner() == "ANY") and (c.get_Attacktype() == "Event"))
        {
            AllowHand.push_back(&c);
        }
    }

    if (AllowHand.empty())
    {
        throw std::runtime_error("AllowHand is empty (Event)");
        msg.push_back("AllowHand is empty (Event)");

        return; // ................
    }

    vector<string> entries;
    int selected = 0;

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
                                        text(turn->get_name() + " Choose Card for Arrack") | bold | center,
                                        separator(),
                                        menu->Render() | border,
                                        filler(),
                                        confirm->Render() | center,
                                    }) |
                                    size(WIDTH, EQUAL, 30);
                         });

    auto graph = Graph_Box(board->get_spaces());

    Element hand;

   if (turn->get_character()->get_name() == "DRACULA" or turn->get_comrade()[0]->get_name() == "SISTERS")
        hand = Dracula_Hand(p1, p2);


    else if (turn->get_character()->get_name() == "SHERLOCKHOLMES" or turn->get_comrade()[0]->get_name() == "Dr_Watson")
        hand = Sherlock_Hand(p1, p2);

    else if (turn->get_character()->get_name() == "InvisibleMan" or turn->get_comrade()[0]->get_name() == "FOG")
        hand = InvisibleMan_Hand(p1, p2);

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

    selected_Card = AllowHand[selected];

     turn->set_selected_card(AllowHand.at(selected));
        AllowHand[selected]->set_user_card(SelectedHero);
}


void Ftxui_Front::DeclareWinner(Heroes *winner)
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

    Color winnerColor = (winner->get_name() == "DRACULA") ? Color::Red1 : Color::Blue1;

    auto renderer = Renderer(component,
                             [&]
                             {
                                 return vbox({
                                            text("") ,
                                            text("🏆  V I C T O R Y  🏆") | bold | color(Color::Gold1) | center,
                                            text(""),
                                            separator(),
                                            text(""),
                                            text(winner->get_name()) | bold | color(winnerColor) | center | flex,
                                            text("HAS WON THE GAME!") | bold | color(Color::White) | center,
                                            text(""),
                                            separator(),
                                            text(""),
                                            component->Render() | center,
                                        }) |
                                        border | color(Color::LightGoldenrod1) | center | size(WIDTH, GREATER_THAN, 40) | size(HEIGHT, GREATER_THAN, 12);
                             });

    screen.Loop(renderer);
}

void Ftxui_Front::Show_Help()
{
    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    std::vector<std::string> topics = {
        "Objective",
        "Setup",
        "Turn Actions",
        "Movement & Portals",
        "Combat",
        "Cards & Hand",
        "Special Abilities",
        "Winning the Game",
        "Back",
    };

    std::vector<std::vector<std::string>> pages = {
        { // Objective
            "UNMATCHED pits SHERLOCK HOLMES and DR. WATSON against",
            "DRACULA and his three vampire SISTERS on the streets of",
            "London.",
            "",
            "Each side takes turns moving fighters around the board,",
            "attacking the other side, and playing cards, trying to",
            "destroy the other team before that happens to them.",
        },
        { // Setup
            "Two players play. Each first enters a name and an age.",
            "",
            "The younger player and the older player then pick sides:",
            "one plays DRACULA with his three Sisters as comrades, the",
            "other plays SHERLOCK HOLMES with DR. WATSON as a comrade.",
            "",
            "The younger player always takes the first turn, and turns",
            "alternate between the two players after that.",
            "",
            "Starting Houses: the Sisters begin in Houses 1, 2 and 4,",
            "DRACULA begins in House 3, SHERLOCK HOLMES begins in House",
            "6, and DR. WATSON begins in House 32.",
        },
        { // Turn Actions
            "Every turn you get 2 actions, in any order, chosen from:",
            "",
            "  Attack   - fight with one of your fighters.",
            "  Maneuver - move one of your fighters.",
            "  Event    - play one of that fighter's Event cards.",
            "  Back     - end your turn early.",
            "",
            "Any of your living fighters can take an action, not only",
            "your main hero - so send a Sister or Dr. Watson in instead",
            "of your leader when it helps.",
        },
        { // Movement & Portals
            "During a Maneuver, a fighter can move up to its Movement",
            "value (normally 2) across connected Houses.",
            "",
            "Before moving, you may burn a card from that fighter's",
            "hand to add the card's Boost value to the move instead of",
            "playing it normally.",
            "",
            "Houses 1, 12, 15 and 27 share hidden passages. Moving",
            "between any two of them costs just one step - but the",
            "passage does NOT count as adjacency for attacks or",
            "zone-based cards.",
        },
        { // Combat
            "Melee fighters (DRACULA, the Sisters, SHERLOCK HOLMES) can",
            "only attack a target standing in an adjacent House.",
            "",
            "Ranged fighters (DR. WATSON) can attack any target in their",
            "House's wider zone, even without being adjacent.",
            "",
            "To Attack: pick your fighter, pick a valid target, then",
            "secretly choose an Attack (or Both) card. The defender may",
            "answer with a Defense (or Both) card, or take no card.",
            "",
            "Both cards flip face up together and resolve in order -",
            "Before, then During, then After effects - before damage is",
            "applied. Damage dealt is the attack value minus the",
            "defense value. Both cards are then discarded.",
        },
        { // Cards & Hand
            "Each fighter has their own deck and keeps a hand of cards",
            "drawn from it.",
            "",
            "Cards can be Attack, Defense, Both, or Event cards. Some",
            "cards belong to one specific fighter, while cards marked",
            "ANY can be used by any fighter on that side.",
            "",
            "An Event card is played by itself during the Event action",
            "for its special effect, instead of being used in a fight.",
            "",
            "Watch your deck: if a fighter needs to draw and its deck",
            "is empty, that fighter is damaged instead of drawing.",
        },
        { // Special Abilities
            "DRACULA: at the start of each of his turns, he may bite -",
            "choose one living fighter anywhere on the board, deal it 1",
            "damage, then draw a card. If his deck is empty he takes 2",
            "damage instead of drawing.",
            "",
            "SHERLOCK HOLMES, DR. WATSON and the Sisters have no",
            "built-in per-turn ability - their tricks come from the",
            "Event cards in their hand instead.",
        },
        { // Winning
            "A side wins the instant the opponent's main fighter",
            "(DRACULA or SHERLOCK HOLMES) has been defeated AND every",
            "one of their comrades has also been defeated.",
            "",
            "Keep at least one member of your team standing to stay in",
            "the fight!",
        },
    };

    int selected = 0;

    auto menu = Menu(&topics, &selected);

    auto component = CatchEvent(menu, [&](Event event)
                                {
            if (event == Event::Return)
            {
                if (selected == (int)topics.size() - 1) // Back
                {
                    screen.Exit();
                    return true;
                }
                return true;
            }

            if (event == Event::Escape)
            {
                screen.Exit();
                return true;
            }

            return false; });

    auto renderer = Renderer(component, [&]
                             {
        Elements lines;

        if (selected >= 0 && selected < (int)pages.size())
        {
            for (auto const &line : pages[selected])
                lines.push_back(text(line));
        }
        else
        {
            lines.push_back(text("Press Enter to close this screen."));
        }

        return vbox({
                   text("UNMATCHED - HOW TO PLAY") | bold | center,
                   separator(),
                   hbox({
                       window(text("Topics"), component->Render()) | size(WIDTH, EQUAL, 26),
                       separator(),
                       window(text(topics[selected]), vbox(lines)) | flex,
                   }) | flex,
               }) |
               border | size(WIDTH, GREATER_THAN, 70) | size(HEIGHT, GREATER_THAN, 20); });

    screen.Loop(renderer);
}

void Ftxui_Front::ChooseCardsToTopOfDeck(Heroes *owner, int maxCount, Player *p1, Player *p2, Board *board)
{
    std::vector<Card> chosen;

    while ((int)chosen.size() < maxCount)
    {
        auto &hand = owner->get_hand();

        if (hand.empty()) break; 

        std::vector<std::string> entries;

        for (auto &card : hand)
            entries.push_back(CardTypeToString(card.get_CardType()));

        int selected = 0;
        auto menu = Menu(&entries, &selected);

        ScreenInteractive screen = ScreenInteractive::Fullscreen();
        auto renderer = Renderer(menu, [&]
                                 { return window(
                                       text("Choose exactly " + std::to_string(maxCount) +
                                            " card(s) to put on top of your deck (" +
                                            std::to_string(chosen.size()) + "/" +
                                            std::to_string(maxCount) + " chosen so far)"),
                                       menu->Render()); });
        auto component = CatchEvent(renderer, [&](Event event)
                                    {
            if (event == Event::Return) { screen.Exit(); return true; }
            return false; });
        screen.Loop(component);
        chosen.push_back(hand[selected]);
        hand.erase(hand.begin() + selected);
    }
 
    for (auto it = chosen.rbegin(); it != chosen.rend(); ++it)
        owner->get_deck().push_back(*it);
}

bool Ftxui_Front::ChooseCardToDiscardOrSkip(Player *opponent)
{
    auto &hand = opponent->get_character()->get_hand();

    if (hand.empty())
        return false;

    std::vector<std::string> entries;

    for (auto &card : hand)
        entries.push_back(CardTypeToString(card.get_CardType()));

    entries.push_back("Skip (Don't discard)");

    int selected = 0;

    auto menu = Menu(&entries, &selected);

    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    auto renderer = Renderer(menu, [&]
                             { return window(
                                   text("Opponent may discard one card, or choose Skip"),
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
            return false;

    opponent->get_character()->DiscardCard(selected);

    return true;
}

void Ftxui_Front::MoveFogToken(Heroes *owner, Board *board)
{
    if (owner == nullptr)
        return;
        
    auto screen = ScreenInteractive::Fullscreen();

    int selected = 0;
    std::vector<Space *> AllowSpace;
    std::vector<std::string> entries;

    for (Space &b : board->get_spaces())
    {
        AllowSpace.push_back(&b);
        entries.push_back("Spase" + std::to_string(b.get_number()));
    }

    auto menu = Menu(&entries, &selected);

    auto renderer = Renderer(menu, [&]
                             { return hbox({Graph_Box(board->get_spaces()),
                                            separator(),
                                            window(
                                                text(owner->get_name() + " : move the Fog Token to any space"),
                                                menu->Render())}); });

    auto component = CatchEvent(renderer, [&](Event event)
                                {
        if (event == Event::Return)
        {
            screen.ExitLoopClosure()();
            return true;
        }

        return false; });

    screen.Loop(component);

    if (AllowSpace.empty())
        throw std::runtime_error("AllowSpace is empty");

        owner->get_place()->set_Fog(nullptr);
        owner->set_place(AllowSpace.at(selected));

}


Heroes *Ftxui_Front::SelectComrade(Player *owner, Board *board)
{
    std::vector<Heroes *> alive_comrades;
    std::vector<std::string> entries;

    for (Heroes *comrade : owner->get_comrade())
    {
        if (comrade == nullptr)
            continue;

        if (!comrade->get_islive())
            continue;

        alive_comrades.push_back(comrade);

        entries.push_back(
            comrade->get_name() +
            " (Space " + std::to_string(comrade->get_place()->get_number()) + ")");
    }

    if (alive_comrades.empty())
        return nullptr;

    int selected = 0;

    auto menu = Menu(&entries, &selected);

    auto renderer = Renderer(menu, [&]
                             { return hbox({Graph_Box(board->get_spaces()),
                                            separator(),
                                            window(
                                                text("Choose your comrade"),
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

    return alive_comrades[selected];
}

Space *Ftxui_Front::SelectFogToken(Board *board)
{
    std::vector<Space *> fogSpaces;
    std::vector<std::string> entries;

    for (Space &space : board->get_spaces())
    {
        if (space.get_Fog() != nullptr)
        {
            fogSpaces.push_back(&space);
            entries.push_back("Fog Token (Space " + std::to_string(space.get_number()) + ")");
        }
    }

    if (fogSpaces.empty())
        return nullptr;

    if (fogSpaces.size() == 1)
        return fogSpaces[0];

    int selected = 0;
    auto menu = Menu(&entries, &selected);

    auto renderer = Renderer(menu, [&]
                             { return hbox({Graph_Box(board->get_spaces()),
                                            separator(),
                                            window(text("Choose fog token"), menu->Render())}); });

    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    auto component = CatchEvent(renderer, [&](Event event)
                                {
        if (event == Event::Return)
        {
            screen.Exit();
            return true;
        }
        return false; });

    screen.Loop(component);

    return fogSpaces[selected];
}

void Ftxui_Front::MoveFogTokenDistance(Space *fogSpace, Board *board, int max_distance)
{
    if (fogSpace == nullptr)
        return;

    std::vector<Space *> available_spaces;
    std::vector<std::string> entries;

    std::queue<std::pair<Space *, int>> q;
    std::set<Space *> visited;

    q.push({fogSpace, 0});
    visited.insert(fogSpace);

    while (!q.empty())
    {
        auto [current, distance] = q.front();
        q.pop();

        if (distance == max_distance)
            continue;

        std::vector<Space *> movable_from_current = current->get_neighbor();
        std::vector<Space *> portals_from_current = current->get_portal();
        movable_from_current.insert(movable_from_current.end(),
                                     portals_from_current.begin(),
                                     portals_from_current.end());

        for (Space *next : movable_from_current)
        {
            if (visited.count(next))
                continue;

            visited.insert(next);
            q.push({next, distance + 1});

           if (next != fogSpace && next->get_Fog() == nullptr)
            {
                available_spaces.push_back(next);
                entries.push_back("Space " + std::to_string(next->get_number()));
            }
        }
    }

    if (available_spaces.empty())
        return;

    int selected = 0;
    auto menu = Menu(&entries, &selected);

    auto renderer = Renderer(menu, [&]
                             { return hbox({Graph_Box(board->get_spaces()),
                                            separator(),
                                            window(text("Move Fog Token to..."), menu->Render())}); });

    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    auto component = CatchEvent(renderer, [&](Event event)
                                {
        if (event == Event::Return)
        {
            Heroes *fogMarker = fogSpace->get_Fog();

            fogSpace->set_Fog(nullptr);
            available_spaces[selected]->set_Fog(fogMarker);

            screen.Exit();
            return true;
        }
        return false; });

    screen.Loop(component);


}

void Ftxui_Front::MoveHeroToFogSpace(Heroes *hero, Board *board)
{
    std::vector<Space *> fogSpaces;
    std::vector<std::string> entries;

    for (Space &b : board->get_spaces())
    {
        if (b.get_Fog() != nullptr && b.get_hero() == nullptr)
        {
            fogSpaces.push_back(&b);
            entries.push_back("Space " + std::to_string(b.get_number()));
        }
    }

    if (fogSpaces.empty())
        return;

    int selected = 0;
    auto menu = Menu(&entries, &selected);

    auto renderer = Renderer(menu, [&]
                             { return hbox({Graph_Box(board->get_spaces()),
                                            separator(),
                                            window(text("Move InvisibleMan to a Fog Token space"), menu->Render())}); });

    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    auto component = CatchEvent(renderer, [&](Event event)
                                {
        if (event == Event::Return)
        {
            hero->get_place()->set_hero(nullptr);

            fogSpaces[selected]->set_hero(hero);
            hero->set_place(fogSpaces[selected]);

            screen.Exit();
            return true;
        }
        return false; });

    screen.Loop(component);
}

bool Ftxui_Front::ChooseBetweenTwoEffects(std::string optionA, std::string optionB)
{
    std::vector<std::string> entries = {optionA, optionB};
    int selected = 0;

    auto menu = Menu(&entries, &selected);

    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    auto renderer = Renderer(menu, [&]
                             { return window(text("Choose an effect"), menu->Render()); });

    auto component = CatchEvent(renderer, [&](Event event)
                                {
        if (event == Event::Return)
        {
            screen.Exit();
            return true;
        }
        return false; });

    screen.Loop(component);

    return selected == 0; // true = optionA
}


void Ftxui_Front::MoveFogTokenAnywhere(Space *fogSpace, Board *board)
{
    if (fogSpace == nullptr)
        return;

    std::vector<Space *> AllowSpace;
    std::vector<std::string> entries;

    for (Space &b : board->get_spaces())
    {
        if (&b != fogSpace && b.get_Fog() == nullptr)
        {
            AllowSpace.push_back(&b);
            entries.push_back("Space " + std::to_string(b.get_number()));
        }
    }

    int selected = 0;
    auto menu = Menu(&entries, &selected);

    auto renderer = Renderer(menu, [&]
                             { return hbox({Graph_Box(board->get_spaces()),
                                            separator(),
                                            window(text("Move the Fog Token to..."), menu->Render())}); });

    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    auto component = CatchEvent(renderer, [&](Event event)
                                {
        if (event == Event::Return)
        {
            Heroes *fogMarker = fogSpace->get_Fog();

            fogSpace->set_Fog(nullptr);
            AllowSpace[selected]->set_Fog(fogMarker);

            screen.Exit();
            return true;
        }
        return false; });

    screen.Loop(component);
}

Space *Ftxui_Front::MoveFogTokenToEmptySpace(Space *fogSpace, Board *board)
{
    if (fogSpace == nullptr)
        return nullptr;

    std::vector<Space *> AllowSpace;
    std::vector<std::string> entries;

    for (Space &b : board->get_spaces())
    {
        if (&b != fogSpace && b.get_hero() == nullptr && b.get_Fog() == nullptr)
        {
            AllowSpace.push_back(&b);
            entries.push_back("Space " + std::to_string(b.get_number()));
        }
    }

    if (AllowSpace.empty())
        return nullptr;

    int selected = 0;
    auto menu = Menu(&entries, &selected);

    auto renderer = Renderer(menu, [&]
                             { return hbox({Graph_Box(board->get_spaces()),
                                            separator(),
                                            window(text("Move the Fog Token to an empty space"), menu->Render())}); });

    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    auto component = CatchEvent(renderer, [&](Event event)
                                {
        if (event == Event::Return)
        {
            screen.Exit();
            return true;
        }
        return false; });

    screen.Loop(component);

    Heroes *fogMarker = fogSpace->get_Fog();
    fogSpace->set_Fog(nullptr);

    Space *destination = AllowSpace[selected];
    destination->set_Fog(fogMarker);

    return destination;
}

Heroes *Ftxui_Front::SelectAdjacentHero(Heroes *center, Board *board)
{
    std::vector<Heroes *> fighters;
    std::vector<std::string> entries;

    for (Space &space : board->get_spaces())
    {
        Heroes *hero = space.get_hero();

        if (hero == nullptr || hero == center || !hero->get_islive())
            continue;

        if (!board->is_Adjacent(center->get_place(), &space))
            continue;

        fighters.push_back(hero);
        entries.push_back(hero->get_name() + " (Space " + std::to_string(space.get_number()) + ")");
    }

    if (fighters.empty())
        return nullptr;

    int selected = 0;
    auto menu = Menu(&entries, &selected);

    auto renderer = Renderer(menu, [&]
                             { return hbox({Graph_Box(board->get_spaces()),
                                            separator(),
                                            window(text("Choose adjacent fighter"), menu->Render())}); });

    ScreenInteractive screen = ScreenInteractive::Fullscreen();

    auto component = CatchEvent(renderer, [&](Event event)
                                {
        if (event == Event::Return)
        {
            screen.Exit();
            return true;
        }
        return false; });

    screen.Loop(component);

    return fighters[selected];
}

void Ftxui_Front::PlaceHeroOnBoard(Heroes *hero, Board *board)
{
    std::vector<Space *> AllowSpace;
    std::vector<std::string> entries;

    for (Space &b : board->get_spaces())
    {
        if (b.get_hero() == nullptr)
        {
            AllowSpace.push_back(&b);
            entries.push_back("Space " + std::to_string(b.get_number()));
        }
    }

    if (AllowSpace.empty())
        return;

    int selected = 0;
    auto menu = Menu(&entries, &selected);

    auto renderer = Renderer(menu, [&]
                             { return hbox({Graph_Box(board->get_spaces()),
                                            separator(),
                                            window(text("Place InvisibleMan on any space"), menu->Render())}); });

    auto screen = ScreenInteractive::Fullscreen();

    auto component = CatchEvent(renderer, [&](Event event)
                                {
        if (event == Event::Return)
        {
            screen.ExitLoopClosure()();
            return true;
        }
        return false; });

    screen.Loop(component);

    hero->set_place(AllowSpace[selected]);
    AllowSpace[selected]->set_hero(hero);
}