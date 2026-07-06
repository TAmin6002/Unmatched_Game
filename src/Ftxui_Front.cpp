#include "Ftxui_Front.h"

using namespace std;
using namespace ftxui;

enum ::e_Menu Ftxui_Front::Menu()
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
`
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
