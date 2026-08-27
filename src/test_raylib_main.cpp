#include "raylib.h"
#include "Raylib.h"
#include "Player.h"
#include "Board.h"
#include "Dracula.h"

int main()
{
    InitWindow(960, 720, "Unmatched - Raylib Test");
    SetTargetFPS(60);

    Raylib RF;
    Player p1, p2;
    Board board;

    RF.Players_Info_List(&p1, &p2);

    std::vector<int> choices = RF.Det_characters(&p1, &p2);

    static Dracula d1, d2;
    p1.set_character(&d1);
    p2.set_character(&d2);

    RF.catch_place(&p1, &p2, &board);

    CloseWindow();
    return 0;
}
