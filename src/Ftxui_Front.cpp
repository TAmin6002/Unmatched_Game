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
    int choice = -1;
    auto screen = ScreenInteractive::Fullscreen();

    auto complete_Button = Button("Completed", [&]
                                  { screen.Exit(); }) |
                           color(Color::Green);

    auto Dracula_Button = Button("DRACULA", [&]
                                 { choice = 0; }) |
                          center | flex;

    auto Sherlock_Button = Button("SHERLOCK HOLMES", [&]
                                  { choice = 1; }) |
                           center | flex;

    auto container = Container::Horizontal({
        Dracula_Button,
        Sherlock_Button,
    });

    auto renderer = Renderer(container, [&]
                             { return vbox({
                                          text((p1->get_age() <= p2->get_age() ? p1->get_name() : p2->get_name()) + " select your character") | bold | center,
                                          separator(),
                                          hbox({
                                              Dracula_Button->Render(),
                                              separator(),

                                              Sherlock_Button->Render(),
                                              separator(),

                                          }) | flex,
                                          complete_Button->Render(),
                                      }) |
                                      border; });

    auto component = CatchEvent(renderer, [&](Event event)
                                { return complete_Button->OnEvent(event); });

    screen.Loop(component);

    return choice;
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

Element Graph_Box(vector<Space> spaces)
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
    c.DrawText(8, 5, (spaces[0].get_hero() == nullptr ? "1" : (spaces[0].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(31, 6, 3, Color::Cyan);
    c.DrawText(30, 5, (spaces[1].get_hero() == nullptr ? "2" : (spaces[1].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(50, 14, 3, Color::Cyan);
    c.DrawText(49, 13, (spaces[2].get_hero() == nullptr ? "3" : (spaces[2].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(54, 25, 3, Color::Cyan);
    c.DrawText(53, 24, (spaces[3].get_hero() == nullptr ? "4" : (spaces[3].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(31, 19, 3, Color::Cyan);
    c.DrawText(30, 18, (spaces[4].get_hero() == nullptr ? "5" : (spaces[4].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(9, 19, 3, Color::Cyan);
    c.DrawText(8, 18, (spaces[5].get_hero() == nullptr ? "6" : (spaces[5].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(31, 20, 3, Color::Cyan);
    c.DrawText(30, 19, (spaces[6].get_hero() == nullptr ? "7" : (spaces[6].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(9, 31, 3, Color::Cyan);
    c.DrawText(8, 30, (spaces[7].get_hero() == nullptr ? "8" : (spaces[7].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(10, 38, 3, Color::Cyan);
    c.DrawText(9, 37, (spaces[8].get_hero() == nullptr ? "9" : (spaces[8].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(51, 37, 3, Color::Cyan);
    c.DrawText(50, 36, (spaces[9].get_hero() == nullptr ? "10" : (spaces[9].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(75, 33, 3, Color::Cyan);
    c.DrawText(74, 32, (spaces[10].get_hero() == nullptr ? "11" : (spaces[10].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(92, 29, 3, Color::Cyan);
    c.DrawText(91, 28, (spaces[11].get_hero() == nullptr ? "12" : (spaces[11].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(127, 33, 3, Color::Cyan);
    c.DrawText(126, 32, (spaces[12].get_hero() == nullptr ? "13" : (spaces[12].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(81, 44, 3, Color::Cyan);
    c.DrawText(80, 43, (spaces[13].get_hero() == nullptr ? "14" : (spaces[13].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(19, 48, 3, Color::Cyan);
    c.DrawText(18, 47, (spaces[14].get_hero() == nullptr ? "15" : (spaces[14].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(41, 56, 3, Color::Cyan);
    c.DrawText(40, 55, (spaces[15].get_hero() == nullptr ? "16" : (spaces[15].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(62, 49, 3, Color::Cyan);
    c.DrawText(61, 48, (spaces[16].get_hero() == nullptr ? "17" : (spaces[16].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(81, 56, 3, Color::Cyan);
    c.DrawText(80, 55, (spaces[17].get_hero() == nullptr ? "18" : (spaces[17].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(107, 48, 3, Color::Cyan);
    c.DrawText(106, 47, (spaces[18].get_hero() == nullptr ? "19" : (spaces[18].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(130, 55, 3, Color::Cyan);
    c.DrawText(129, 54, (spaces[19].get_hero() == nullptr ? "20" : (spaces[19].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(150, 60, 3, Color::Cyan);
    c.DrawText(149, 59, (spaces[20].get_hero() == nullptr ? "21" : (spaces[20].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(164, 48, 3, Color::Cyan);
    c.DrawText(163, 47, (spaces[21].get_hero() == nullptr ? "22" : (spaces[21].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(164, 33, 3, Color::Cyan);
    c.DrawText(163, 32, (spaces[22].get_hero() == nullptr ? "23" : (spaces[22].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(148, 33, 3, Color::Cyan);
    c.DrawText(147, 32, (spaces[23].get_hero() == nullptr ? "24" : (spaces[23].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(157, 26, 3, Color::Cyan);
    c.DrawText(156, 25, (spaces[24].get_hero() == nullptr ? "25" : (spaces[24].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(157, 14, 3, Color::Cyan);
    c.DrawText(156, 13, (spaces[25].get_hero() == nullptr ? "26" : (spaces[25].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(179, 6, 3, Color::Cyan);
    c.DrawText(178, 5, (spaces[26].get_hero() == nullptr ? "27" : (spaces[26].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(157, 6, 3, Color::Cyan);
    c.DrawText(156, 5, (spaces[27].get_hero() == nullptr ? "28" : (spaces[27].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(130, 14, 3, Color::Cyan);
    c.DrawText(129, 13, (spaces[28].get_hero() == nullptr ? "29" : (spaces[28].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(107, 6, 3, Color::Cyan);
    c.DrawText(106, 5, (spaces[29].get_hero() == nullptr ? "30" : (spaces[29].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(92, 14, 3, Color::Cyan);
    c.DrawText(91, 13, (spaces[30].get_hero() == nullptr ? "31" : (spaces[30].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    c.DrawPointCircle(73, 6, 3, Color::Cyan);
    c.DrawText(72, 5, (spaces[31].get_hero() == nullptr ? "32" : (spaces[31].get_hero()->get_name().substr(0, 2))), Color::Yellow);

    return canvas(std::move(c));
}

Element Dracula_Hand(Player *p1, Player *p2)
{
    Player *dracula_player = (p1->get_character()->get_name() == "DRACULA") ? p1 : p2;

    vector<Card> Hand_Cards = dracula_player->get_character()->get_hand();

    return hbox({text("Dracula - Hand ") | center | border | size(WIDTH, EQUAL, 55) | size(HEIGHT, EQUAL, 10)});
}

Component Ftxui_Front::ChooseAction(Player *p1, Player *p2, ScreenInteractive *screen)
{

    static std::vector<std::string> entries = {
        "Attack",
        "Maneuver",
        "Scheme",
        "Back"};

    static int selected = 0;
    static int action = -1;

    auto menu = Menu(&entries, &selected);

    auto component = CatchEvent(menu, [&](Event event)
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
    Player *sherlock_player = (p1->get_character()->get_name() == "SHERLOCKHOLMES") ? p1 : p2;
    return hbox({text("Sherlock - Hand") | center | border | size(WIDTH, EQUAL, 55) | size(HEIGHT, EQUAL, 10)});
}

void Ftxui_Front::chose_comrad_place(Player *p1, Player *p2, Board *board)
{
    Player *dracula_player = (p1->get_character()->get_name() == "DRACULA") ? p1 : p2;
    Player *sherlock_player = (p1->get_character()->get_name() == "SHERLOCKHOLMES") ? p1 : p2;

    {

        auto screen = ScreenInteractive::Fullscreen();

        vector<Space *> zone = dracula_player->get_character()->get_place()->get_zone();

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
                for(auto c : choices) if(c == selected) duplicate = true;
                
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

    vector<string> entries;

    for (auto h : fighters)
        entries.push_back(h->get_name());

    int selected = 0;

    if (fighters.empty())
    {
        // هیچ هدفی وجود ندارد
        // exeption ... !!!!!!!!!!!!!!!!!!!!!!
        return;
    }

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
            // all characters (sherlock and dracula) are MELEE

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
            else if (Attacker->get_Attacktype() == "RANGE")
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

        vector<string> entries;

        for (auto h : defenders)
            entries.push_back(h->get_name());

        int selected = 0;

        cout << "defenders size = "
             << defenders.size()
             << endl;
             
        if (defenders.empty())
        {
            throw std::runtime_error("Attack is not possible.");
            return;
        }

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
}

void Ftxui_Front::main_map(Player *p1, Player *p2, Board *board, Player *turn)
{
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
}

std::vector<std::string> &Ftxui_Front::get_msg()
{
    return msg;
}
