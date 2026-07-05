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
    bool play = false;

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
                return true;
            }
        }
        return false; });

    auto renderer = Renderer(component, [&]
                             { return vbox({text("UNMATCHED") | bold | center,
                                            separator(),
                                            component->Render()}) |
                                      border; });

    // screen.Loop(renderer);

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

void Ftxui_Front::Players_Info_List()
{
    auto renderer = Renderer([&]
                             { return hbox({
                                          text("LEFT") | center | flex,
                                          separator(),
                                          text("RIGHT") | center | flex,
                                      }) |
                                      border; });
}
