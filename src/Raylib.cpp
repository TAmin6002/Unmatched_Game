#include <cmath> 
#include<algorithm>
#include "Raylib.h"

using namespace std;


namespace BoardVisual
{
    constexpr int kSpaceCount = 32;

    constexpr Vector2 kLayout[kSpaceCount] = {
        {9, 6},    {31, 6},   {50, 14},  {54, 25},  {31, 19},  {9, 19},   {31, 28},  {9, 31},
        {10, 38},  {51, 37},  {75, 33},  {92, 29},  {127, 33}, {81, 44},  {19, 48},  {41, 56},
        {62, 49},  {81, 56},  {107, 48}, {130, 55}, {150, 60}, {164, 48}, {164, 33}, {148, 33},
        {157, 26}, {157, 14}, {179, 6},  {157, 6},  {130, 14}, {107, 6},  {92, 14},  {73, 6},
    };

    constexpr int kCorridors[][2] = {
        {0, 1}, {0, 5}, {1, 2}, {2, 3}, {2, 31}, {3, 4}, {3, 6}, {4, 5}, {5, 7}, {6, 7}, {7, 8},
        {8, 9}, {8, 14}, {9, 10}, {10, 11}, {10, 13}, {11, 12},
        {12, 18}, {12, 28}, {12, 24}, {12, 23}, {12, 19},
        {13, 16}, {13, 17}, {13, 18}, {14, 15}, {15, 16}, {16, 17}, {17, 18}, {18, 19}, {19, 20},
        {20, 21}, {21, 22}, {22, 23}, {22, 24}, {24, 25}, {25, 27}, {26, 27}, {27, 28}, {28, 29},
        {29, 30}, {29, 31}, {30, 31},
    };

    constexpr int kPortals[][2] = {
        {0, 11}, {0, 14}, {0, 26}, {11, 14}, {11, 26}, {14, 26},
    };

    constexpr float kMinX = 9.0f, kMaxX = 179.0f, kMinY = 6.0f, kMaxY = 66.0f;

    struct MapTransform { float offsetX, offsetY, scale; };

    inline float Clampf(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

    inline MapTransform Fit(Rectangle area)
    {
        float designW = kMaxX - kMinX;
        float designH = kMaxY - kMinY;

        float pad   = fminf(area.width, area.height) * 0.09f;
        float scale = fminf((area.width - pad * 2.0f) / designW, (area.height - pad * 2.0f) / designH);

        float offsetX = area.x + (area.width  - designW * scale) * 0.5f - kMinX * scale;
        float offsetY = area.y + (area.height - designH * scale) * 0.5f - kMinY * scale;

        return {offsetX, offsetY, scale};
    }

    inline Vector2 ToScreen(const MapTransform &t, Vector2 p)
    {
        return {t.offsetX + p.x * t.scale, t.offsetY + p.y * t.scale};
    }

    inline Vector2 SpacePos(const MapTransform &t, int spaceIndex)
    {
        return ToScreen(t, kLayout[spaceIndex]);
    }

    inline float NodeRadius(const MapTransform &t)
    {
        return Clampf(t.scale * 4.6f, 9.0f, 30.0f);
    }
}

Font &Raylib::GetGameFont()
{
    if (!fontLoaded)
    {
        if (FileExists("assets/Cinzel/static/Cinzel-Regular.ttf"))
        {
           
            gothicFont = LoadFontEx("assets/Cinzel/static/Cinzel-Regular.ttf", 128, nullptr, 0);
            SetTextureFilter(gothicFont.texture, TEXTURE_FILTER_BILINEAR);
        }
        else
        {
            gothicFont = GetGameFont(); 
        }
        fontLoaded = true;
    }
    return gothicFont;
}

void Raylib::UnloadGameFont()
{
    if (fontLoaded && gothicFont.texture.id != GetGameFont().texture.id)
        UnloadFont(gothicFont);
    fontLoaded = false;
}

std::vector<std::string> &Raylib::get_msg()
{
    return msg;
}

int Raylib::get_number_of_choose()
{
    return number_of_choose;
}

void Raylib::set_number_of_choose(int n)
{
    number_of_choose = n;
}

enum ::e_Menu Raylib::Menu_()
{
    Texture2D background{};
    bool hasBackground = FileExists("assets/menu_background.png");
    if (hasBackground)
        background = LoadTexture("assets/menu_background.png");

    Music menuMusic{};
    bool hasMusic = FileExists("assets/Musics/menu.mp3");
    if (hasMusic)
    {
        menuMusic = LoadMusicStream("assets/Musics/menu.mp3");
        menuMusic.looping = true;
        PlayMusicStream(menuMusic);
    }

    std::vector<e_Menu> values = {e_Menu::Play, e_Menu::Continue, e_Menu::Help, e_Menu::Exit};


    struct Hotspot { float x, y, w, h; };
    Hotspot hotspots[4] = {
        {0.0096f, 0.4092f, 0.2715f, 0.0893f}, // Play
        {0.0096f, 0.5314f, 0.2715f, 0.0893f}, // Continue
        {0.0096f, 0.6536f, 0.2715f, 0.0893f}, // Help
        {0.0096f, 0.7758f, 0.2715f, 0.0893f}, // Exit
    };

    int selected = 0;
    float scale = 1.0f; 
    const float growTarget = 1.08f;

    while (!WindowShouldClose())
    {
        if (hasMusic)
            UpdateMusicStream(menuMusic);

        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        Rectangle buttons[4];
        for (int i = 0; i < 4; i++)
            buttons[i] = {hotspots[i].x * screenW, hotspots[i].y * screenH,
                          hotspots[i].w * screenW, hotspots[i].h * screenH};

        // --- input ---
        Vector2 mouse = GetMousePosition();
        bool mouseOverButton = false;
        int hoveredThisFrame = selected;
        for (int i = 0; i < 4; i++)
            if (CheckCollisionPointRec(mouse, buttons[i])) { hoveredThisFrame = i; mouseOverButton = true; }

        int prevSelected = selected;
        selected = hoveredThisFrame;
        SetMouseCursor(mouseOverButton ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) selected = (selected + 1) % 4;
        if (IsKeyPressed(KEY_UP)   || IsKeyPressed(KEY_W)) selected = (selected + 3) % 4;

        if (selected != prevSelected) scale = 1.0f; 

        bool confirmed = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) ||
                          (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverButton);

        // smoothly ease scale up to the target
        float dt = GetFrameTime();
        float t = dt * 12.0f;
        if (t > 1.0f) t = 1.0f;
        scale += (growTarget - scale) * t;

        // --- draw ---
        BeginDrawing();
        ClearBackground(BLACK);

        if (hasBackground)
        {
            DrawTexturePro(background, {0, 0, (float)background.width, (float)background.height},
                            {0, 0, (float)screenW, (float)screenH}, {0, 0}, 0, WHITE);


            Rectangle src = {hotspots[selected].x * background.width, hotspots[selected].y * background.height,
                              hotspots[selected].w * background.width, hotspots[selected].h * background.height};
            Rectangle base = buttons[selected];

            float destW = base.width * scale;
            float destH = base.height * scale;
            Vector2 center = {base.x + base.width / 2, base.y + base.height / 2};

            Rectangle dest = {center.x, center.y, destW, destH};
            Vector2 origin = {destW / 2, destH / 2};
            DrawTexturePro(background, src, dest, origin, 0.0f, WHITE);
        }
        else
        {
            DrawRectangleGradientV(0, 0, screenW, screenH, Color{16, 15, 14, 255}, Color{34, 12, 12, 255});
        }

        EndDrawing();

        if (confirmed)
            break;
    }

    if (hasMusic)
    {
        StopMusicStream(menuMusic);
        UnloadMusicStream(menuMusic);
    }

    if (hasBackground)
        UnloadTexture(background);

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    return values[selected];
}

void Raylib::Players_Info_List(Player *p1, Player *p2)
{
    Texture2D background{};
    bool hasBackground = FileExists("assets/menusimple.png");
    if (hasBackground)
        background = LoadTexture("assets/menusimple.png");


    Music menuMusic{};

    bool hasMusic = FileExists("assets/Musics/menu.mp3");
    if (hasMusic)
    {
        menuMusic = LoadMusicStream("assets/Musics/menu.mp3");
        menuMusic.looping = true;
        PlayMusicStream(menuMusic);
    }


    Color gold      = Color{198, 170, 108, 255};
    Color goldDim   = Color{130, 110, 70, 255};
    Color panelBg   = Color{18, 16, 15, 200};
    Color fieldBg   = Color{10, 9, 8, 220};
    Color textDim   = Color{190, 185, 175, 255};

    std::string name_p1, age_p1, name_p2, age_p2;
    std::string *fields[4] = {&name_p1, &age_p1, &name_p2, &age_p2};
    
    int active = 0; 
    float cursorBlink = 0.0f;

    while (!WindowShouldClose())
    {
        if (hasMusic)
            UpdateMusicStream(menuMusic);
            
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        int margin  = (int)(screenW * 0.015f);
        float panelW = screenW * 0.38f;
        float panelH = screenH * 0.46f;
        float panelY = screenH * 0.32f;
        Rectangle panel1 = {screenW * 0.06f, panelY, panelW, panelH};
        Rectangle panel2 = {screenW * 0.56f, panelY, panelW, panelH};

        float fieldW = panelW * 0.86f;
        float fieldH = screenH * 0.065f;
        float pad    = panelW * 0.07f;

        Rectangle nameBox1 = {panel1.x + pad, panel1.y + panelH * 0.32f, fieldW, fieldH};
        Rectangle ageBox1  = {panel1.x + pad, panel1.y + panelH * 0.62f, fieldW, fieldH};
        Rectangle nameBox2 = {panel2.x + pad, panel2.y + panelH * 0.32f, fieldW, fieldH};
        Rectangle ageBox2  = {panel2.x + pad, panel2.y + panelH * 0.62f, fieldW, fieldH};
        Rectangle boxes[4] = {nameBox1, ageBox1, nameBox2, ageBox2};

        Rectangle beginBtn = {screenW / 2.0f - screenW * 0.09f, screenH * 0.86f, screenW * 0.18f, screenH * 0.07f};

        bool valid = !name_p1.empty() && !name_p2.empty() && !age_p1.empty() && !age_p2.empty()
                     && age_p1 != "0" && age_p2 != "0";

        // --- input ---
        Vector2 mouse = GetMousePosition();
        bool overBegin = CheckCollisionPointRec(mouse, beginBtn);
        bool overAnyField = false;
        for (int i = 0; i < 4; i++)
        {
            if (CheckCollisionPointRec(mouse, boxes[i]))
            {
                overAnyField = true;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) active = i;
            }
        }
        SetMouseCursor((overBegin && valid) || overAnyField ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        bool isAgeField = (active == 1 || active == 3);
        int key = GetCharPressed();
        while (key > 0)
        {
            std::string &target = *fields[active];
            if (isAgeField)
            {
                if (key >= '0' && key <= '9' && target.size() < 3)
                    target += (char)key;
            }
            else
            {
                if (key >= 32 && key <= 125 && target.size() < 18)
                    target += (char)key;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !fields[active]->empty())
            fields[active]->pop_back();

        if (IsKeyPressed(KEY_TAB))
            active = IsKeyDown(KEY_LEFT_SHIFT) ? (active + 3) % 4 : (active + 1) % 4;

        bool confirmed = false;
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
        {
            if (active < 3) active++;
            else if (valid) confirmed = true;
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overBegin && valid)
            confirmed = true;

        cursorBlink += GetFrameTime();
        bool showCursor = fmodf(cursorBlink, 1.0f) < 0.5f;

        // --- draw ---
        BeginDrawing();
        ClearBackground(BLACK);

        if (hasBackground)
        {
            DrawTexturePro(background, {0, 0, (float)background.width, (float)background.height},
                            {0, 0, (float)screenW, (float)screenH}, {0, 0}, 0, WHITE);
            DrawRectangle(0, 0, screenW, screenH, Fade(BLACK, 0.72f));
        }
        else
        {
            DrawRectangleGradientV(0, 0, screenW, screenH, Color{16, 15, 14, 255}, Color{34, 12, 12, 255});
        }

        // outer ornamental frame
        Rectangle frame = {(float)margin, (float)margin, (float)(screenW - margin * 2), (float)(screenH - margin * 2)};
        DrawRectangleLinesEx(frame, 2.0f, gold);
        int cornerLen = (int)(screenW * 0.02f);
        auto corner = [&](float x, float y, int dx, int dy) {
            DrawLineEx({x, y}, {x + dx * (float)cornerLen, y}, 2.0f, gold);
            DrawLineEx({x, y}, {x, y + dy * (float)cornerLen}, 2.0f, gold);
        };
        corner(frame.x, frame.y, 1, 1);
        corner(frame.x + frame.width, frame.y, -1, 1);
        corner(frame.x, frame.y + frame.height, 1, -1);
        corner(frame.x + frame.width, frame.y + frame.height, -1, -1);

        // title
        const char *title = "THE HUNTERS GATHER";
        int titleSize = (int)(screenH * 0.06f);
        Vector2 titleDim = MeasureTextEx(GetGameFont(), title, (float)titleSize, 5.0f);
        Vector2 titlePos = {screenW / 2.0f - titleDim.x / 2.0f, (float)(screenH * 0.08f)};
        DrawTextEx(GetGameFont(), title, {titlePos.x + 3, titlePos.y + 3}, (float)titleSize, 5.0f, Fade(BLACK, 0.7f));
        DrawTextEx(GetGameFont(), title, titlePos, (float)titleSize, 5.0f, Color{188, 182, 172, 255});

        const char *subtitle = "ENTER EACH PLAYER'S NAME AND AGE";
        int subSize = (int)(screenH * 0.017f);
        Vector2 subDim = MeasureTextEx(GetGameFont(), subtitle, (float)subSize, 3.0f);
        float subY = titlePos.y + titleDim.y + 6;
        DrawTextEx(GetGameFont(), subtitle, {screenW / 2.0f - subDim.x / 2.0f, subY}, (float)subSize, 3.0f, gold);

        float dividerY = subY + subSize + (float)(screenH * 0.015f);
        float dividerHalfW = screenW * 0.07f;
        DrawLineEx({screenW / 2.0f - dividerHalfW, dividerY}, {screenW / 2.0f - 14, dividerY}, 2.0f, goldDim);
        DrawLineEx({screenW / 2.0f + 14, dividerY}, {screenW / 2.0f + dividerHalfW, dividerY}, 2.0f, goldDim);
        DrawPoly({screenW / 2.0f, dividerY}, 4, 7.0f, 45.0f, gold);

        // panels
        auto drawPanel = [&](Rectangle panel, const char *heading, Rectangle nameBox, Rectangle ageBox,
                              std::string &nameVal, std::string &ageVal, int nameIdx, int ageIdx)
        {
            DrawRectangleRounded(panel, 0.04f, 8, panelBg);
            DrawRectangleLinesEx(panel, 2.0f, goldDim);

            int headSize = (int)(screenH * 0.028f);
            Vector2 headDim = MeasureTextEx(GetGameFont(), heading, (float)headSize, 3.0f);
            DrawTextEx(GetGameFont(), heading,
                       {panel.x + panel.width / 2 - headDim.x / 2, panel.y + panelH * 0.08f},
                       (float)headSize, 3.0f, gold);

            auto drawField = [&](Rectangle box, const char *label, std::string &value, int idx)
            {
                int labelSize = (int)(screenH * 0.016f);
                DrawTextEx(GetGameFont(), label, {box.x, box.y - labelSize - 6}, (float)labelSize, 2.0f, textDim);

                bool isActive = (active == idx);
                DrawRectangleRec(box, fieldBg);
                DrawRectangleLinesEx(box, isActive ? 2.5f : 1.5f, isActive ? gold : goldDim);

                int textSize = (int)(fieldH * 0.42f);
                std::string shown = value;
                if (isActive && showCursor) shown += "|";
                DrawTextEx(GetGameFont(), shown.c_str(),
                           {box.x + 14, box.y + box.height / 2 - textSize / 2.0f},
                           (float)textSize, 2.0f, RAYWHITE);
            };

            drawField(nameBox, "NAME", nameVal, nameIdx);
            drawField(ageBox, "AGE", ageVal, ageIdx);
        };

        drawPanel(panel1, "PLAYER 1", nameBox1, ageBox1, name_p1, age_p1, 0, 1);
        drawPanel(panel2, "PLAYER 2", nameBox2, ageBox2, name_p2, age_p2, 2, 3);

        // begin button
        Color btnFill = valid ? (overBegin ? Color{178, 150, 88, 255} : Color{150, 126, 72, 255})
                               : Color{40, 38, 36, 255};
        Color btnBorder = valid ? gold : goldDim;
        Color btnText = valid ? Color{20, 16, 12, 255} : Color{110, 105, 98, 255};

        DrawRectangleRounded(beginBtn, 0.2f, 8, btnFill);
        DrawRectangleLinesEx(beginBtn, 2.0f, btnBorder);

        const char *btnLabel = "BEGIN";
        int btnTextSize = (int)(beginBtn.height * 0.4f);

        Vector2 btnDim = MeasureTextEx(GetGameFont(), btnLabel, (float)btnTextSize, 3.0f);
        DrawTextEx(GetGameFont(), btnLabel,
                   {beginBtn.x + beginBtn.width / 2 - btnDim.x / 2, beginBtn.y + beginBtn.height / 2 - btnDim.y / 2},
                   (float)btnTextSize, 3.0f, btnText);

        if (!valid)
        {
            const char *hint = "Fill in both names and ages to continue";
            int hintSize = (int)(screenH * 0.015f);

            Vector2 hDim = MeasureTextEx(GetGameFont(), hint, (float)hintSize, 2.0f);

            DrawTextEx(GetGameFont(), hint, {screenW / 2.0f - hDim.x / 2.0f, beginBtn.y + beginBtn.height + 10},
                       (float)hintSize, 2.0f, Color{150, 145, 138, 255});
        }

        const char *footer = "TAB to switch field   -   ENTER to continue";
        int footSize = (int)(screenH * 0.014f);

        Vector2 fDim = MeasureTextEx(GetGameFont(), footer, (float)footSize, 2.0f);

        DrawTextEx(GetGameFont(), footer, {screenW / 2.0f - fDim.x / 2.0f, (float)(screenH - screenH * 0.04f)},
                   (float)footSize, 2.0f, Color{140, 135, 128, 255});

        EndDrawing();

        if (confirmed)
            break;
    }

    if (hasBackground)
        UnloadTexture(background);

    
    if (hasMusic)
    {
        StopMusicStream(menuMusic);
        UnloadMusicStream(menuMusic);
    }

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    p1->set_name(name_p1);
    p1->set_age(std::stoi(age_p1)); 
    p2->set_name(name_p2);
    p2->set_age(std::stoi(age_p2));
}

std::vector<int> Raylib::Det_characters(Player *p1, Player *p2) 
{

    
    struct HeroOption
    {
        std::string label;
        std::string texture_path;
    };

    static const std::vector<HeroOption> master_list = {
        {"DRACULA", "assets/dracula/mydracula.png"},
        {"SHERLOCK HOLMES", "assets/sherlock/mysherlock.png"},
        {"INVISIBLE MAN", "assets/invisibleMan/myinv.png"},
    };

    Player *younger = (p1->get_age() <= p2->get_age()) ? p1 : p2;
    Player *older = (p1->get_age() <= p2->get_age()) ? p2 : p1;

    Color gold = Color{198, 170, 108, 255};

    Texture2D background{};
    bool hasBackground = FileExists("assets/catchbackground.png");
    if (hasBackground)
        background = LoadTexture("assets/catchbackground.png");

        Music heroMusic{};
    bool hasMusic = FileExists("assets/Musics/detcharacter.mp3");
    if (hasMusic)
    {
        heroMusic = LoadMusicStream("assets/Musics/detcharacter.mp3");
        heroMusic.looping = true;
        PlayMusicStream(heroMusic);
    }

    auto pick_hero = [&](const std::vector<int> &indices, const std::string &playerName) -> int
    {
        int count = (int)indices.size();

        std::vector<Texture2D> textures(count);
        std::vector<bool> hasTexture(count, false);
        for (int i = 0; i < count; i++)
        {
            const std::string &path = master_list[indices[i]].texture_path;
            if (FileExists(path.c_str()))
            {
                textures[i] = LoadTexture(path.c_str());
                hasTexture[i] = true;
            }
        }

        int selected = -1; 
        int hovered = -1;
        float lift = 0.0f; 

        while (!WindowShouldClose())
        {
              if (hasMusic)
                UpdateMusicStream(heroMusic);
                
            int screenW = GetScreenWidth();
            int screenH = GetScreenHeight();

            float colW = screenW / (float)count;
            float baseline = screenH * 0.90f;   
            float maxPortraitH = screenH * 0.68f;

            std::vector<Rectangle> heroRects(count);
            for (int i = 0; i < count; i++)
            {
                float colCenterX = colW * i + colW / 2.0f;
                float destH = maxPortraitH;
                float destW = destH;
                if (hasTexture[i])
                {
                    float texRatio = (float)textures[i].width / (float)textures[i].height;
                    destW = destH * texRatio;
                    float maxW = colW * 0.92f;
                    if (destW > maxW) { destW = maxW; destH = destW / texRatio; }
                }
                else
                {
                    destW = colW * 0.5f;
                }
                heroRects[i] = {colCenterX - destW / 2.0f, baseline - destH, destW, destH};
            }

            // --- input ---
            Vector2 mouse = GetMousePosition();
            hovered = -1;
            for (int i = 0; i < count; i++)
                if (CheckCollisionPointRec(mouse, heroRects[i])) hovered = i;

            SetMouseCursor(hovered != -1 ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) selected = (selected + 1 + count) % count;
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) selected = (selected - 1 + count) % count;

           
            int displayHighlight = (hovered != -1) ? hovered : selected;

            bool confirmed = false;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hovered != -1)
            {
                selected = hovered;
                confirmed = true;
            }
            else if (displayHighlight != -1 && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)))
            {
                selected = displayHighlight; 
                confirmed = true;
            }

            float dt = GetFrameTime();
            float t = dt * 10.0f;
            if (t > 1.0f) t = 1.0f;
            lift += ((displayHighlight != -1 ? 1.0f : 0.0f) - lift) * t;

            // --- draw ---
            BeginDrawing();
            if (hasBackground)
            {
                DrawTexturePro(background, {0, 0, (float)background.width, (float)background.height},
                                {0, 0, (float)screenW, (float)screenH}, {0, 0}, 0, WHITE);
            }
            else
            {
                DrawRectangleGradientV(0, 0, screenW, screenH, Color{16, 15, 14, 255}, Color{34, 12, 12, 255});
            }

            std::string title = playerName + ", CHOOSE YOUR HERO";
            int titleSize = (int)(screenH * 0.045f);

            Vector2 titleDim = MeasureTextEx(GetGameFont(), title.c_str(), (float)titleSize, 4.0f);
            Vector2 titlePos = {screenW / 2.0f - titleDim.x / 2.0f, screenH * 0.05f};
            DrawTextEx(GetGameFont(), title.c_str(), {titlePos.x + 3, titlePos.y + 3},
                       (float)titleSize, 4.0f, Fade(BLACK, 0.6f));
            DrawTextEx(GetGameFont(), title.c_str(), titlePos, (float)titleSize, 4.0f, gold);

            for (int i = 0; i < count; i++)
            {
                float colCenterX = colW * i + colW / 2.0f;
                bool isHighlighted = (i == displayHighlight);
                float thisLift = isHighlighted ? lift : 0.0f;
                float riseAmount = thisLift * (screenH * 0.03f);
                float scale = 1.0f + thisLift * 0.08f;

                if (isHighlighted && thisLift > 0.01f)
                {
                    float glowR = colW * 0.55f * (0.85f + thisLift * 0.15f);
                    Color glowIn = Fade(gold, 0.28f * thisLift);
                    Color glowOut = Fade(gold, 0.0f);
                    DrawCircleGradient(Vector2{colCenterX, baseline - maxPortraitH * 0.35f},
                    glowR, glowIn, glowOut);
                }

                Rectangle base = heroRects[i];
                Rectangle dest = {base.x - base.width * (scale - 1.0f) / 2.0f,
                                   base.y - base.height * (scale - 1.0f) - riseAmount,
                                   base.width * scale, base.height * scale};

                if (hasTexture[i])
                {
                    Texture2D &tex = textures[i];

                    DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height},
                                    {dest.x + 6, dest.y + 8, dest.width, dest.height}, {0, 0}, 0,
                                    Fade(BLACK, isHighlighted ? 0.45f : 0.3f));

                    Color tint = isHighlighted ? WHITE : Color{170, 165, 160, 255}; // dim the rest
                    DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height}, dest, {0, 0}, 0, tint);
                }
                else
                {
                    const char *missing = "?";
                    int qSize = (int)(dest.height * 0.4f);
                    Vector2 qDim = MeasureTextEx(GetGameFont(), missing, (float)qSize, 2.0f);
                    DrawTextEx(GetGameFont(), missing,
                               {dest.x + dest.width / 2.0f - qDim.x / 2.0f, dest.y + dest.height / 2.0f - qDim.y / 2.0f},
                               (float)qSize, 2.0f, isHighlighted ? gold : Color{110, 105, 98, 255});
                }

                const std::string &label = master_list[indices[i]].label;
                int labelSize = (int)(screenH * 0.026f);
                Vector2 labelDim = MeasureTextEx(GetGameFont(), label.c_str(), (float)labelSize, 2.0f);
                Vector2 labelPos = {colCenterX - labelDim.x / 2.0f, baseline + screenH * 0.02f - riseAmount};
                DrawTextEx(GetGameFont(), label.c_str(), labelPos, (float)labelSize, 2.0f,
                           isHighlighted ? gold : Color{200, 197, 190, 255});

                if (isHighlighted && thisLift > 0.01f)
                {
                    float underlineW = labelDim.x * (0.9f + thisLift * 0.1f);
                    float underlineY = labelPos.y + labelDim.y + 6;
                    DrawLineEx({colCenterX - underlineW / 2.0f, underlineY},
                               {colCenterX + underlineW / 2.0f, underlineY}, 2.0f, Fade(gold, thisLift));
                }
            }

            const char *footer = "CLICK a hero to choose them   -   or use ARROWS + ENTER";
            int footSize = (int)(screenH * 0.016f);
            Vector2 fDim = MeasureTextEx(GetGameFont(), footer, (float)footSize, 2.0f);
            DrawTextEx(GetGameFont(), footer,
                       {screenW / 2.0f - fDim.x / 2.0f, screenH * 0.955f},
                       (float)footSize, 2.0f, Color{140, 135, 128, 255});

            EndDrawing();

            if (confirmed)
                break;
        }

        for (int i = 0; i < count; i++)
            if (hasTexture[i])
                UnloadTexture(textures[i]);

        SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        return indices[selected];
    };

    std::vector<int> allIndices = {0, 1, 2};
    int younger_choice = pick_hero(allIndices, younger->get_name());

    std::vector<int> remaining;
    for (int i : allIndices)
        if (i != younger_choice)
            remaining.push_back(i);

    int older_choice = pick_hero(remaining, older->get_name());

    if (hasMusic)
    {
        StopMusicStream(heroMusic);
        UnloadMusicStream(heroMusic);
    }

    if (hasBackground)
        UnloadTexture(background);

    return {younger_choice, older_choice};
}

void Raylib::catch_place(Player *p1, Player *p2, Board *board)
{
    if (p1 == nullptr || p2 == nullptr || board == nullptr)
        return;

    using namespace BoardVisual;

    Player *younger = (p1->get_age() <= p2->get_age()) ? p1 : p2;
    Player *older   = (p1->get_age() <= p2->get_age()) ? p2 : p1;

    const int optionSpaces[2] = {2, 23};

    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};

    Texture2D background{};
    bool hasBackground = FileExists("assets/catchbackground.png");
    if (hasBackground)
        background = LoadTexture("assets/catchbackground.png");

    Music placeMusic{};
    bool hasMusic = FileExists("assets/Musics/detcharacter.mp3");
    if (hasMusic)
    {
        placeMusic = LoadMusicStream("assets/Musics/detcharacter.mp3");
        placeMusic.looping = true;
        PlayMusicStream(placeMusic);
    }

    int selected = -1;
    float lift[2] = {0.0f, 0.0f};

    while (!WindowShouldClose())
    {

        if (hasMusic)
            UpdateMusicStream(placeMusic);

        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        Rectangle mapRect = {screenW * 0.06f, screenH * 0.17f, screenW * 0.88f, screenH * 0.72f};

        MapTransform xform = Fit(mapRect);
        float radius = NodeRadius(xform);

        Vector2 optionPos[2] = {SpacePos(xform, optionSpaces[0]), SpacePos(xform, optionSpaces[1])};

        // --- input ---
        Vector2 mouse = GetMousePosition();
        int hovered = -1;
        for (int i = 0; i < 2; i++)
            if (CheckCollisionPointCircle(mouse, optionPos[i], radius * 1.8f))
                hovered = i;

        SetMouseCursor(hovered != -1 ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
            selected = (selected == -1) ? 0 : 1 - selected;

        bool confirmed = false;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hovered != -1)
        {
            selected = hovered;
            confirmed = true;
        }
        else if (selected != -1 && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)))
        {
            confirmed = true;
        }

        int displayHighlight = (hovered != -1) ? hovered : selected;

        float dt = GetFrameTime();
        float tt = fminf(dt * 10.0f, 1.0f);
        for (int i = 0; i < 2; i++)
            lift[i] += ((displayHighlight == i ? 1.0f : 0.0f) - lift[i]) * tt;

        // --- draw ---
        BeginDrawing();

        if (hasBackground)
        {
            DrawTexturePro(background, {0, 0, (float)background.width, (float)background.height},
                            {0, 0, (float)screenW, (float)screenH}, {0, 0}, 0, WHITE);
            DrawRectangle(0, 0, screenW, screenH, Fade(BLACK, 0.55f));
        }
        else
        {
            DrawRectangleGradientV(0, 0, screenW, screenH, Color{16, 15, 14, 255}, Color{34, 12, 12, 255});
        }

        std::string title = younger->get_name() + ", CHOOSE YOUR STARTING ROOM";
        int titleSize = (int)(screenH * 0.04f);
        Vector2 titleDim = MeasureTextEx(GetGameFont(), title.c_str(), (float)titleSize, 4.0f);
        Vector2 titlePos = {screenW / 2.0f - titleDim.x / 2.0f, screenH * 0.045f};
        DrawTextEx(GetGameFont(), title.c_str(), {titlePos.x + 3, titlePos.y + 3}, (float)titleSize, 4.0f, Fade(BLACK, 0.6f));
        DrawTextEx(GetGameFont(), title.c_str(), titlePos, (float)titleSize, 4.0f, gold);

        const char *subtitle = "Your rival's hero will begin on the opposite side of the mansion";
        int subSize = (int)(screenH * 0.017f);
        Vector2 subDim = MeasureTextEx(GetGameFont(), subtitle, (float)subSize, 2.0f);
        DrawTextEx(GetGameFont(), subtitle, {screenW / 2.0f - subDim.x / 2.0f, titlePos.y + titleDim.y + 8},
                   (float)subSize, 2.0f, textDim);

        DrawBoardMap(board, mapRect);

        for (int i = 0; i < 2; i++)
        {
            bool isHighlighted = (i == displayHighlight);
            float thisLift = lift[i];

            if (thisLift > 0.01f)
            {
                float glowR = radius * (2.1f + thisLift * 0.5f);
                DrawCircleGradient(optionPos[i], glowR,
                                    Fade(gold, 0.35f * thisLift), Fade(gold, 0.0f));
            }

            DrawRing(optionPos[i], radius + 3.0f, radius + 6.0f, 0, 360, 28,
                     isHighlighted ? gold : Fade(gold, 0.55f));

            std::string tag = "Room " + std::to_string(board->get_spaces()[optionSpaces[i]].get_number());
            int tagSize = (int)(screenH * 0.02f);
            Vector2 tagDim = MeasureTextEx(GetGameFont(), tag.c_str(), (float)tagSize, 2.0f);
            Vector2 tagPos = {optionPos[i].x - tagDim.x / 2.0f, optionPos[i].y + radius + 10.0f};
            DrawTextEx(GetGameFont(), tag.c_str(), {tagPos.x + 1, tagPos.y + 1}, (float)tagSize, 2.0f, Fade(BLACK, 0.6f));
            DrawTextEx(GetGameFont(), tag.c_str(), tagPos, (float)tagSize, 2.0f, isHighlighted ? gold : textDim);
        }

        const char *footer = "CLICK a glowing room to start there   -   or use ARROWS + ENTER";
        int footSize = (int)(screenH * 0.016f);
        Vector2 fDim = MeasureTextEx(GetGameFont(), footer, (float)footSize, 2.0f);
        DrawTextEx(GetGameFont(), footer, {screenW / 2.0f - fDim.x / 2.0f, screenH * 0.965f},
                   (float)footSize, 2.0f, Color{140, 135, 128, 255});

        EndDrawing();

        if (confirmed)
            break;
    }

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (hasMusic)
    {
        StopMusicStream(placeMusic);
        UnloadMusicStream(placeMusic);
    }

    if (hasBackground)
        UnloadTexture(background);

    if (selected == -1)
        selected = 0; // wind

    int youngerSpace = optionSpaces[selected];
    int olderSpace   = optionSpaces[1 - selected];

    younger->get_character()->set_place(&board->get_spaces()[youngerSpace]);
    board->get_spaces()[youngerSpace].set_hero(younger->get_character());

    older->get_character()->set_place(&board->get_spaces()[olderSpace]);
    board->get_spaces()[olderSpace].set_hero(older->get_character());
}


void Raylib::ChooseAction(Player *, Player *)
{
    static const std::vector<std::string> entries = {"Attack", "Maneuver", "Event", "Save", "Back"};
    int choice = SlotMenu(entries, "CHOOSE ACTION");
    set_number_of_choose(choice == -1 ? (int)entries.size() - 1 : choice); 
}

void Raylib::main_map(Player *p1, Player *p2, Board *board, Player *turn)
{
    if (p1 == nullptr || p2 == nullptr || board == nullptr || turn == nullptr)
        return;

  Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
Color panelBg  = Color{10, 9, 8, 215};    Texture2D background{};
    bool hasBackground = FileExists("assets/catchbackground.png");
    if (hasBackground)
        background = LoadTexture("assets/catchbackground.png");

    Music mapMusic{};
    bool hasMusic = FileExists("assets/Musics/main_map.mp3");
    if (hasMusic)
    {
        mapMusic = LoadMusicStream("assets/Musics/main_map.mp3");
        mapMusic.looping = true;
        PlayMusicStream(mapMusic);
    }

    static const std::vector<std::string> entries = {"Attack", "Maneuver", "Event", "Save", "Back"};

    int selected = 0;

    while (!WindowShouldClose())
    {
        if (hasMusic)
            UpdateMusicStream(mapMusic);

        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();
        auto clampf = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

        int entryTextSize = (int)clampf(screenH * 0.024f, 15.0f, 26.0f);
        float entryLineH  = entryTextSize * 1.55f;
        float listH       = entries.size() * entryLineH;

        int menuTitleSize = (int)(entryTextSize * 0.8f);
        float titleGap    = entryTextSize * 0.9f;
        float titleBlockH = menuTitleSize + titleGap;

        int footSize   = (int)clampf(screenH * 0.013f, 10.0f, 18.0f);
        float footGap  = entryTextSize * 0.7f;  
        float footBlockH = footGap + footSize;

        float bottomMargin = screenH * 0.03f;    
        float dividerGap   = screenH * 0.02f;   

        float bottomBlockH = dividerGap + titleBlockH + listH + footBlockH + bottomMargin;

        // ---------- layout ----------
        float margin = screenW * 0.015f;
        float boxW   = clampf(screenW * 0.135f, 220.0f, 320.0f);

        float topY = screenH * 0.085f;
        float topH = (float)screenH - topY - bottomBlockH; 

        Rectangle turnBanner = {margin, screenH * 0.02f, (float)screenW - margin * 2, screenH * 0.05f};
        Rectangle heroBox1   = {margin, topY, boxW, topH};
        Rectangle heroBox2   = {(float)screenW - margin - boxW, topY, boxW, topH};
        Rectangle mapRect    = {heroBox1.x + boxW + margin, topY,
                                 heroBox2.x - (heroBox1.x + boxW) - margin * 2, topH};

        float dividerY = mapRect.y + mapRect.height + dividerGap;
        float titleY   = dividerY + screenH * 0.012f;
        float listY    = titleY + titleBlockH;
        float footerY  = listY + listH + footGap;

        // ---------- input ----------
        Vector2 mouse = GetMousePosition();
        int hovered = -1;
        for (size_t i = 0; i < entries.size(); i++)
        {
            Vector2 dim = MeasureTextEx(GetGameFont(), entries[i].c_str(), (float)entryTextSize, 1.0f);
            Rectangle itemRect = {screenW / 2.0f - dim.x / 2.0f - 20.0f, listY + entryLineH * i,
                                   dim.x + 40.0f, entryLineH};
            if (CheckCollisionPointRec(mouse, itemRect))
                hovered = (int)i;
        }
        SetMouseCursor(hovered != -1 ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            selected = (selected == 0) ? (int)entries.size() - 1 : selected - 1;
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            selected = (selected + 1) % (int)entries.size();
        if (hovered != -1)
            selected = hovered;

        bool confirmed = false;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hovered != -1)
            confirmed = true;
        else if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
            confirmed = true;

        // ---------- draw ----------
        BeginDrawing();

        if (hasBackground)
        {
            DrawTexturePro(background, {0, 0, (float)background.width, (float)background.height},
                            {0, 0, (float)screenW, (float)screenH}, {0, 0}, 0, WHITE);
            DrawRectangle(0, 0, screenW, screenH, Fade(BLACK, 0.45f));
        }
        else
        {
            DrawRectangleGradientV(0, 0, screenW, screenH, Color{16, 15, 14, 255}, Color{34, 12, 12, 255});
        }

        DrawRectangleRounded(turnBanner, 0.3f, 8, panelBg);
        DrawRectangleRoundedLines(turnBanner, 0.3f, 8, Fade(gold, 0.6f));
        std::string turnText = "TURN : " + turn->get_name();
        int turnSize = (int)(turnBanner.height * 0.55f);
        Vector2 turnDim = MeasureTextEx(GetGameFont(), turnText.c_str(), (float)turnSize, 2.0f);
        DrawTextEx(GetGameFont(), turnText.c_str(),
                   {turnBanner.x + turnBanner.width / 2.0f - turnDim.x / 2.0f,
                    turnBanner.y + turnBanner.height / 2.0f - turnDim.y / 2.0f},
                   (float)turnSize, 2.0f, gold);

        DrawHeroBox(p1, heroBox1);
        DrawBoardMap(board, mapRect);
        DrawHeroBox(p2, heroBox2);

        float dividerW = screenW * 0.16f;
        DrawLineEx({screenW / 2.0f - dividerW / 2.0f, dividerY}, {screenW / 2.0f + dividerW / 2.0f, dividerY},
                    1.0f, Fade(gold, 0.5f));
        DrawCircleV({screenW / 2.0f, dividerY}, 2.5f, gold);

        const char *menuTitle = "CHOOSE ACTION";
        Vector2 menuTitleDim = MeasureTextEx(GetGameFont(), menuTitle, (float)menuTitleSize, 3.0f);
        DrawTextEx(GetGameFont(), menuTitle, {screenW / 2.0f - menuTitleDim.x / 2.0f, titleY},
                   (float)menuTitleSize, 3.0f, gold);

        for (size_t i = 0; i < entries.size(); i++)
        {
            bool isSelected = ((int)i == selected);

            Vector2 dim = MeasureTextEx(GetGameFont(), entries[i].c_str(), (float)entryTextSize, 1.0f);
            Vector2 pos = {screenW / 2.0f - dim.x / 2.0f, listY + entryLineH * i};

            if (isSelected)
            {
                float markX = pos.x - entryTextSize * 0.9f;
                DrawTextEx(GetGameFont(), ">", {markX, pos.y}, (float)entryTextSize, 1.0f, gold);
            }

            DrawTextEx(GetGameFont(), entries[i].c_str(), {pos.x + 1, pos.y + 1}, (float)entryTextSize, 1.0f, Fade(BLACK, 0.6f));
            DrawTextEx(GetGameFont(), entries[i].c_str(), pos, (float)entryTextSize, 1.0f, isSelected ? gold : textDim);
        }

        const char *footer = "UP/DOWN or hover to choose  -  ENTER or CLICK to confirm";
        Vector2 fDim = MeasureTextEx(GetGameFont(), footer, (float)footSize, 2.0f);
        DrawTextEx(GetGameFont(), footer, {screenW / 2.0f - fDim.x / 2.0f, footerY},
                   (float)footSize, 2.0f, Color{140, 135, 128, 255});

        EndDrawing();

        if (confirmed)
            break;
    }

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (hasMusic)
    {
        StopMusicStream(mapMusic);
        UnloadMusicStream(mapMusic);
    }

    if (hasBackground)
        UnloadTexture(background);

    set_number_of_choose(selected);
}

void Raylib::DrawHeroBox(Player *player, Rectangle area)
{
    if (player == nullptr || player->get_character() == nullptr)
        return;

    Heroes *hero = player->get_character();
    std::string hero_name = hero->get_name();

    Color gold     = Color{198, 170, 108, 255};
    Color panelBg  = Color{10, 9, 8, 140};
    Color textDim  = Color{190, 185, 175, 255};
    Color deadCol  = Color{140, 60, 60, 255};

    std::string comrade_title;
    Color titleColor = WHITE;
    Color comradeColor = textDim;
    int maxHealth = hero->get_Health();

    if (hero_name == "DRACULA")
    {
        comrade_title = "SISTERS";
        titleColor    = Color{200, 40, 40, 255};
        comradeColor  = Color{180, 120, 255, 255};
        maxHealth     = 13;
    }
    else if (hero_name == "SHERLOCKHOLMES")
    {
        comrade_title = "DR. WATSON";
        titleColor    = Color{70, 130, 220, 255};
        comradeColor  = Color{245, 205, 85, 255};
        maxHealth     = 16;
    }
    else if (hero_name == "InvisibleMan")
    {
        comrade_title = "FOG";
        titleColor    = Color{210, 210, 215, 255};
        comradeColor  = Color{150, 150, 150, 255};
        maxHealth     = 15;
    }

    auto clampf = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

    int nameSize = (int)clampf(area.width * 0.11f, 14.0f, 24.0f);
    int textSize = (int)clampf(area.width * 0.065f, 10.0f, 16.0f);
    float lineH  = textSize * 1.9f;
    float pad    = area.width * 0.08f;

    int lineCount = 1 + 5 ;
    if (!player->get_comrade().empty())
        lineCount += 1 /*comrade title*/ + (int)player->get_comrade().size();

    float contentH = lineCount * lineH + pad * 1.6f
                    + (!player->get_comrade().empty() ? lineH * 0.75f : 0.0f); 

    Rectangle box = {area.x, area.y, area.width, contentH};

    // ---------- panel ----------
    float roundness = 12.0f / fminf(box.width, box.height);
    DrawRectangleRounded(box, roundness, 10, panelBg);
    DrawRectangleRoundedLines(box, roundness, 10, Fade(gold, 0.6f));

    float x = box.x + pad;
    float y = box.y + pad * 0.8f;

    auto drawLine = [&](const std::string &s, Color col, int size)
    {
        DrawTextEx(GetGameFont(), s.c_str(), {x + 1, y + 1}, (float)size, 1.0f, Fade(BLACK, 0.6f));

        DrawTextEx(GetGameFont(), s.c_str(), {x,        y},        (float)size, 1.0f, col);
        DrawTextEx(GetGameFont(), s.c_str(), {x + 0.6f, y},        (float)size, 1.0f, col);
        DrawTextEx(GetGameFont(), s.c_str(), {x,        y + 0.6f}, (float)size, 1.0f, col);

        y += lineH;
    };

    // ---------- hero header ----------
    drawLine(hero_name, titleColor, nameSize);

    drawLine("Health : " + std::to_string(hero->get_Health()) + " / " + std::to_string(maxHealth),
              hero->get_islive() ? textDim : deadCol, textSize);
    drawLine("Action : " + std::to_string(2 - player->get_count()), textDim, textSize);
    drawLine("Card in hand : "    + std::to_string(hero->get_hand().size()),    textDim, textSize);
    drawLine("Card in deck : "    + std::to_string(hero->get_deck().size()),    textDim, textSize);
    drawLine("Card in discard : " + std::to_string(hero->get_discard().size()), textDim, textSize);

    // ---------- comrades / tokens ----------
    if (!player->get_comrade().empty())
    {
        y += lineH * 0.35f;
        DrawLineEx({x, y}, {box.x + box.width - pad, y}, 1.0f, Fade(gold, 0.5f));
        y += lineH * 0.4f;

        drawLine(comrade_title, comradeColor, (int)(nameSize * 0.75f));

        for (Heroes *c : player->get_comrade())
        {
            if (c == nullptr)
                continue;

            std::string label = (c->get_number() == 0)
                                     ? c->get_name()
                                     : c->get_name() + std::to_string(c->get_number());

            std::string line = label + " : " + std::to_string(c->get_Health());
            drawLine(line, c->get_islive() ? textDim : deadCol, textSize);
        }
    }
}

void Raylib::DrawBoardMap(Board *board, Rectangle area)
{
    if (board == nullptr)
        return;

    using namespace BoardVisual;

    MapTransform xform = Fit(area);
    auto toScreen = [&](Vector2 p) -> Vector2 { return ToScreen(xform, p); };
    auto clampf = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

    Color gold        = Color{176, 182, 168, 255};  
    Color corridorCol = Color{92, 102, 88, 255};    
    Color portalCol   = Color{140, 118, 178, 190};   
    Color nodeFill    = Color{14, 18, 17, 255};      
    Color textDim     = Color{188, 196, 190, 255};   

    // ---------- corridors (drawn first, beneath the rooms) ----------
    float lineThick = clampf(xform.scale * 0.9f, 1.4f, 3.0f);

    for (auto &e : kCorridors)
    {
        Vector2 p1 = toScreen(kLayout[e[0]]);
        Vector2 p2 = toScreen(kLayout[e[1]]);
        DrawLineEx(p1, p2, lineThick + 1.5f, Fade(BLACK, 0.35f)); // soft ink shadow
        DrawLineEx(p1, p2, lineThick, corridorCol);
    }



    // ---------- rooms ----------
    float radius = NodeRadius(xform);

    bool isPortalRoom[32] = {false};
    for (auto &e : kPortals)
    {
        isPortalRoom[e[0]] = true;
        isPortalRoom[e[1]] = true;
    }

    auto heroColor = [](const std::string &name) -> Color
    {
        if (name == "DRACULA")        return Color{176, 32, 42, 255};
        if (name == "SISTERS")        return Color{150, 96, 205, 255};
        if (name == "SHERLOCKHOLMES") return Color{70, 110, 190, 255};
        if (name == "Dr_Watson")      return Color{214, 158, 66, 255};
        if (name == "InvisibleMan")   return Color{205, 205, 210, 255};
        return Color{170, 170, 170, 255};
    };

    auto heroLabel = [](Heroes *h) -> std::string
    {
        std::string n = h->get_name();
        std::string base;

        if (n == "DRACULA")             base = "DR";
        else if (n == "SISTERS")        base = "S";
        else if (n == "SHERLOCKHOLMES") base = "SH";
        else if (n == "Dr_Watson")      base = "W";
        else if (n == "InvisibleMan")   base = "IM";
        else                            base = n.substr(0, std::min<size_t>(2, n.size()));

        if (h->get_number() != 0)
            base += std::to_string(h->get_number());

        return base;
    };

    std::vector<Space> &spaces = board->get_spaces();

    for (int i = 0; i < 32 && i < (int)spaces.size(); i++)
    {
        Vector2 pos = toScreen(kLayout[i]);
        Space &sp = spaces[i];

        Heroes *hero = sp.get_hero();
        bool occupied = (hero != nullptr && hero->get_name() != "FOG");

        if (isPortalRoom[i])
            DrawPoly(pos, 4, radius * 1.55f, 45.0f, Fade(portalCol, 0.22f));

        if (sp.get_Fog() != nullptr)
            DrawRing(pos, radius + 2.0f, radius + 6.0f, 0, 360, 24, Fade(Color{150, 150, 155, 255}, 0.55f));

        DrawCircleV({pos.x + 2.0f, pos.y + 3.0f}, radius, Fade(BLACK, 0.45f));

        DrawCircleV(pos, radius, occupied ? heroColor(hero->get_name()) : nodeFill);

        Color ringCol = (occupied && !hero->get_islive()) ? Color{120, 40, 40, 255} : gold;
        DrawRing(pos, radius - 2.0f, radius, 0, 360, 28, ringCol);

        std::string label = occupied ? heroLabel(hero) : std::to_string(sp.get_number());
        int fontSize = (int)clampf(radius * 0.8f, 7.0f, 15.0f);
        Vector2 dim = MeasureTextEx(GetGameFont(), label.c_str(), (float)fontSize, 1.0f);
        Vector2 textPos = {pos.x - dim.x / 2.0f, pos.y - dim.y / 2.0f};

        DrawTextEx(GetGameFont(), label.c_str(), {textPos.x + 1, textPos.y + 1}, (float)fontSize, 1.0f, Fade(BLACK, 0.6f));
        DrawTextEx(GetGameFont(), label.c_str(), textPos, (float)fontSize, 1.0f, occupied ? WHITE : textDim);
    }
}

void Raylib::choose_comrad_place(Player *p1, Player *p2, Board *board)
{
    if (p1 == nullptr || p2 == nullptr || board == nullptr)
    return;

    using namespace BoardVisual;
    
    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
    Color green   = Color{110, 200, 120, 255};
    
    Texture2D background{};
    bool hasBackground = FileExists("assets/catchbackground.png");
    if (hasBackground)
    background = LoadTexture("assets/catchbackground.png");
    
    Music placeMusic{};
    bool hasMusic = FileExists("assets/Musics/detcharacter.mp3");
    if (hasMusic)
    {
        placeMusic = LoadMusicStream("assets/Musics/detcharacter.mp3");
        placeMusic.looping = true;
        PlayMusicStream(placeMusic);
    }

    Space *spaceBase = &board->get_spaces()[0]; 
    
  
    auto place_comrades = [&](Player *player, const std::string &comrade_label, bool isFogToken)
    {
        auto clampf_local = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

        int needed = (int)player->get_comrade().size();
        if (needed == 0)
        return;

        std::vector<Space *> zone = player->get_character()->get_place()->get_zone();
        if (zone.empty())
        return;

        std::vector<int> zoneIndices;
        zoneIndices.reserve(zone.size());
        for (Space *s : zone)
        zoneIndices.push_back((int)(s - spaceBase));

        std::vector<int> chosen; 
        int selected = -1;
        
        while (!WindowShouldClose())
        {
            if (hasMusic)
            UpdateMusicStream(placeMusic);
            
            int screenW = GetScreenWidth();
            int screenH = GetScreenHeight();
            
            Rectangle mapRect = {screenW * 0.06f, screenH * 0.17f, screenW * 0.88f, screenH * 0.72f};
            MapTransform xform = Fit(mapRect);
            float radius = NodeRadius(xform);

            // --- input ---
            Vector2 mouse = GetMousePosition();
            int hovered = -1;
            for (size_t i = 0; i < zoneIndices.size(); i++)
                if (CheckCollisionPointCircle(mouse, SpacePos(xform, zoneIndices[i]), radius * 1.8f))
                    hovered = (int)i;

            auto isChosen = [&](int zoneIdx)
            {
                for (int c : chosen)
                    if (c == zoneIdx)
                        return true;
                return false;
            };

            bool hoveredTaken = (hovered != -1) && isChosen(hovered);
            SetMouseCursor((hovered != -1 && !hoveredTaken) ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

            if (!zoneIndices.empty() && (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)))
                selected = (selected <= 0) ? (int)zoneIndices.size() - 1 : selected - 1;
            if (!zoneIndices.empty() && (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)))
                selected = (selected == -1) ? 0 : (selected + 1) % (int)zoneIndices.size();

            int pick = -1;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hovered != -1 && !hoveredTaken)
                pick = hovered;
            else if (selected != -1 && !isChosen(selected) && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)))
                pick = selected;

            if (pick != -1)
                chosen.push_back(pick);

            int displayHighlight = (hovered != -1) ? hovered : selected;

            // --- draw ---
            BeginDrawing();

            if (hasBackground)
            {
                DrawTexturePro(background, {0, 0, (float)background.width, (float)background.height},
                                {0, 0, (float)screenW, (float)screenH}, {0, 0}, 0, WHITE);
                DrawRectangle(0, 0, screenW, screenH, Fade(BLACK, 0.55f));
            }
            else
            {
                DrawRectangleGradientV(0, 0, screenW, screenH, Color{16, 15, 14, 255}, Color{34, 12, 12, 255});
            }

            std::string title = player->get_name() + ", PLACE YOUR " + comrade_label;
            int titleSize = (int)(screenH * 0.04f);
            Vector2 titleDim = MeasureTextEx(GetGameFont(), title.c_str(), (float)titleSize, 4.0f);
            Vector2 titlePos = {screenW / 2.0f - titleDim.x / 2.0f, screenH * 0.045f};
            DrawTextEx(GetGameFont(), title.c_str(), {titlePos.x + 3, titlePos.y + 3}, (float)titleSize, 4.0f, Fade(BLACK, 0.6f));
            DrawTextEx(GetGameFont(), title.c_str(), titlePos, (float)titleSize, 4.0f, gold);

            std::string subtitle = "Selected " + std::to_string(chosen.size()) + " / " + std::to_string(needed) + " room(s)";
            int subSize = (int)(screenH * 0.017f);
            Vector2 subDim = MeasureTextEx(GetGameFont(), subtitle.c_str(), (float)subSize, 2.0f);
            DrawTextEx(GetGameFont(), subtitle.c_str(), {screenW / 2.0f - subDim.x / 2.0f, titlePos.y + titleDim.y + 8},
                       (float)subSize, 2.0f, (int)chosen.size() == needed ? green : textDim);

            DrawBoardMap(board, mapRect);

            for (size_t i = 0; i < zoneIndices.size(); i++)
            {
                Vector2 pos = SpacePos(xform, zoneIndices[i]);
                bool taken = isChosen((int)i);
                bool isHighlighted = ((int)i == displayHighlight) && !taken;

                if (isHighlighted)
                {
                    float glowR = radius * 2.3f;
                    DrawCircleGradient(pos, glowR, Fade(gold, 0.35f), Fade(gold, 0.0f));
                }

                Color ringCol = taken ? green : (isHighlighted ? gold : Fade(gold, 0.55f));
                DrawRing(pos, radius + 3.0f, radius + 6.0f, 0, 360, 28, ringCol);

                if (taken)
                {
                    const char *mark = "OK";
                    int markSize = (int)clampf_local(radius * 0.7f, 8.0f, 16.0f);
                    Vector2 mDim = MeasureTextEx(GetGameFont(), mark, (float)markSize, 1.0f);
                    Vector2 mPos = {pos.x - mDim.x / 2.0f, pos.y - radius - mDim.y - 6.0f};
                    DrawTextEx(GetGameFont(), mark, {mPos.x + 1, mPos.y + 1}, (float)markSize, 1.0f, Fade(BLACK, 0.6f));
                    DrawTextEx(GetGameFont(), mark, mPos, (float)markSize, 1.0f, green);
                }
            }

            const char *footer = "CLICK an open, glowing room to place a comrade there   -   or use ARROWS + ENTER";
            int footSize = (int)(screenH * 0.016f);
            Vector2 fDim = MeasureTextEx(GetGameFont(), footer, (float)footSize, 2.0f);
            DrawTextEx(GetGameFont(), footer, {screenW / 2.0f - fDim.x / 2.0f, screenH * 0.965f},
                       (float)footSize, 2.0f, Color{140, 135, 128, 255});

            EndDrawing();

            if ((int)chosen.size() == needed)
                break;
        }

        for (int i = 0; i < needed; i++)
        {
            Space *dest = zone[chosen[i]];
            player->get_comrade()[i]->set_place(dest);

            if (isFogToken)
                dest->set_Fog(player->get_comrade()[i]);
            else
                dest->set_hero(player->get_comrade()[i]);
        }
    };

    for (Player *player : {p1, p2})
    {
        std::string hero_name = player->get_character()->get_name();

        if (hero_name == "DRACULA")
            place_comrades(player, "SISTERS", false);
        else if (hero_name == "SHERLOCKHOLMES")
            place_comrades(player, "DR. WATSON", false);
        else if (hero_name == "InvisibleMan")
            place_comrades(player, "FOG TOKENS", true);
    }

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (hasMusic)
    {
        StopMusicStream(placeMusic);
        UnloadMusicStream(placeMusic);
    }

    if (hasBackground)
        UnloadTexture(background);
}



bool Raylib::AskUseSpecialAbility(Heroes *hero, Board *board)
{
    if (hero == nullptr)
        return false;

    std::vector<std::string> entries = {"Yes - use special ability", "No - skip it"};

    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
    Color panelBg = Color{10, 9, 8, 255};

    auto clampf = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

    int selected = 0;
    bool confirmed = false;
    bool backOut = false;

    while (!WindowShouldClose())
    {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        int entryTextSize = (int)clampf(screenH * 0.026f, 15.0f, 28.0f);
        float entryLineH  = entryTextSize * 1.6f;
        float listH       = entries.size() * entryLineH;

        int titleSize  = (int)(entryTextSize * 1.1f);
        float titleGap = entryTextSize * 0.9f;

        int btnTextSize = (int)(entryTextSize * 0.9f);
        float btnH      = btnTextSize * 2.2f;

        float panelW = clampf(screenW * 0.30f, 320.0f, 480.0f);
        float pad    = panelW * 0.08f;
        float panelH = titleSize + titleGap + listH + entryLineH * 0.6f + btnH + pad * 2.2f;

        Rectangle mapRect = {screenW * 0.02f, screenH * 0.06f, screenW * 0.58f, screenH * 0.88f};

        float panelMargin = screenW * 0.025f;
        Rectangle panel = {
            screenW - panelW - panelMargin,
            clampf(screenH / 2.0f - panelH / 2.0f, screenH * 0.04f, screenH * 0.96f - panelH),
            panelW, panelH};
        panel.x = std::max(panel.x, mapRect.x + mapRect.width + panelMargin);

        float x      = panel.x + pad;
        float titleY = panel.y + pad;
        float listY  = titleY + titleSize + titleGap;

        std::vector<Rectangle> rowRects(entries.size());
        for (size_t i = 0; i < entries.size(); i++)
            rowRects[i] = {x, listY + entryLineH * i, panel.width - pad * 2, entryLineH};

        float buttonsY = listY + listH + entryLineH * 0.6f;
        Rectangle confirmBtn = {x, buttonsY, (panel.width - pad * 2) * 0.46f, btnH};
        Rectangle backBtn    = {x + (panel.width - pad * 2) * 0.54f, buttonsY, (panel.width - pad * 2) * 0.46f, btnH};

        Vector2 mouse = GetMousePosition();
        int hoveredRow = -1;
        for (size_t i = 0; i < rowRects.size(); i++)
            if (CheckCollisionPointRec(mouse, rowRects[i]))
                hoveredRow = (int)i;

        bool overConfirm = CheckCollisionPointRec(mouse, confirmBtn);
        bool overBack    = CheckCollisionPointRec(mouse, backBtn);

        SetMouseCursor((hoveredRow != -1 || overConfirm || overBack) ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            selected = (selected + 1) % (int)entries.size();
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            selected = (selected - 1 + (int)entries.size()) % (int)entries.size();
        if (hoveredRow != -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            selected = hoveredRow;

        if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overConfirm) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
            confirmed = true;
        else if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overBack) || IsKeyPressed(KEY_ESCAPE))
            backOut = true;

        BeginDrawing();
        ClearBackground(Color{16, 15, 14, 255});

        DrawBoardMap(board, mapRect);

        DrawRectangleRounded(panel, 0.08f, 10, panelBg);
        DrawRectangleRoundedLines(panel, 0.08f, 10, Fade(gold, 0.6f));

        std::string title = hero->get_name() + ", USE SPECIAL ABILITY?";
        int titleFit = titleSize;
        Vector2 titleDim = MeasureTextEx(GetGameFont(), title.c_str(), (float)titleFit, 2.0f);
        while (titleDim.x > panel.width - pad * 2 && titleFit > 8)
        {
            titleFit -= 1;
            titleDim = MeasureTextEx(GetGameFont(), title.c_str(), (float)titleFit, 2.0f);
        }
        DrawTextEx(GetGameFont(), title.c_str(), {panel.x + panel.width / 2.0f - titleDim.x / 2.0f, titleY},
                   (float)titleFit, 2.0f, gold);

        for (size_t i = 0; i < entries.size(); i++)
        {
            bool isSelected = ((int)i == selected);
            Vector2 pos = {x + 10.0f, rowRects[i].y + (entryLineH - entryTextSize) / 2.0f};

            if (isSelected)
                DrawRectangleRounded(rowRects[i], 0.25f, 8, Fade(gold, 0.18f));
            if ((int)i == hoveredRow)
                DrawRectangleRoundedLines(rowRects[i], 0.25f, 8, Fade(gold, 0.5f));

            DrawTextEx(GetGameFont(), entries[i].c_str(), {pos.x + 1, pos.y + 1}, (float)entryTextSize, 1.0f, Fade(BLACK, 0.6f));
            DrawTextEx(GetGameFont(), entries[i].c_str(), pos, (float)entryTextSize, 1.0f, isSelected ? gold : textDim);
        }

        auto drawButton = [&](Rectangle r, const std::string &label, bool hovered)
        {
            DrawRectangleRounded(r, 0.25f, 8, hovered ? Fade(gold, 0.3f) : panelBg);
            DrawRectangleRoundedLines(r, 0.25f, 8, Fade(gold, 0.6f));
            Vector2 dim = MeasureTextEx(GetGameFont(), label.c_str(), (float)btnTextSize, 1.0f);
            Vector2 bp  = {r.x + r.width / 2.0f - dim.x / 2.0f, r.y + r.height / 2.0f - dim.y / 2.0f};
            DrawTextEx(GetGameFont(), label.c_str(), bp, (float)btnTextSize, 1.0f, hovered ? WHITE : textDim);
        };

        drawButton(confirmBtn, "Confirm", overConfirm);
        drawButton(backBtn, "Decline", overBack);

        const char *footer = "CLICK an option, or use UP/DOWN + ENTER   -   ESC to decline";
        int footSize = (int)clampf(screenH * 0.014f, 10.0f, 18.0f);
        Vector2 fDim = MeasureTextEx(GetGameFont(), footer, (float)footSize, 2.0f);
        DrawTextEx(GetGameFont(), footer, {screenW / 2.0f - fDim.x / 2.0f, screenH * 0.965f},
                   (float)footSize, 2.0f, Color{140, 135, 128, 255});

        EndDrawing();

        if (confirmed || backOut)
            break;
    }

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (backOut)
        return false;

    return selected == 0;
}


bool Raylib::AskBurnCardForMove(Heroes *hero, Board *board)
{
    if (hero == nullptr)
        return false;

    std::vector<std::string> entries = {"Yes - burn a card to move further", "No - move normally"};

    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
    Color panelBg = Color{10, 9, 8, 255};

    auto clampf = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

    int selected = 0;
    bool confirmed = false;
    bool backOut = false;

    while (!WindowShouldClose())
    {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        int entryTextSize = (int)clampf(screenH * 0.026f, 15.0f, 28.0f);
        float entryLineH  = entryTextSize * 1.6f;
        float listH       = entries.size() * entryLineH;

        int titleSize  = (int)(entryTextSize * 1.1f);
        float titleGap = entryTextSize * 0.9f;

        int btnTextSize = (int)(entryTextSize * 0.9f);
        float btnH      = btnTextSize * 2.2f;

        float panelW = clampf(screenW * 0.30f, 320.0f, 480.0f);
        float pad    = panelW * 0.08f;
        float panelH = titleSize + titleGap + listH + entryLineH * 0.6f + btnH + pad * 2.2f;

      
        Rectangle mapRect = {screenW * 0.02f, screenH * 0.06f, screenW * 0.58f, screenH * 0.88f};

        float panelMargin = screenW * 0.025f;
        Rectangle panel = {
            screenW - panelW - panelMargin,
            clampf(screenH / 2.0f - panelH / 2.0f, screenH * 0.04f, screenH * 0.96f - panelH),
            panelW, panelH};
        panel.x = std::max(panel.x, mapRect.x + mapRect.width + panelMargin);

        float x     = panel.x + pad;
        float titleY = panel.y + pad;
        float listY  = titleY + titleSize + titleGap;

        std::vector<Rectangle> rowRects(entries.size());
        for (size_t i = 0; i < entries.size(); i++)
            rowRects[i] = {x, listY + entryLineH * i, panel.width - pad * 2, entryLineH};

        float buttonsY = listY + listH + entryLineH * 0.6f;
        Rectangle confirmBtn = {x, buttonsY, (panel.width - pad * 2) * 0.46f, btnH};
        Rectangle backBtn    = {x + (panel.width - pad * 2) * 0.54f, buttonsY, (panel.width - pad * 2) * 0.46f, btnH};

        // ---------- input ----------
        Vector2 mouse = GetMousePosition();
        int hoveredRow = -1;
        for (size_t i = 0; i < rowRects.size(); i++)
            if (CheckCollisionPointRec(mouse, rowRects[i]))
                hoveredRow = (int)i;

        bool overConfirm = CheckCollisionPointRec(mouse, confirmBtn);
        bool overBack    = CheckCollisionPointRec(mouse, backBtn);

        SetMouseCursor((hoveredRow != -1 or overConfirm or overBack) ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            selected = (selected + 1) % (int)entries.size();
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            selected = (selected - 1 + (int)entries.size()) % (int)entries.size();
        if (hoveredRow != -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            selected = hoveredRow;

        if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overConfirm) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
            confirmed = true;
        else if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overBack) || IsKeyPressed(KEY_ESCAPE))
            backOut = true;

        // ---------- draw ----------
        BeginDrawing();
        ClearBackground(Color{16, 15, 14, 255});

        DrawBoardMap(board, mapRect);

        DrawRectangleRounded(panel, 0.08f, 10, panelBg);
        DrawRectangleRoundedLines(panel, 0.08f, 10, Fade(gold, 0.6f));

        std::string title = hero->get_name() + ", BURN A CARD TO MOVE FURTHER?";
        int titleFit = titleSize;
        Vector2 titleDim = MeasureTextEx(GetGameFont(), title.c_str(), (float)titleFit, 2.0f);
        while (titleDim.x > panel.width - pad * 2 && titleFit > 8)
        {
            titleFit -= 1;
            titleDim = MeasureTextEx(GetGameFont(), title.c_str(), (float)titleFit, 2.0f);
        }
        DrawTextEx(GetGameFont(), title.c_str(), {panel.x + panel.width / 2.0f - titleDim.x / 2.0f, titleY},
                   (float)titleFit, 2.0f, gold);

        for (size_t i = 0; i < entries.size(); i++)
        {
            bool isSelected = ((int)i == selected);
            Vector2 pos = {x + 10.0f, rowRects[i].y + (entryLineH - entryTextSize) / 2.0f};

            if (isSelected)
                DrawRectangleRounded(rowRects[i], 0.25f, 8, Fade(gold, 0.18f));
            if ((int)i == hoveredRow)
                DrawRectangleRoundedLines(rowRects[i], 0.25f, 8, Fade(gold, 0.5f));

            DrawTextEx(GetGameFont(), entries[i].c_str(), {pos.x + 1, pos.y + 1}, (float)entryTextSize, 1.0f, Fade(BLACK, 0.6f));
            DrawTextEx(GetGameFont(), entries[i].c_str(), pos, (float)entryTextSize, 1.0f, isSelected ? gold : textDim);
        }

        auto drawButton = [&](Rectangle r, const std::string &label, bool hovered)
        {
            DrawRectangleRounded(r, 0.25f, 8, hovered ? Fade(gold, 0.3f) : panelBg);
            DrawRectangleRoundedLines(r, 0.25f, 8, Fade(gold, 0.6f));
            Vector2 dim = MeasureTextEx(GetGameFont(), label.c_str(), (float)btnTextSize, 1.0f);
            Vector2 bp  = {r.x + r.width / 2.0f - dim.x / 2.0f, r.y + r.height / 2.0f - dim.y / 2.0f};
            DrawTextEx(GetGameFont(), label.c_str(), bp, (float)btnTextSize, 1.0f, hovered ? WHITE : textDim);
        };

        drawButton(confirmBtn, "Confirm", overConfirm);
        drawButton(backBtn, "Decline", overBack);

        const char *footer = "CLICK an option, or use UP/DOWN + ENTER   -   ESC to decline";
        int footSize = (int)clampf(screenH * 0.014f, 10.0f, 18.0f);
        Vector2 fDim = MeasureTextEx(GetGameFont(), footer, (float)footSize, 2.0f);
        DrawTextEx(GetGameFont(), footer, {screenW / 2.0f - fDim.x / 2.0f, screenH * 0.965f},
                   (float)footSize, 2.0f, Color{140, 135, 128, 255});

        EndDrawing();

        if (confirmed || backOut)
            break;
    }

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (backOut)
        return false;

    return selected == 0; // entry 0 = "Yes"
}

static bool Exist_path(const std::vector<Space *> &zone, Space *target)
{
    if (zone.empty())
        throw std::runtime_error("zone (path) is empty.");

    for (Space *z : zone)
        if (z == target)
            return true;

    return false;
}

bool Raylib::Attakcer_Heroes_Menu(Player *player, Board *board, Heroes *&hero)
{
    hero = nullptr;

    if (player == nullptr)
        return false;

    std::vector<Heroes *> fighters;

    if (player->get_character() != nullptr and player->get_character()->get_islive()
        and player->get_character()->get_place() != nullptr
        and !player->get_character()->get_PendingPlacement())
        fighters.push_back(player->get_character());

    for (auto c : player->get_comrade())
    {
        if (c != nullptr and c->get_islive()
            and c->get_place() != nullptr
            and !c->get_PendingPlacement()
            and c->get_name() != "FOG")
            fighters.push_back(c);
    }

    if (fighters.empty())
        throw std::runtime_error("No valid target to attack.");

    std::vector<std::string> entries;
    for (auto h : fighters)
        entries.push_back((h->get_number() == 0 ? "" : std::to_string(h->get_number())) + h->get_name());

    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
Color panelBg = Color{10, 9, 8, 215};
    auto clampf = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

    int selected = 0;
    bool confirmed = false;
    bool backOut = false;

    while (!WindowShouldClose())
    {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        int entryTextSize = (int)clampf(screenH * 0.026f, 15.0f, 28.0f);
        float entryLineH  = entryTextSize * 1.6f;
        float listH       = entries.size() * entryLineH;

        int titleSize  = (int)(entryTextSize * 1.1f);
        float titleGap = entryTextSize * 0.9f;

        int btnTextSize = (int)(entryTextSize * 0.9f);
        float btnH      = btnTextSize * 2.2f;

        float panelW = clampf(screenW * 0.28f, 260.0f, 380.0f);
        float pad    = panelW * 0.08f;
        float panelH = titleSize + titleGap + listH + entryLineH * 0.6f + btnH + pad * 2.2f;

        Rectangle panel = {screenW * 0.04f, screenH / 2.0f - panelH / 2.0f, panelW, panelH};
        Rectangle mapRect = {panel.x + panel.width + screenW * 0.04f, screenH * 0.08f,
                              screenW - (panel.x + panel.width) - screenW * 0.08f, screenH * 0.84f};

        float x     = panel.x + pad;
        float titleY = panel.y + pad;
        float listY  = titleY + titleSize + titleGap;

        std::vector<Rectangle> rowRects(entries.size());
        for (size_t i = 0; i < entries.size(); i++)
            rowRects[i] = {x, listY + entryLineH * i, panel.width - pad * 2, entryLineH};

        float buttonsY = listY + listH + entryLineH * 0.6f;
        Rectangle confirmBtn = {x, buttonsY, (panel.width - pad * 2) * 0.46f, btnH};
        Rectangle backBtn    = {x + (panel.width - pad * 2) * 0.54f, buttonsY, (panel.width - pad * 2) * 0.46f, btnH};

        // ---------- input ----------
        Vector2 mouse = GetMousePosition();
        int hoveredRow = -1;
        for (size_t i = 0; i < rowRects.size(); i++)
            if (CheckCollisionPointRec(mouse, rowRects[i]))
                hoveredRow = (int)i;

        bool overConfirm = CheckCollisionPointRec(mouse, confirmBtn);
        bool overBack    = CheckCollisionPointRec(mouse, backBtn);

        SetMouseCursor((hoveredRow != -1 or overConfirm or overBack) ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            selected = (selected + 1) % (int)entries.size();
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            selected = (selected - 1 + (int)entries.size()) % (int)entries.size();
        if (hoveredRow != -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            selected = hoveredRow;

        if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overConfirm) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
            confirmed = true;
        else if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overBack) || IsKeyPressed(KEY_ESCAPE))
            backOut = true;

        // ---------- draw ----------
        BeginDrawing();
        ClearBackground(Color{16, 15, 14, 255});

        DrawBoardMap(board, mapRect);

        DrawRectangleRounded(panel, 0.08f, 10, panelBg);
        DrawRectangleRoundedLines(panel, 0.08f, 10, Fade(gold, 0.6f));

        std::string title = player->get_name() + ", CHOOSE ATTACKER";
        Vector2 titleDim = MeasureTextEx(GetGameFont(), title.c_str(), (float)titleSize, 2.0f);
        DrawTextEx(GetGameFont(), title.c_str(), {panel.x + panel.width / 2.0f - titleDim.x / 2.0f, titleY},
                   (float)titleSize, 2.0f, gold);

        for (size_t i = 0; i < entries.size(); i++)
        {
            bool isSelected = ((int)i == selected);
            Vector2 pos = {x + 10.0f, rowRects[i].y + (entryLineH - entryTextSize) / 2.0f};

            if (isSelected)
                DrawRectangleRounded(rowRects[i], 0.25f, 8, Fade(gold, 0.18f));
            if ((int)i == hoveredRow)
                DrawRectangleRoundedLines(rowRects[i], 0.25f, 8, Fade(gold, 0.5f));

            DrawTextEx(GetGameFont(), entries[i].c_str(), {pos.x + 1, pos.y + 1}, (float)entryTextSize, 1.0f, Fade(BLACK, 0.6f));
            DrawTextEx(GetGameFont(), entries[i].c_str(), pos, (float)entryTextSize, 1.0f, isSelected ? gold : textDim);
        }

        auto drawButton = [&](Rectangle r, const std::string &label, bool hovered)
        {
            DrawRectangleRounded(r, 0.25f, 8, hovered ? Fade(gold, 0.3f) : panelBg);
            DrawRectangleRoundedLines(r, 0.25f, 8, Fade(gold, 0.6f));
            Vector2 dim = MeasureTextEx(GetGameFont(), label.c_str(), (float)btnTextSize, 1.0f);
            Vector2 bp  = {r.x + r.width / 2.0f - dim.x / 2.0f, r.y + r.height / 2.0f - dim.y / 2.0f};
            DrawTextEx(GetGameFont(), label.c_str(), bp, (float)btnTextSize, 1.0f, hovered ? WHITE : textDim);
        };

        drawButton(confirmBtn, "Confirm", overConfirm);
        drawButton(backBtn, "Back", overBack);

        const char *footer = "CLICK a fighter, or use UP/DOWN + ENTER   -   ESC to go back";
        int footSize = (int)clampf(screenH * 0.014f, 10.0f, 18.0f);
        Vector2 fDim = MeasureTextEx(GetGameFont(), footer, (float)footSize, 2.0f);
        DrawTextEx(GetGameFont(), footer, {screenW / 2.0f - fDim.x / 2.0f, screenH * 0.965f},
                   (float)footSize, 2.0f, Color{140, 135, 128, 255});

        EndDrawing();

        if (confirmed || backOut)
            break;
    }

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (!confirmed)
        return false;

    hero = fighters[selected];
    return true;
}

bool Raylib::Defender_Heroes_Menu(Player *player, Board *board, Heroes *&defender, Heroes *&Attacker)
{
    defender = nullptr;

    if (player == nullptr)
        return false;

    if (Attacker == nullptr)
        throw std::runtime_error("Attacker is nullptr");

    if (Attacker->get_place() == nullptr)
        throw std::runtime_error("Attacker has no place.");

    std::vector<Heroes *> defenders;

    try
    {
        if (Attacker->get_Attacktype() == "MELEE")
        {
            if (player->get_character() != nullptr and player->get_character()->get_islive()
                and Exist_path(Attacker->get_place()->get_neighbor(), player->get_character()->get_place()))
                defenders.push_back(player->get_character());

            for (auto c : player->get_comrade())
            {
                if (c != nullptr and c->get_islive() and c->get_name() != "FOG"
                    and Exist_path(Attacker->get_place()->get_neighbor(), c->get_place()))
                    defenders.push_back(c);
            }
        }
        else if (Attacker->get_Attacktype() == "RANGED")
        {
            if (player->get_character() != nullptr and player->get_character()->get_islive()
                and Exist_path(Attacker->get_place()->get_zone(), player->get_character()->get_place()))
                defenders.push_back(player->get_character());

            for (auto c : player->get_comrade())
            {
                if (c != nullptr and c->get_islive() and c->get_name() != "FOG"
                    and Exist_path(Attacker->get_place()->get_zone(), c->get_place()))
                    defenders.push_back(c);
            }
        }
    }
    catch (const std::exception &e)
    {
        msg.push_back(e.what());
    }

    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
Color panelBg = Color{10, 9, 8, 215};
    auto clampf = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

    if (defenders.empty())
    {
        bool backOut = false;

        while (!WindowShouldClose())
        {
            int screenW = GetScreenWidth();
            int screenH = GetScreenHeight();

            int msgSize     = (int)clampf(screenH * 0.03f, 16.0f, 30.0f);
            int btnTextSize = (int)(msgSize * 0.75f);
            float btnW      = clampf(screenW * 0.14f, 140.0f, 220.0f);
            float btnH      = btnTextSize * 2.2f;

            Rectangle mapRect = {screenW * 0.08f, screenH * 0.08f, screenW * 0.84f, screenH * 0.42f};
            Rectangle backBtn = {screenW / 2.0f - btnW / 2.0f, screenH * 0.58f, btnW, btnH};

            Vector2 mouse = GetMousePosition();
            bool overBack = CheckCollisionPointRec(mouse, backBtn);
            SetMouseCursor(overBack ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

            if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overBack) ||
                IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_ESCAPE))
                backOut = true;

            BeginDrawing();
            ClearBackground(Color{16, 15, 14, 255});

            DrawBoardMap(board, mapRect);

            const char *message = "No opponent fighter to attack";
            Vector2 mDim = MeasureTextEx(GetGameFont(), message, (float)msgSize, 2.0f);
            DrawTextEx(GetGameFont(), message, {screenW / 2.0f - mDim.x / 2.0f, screenH * 0.5f},
                       (float)msgSize, 2.0f, Color{200, 70, 70, 255});

            DrawRectangleRounded(backBtn, 0.25f, 8, overBack ? Fade(gold, 0.3f) : panelBg);
            DrawRectangleRoundedLines(backBtn, 0.25f, 8, Fade(gold, 0.6f));
            Vector2 bDim = MeasureTextEx(GetGameFont(), "Back", (float)btnTextSize, 1.0f);
            DrawTextEx(GetGameFont(), "Back",
                       {backBtn.x + backBtn.width / 2.0f - bDim.x / 2.0f, backBtn.y + backBtn.height / 2.0f - bDim.y / 2.0f},
                       (float)btnTextSize, 1.0f, overBack ? WHITE : textDim);

            EndDrawing();

            if (backOut)
                break;
        }

        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        return false;
    }

    std::vector<std::string> entries;
    for (auto h : defenders)
        entries.push_back((h->get_number() == 0 ? "" : std::to_string(h->get_number())) + h->get_name());

    int selected = 0;
    bool confirmed = false;
    bool backOut = false;

    while (!WindowShouldClose())
    {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        int entryTextSize = (int)clampf(screenH * 0.026f, 15.0f, 28.0f);
        float entryLineH  = entryTextSize * 1.6f;
        float listH       = entries.size() * entryLineH;

        int titleSize  = (int)(entryTextSize * 1.1f);
        float titleGap = entryTextSize * 0.9f;

        int btnTextSize = (int)(entryTextSize * 0.9f);
        float btnH      = btnTextSize * 2.2f;

        float panelW = clampf(screenW * 0.28f, 260.0f, 380.0f);
        float pad    = panelW * 0.08f;
        float panelH = titleSize + titleGap + listH + entryLineH * 0.6f + btnH + pad * 2.2f;

        Rectangle panel = {screenW * 0.04f, screenH / 2.0f - panelH / 2.0f, panelW, panelH};
        Rectangle mapRect = {panel.x + panel.width + screenW * 0.04f, screenH * 0.08f,
                              screenW - (panel.x + panel.width) - screenW * 0.08f, screenH * 0.84f};

        float x      = panel.x + pad;
        float titleY = panel.y + pad;
        float listY  = titleY + titleSize + titleGap;

        std::vector<Rectangle> rowRects(entries.size());
        for (size_t i = 0; i < entries.size(); i++)
            rowRects[i] = {x, listY + entryLineH * i, panel.width - pad * 2, entryLineH};

        float buttonsY = listY + listH + entryLineH * 0.6f;
        Rectangle confirmBtn = {x, buttonsY, (panel.width - pad * 2) * 0.46f, btnH};
        Rectangle backBtn    = {x + (panel.width - pad * 2) * 0.54f, buttonsY, (panel.width - pad * 2) * 0.46f, btnH};

        // ---------- input ----------
        Vector2 mouse = GetMousePosition();
        int hoveredRow = -1;
        for (size_t i = 0; i < rowRects.size(); i++)
            if (CheckCollisionPointRec(mouse, rowRects[i]))
                hoveredRow = (int)i;

        bool overConfirm = CheckCollisionPointRec(mouse, confirmBtn);
        bool overBack    = CheckCollisionPointRec(mouse, backBtn);

        SetMouseCursor((hoveredRow != -1 or overConfirm or overBack) ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            selected = (selected + 1) % (int)entries.size();
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            selected = (selected - 1 + (int)entries.size()) % (int)entries.size();
        if (hoveredRow != -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            selected = hoveredRow;

        if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overConfirm) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
            confirmed = true;
        else if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overBack) || IsKeyPressed(KEY_ESCAPE))
            backOut = true;

        // ---------- draw ----------
        BeginDrawing();
        ClearBackground(Color{16, 15, 14, 255});

        DrawBoardMap(board, mapRect);

        DrawRectangleRounded(panel, 0.08f, 10, panelBg);
        DrawRectangleRoundedLines(panel, 0.08f, 10, Fade(gold, 0.6f));

        std::string title = player->get_name() + ", CHOOSE DEFENDER";
        Vector2 titleDim = MeasureTextEx(GetGameFont(), title.c_str(), (float)titleSize, 2.0f);
        DrawTextEx(GetGameFont(), title.c_str(), {panel.x + panel.width / 2.0f - titleDim.x / 2.0f, titleY},
                   (float)titleSize, 2.0f, gold);

        for (size_t i = 0; i < entries.size(); i++)
        {
            bool isSelected = ((int)i == selected);
            Vector2 pos = {x + 10.0f, rowRects[i].y + (entryLineH - entryTextSize) / 2.0f};

            if (isSelected)
                DrawRectangleRounded(rowRects[i], 0.25f, 8, Fade(gold, 0.18f));
            if ((int)i == hoveredRow)
                DrawRectangleRoundedLines(rowRects[i], 0.25f, 8, Fade(gold, 0.5f));

            DrawTextEx(GetGameFont(), entries[i].c_str(), {pos.x + 1, pos.y + 1}, (float)entryTextSize, 1.0f, Fade(BLACK, 0.6f));
            DrawTextEx(GetGameFont(), entries[i].c_str(), pos, (float)entryTextSize, 1.0f, isSelected ? gold : textDim);
        }

        auto drawButton = [&](Rectangle r, const std::string &label, bool hovered)
        {
            DrawRectangleRounded(r, 0.25f, 8, hovered ? Fade(gold, 0.3f) : panelBg);
            DrawRectangleRoundedLines(r, 0.25f, 8, Fade(gold, 0.6f));
            Vector2 dim = MeasureTextEx(GetGameFont(), label.c_str(), (float)btnTextSize, 1.0f);
            Vector2 bp  = {r.x + r.width / 2.0f - dim.x / 2.0f, r.y + r.height / 2.0f - dim.y / 2.0f};
            DrawTextEx(GetGameFont(), label.c_str(), bp, (float)btnTextSize, 1.0f, hovered ? WHITE : textDim);
        };

        drawButton(confirmBtn, "Confirm", overConfirm);
        drawButton(backBtn, "Back", overBack);

        const char *footer = "CLICK a target, or use UP/DOWN + ENTER   -   ESC to go back";
        int footSize = (int)clampf(screenH * 0.014f, 10.0f, 18.0f);
        Vector2 fDim = MeasureTextEx(GetGameFont(), footer, (float)footSize, 2.0f);
        DrawTextEx(GetGameFont(), footer, {screenW / 2.0f - fDim.x / 2.0f, screenH * 0.965f},
                   (float)footSize, 2.0f, Color{140, 135, 128, 255});

        EndDrawing();

        if (confirmed || backOut)
            break;
    }

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (!confirmed)
        return false;

    defender = defenders[selected];
    return true;
}


static Player *FindOwnerPlayer(Player *p1, Player *p2, Heroes *hero)
{
    for (Player *p : {p1, p2})
    {
        if (p == nullptr)
            continue;

        if (p->get_character() == hero)
            return p;

        for (Heroes *c : p->get_comrade())
            if (c == hero)
                return p;
    }

    return nullptr;
}

static std::string CardTypeToString(CardType card)
{
    switch (card)
    {
        case CardType::Feedingfrenzy:               return "Feeding Frenzy";
        case CardType::Mistform:                     return "Mistform";
        case CardType::Ambush:                       return "Ambush";
        case CardType::Baptism_of_blood:             return "Baptism of Blood";
        case CardType::Beastform:                     return "Beastform";
        case CardType::Dash:                          return "Dash";
        case CardType::Exploit:                       return "Exploit";
        case CardType::Look_into_my_eyes:             return "Look Into My Eyes";
        case CardType::Prey_upon:                     return "Prey Upon";
        case CardType::Ravening_Seduction:            return "Ravening Seduction";
        case CardType::Thirst_for_sustenance:         return "Thirst for Sustenance";
        case CardType::Feint:                         return "Feint";
        case CardType::Administer_Aid:                return "Administer Aid";
        case CardType::Confirm_Suspicion:             return "Confirm Suspicion";
        case CardType::Counterpunch:                  return "Counterpunch";
        case CardType::Deduce_Strategy:               return "Deduce Strategy";
        case CardType::Education_Never_Ends:          return "Education Never Ends";
        case CardType::Elementary:                    return "Elementary";
        case CardType::Eliminate_The_Impossible:      return "Eliminate the Impossible";
        case CardType::Feint2:                        return "Feint";
        case CardType::Fixed_Point_in_a_Changing_Age: return "Fixed Point";
        case CardType::Master_of_Disguise:            return "Master of Disguise";
        case CardType::The_Game_Is_Afoot:             return "The Game Is Afoot";
        case CardType::Service_Revolver:              return "Service Revolver";
        case CardType::Study_Methods:                 return "Study Methods";
        case CardType::Coded_Notes:                   return "Coded Notes";
        case CardType::Confound:                      return "Confound";
        case CardType::Covert_Preparation:            return "Covert Preparation";
        case CardType::Dreaming_of_Revenge:           return "Dreaming of Revenge";
        case CardType::Emerge_From_Mist:              return "Emerge From Mist";
        case CardType::Impossible_to_See:             return "Impossible to See";
        case CardType::Into_Thin_Air:                 return "Into Thin Air";
        case CardType::Lurking:                       return "Lurking";
        case CardType::Reign_Thrror:                  return "Reign of Terror";
        case CardType::Rolling_Fog:                   return "Rolling Fog";
        case CardType::Slip_Away:                     return "Slip Away";
        case CardType::Step_Lightly:                  return "Step Lightly";
        case CardType::Vanish:                        return "Vanish";
    }

    return "Unknown Card";
}


static void DrawCardBox(Font &font, Rectangle box, Card *card, const std::string &title); 


static std::string HeroAssetFolder(const std::string &heroName)
{
    if (heroName == "DRACULA" or heroName == "SISTERS" or heroName == "FOG")
        return "dracula";

    if (heroName == "SHERLOCKHOLMES" or heroName == "Dr_Watson")
        return "holms";

    if (heroName == "InvisibleMan")
        return "InvisibleMan";

    return "";
}


static std::string CardImageFileName(CardType type, const std::string &heroFolder)
{
   
    if (type == CardType::Feint)
        return (heroFolder == "holms") ? "feint (2)" : "feint (1)";

    switch (type)
    {
        case CardType::Feedingfrenzy:               return "feeding-frenzy";
        case CardType::Mistform:                     return "mistform";
        case CardType::Ambush:                       return "ambush";
        case CardType::Baptism_of_blood:             return "baptism-of-blood";
        case CardType::Beastform:                     return "beastform";
        case CardType::Dash:                          return "dash";
        case CardType::Exploit:                       return "exploit";
        case CardType::Look_into_my_eyes:             return "look-into-my-eyes";
        case CardType::Prey_upon:                     return "prey-upon";
        case CardType::Ravening_Seduction:            return "ravening-seduction";
        case CardType::Thirst_for_sustenance:         return "thirst-for-sustenance";
        case CardType::Feint:                         return "feint (1)"; // unreachable — handled above

        case CardType::Administer_Aid:                return "administer-aid";
        case CardType::Confirm_Suspicion:             return "confirm-suspicion";
        case CardType::Counterpunch:                  return "counterpunch";
        case CardType::Deduce_Strategy:               return "deduce-strategy";
        case CardType::Education_Never_Ends:          return "education-never-ends";
        case CardType::Elementary:                    return "elementary";
        case CardType::Eliminate_The_Impossible:      return "eliminate-the-impossible";
        case CardType::Feint2:                        return "feint (2)";
        case CardType::Fixed_Point_in_a_Changing_Age: return "fixed-point-in-a-changing-age";
        case CardType::Master_of_Disguise:            return "master-of-disguise";
        case CardType::The_Game_Is_Afoot:             return "the-game-is-afoot";
        case CardType::Service_Revolver:              return "service-revolver";
        case CardType::Study_Methods:                 return "study-methods";

        case CardType::Coded_Notes:                   return "coded-notes";
        case CardType::Confound:                      return "confound";
        case CardType::Covert_Preparation:            return "covert-preparation";
        case CardType::Dreaming_of_Revenge:           return "dreaming-of-revenge";
        case CardType::Emerge_From_Mist:              return "emerge-from-mist";
        case CardType::Impossible_to_See:             return "impossible-to-see";
        case CardType::Into_Thin_Air:                 return "into-thin-air";
        case CardType::Lurking:                       return "lurking";
        case CardType::Reign_Thrror:                  return "reign-of-terror";
        case CardType::Rolling_Fog:                   return "rolling-fog";
        case CardType::Slip_Away:                     return "slip-away";
        case CardType::Step_Lightly:                  return "step-lightly";
        case CardType::Vanish:                        return "vanish";
    }

    return "";
}

static std::string CardImageFile(Card *card, const std::string &heroFolder)
{
    if (card == nullptr or heroFolder.empty())
        return "";

    std::string fileName = CardImageFileName(card->get_CardType(), heroFolder);
    if (fileName.empty())
        return "";

    return "assets/cards/" + heroFolder + "/" + fileName + ".png";
}

static bool FileExistsAnyExt(const std::string &pathNoExt, std::string &foundPath)
{
    static const char *exts[] = {".png", ".jpg", ".jpeg", ".PNG", ".JPG", ".JPEG"};
    for (const char *ext : exts)
    {
        std::string candidate = pathNoExt + ext;
        if (FileExists(candidate.c_str()))
        {
            foundPath = candidate;
            return true;
        }
    }
    return false;
}


static std::string CardTimingToString(CardTiming timing);

static int CardImageSelectionMenu(Font &font, const std::string &title, const std::string &heroFolder,
                                   const std::vector<Card *> &cards, bool includeNoDefense)
{
    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
    Color panelBg = Color{10, 9, 8, 170};

    auto clampf = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

    int slotCount = (int)cards.size() + (includeNoDefense ? 1 : 0);

    Texture2D background{};
    bool hasBackground = FileExists("assets/combatbackground.png");
    if (hasBackground)
        background = LoadTexture("assets/combatbackground.png");

    Music combatMusic{};
    bool hasMusic = FileExists("assets/Musics/combat.mp3");
    if (hasMusic)
    {
        combatMusic = LoadMusicStream("assets/Musics/combat.mp3");
        combatMusic.looping = true;
        PlayMusicStream(combatMusic);
    }

    std::vector<Texture2D> textures(slotCount);
    std::vector<bool> hasTexture(slotCount, false);
    for (int i = 0; i < (int)cards.size(); i++)
    {
        std::string path = CardImageFile(cards[i], heroFolder);
        if (!path.empty() and FileExists(path.c_str()))
        {
            textures[i] = LoadTexture(path.c_str());
            hasTexture[i] = true;
        }
    }

    int selected = 0;
    bool confirmed = false;
    bool backOut = false;

    while (!WindowShouldClose())
    {
        if (hasMusic)
            UpdateMusicStream(combatMusic);

        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        float baseline  = screenH * 0.86f;
        float cardH     = clampf(screenH * 0.5f, 160.0f, 480.0f);
        float colW      = (screenW * 0.86f) / slotCount;
        float rowStartX = screenW * 0.07f;

        std::vector<Rectangle> slotRects(slotCount);
        for (int i = 0; i < slotCount; i++)
        {
            float cardW = fminf(cardH * 0.714f, colW * 0.9f);
            float colCenterX = rowStartX + colW * i + colW / 2.0f;
            slotRects[i] = {colCenterX - cardW / 2.0f, baseline - cardH, cardW, cardH};
        }

        Rectangle backBtn = {screenW * 0.03f, screenH * 0.04f, screenW * 0.09f, screenH * 0.055f};

        // ---------- input ----------
        Vector2 mouse = GetMousePosition();
        int hovered = -1;
        for (int i = 0; i < slotCount; i++)
            if (CheckCollisionPointRec(mouse, slotRects[i]))
                hovered = i;

        bool overBack = CheckCollisionPointRec(mouse, backBtn);
        int displayHighlight = (hovered != -1) ? hovered : selected;

        SetMouseCursor((hovered != -1 or overBack) ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) selected = (displayHighlight + 1) % slotCount;
        if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) selected = (displayHighlight - 1 + slotCount) % slotCount;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hovered != -1)
        {
            selected = hovered;
            confirmed = true;
        }
        else if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
        {
            selected = displayHighlight;
            confirmed = true;
        }
        else if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overBack) || IsKeyPressed(KEY_ESCAPE))
        {
            backOut = true;
        }

        // ---------- draw ----------
        BeginDrawing();

        if (hasBackground)
        {
            DrawTexturePro(background, {0, 0, (float)background.width, (float)background.height},
                            {0, 0, (float)screenW, (float)screenH}, {0, 0}, 0, WHITE);
            DrawRectangle(0, 0, screenW, screenH, Fade(BLACK, 0.4f));
        }
        else
        {
            DrawRectangleGradientV(0, 0, screenW, screenH, Color{16, 15, 14, 255}, Color{34, 12, 12, 255});
        }

        int titleSize = (int)clampf(screenH * 0.04f, 20.0f, 44.0f);
        Vector2 titleDim = MeasureTextEx(font, title.c_str(), (float)titleSize, 3.0f);
        Vector2 titlePos = {screenW / 2.0f - titleDim.x / 2.0f, screenH * 0.05f};
        DrawTextEx(font, title.c_str(), {titlePos.x + 2, titlePos.y + 2}, (float)titleSize, 3.0f, Fade(BLACK, 0.6f));
        DrawTextEx(font, title.c_str(), titlePos, (float)titleSize, 3.0f, gold);

        for (int i = 0; i < slotCount; i++)
        {
            bool isHighlighted = (i == displayHighlight);
            bool isNoDefenseSlot = includeNoDefense and (i == (int)cards.size());
            Card *thisCard = isNoDefenseSlot ? nullptr : cards[i];

            float riseAmount = isHighlighted ? screenH * 0.025f : 0.0f;
            float scale      = isHighlighted ? 1.07f : 1.0f;

            Rectangle base = slotRects[i];
            Rectangle dest = {base.x - base.width * (scale - 1.0f) / 2.0f,
                               base.y - base.height * (scale - 1.0f) - riseAmount,
                               base.width * scale, base.height * scale};

            if (isHighlighted)
            {
                float glowR = base.width * 0.9f;
                DrawCircleGradient(Vector2{base.x + base.width / 2.0f, base.y + base.height * 0.5f},
                                    glowR, Fade(gold, 0.22f), Fade(gold, 0.0f));
            }

            if (!isNoDefenseSlot and hasTexture[i])
            {
                Texture2D &tex = textures[i];

                DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height},
                                {dest.x + 5, dest.y + 7, dest.width, dest.height}, {0, 0}, 0,
                                Fade(BLACK, isHighlighted ? 0.45f : 0.3f));

                Color tint = isHighlighted ? WHITE : Color{175, 170, 165, 255};
                DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height}, dest, {0, 0}, 0, tint);
                DrawRectangleRoundedLines(dest, 0.06f, 8, isHighlighted ? gold : Fade(gold, 0.35f));

                std::string valLabel = "Value " + std::to_string(thisCard->get_amount()) +
                                        "   |   " + CardTimingToString(thisCard->get_CardTiming());
                int labelSize = (int)clampf(screenH * 0.02f, 12.0f, 22.0f);
                Vector2 labelDim = MeasureTextEx(font, valLabel.c_str(), (float)labelSize, 1.0f);
                DrawTextEx(font, valLabel.c_str(),
                           {dest.x + dest.width / 2.0f - labelDim.x / 2.0f, dest.y + dest.height + 6},
                           (float)labelSize, 1.0f, isHighlighted ? gold : textDim);
            }
            else
            {
                std::string boxTitle = isNoDefenseSlot ? "DEFENSE" : CardTypeToString(thisCard->get_CardType());
                DrawCardBox(font, dest, thisCard, boxTitle);
                DrawRectangleRoundedLines(dest, 0.06f, 8, isHighlighted ? gold : Fade(gold, 0.35f));
            }
        }

        DrawRectangleRounded(backBtn, 0.25f, 8, overBack ? Fade(gold, 0.3f) : panelBg);
        DrawRectangleRoundedLines(backBtn, 0.25f, 8, Fade(gold, 0.6f));
        int backTextSize = (int)clampf(screenH * 0.02f, 12.0f, 22.0f);
        Vector2 backDim = MeasureTextEx(font, "Back", (float)backTextSize, 1.0f);
        DrawTextEx(font, "Back",
                   {backBtn.x + backBtn.width / 2.0f - backDim.x / 2.0f, backBtn.y + backBtn.height / 2.0f - backDim.y / 2.0f},
                   (float)backTextSize, 1.0f, overBack ? WHITE : textDim);

        const char *footer = "CLICK a card to choose it, or use ARROWS + ENTER   -   ESC to go back";
        int footSize = (int)clampf(screenH * 0.014f, 10.0f, 18.0f);
        Vector2 fDim = MeasureTextEx(font, footer, (float)footSize, 2.0f);
        DrawTextEx(font, footer, {screenW / 2.0f - fDim.x / 2.0f, screenH * 0.965f},
                   (float)footSize, 2.0f, Color{140, 135, 128, 255});

        EndDrawing();

        if (confirmed || backOut)
            break;
    }

    for (int i = 0; i < (int)cards.size(); i++)
        if (hasTexture[i])
            UnloadTexture(textures[i]);

    if (hasMusic)
    {
        StopMusicStream(combatMusic);
        UnloadMusicStream(combatMusic);
    }

    if (hasBackground)
        UnloadTexture(background);

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    return backOut ? -1 : selected;
}

bool Raylib::Attacker_selected_card(Heroes *Attacker, Heroes *Defender, Player *p1, Player *p2, Board *board, Card *&Attacker_Card)
{
    Attacker_Card = nullptr;

    if (Attacker == nullptr or p1 == nullptr or p2 == nullptr)
        return false;

    Player *ownerPlayer = FindOwnerPlayer(p1, p2, Attacker);
    if (ownerPlayer == nullptr)
        throw std::runtime_error("Attacker does not belong to either player.");

    Heroes *handOwner = ownerPlayer->get_character(); 

    std::vector<Card *> AllowHand;
    for (Card &c : handOwner->get_hand())
    {
        if ((c.get_owner() == Attacker->get_name() or c.get_owner() == "ANY") and
            (c.get_Attacktype() == "Attack" or c.get_Attacktype() == "Both"))
            AllowHand.push_back(&c);
    }

    if (AllowHand.empty())
        return false; 

    std::string heroFolder = HeroAssetFolder(handOwner->get_name());
    int selected = CardImageSelectionMenu(GetGameFont(), Attacker->get_name() + ", CHOOSE ATTACK CARD",
                                           heroFolder, AllowHand, false);

    if (selected < 0)
        return false;

    Attacker_Card = AllowHand[selected];
    ownerPlayer->set_selected_card(Attacker_Card);
    Attacker_Card->set_user_card(Attacker);

    return true;
}


bool Raylib::Defender_selected_card(Heroes *Attacker, Heroes *Defender, Player *p1, Player *p2, Board *board, Card *&Defender_Card)
{
    Defender_Card = nullptr;

    if (Defender == nullptr or p1 == nullptr or p2 == nullptr)
        return false;

    Player *ownerPlayer = FindOwnerPlayer(p1, p2, Defender);
    if (ownerPlayer == nullptr)
        throw std::runtime_error("Defender does not belong to either player.");

    Heroes *handOwner = ownerPlayer->get_character(); 

    std::vector<Card *> AllowHand;
    for (Card &c : handOwner->get_hand())
    {
        if ((c.get_owner() == Defender->get_name() or c.get_owner() == "ANY") and
            (c.get_Attacktype() == "Defense" or c.get_Attacktype() == "Both"))
            AllowHand.push_back(&c);
    }

    std::string heroFolder = HeroAssetFolder(handOwner->get_name());
    int selected = CardImageSelectionMenu(GetGameFont(), Defender->get_name() + ", CHOOSE DEFENSE CARD",
                                           heroFolder, AllowHand, true);

    if (selected < 0)
        return false;

    if (selected == (int)AllowHand.size()) // chose "No Defense"
    {
        Defender_Card = nullptr;
        ownerPlayer->set_selected_card(nullptr);
        return true;
    }

    Defender_Card = AllowHand[selected];
    ownerPlayer->set_selected_card(Defender_Card);
    Defender_Card->set_user_card(Defender);

    return true;
}


static std::string CardTimingToString(CardTiming timing)
{
    switch (timing)
    {
        case CardTiming::Before:    return "Before";
        case CardTiming::During:    return "During";
        case CardTiming::After:     return "After";
        case CardTiming::Immediate: return "Immediate";
    }

    return "Unknown";
}

static void DrawCardBox(Font &font, Rectangle box, Card *card, const std::string &title)
{
    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
    Color panelBg = Color{10, 9, 8, 200};
    Color red     = Color{200, 60, 60, 255};

    DrawRectangleRounded(box, 0.06f, 10, panelBg);
    DrawRectangleRoundedLines(box, 0.06f, 10, Fade(gold, 0.6f));

    int titleSize = (int)(box.height * 0.09f);
    Vector2 titleDim = MeasureTextEx(font, title.c_str(), (float)titleSize, 2.0f);
    float y = box.y + box.height * 0.06f;
    DrawTextEx(font, title.c_str(), {box.x + box.width / 2.0f - titleDim.x / 2.0f, y}, (float)titleSize, 2.0f, gold);
    y += titleSize * 1.6f;

    DrawLineEx({box.x + box.width * 0.08f, y}, {box.x + box.width * 0.92f, y}, 1.5f, Fade(gold, 0.5f));
    y += box.height * 0.06f;

    if (card == nullptr)
    {
        int msgSize = (int)(box.height * 0.08f);
        const char *msg = "NO DEFENSE";
        Vector2 msgDim = MeasureTextEx(font, msg, (float)msgSize, 2.0f);
        DrawTextEx(font, msg,
                   {box.x + box.width / 2.0f - msgDim.x / 2.0f, box.y + box.height / 2.0f - msgDim.y / 2.0f},
                   (float)msgSize, 2.0f, red);
        return;
    }

    int nameSize = (int)(box.height * 0.075f);
    std::string name = CardTypeToString(card->get_CardType());
    Vector2 nameDim = MeasureTextEx(font, name.c_str(), (float)nameSize, 1.0f);
    DrawTextEx(font, name.c_str(), {box.x + box.width / 2.0f - nameDim.x / 2.0f, y}, (float)nameSize, 1.0f, WHITE);
    y += nameSize * 1.6f;

    int lineSize = (int)(box.height * 0.06f);
    float lineGap = lineSize * 1.5f;
    float lx = box.x + box.width * 0.12f;

    auto drawLine = [&](const std::string &label, const std::string &value)
    {
        std::string line = label + value;
        DrawTextEx(font, line.c_str(), {lx, y}, (float)lineSize, 1.0f, textDim);
        y += lineGap;
    };

    drawLine("Amount: ", std::to_string(card->get_amount()));
    drawLine("Type:   ", card->get_Attacktype());
    drawLine("Boost:  ", std::to_string(card->get_Boost()));
    drawLine("Owner:  ", card->get_owner());
    drawLine("Time:   ", CardTimingToString(card->get_CardTiming()));
}


void Raylib::Reveal_Combat(Heroes *attacker, Heroes *defender, Card *attack_card, Card *defense_card)
{
    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
    Color panelBg = Color{10, 9, 8, 200};

    auto clampf = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

    std::string attackerFolder = HeroAssetFolder(attacker != nullptr ? attacker->get_name() : "");
    std::string defenderFolder = HeroAssetFolder(defender != nullptr ? defender->get_name() : "");

    std::string attackPath = CardImageFile(attack_card, attackerFolder);
    std::string defensePath = CardImageFile(defense_card, defenderFolder);

    Texture2D attackTex{}, defenseTex{};
    bool hasAttackTex = !attackPath.empty() and FileExists(attackPath.c_str());
    bool hasDefenseTex = defense_card != nullptr and !defensePath.empty() and FileExists(defensePath.c_str());

    if (hasAttackTex)
        attackTex = LoadTexture(attackPath.c_str());
    if (hasDefenseTex)
        defenseTex = LoadTexture(defensePath.c_str());

    while (!WindowShouldClose())
    {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        int titleSize = (int)clampf(screenH * 0.045f, 26.0f, 54.0f);
        int nameSize  = (int)clampf(screenH * 0.03f, 18.0f, 32.0f);

        float boxW = clampf(screenW * 0.24f, 260.0f, 380.0f);
        float boxH = clampf(screenH * 0.55f, 320.0f, 460.0f);
        float gap  = screenW * 0.05f;

        float totalW = boxW * 2 + gap;
        float startX = screenW / 2.0f - totalW / 2.0f;
        float boxY   = screenH * 0.30f;

        Rectangle attackBox  = {startX, boxY, boxW, boxH};
        Rectangle defenseBox = {startX + boxW + gap, boxY, boxW, boxH};

        int btnTextSize = (int)clampf(screenH * 0.024f, 16.0f, 26.0f);
        float btnW = clampf(screenW * 0.18f, 220.0f, 320.0f);
        float btnH = btnTextSize * 2.4f;
        Rectangle resolveBtn = {screenW / 2.0f - btnW / 2.0f, screenH * 0.90f, btnW, btnH};

        // ---------- input ----------
        Vector2 mouse = GetMousePosition();
        bool overResolve = CheckCollisionPointRec(mouse, resolveBtn);
        SetMouseCursor(overResolve ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        bool resolved = (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overResolve) ||
                         IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_SPACE);

        // ---------- draw ----------
        BeginDrawing();
        ClearBackground(Color{16, 15, 14, 255});

        const char *title = "COMBAT";
        Vector2 titleDim = MeasureTextEx(GetGameFont(), title, (float)titleSize, 3.0f);
        DrawTextEx(GetGameFont(), title, {screenW / 2.0f - titleDim.x / 2.0f, screenH * 0.08f},
                   (float)titleSize, 3.0f, gold);

        Vector2 atkNameDim = MeasureTextEx(GetGameFont(), attacker->get_name().c_str(), (float)nameSize, 1.0f);
        DrawTextEx(GetGameFont(), attacker->get_name().c_str(),
                   {attackBox.x + attackBox.width / 2.0f - atkNameDim.x / 2.0f, boxY - nameSize * 1.6f},
                   (float)nameSize, 1.0f, WHITE);

        Vector2 defNameDim = MeasureTextEx(GetGameFont(), defender->get_name().c_str(), (float)nameSize, 1.0f);
        DrawTextEx(GetGameFont(), defender->get_name().c_str(),
                   {defenseBox.x + defenseBox.width / 2.0f - defNameDim.x / 2.0f, boxY - nameSize * 1.6f},
                   (float)nameSize, 1.0f, WHITE);

        if (hasAttackTex)
        {
            DrawTexturePro(attackTex, {0, 0, (float)attackTex.width, (float)attackTex.height},
                           {attackBox.x + 5, attackBox.y + 7, attackBox.width, attackBox.height},
                           {0, 0}, 0, Fade(BLACK, 0.35f));
            DrawTexturePro(attackTex, {0, 0, (float)attackTex.width, (float)attackTex.height},
                           attackBox, {0, 0}, 0, WHITE);
            DrawRectangleRoundedLines(attackBox, 0.06f, 8, gold);

            std::string valLabel = "Value " + std::to_string(attack_card->get_amount()) +
                                    "   |   " + CardTimingToString(attack_card->get_CardTiming());
            int labelSize = (int)clampf(screenH * 0.022f, 14.0f, 24.0f);
            Vector2 labelDim = MeasureTextEx(GetGameFont(), valLabel.c_str(), (float)labelSize, 1.0f);
            DrawTextEx(GetGameFont(), valLabel.c_str(),
                       {attackBox.x + attackBox.width / 2.0f - labelDim.x / 2.0f, attackBox.y + attackBox.height + 8},
                       (float)labelSize, 1.0f, gold);
        }
        else
        {
            DrawCardBox(GetGameFont(), attackBox, attack_card, "ATTACK");
        }

        if (hasDefenseTex)
        {
            DrawTexturePro(defenseTex, {0, 0, (float)defenseTex.width, (float)defenseTex.height},
                           {defenseBox.x + 5, defenseBox.y + 7, defenseBox.width, defenseBox.height},
                           {0, 0}, 0, Fade(BLACK, 0.35f));
            DrawTexturePro(defenseTex, {0, 0, (float)defenseTex.width, (float)defenseTex.height},
                           defenseBox, {0, 0}, 0, WHITE);
            DrawRectangleRoundedLines(defenseBox, 0.06f, 8, gold);

            std::string valLabel = "Value " + std::to_string(defense_card->get_amount()) +
                                    "   |   " + CardTimingToString(defense_card->get_CardTiming());
            int labelSize = (int)clampf(screenH * 0.022f, 14.0f, 24.0f);
            Vector2 labelDim = MeasureTextEx(GetGameFont(), valLabel.c_str(), (float)labelSize, 1.0f);
            DrawTextEx(GetGameFont(), valLabel.c_str(),
                       {defenseBox.x + defenseBox.width / 2.0f - labelDim.x / 2.0f, defenseBox.y + defenseBox.height + 8},
                       (float)labelSize, 1.0f, gold);
        }
        else
        {
            DrawCardBox(GetGameFont(), defenseBox, defense_card, "DEFENSE");
        }

        int vsSize = (int)clampf(screenH * 0.05f, 28.0f, 60.0f);
        const char *vs = "VS";

        Vector2 vsDim = MeasureTextEx(GetGameFont(), vs, (float)vsSize, 2.0f);
        DrawTextEx(GetGameFont(), vs, {screenW / 2.0f - vsDim.x / 2.0f, boxY + boxH / 2.0f - vsDim.y / 2.0f},
                   (float)vsSize, 2.0f, Fade(gold, 0.85f));

        DrawRectangleRounded(resolveBtn, 0.25f, 8, overResolve ? Fade(gold, 0.3f) : panelBg);
        DrawRectangleRoundedLines(resolveBtn, 0.25f, 8, Fade(gold, 0.6f));
        const char *btnLabel = "Resolve Combat";

        Vector2 btnDim = MeasureTextEx(GetGameFont(), btnLabel, (float)btnTextSize, 1.0f);
        DrawTextEx(GetGameFont(), btnLabel,
                   {resolveBtn.x + resolveBtn.width / 2.0f - btnDim.x / 2.0f,
                    resolveBtn.y + resolveBtn.height / 2.0f - btnDim.y / 2.0f},
                   (float)btnTextSize, 1.0f, overResolve ? WHITE : textDim);

        EndDrawing();

        if (resolved)
            break;
    }

    if (hasAttackTex)
        UnloadTexture(attackTex);
    if (hasDefenseTex)
        UnloadTexture(defenseTex);

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

static int PickSpaceOnMap(Raylib &self, Board *board, const std::vector<int> &spaceIndices,
                           const std::string &title, const std::string &subtitle = "")
{
    if (spaceIndices.empty())
        return -1;

    using namespace BoardVisual;

    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
    Color green   = Color{110, 200, 120, 255};

    int selected = -1;
    bool confirmed = false;
    bool cancelled = false;

    while (!WindowShouldClose())
    {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        Rectangle mapRect = {screenW * 0.06f, screenH * 0.17f, screenW * 0.88f, screenH * 0.72f};
        MapTransform xform = Fit(mapRect);
        float radius = NodeRadius(xform);

        Vector2 mouse = GetMousePosition();
        int hovered = -1;
        for (size_t i = 0; i < spaceIndices.size(); i++)
            if (CheckCollisionPointCircle(mouse, SpacePos(xform, spaceIndices[i]), radius * 1.8f))
                hovered = (int)i;

        SetMouseCursor(hovered != -1 ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
            selected = (selected == -1) ? 0 : (selected + 1) % (int)spaceIndices.size();
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
            selected = (selected <= 0) ? (int)spaceIndices.size() - 1 : selected - 1;

        if (hovered != -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            selected = hovered;
            confirmed = true;
        }
        else if (selected != -1 && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)))
            confirmed = true;
        else if (IsKeyPressed(KEY_ESCAPE))
            cancelled = true;

        BeginDrawing();
        ClearBackground(Color{16, 15, 14, 255});

        int titleSize = (int)(screenH * 0.036f);
        Vector2 titleDim = MeasureTextEx(self.GetGameFont(), title.c_str(), (float)titleSize, 3.0f);
        Vector2 titlePos = {screenW / 2.0f - titleDim.x / 2.0f, screenH * 0.045f};
        DrawTextEx(self.GetGameFont(), title.c_str(), {titlePos.x + 2, titlePos.y + 2}, (float)titleSize, 3.0f, Fade(BLACK, 0.6f));
        DrawTextEx(self.GetGameFont(), title.c_str(), titlePos, (float)titleSize, 3.0f, gold);

        if (!subtitle.empty())
        {
            int subSize = (int)(screenH * 0.017f);
            Vector2 subDim = MeasureTextEx(self.GetGameFont(), subtitle.c_str(), (float)subSize, 2.0f);
            DrawTextEx(self.GetGameFont(), subtitle.c_str(), {screenW / 2.0f - subDim.x / 2.0f, titlePos.y + titleDim.y + 8},
                       (float)subSize, 2.0f, textDim);
        }

        self.DrawBoardMap(board, mapRect);

        for (size_t i = 0; i < spaceIndices.size(); i++)
        {
            Vector2 pos = SpacePos(xform, spaceIndices[i]);
            bool isSelected = ((int)i == selected);
            bool isHighlighted = ((int)i == hovered) || isSelected;

            if (isHighlighted)
            {
                float glowR = radius * 2.3f;
                DrawCircleGradient(pos, glowR, Fade(isSelected ? green : gold, 0.35f), Fade(gold, 0.0f));
            }

            Color ringCol = isSelected ? green : (isHighlighted ? gold : Fade(gold, 0.55f));
            DrawRing(pos, radius + 3.0f, radius + 6.0f, 0, 360, 28, ringCol);
        }

        const char *footer = "CLICK a glowing room to choose it   -   or use ARROWS + ENTER   -   ESC to cancel";
        int footSize = (int)fminf(fmaxf(screenH * 0.016f, 10.0f), 18.0f);
        Vector2 fDim = MeasureTextEx(self.GetGameFont(), footer, (float)footSize, 2.0f);
        DrawTextEx(self.GetGameFont(), footer, {screenW / 2.0f - fDim.x / 2.0f, screenH * 0.965f},
                   (float)footSize, 2.0f, Color{140, 135, 128, 255});

        EndDrawing();

        if (confirmed || cancelled)
            break;
    }

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (!confirmed || selected == -1)
        return -1;

    return selected;
}

void Raylib::put_in_any_space(Heroes *fighter, Board *board) // Gives every fighter to every empty house.
{
    if (fighter == nullptr || board == nullptr)
        return;

    Space *spaceBase = &board->get_spaces()[0];
    std::vector<int> spaceIndices;

    for (Space &s : board->get_spaces())
        if (s.get_hero() == nullptr)
            spaceIndices.push_back((int)(&s - spaceBase));

    if (spaceIndices.empty())
        throw std::runtime_error("AllowHand is empty");

    int pick = PickSpaceOnMap(*this, board, spaceIndices, fighter->get_name() + ", CHOOSE ANY EMPTY HOUSE");
    if (pick == -1)
        pick = 0; // this action is mandatory; ESC just accepts the first candidate

    Space *destination = &board->get_spaces()[spaceIndices[pick]];

    if (fighter->get_place() != nullptr)
        fighter->get_place()->set_hero(nullptr);

    fighter->set_place(destination);
    destination->set_hero(fighter);
}

void Raylib::Revive_Sister(Heroes *sister, Heroes *dracula, Board *board) // He revives the defeated sister and places her in every house.
{
    if (sister == nullptr || dracula == nullptr || board == nullptr || dracula->get_place() == nullptr)
        return;

    Space *spaceBase = &board->get_spaces()[0];
    std::vector<int> spaceIndices;

    for (Space *space : dracula->get_place()->get_zone())
        if (space->get_hero() == nullptr)
            spaceIndices.push_back((int)(space - spaceBase));

    if (spaceIndices.empty())
        throw std::runtime_error("No available adjacent spaces to move.");

    int pick = PickSpaceOnMap(*this, board, spaceIndices, "CHOOSE A SPACE TO REVIVE " + sister->get_name());
    if (pick == -1)
        pick = 0;

    Space *destination = &board->get_spaces()[spaceIndices[pick]];
    sister->set_place(destination);
    destination->set_hero(sister);
}

int Raylib::DiscardCards(Heroes *dracula) // Shows the fighter's hand so the player can draw cards from it without restriction.
{
    if (dracula == nullptr)
        return 0;

    int discard_count = 0;
    std::string heroFolder = HeroAssetFolder(dracula->get_name());

    while (!dracula->get_hand().empty())
    {
        std::vector<Card *> handPtrs;
        for (Card &c : dracula->get_hand())
            handPtrs.push_back(&c);

        int selected = CardImageSelectionMenu(GetGameFont(),
                                               dracula->get_name() + ", CHOOSE A CARD TO DISCARD  (ESC when done)",
                                               heroFolder, handPtrs, false);

        if (selected < 0)
            break; // player is done discarding

        dracula->DiscardCard(selected);
        discard_count++;
    }

    return discard_count;
}


void Raylib::MoveHero(Heroes *hero, Board *board, int max_distance, Player *p1, Player *p2) // Moves the fighter up to a specified number of spaces.
{
    if (hero == nullptr || board == nullptr || hero->get_place() == nullptr || max_distance <= 0)
        return;

    using namespace BoardVisual;

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

    Space *spaceBase = &board->get_spaces()[0];

    std::vector<Space *> available_spaces;

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

        for (Space *next : movable_from_current)
        {
            if (visited.count(next))
                continue;

            visited.insert(next);
            q.push({next, distance + 1});

            if (next->get_hero() == nullptr)
                available_spaces.push_back(next);
        }
    }

    if (available_spaces.empty())
        throw std::runtime_error("No available spaces to move to.");

    std::vector<int> spaceIndices;
    spaceIndices.reserve(available_spaces.size());
    for (Space *s : available_spaces)
        spaceIndices.push_back((int)(s - spaceBase));

    // ---------- interactive map selection ----------
    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
    Color green   = Color{110, 200, 120, 255};

    auto clampf = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

    int selected = -1;
    bool confirmed = false;
    bool cancelled = false;

    while (!WindowShouldClose())
    {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        Rectangle mapRect = {screenW * 0.06f, screenH * 0.17f, screenW * 0.88f, screenH * 0.72f};
        MapTransform xform = Fit(mapRect);
        float radius = NodeRadius(xform);

        // ---------- input ----------
        Vector2 mouse = GetMousePosition();
        int hovered = -1;
        for (size_t i = 0; i < spaceIndices.size(); i++)
            if (CheckCollisionPointCircle(mouse, SpacePos(xform, spaceIndices[i]), radius * 1.8f))
                hovered = (int)i;

        SetMouseCursor(hovered != -1 ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        if (!spaceIndices.empty() && (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)))
            selected = (selected == -1) ? 0 : (selected + 1) % (int)spaceIndices.size();
        if (!spaceIndices.empty() && (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)))
            selected = (selected <= 0) ? (int)spaceIndices.size() - 1 : selected - 1;

        if (hovered != -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            selected = hovered;
            confirmed = true;
        }
        else if (selected != -1 && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)))
            confirmed = true;
        else if (IsKeyPressed(KEY_ESCAPE))
            cancelled = true;

        // ---------- draw ----------
        BeginDrawing();
        ClearBackground(Color{16, 15, 14, 255});

        std::string title = hero->get_name() + ", CHOOSE A SPACE TO MOVE TO";
        int titleSize = (int)(screenH * 0.036f);
        Vector2 titleDim = MeasureTextEx(GetGameFont(), title.c_str(), (float)titleSize, 3.0f);
        Vector2 titlePos = {screenW / 2.0f - titleDim.x / 2.0f, screenH * 0.045f};
        DrawTextEx(GetGameFont(), title.c_str(), {titlePos.x + 2, titlePos.y + 2}, (float)titleSize, 3.0f, Fade(BLACK, 0.6f));
        DrawTextEx(GetGameFont(), title.c_str(), titlePos, (float)titleSize, 3.0f, gold);

        std::string subtitle = "Movement remaining: " + std::to_string(max_distance);
        int subSize = (int)(screenH * 0.017f);
        Vector2 subDim = MeasureTextEx(GetGameFont(), subtitle.c_str(), (float)subSize, 2.0f);
        DrawTextEx(GetGameFont(), subtitle.c_str(), {screenW / 2.0f - subDim.x / 2.0f, titlePos.y + titleDim.y + 8},
                   (float)subSize, 2.0f, textDim);

        DrawBoardMap(board, mapRect);

        for (size_t i = 0; i < spaceIndices.size(); i++)
        {
            Vector2 pos = SpacePos(xform, spaceIndices[i]);
            bool isSelected = ((int)i == selected);
            bool isHighlighted = ((int)i == hovered) || isSelected;

            if (isHighlighted)
            {
                float glowR = radius * 2.3f;
                DrawCircleGradient(pos, glowR, Fade(isSelected ? green : gold, 0.35f), Fade(gold, 0.0f));
            }

            Color ringCol = isSelected ? green : (isHighlighted ? gold : Fade(gold, 0.55f));
            DrawRing(pos, radius + 3.0f, radius + 6.0f, 0, 360, 28, ringCol);
        }

        const char *footer = "CLICK a glowing room to move there   -   or use ARROWS + ENTER   -   ESC to stay put";
        int footSize = (int)clampf(screenH * 0.016f, 10.0f, 18.0f);
        Vector2 fDim = MeasureTextEx(GetGameFont(), footer, (float)footSize, 2.0f);
        DrawTextEx(GetGameFont(), footer, {screenW / 2.0f - fDim.x / 2.0f, screenH * 0.965f},
                   (float)footSize, 2.0f, Color{140, 135, 128, 255});

        EndDrawing();

        if (confirmed || cancelled)
            break;
    }

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (!confirmed || selected == -1)
        return;

    Space *destination = available_spaces[selected];
    Space *current = hero->get_place();

    current->set_hero(nullptr);
    destination->set_hero(hero);
    hero->set_place(destination);
}

static int PickFromListWithMap(Raylib &self, Board *board, const std::string &title,
                                const std::vector<std::string> &entries)
{
    if (entries.empty())
        return -1;

    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
    Color panelBg = Color{10, 9, 8, 215};
    auto clampf = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

    int selected = 0;
    bool confirmed = false;
    bool backOut = false;

    while (!WindowShouldClose())
    {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        int entryTextSize = (int)clampf(screenH * 0.026f, 15.0f, 28.0f);
        float entryLineH  = entryTextSize * 1.6f;
        float listH       = entries.size() * entryLineH;

        int titleSize  = (int)(entryTextSize * 1.1f);
        float titleGap = entryTextSize * 0.9f;

        int btnTextSize = (int)(entryTextSize * 0.9f);
        float btnH      = btnTextSize * 2.2f;

        float panelW = clampf(screenW * 0.28f, 260.0f, 380.0f);
        float pad    = panelW * 0.08f;
        float panelH = titleSize + titleGap + listH + entryLineH * 0.6f + btnH + pad * 2.2f;

        Rectangle panel = {screenW * 0.04f, screenH / 2.0f - panelH / 2.0f, panelW, panelH};
        Rectangle mapRect = {panel.x + panel.width + screenW * 0.04f, screenH * 0.08f,
                              screenW - (panel.x + panel.width) - screenW * 0.08f, screenH * 0.84f};

        float x      = panel.x + pad;
        float titleY = panel.y + pad;
        float listY  = titleY + titleSize + titleGap;

        std::vector<Rectangle> rowRects(entries.size());
        for (size_t i = 0; i < entries.size(); i++)
            rowRects[i] = {x, listY + entryLineH * i, panel.width - pad * 2, entryLineH};

        float buttonsY = listY + listH + entryLineH * 0.6f;
        Rectangle confirmBtn = {x, buttonsY, (panel.width - pad * 2) * 0.46f, btnH};
        Rectangle backBtn    = {x + (panel.width - pad * 2) * 0.54f, buttonsY, (panel.width - pad * 2) * 0.46f, btnH};

        Vector2 mouse = GetMousePosition();
        int hoveredRow = -1;
        for (size_t i = 0; i < rowRects.size(); i++)
            if (CheckCollisionPointRec(mouse, rowRects[i]))
                hoveredRow = (int)i;

        bool overConfirm = CheckCollisionPointRec(mouse, confirmBtn);
        bool overBack    = CheckCollisionPointRec(mouse, backBtn);

        SetMouseCursor((hoveredRow != -1 || overConfirm || overBack) ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            selected = (selected + 1) % (int)entries.size();
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            selected = (selected - 1 + (int)entries.size()) % (int)entries.size();
        if (hoveredRow != -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            selected = hoveredRow;

        if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overConfirm) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
            confirmed = true;
        else if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overBack) || IsKeyPressed(KEY_ESCAPE))
            backOut = true;

        BeginDrawing();
        ClearBackground(Color{16, 15, 14, 255});

        self.DrawBoardMap(board, mapRect);

        DrawRectangleRounded(panel, 0.08f, 10, panelBg);
        DrawRectangleRoundedLines(panel, 0.08f, 10, Fade(gold, 0.6f));

        Vector2 titleDim = MeasureTextEx(self.GetGameFont(), title.c_str(), (float)titleSize, 2.0f);
        DrawTextEx(self.GetGameFont(), title.c_str(), {panel.x + panel.width / 2.0f - titleDim.x / 2.0f, titleY},
                   (float)titleSize, 2.0f, gold);

        for (size_t i = 0; i < entries.size(); i++)
        {
            bool isSelected = ((int)i == selected);
            Vector2 pos = {x + 10.0f, rowRects[i].y + (entryLineH - entryTextSize) / 2.0f};

            if (isSelected)
                DrawRectangleRounded(rowRects[i], 0.25f, 8, Fade(gold, 0.18f));
            if ((int)i == hoveredRow)
                DrawRectangleRoundedLines(rowRects[i], 0.25f, 8, Fade(gold, 0.5f));

            DrawTextEx(self.GetGameFont(), entries[i].c_str(), {pos.x + 1, pos.y + 1}, (float)entryTextSize, 1.0f, Fade(BLACK, 0.6f));
            DrawTextEx(self.GetGameFont(), entries[i].c_str(), pos, (float)entryTextSize, 1.0f, isSelected ? gold : textDim);
        }

        auto drawButton = [&](Rectangle r, const std::string &label, bool hovered)
        {
            DrawRectangleRounded(r, 0.25f, 8, hovered ? Fade(gold, 0.3f) : panelBg);
            DrawRectangleRoundedLines(r, 0.25f, 8, Fade(gold, 0.6f));
            Vector2 dim = MeasureTextEx(self.GetGameFont(), label.c_str(), (float)btnTextSize, 1.0f);
            Vector2 bp  = {r.x + r.width / 2.0f - dim.x / 2.0f, r.y + r.height / 2.0f - dim.y / 2.0f};
            DrawTextEx(self.GetGameFont(), label.c_str(), bp, (float)btnTextSize, 1.0f, hovered ? WHITE : textDim);
        };

        drawButton(confirmBtn, "Confirm", overConfirm);
        drawButton(backBtn, "Back", overBack);

        const char *footer = "CLICK an entry, or use UP/DOWN + ENTER   -   ESC to go back";
        int footSize = (int)clampf(screenH * 0.014f, 10.0f, 18.0f);
        Vector2 fDim = MeasureTextEx(self.GetGameFont(), footer, (float)footSize, 2.0f);
        DrawTextEx(self.GetGameFont(), footer, {screenW / 2.0f - fDim.x / 2.0f, screenH * 0.965f},
                   (float)footSize, 2.0f, Color{140, 135, 128, 255});

        EndDrawing();

        if (confirmed || backOut)
            break;
    }

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (!confirmed)
        return -1;

    return selected;
}

Heroes *Raylib::SelectHero(Board *board, Heroes *exclude)
{
    if (board == nullptr)
        return nullptr;

    std::vector<Heroes *> fighters;
    std::vector<std::string> entries;

    for (Space &space : board->get_spaces())
    {
        Heroes *hero = space.get_hero();

        if (hero == nullptr || !hero->get_islive() || hero == exclude)
            continue;

        fighters.push_back(hero);
        entries.push_back(hero->get_name() + " (Space " + std::to_string(space.get_number()) + ")");
    }

    if (fighters.empty())
        return nullptr;

    int pick = PickFromListWithMap(*this, board, "CHOOSE FIGHTER", entries);
    if (pick == -1)
        return nullptr;

    return fighters[pick];
}

void Raylib::PlaceHeroAdjacent(Heroes *hero, Heroes *target, Board *board)
{
    if (hero == nullptr || target == nullptr)
        throw std::runtime_error("Hero is nullptr.");

    if (target->get_place() == nullptr)
    {
        msg.push_back("target is nullptr (PlaceHeroAdjacent)");
        return;
    }

    Space *spaceBase = &board->get_spaces()[0];
    std::vector<int> spaceIndices;

    for (Space *space : target->get_place()->get_zone())
        if (space->get_hero() == nullptr)
            spaceIndices.push_back((int)(space - spaceBase));

    if (spaceIndices.empty())
        throw std::runtime_error("No adjacent empty spaces.");

    int pick = PickSpaceOnMap(*this, board, spaceIndices,
                               hero->get_name() + ", CHOOSE A SPACE ADJACENT TO " + target->get_name());
    if (pick == -1)
        pick = 0;

    Space *destination = &board->get_spaces()[spaceIndices[pick]];

    if (hero->get_place() != nullptr)
        hero->get_place()->set_hero(nullptr);

    destination->set_hero(hero);
    hero->set_place(destination);
}


void Raylib::ShowHand(Heroes *owner, Player *, Player *, Board *)
{
    if (owner == nullptr)
        return;

    std::vector<Card *> handPtrs;
    for (Card &c : owner->get_hand())
        handPtrs.push_back(&c);

    std::string heroFolder = HeroAssetFolder(owner->get_name());

    CardImageSelectionMenu(GetGameFont(), owner->get_name() + " - HAND  (ENTER or ESC to continue)",
                            heroFolder, handPtrs, false);
}


Card *Raylib::ChooseCardFromHand(Player *owner, Player *, Player *, Board *board) 
{
    if (owner == nullptr || owner->get_character() == nullptr)
        throw std::runtime_error("No card holder.");

    Heroes *handOwner = owner->get_character();
    auto &hand = handOwner->get_hand();

    if (hand.empty())
        throw std::runtime_error("Hand is empty.");

    std::vector<Card *> allCards;
    for (Card &c : hand)
        allCards.push_back(&c);

    std::string heroFolder = HeroAssetFolder(handOwner->get_name());

    int selected = CardImageSelectionMenu(GetGameFont(), handOwner->get_name() + ", CHOOSE A CARD TO BURN",
                                           heroFolder, allCards, false);

    if (selected < 0)
        selected = 0; 

    return &hand[selected];
}

bool Raylib::Event_Selected_Card(Heroes *SelectedHero, Player *turn, Player *p1, Player *p2, Board *board, Card *&selected_Card)
{
    selected_Card = nullptr;

    if (SelectedHero == nullptr or turn == nullptr or turn->get_character() == nullptr)
        return false;

    Heroes *handOwner = turn->get_character();

    std::vector<Card *> AllowHand;
    for (Card &c : handOwner->get_hand())
    {
        if ((c.get_owner() == SelectedHero->get_name() or c.get_owner() == "ANY") and
            c.get_Attacktype() == "Event")
            AllowHand.push_back(&c);
    }

    if (AllowHand.empty())
    {
        msg.push_back(SelectedHero->get_name() + " has no Event card available to draw.");
        return false;
    }

   
    std::string heroFolder = HeroAssetFolder(handOwner->get_name());
    int selected = CardImageSelectionMenu(GetGameFont(), SelectedHero->get_name() + ", CHOOSE AN EVENT CARD",
                                           heroFolder, AllowHand, false);

    if (selected < 0) 
        return false;

    selected_Card = AllowHand[selected];
    turn->set_selected_card(selected_Card);
    selected_Card->set_user_card(SelectedHero);

    return true;
}

void Raylib::DeclareWinner(Heroes *winner)
{
    if (winner == nullptr)
        return;

    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
    Color panelBg = Color{10, 9, 8, 230};

    Color winnerColor = (winner->get_name() == "DRACULA") ? Color{200, 40, 40, 255} : Color{70, 130, 220, 255};

    auto clampf = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

    bool confirmed = false;

    while (!WindowShouldClose())
    {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        float panelW = clampf(screenW * 0.5f, 480.0f, 900.0f);
        float panelH = clampf(screenH * 0.42f, 260.0f, 460.0f);
        Rectangle panel = {screenW / 2.0f - panelW / 2.0f, screenH / 2.0f - panelH / 2.0f, panelW, panelH};

        BeginDrawing();
        ClearBackground(Color{16, 15, 14, 255});

        DrawRectangleRounded(panel, 0.08f, 12, panelBg);
        DrawRectangleRoundedLines(panel, 0.08f, 12, Fade(gold, 0.7f));

        auto centerText = [&](const std::string &s, float y, int size, Color col, float spacing)
        {
            Vector2 dim = MeasureTextEx(GetGameFont(), s.c_str(), (float)size, spacing);
            Vector2 pos = {screenW / 2.0f - dim.x / 2.0f, y};
            DrawTextEx(GetGameFont(), s.c_str(), {pos.x + 2, pos.y + 2}, (float)size, spacing, Fade(BLACK, 0.6f));
            DrawTextEx(GetGameFont(), s.c_str(), pos, (float)size, spacing, col);
            return dim;
        };

        float y = panel.y + panel.height * 0.14f;

        int trophySize = (int)clampf(screenH * 0.05f, 24.0f, 48.0f);
        Vector2 trophyDim = centerText("V I C T O R Y", y, trophySize, gold, 4.0f);
        y += trophyDim.y + panel.height * 0.08f;

        int nameSize = (int)clampf(screenH * 0.045f, 22.0f, 42.0f);
        Vector2 nameDim = centerText(winner->get_name(), y, nameSize, winnerColor, 3.0f);
        y += nameDim.y + panel.height * 0.04f;

        int subSize = (int)clampf(screenH * 0.024f, 14.0f, 24.0f);
        centerText("HAS WON THE GAME!", y, subSize, WHITE, 2.0f);

        const char *footer = "Press ENTER or CLICK anywhere to continue";
        int footSize = (int)clampf(screenH * 0.016f, 11.0f, 18.0f);
        Vector2 fDim = MeasureTextEx(GetGameFont(), footer, (float)footSize, 2.0f);
        DrawTextEx(GetGameFont(), footer,
                   {screenW / 2.0f - fDim.x / 2.0f, panel.y + panel.height - fDim.y - panel.height * 0.08f},
                   (float)footSize, 2.0f, textDim);

        EndDrawing();

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            confirmed = true;

        if (confirmed)
            break;
    }
}

void Raylib::Show_Help()
{
    static const std::vector<std::string> topics = {
        "Objective", "Setup", "Turn Actions", "Movement & Portals", "Combat",
        "Cards & Hand", "Special Abilities", "Winning the Game", "Back",
    };

    static const std::vector<std::vector<std::string>> pages = {
        { "UNMATCHED pits SHERLOCK HOLMES and DR. WATSON against",
          "DRACULA and his three vampire SISTERS on the streets of",
          "London.", "",
          "Each side takes turns moving fighters around the board,",
          "attacking the other side, and playing cards, trying to",
          "destroy the other team before that happens to them." },
        { "Two players play. Each first enters a name and an age.", "",
          "The younger player and the older player then pick sides:",
          "one plays DRACULA with his three Sisters as comrades, the",
          "other plays SHERLOCK HOLMES with DR. WATSON as a comrade.", "",
          "The younger player always takes the first turn, and turns",
          "alternate between the two players after that.", "",
          "Starting Houses: the Sisters begin in Houses 1, 2 and 4,",
          "DRACULA begins in House 3, SHERLOCK HOLMES begins in House",
          "6, and DR. WATSON begins in House 32." },
        { "Every turn you get 2 actions, in any order, chosen from:", "",
          "  Attack   - fight with one of your fighters.",
          "  Maneuver - move one of your fighters.",
          "  Event    - play one of that fighter's Event cards.",
          "  Back     - end your turn early.", "",
          "Any of your living fighters can take an action, not only",
          "your main hero - so send a Sister or Dr. Watson in instead",
          "of your leader when it helps." },
        { "During a Maneuver, a fighter can move up to its Movement",
          "value (normally 2) across connected Houses.", "",
          "Before moving, you may burn a card from that fighter's",
          "hand to add the card's Boost value to the move instead of",
          "playing it normally.", "",
          "Houses 1, 12, 15 and 27 share hidden passages. Moving",
          "between any two of them costs just one step - but the",
          "passage does NOT count as adjacency for attacks or",
          "zone-based cards." },
        { "Melee fighters (DRACULA, the Sisters, SHERLOCK HOLMES) can",
          "only attack a target standing in an adjacent House.", "",
          "Ranged fighters (DR. WATSON) can attack any target in their",
          "House's wider zone, even without being adjacent.", "",
          "To Attack: pick your fighter, pick a valid target, then",
          "secretly choose an Attack (or Both) card. The defender may",
          "answer with a Defense (or Both) card, or take no card.", "",
          "Both cards flip face up together and resolve in order -",
          "Before, then During, then After effects - before damage is",
          "applied. Damage dealt is the attack value minus the",
          "defense value. Both cards are then discarded." },
        { "Each fighter has their own deck and keeps a hand of cards",
          "drawn from it.", "",
          "Cards can be Attack, Defense, Both, or Event cards. Some",
          "cards belong to one specific fighter, while cards marked",
          "ANY can be used by any fighter on that side.", "",
          "An Event card is played by itself during the Event action",
          "for its special effect, instead of being used in a fight.", "",
          "Watch your deck: if a fighter needs to draw and its deck",
          "is empty, that fighter is damaged instead of drawing." },
        { "DRACULA: at the start of each of his turns, he may bite -",
          "choose one living fighter anywhere on the board, deal it 1",
          "damage, then draw a card. If his deck is empty he takes 2",
          "damage instead of drawing.", "",
          "SHERLOCK HOLMES, DR. WATSON and the Sisters have no",
          "built-in per-turn ability - their tricks come from the",
          "Event cards in their hand instead." },
        { "A side wins the instant the opponent's main fighter",
          "(DRACULA or SHERLOCK HOLMES) has been defeated AND every",
          "one of their comrades has also been defeated.", "",
          "Keep at least one member of your team standing to stay in",
          "the fight!" },
    };

    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
    Color panelBg = Color{10, 9, 8, 215};
    auto clampf = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

    int selected = 0;

    while (!WindowShouldClose())
    {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        int topicTextSize = (int)clampf(screenH * 0.022f, 13.0f, 22.0f);
        float topicLineH  = topicTextSize * 1.7f;

        Rectangle outer = {screenW * 0.08f, screenH * 0.08f, screenW * 0.84f, screenH * 0.84f};
        Rectangle topicsPanel = {outer.x, outer.y, outer.width * 0.28f, outer.height};
        Rectangle contentPanel = {topicsPanel.x + topicsPanel.width + screenW * 0.015f, outer.y,
                                   outer.width - topicsPanel.width - screenW * 0.015f, outer.height};

        std::vector<Rectangle> rowRects(topics.size());
        float rowY = topicsPanel.y + topicsPanel.height * 0.09f;
        for (size_t i = 0; i < topics.size(); i++)
        {
            rowRects[i] = {topicsPanel.x + topicsPanel.width * 0.08f, rowY,
                            topicsPanel.width * 0.84f, topicLineH};
            rowY += topicLineH;
        }

        Vector2 mouse = GetMousePosition();
        int hoveredRow = -1;
        for (size_t i = 0; i < rowRects.size(); i++)
            if (CheckCollisionPointRec(mouse, rowRects[i]))
                hoveredRow = (int)i;

        SetMouseCursor(hoveredRow != -1 ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            selected = (selected + 1) % (int)topics.size();
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            selected = (selected - 1 + (int)topics.size()) % (int)topics.size();
        if (hoveredRow != -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            selected = hoveredRow;

        bool activate = (hoveredRow != -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
                        IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER);
        bool exitRequested = IsKeyPressed(KEY_ESCAPE) ||
                             (activate && selected == (int)topics.size() - 1); // "Back"

        BeginDrawing();
        ClearBackground(Color{16, 15, 14, 255});

        std::string title = "UNMATCHED - HOW TO PLAY";
        int titleSize = (int)clampf(screenH * 0.032f, 18.0f, 32.0f);
        Vector2 titleDim = MeasureTextEx(GetGameFont(), title.c_str(), (float)titleSize, 3.0f);
        DrawTextEx(GetGameFont(), title.c_str(), {screenW / 2.0f - titleDim.x / 2.0f, outer.y - titleDim.y - screenH * 0.02f},
                   (float)titleSize, 3.0f, gold);

        DrawRectangleRounded(topicsPanel, 0.05f, 10, panelBg);
        DrawRectangleRoundedLines(topicsPanel, 0.05f, 10, Fade(gold, 0.6f));
        DrawRectangleRounded(contentPanel, 0.05f, 10, panelBg);
        DrawRectangleRoundedLines(contentPanel, 0.05f, 10, Fade(gold, 0.6f));

        for (size_t i = 0; i < topics.size(); i++)
        {
            bool isSelected = ((int)i == selected);

            if (isSelected)
                DrawRectangleRounded(rowRects[i], 0.3f, 8, Fade(gold, 0.18f));
            if ((int)i == hoveredRow)
                DrawRectangleRoundedLines(rowRects[i], 0.3f, 8, Fade(gold, 0.5f));

            Vector2 pos = {rowRects[i].x + 12.0f, rowRects[i].y + (topicLineH - topicTextSize) / 2.0f};
            DrawTextEx(GetGameFont(), topics[i].c_str(), pos, (float)topicTextSize, 1.0f,
                       isSelected ? gold : textDim);
        }

        if (selected >= 0 && selected < (int)pages.size())
        {
            int lineSize = (int)clampf(screenH * 0.019f, 12.0f, 20.0f);
            float lineH  = lineSize * 1.6f;
            float ly = contentPanel.y + contentPanel.height * 0.06f;

            Vector2 headDim = MeasureTextEx(GetGameFont(), topics[selected].c_str(), (float)(lineSize + 4), 2.0f);
            DrawTextEx(GetGameFont(), topics[selected].c_str(),
                       {contentPanel.x + contentPanel.width * 0.05f, ly}, (float)(lineSize + 4), 2.0f, gold);
            ly += headDim.y + lineH * 0.5f;

            for (const std::string &line : pages[selected])
            {
                DrawTextEx(GetGameFont(), line.c_str(),
                           {contentPanel.x + contentPanel.width * 0.05f, ly}, (float)lineSize, 1.0f, textDim);
                ly += lineH;
            }
        }
        else
        {
            const char *msgLine = "Press ENTER to close this screen.";
            DrawTextEx(GetGameFont(), msgLine,
                       {contentPanel.x + contentPanel.width * 0.05f, contentPanel.y + contentPanel.height * 0.06f},
                       (float)clampf(screenH * 0.019f, 12.0f, 20.0f), 1.0f, textDim);
        }

        const char *footer = "UP/DOWN or hover a topic, ENTER to open  -  select Back or press ESC to close";
        int footSize = (int)clampf(screenH * 0.014f, 10.0f, 18.0f);
        Vector2 fDim = MeasureTextEx(GetGameFont(), footer, (float)footSize, 2.0f);
        DrawTextEx(GetGameFont(), footer, {screenW / 2.0f - fDim.x / 2.0f, outer.y + outer.height + screenH * 0.02f},
                   (float)footSize, 2.0f, Color{140, 135, 128, 255});

        EndDrawing();

        if (exitRequested)
            break;
    }

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}


void Raylib::ChooseCardsToTopOfDeck(Heroes *owner, int maxCount, Player *, Player *, Board *)
{
    if (owner == nullptr || maxCount <= 0)
        return;

    std::vector<Card> chosen;
    std::string heroFolder = HeroAssetFolder(owner->get_name());

    while ((int)chosen.size() < maxCount && !owner->get_hand().empty())
    {
        auto &hand = owner->get_hand();

        std::vector<Card *> handPtrs;
        for (Card &c : hand)
            handPtrs.push_back(&c);

        std::string title = owner->get_name() + ", CHOOSE " + std::to_string(maxCount) +
                             " CARD(S) FOR TOP OF DECK (" + std::to_string(chosen.size()) +
                             "/" + std::to_string(maxCount) + " chosen)";

        int selected = CardImageSelectionMenu(GetGameFont(), title, heroFolder, handPtrs, false);

        if (selected < 0)
            break; // player stopped early

        chosen.push_back(hand[selected]);
        hand.erase(hand.begin() + selected);
    }

    for (auto it = chosen.rbegin(); it != chosen.rend(); ++it)
        owner->get_deck().push_back(*it);
}


bool Raylib::ChooseCardToDiscardOrSkip(Player *opponent)
{
    if (opponent == nullptr || opponent->get_character() == nullptr)
        return false;

    Heroes *handOwner = opponent->get_character();
    auto &hand = handOwner->get_hand();

    if (hand.empty())
        return false;

    std::vector<Card *> handPtrs;
    for (Card &c : hand)
        handPtrs.push_back(&c);

    std::string heroFolder = HeroAssetFolder(handOwner->get_name());
    int selected = CardImageSelectionMenu(GetGameFont(),
                                           handOwner->get_name() + ", DISCARD A CARD OR SKIP  (ESC to skip)",
                                           heroFolder, handPtrs, false);

    if (selected < 0)
        return false;

    handOwner->DiscardCard(selected);
    return true;
}


void Raylib::MoveFogToken(Space *fogSpace, Board *board)
{
    if (fogSpace == nullptr || board == nullptr)
        return;

    Space *spaceBase = &board->get_spaces()[0];
    std::vector<int> spaceIndices;

    for (Space &s : board->get_spaces())
        if (&s != fogSpace && s.get_Fog() == nullptr)
            spaceIndices.push_back((int)(&s - spaceBase));

    if (spaceIndices.empty())
        return;

    int pick = PickSpaceOnMap(*this, board, spaceIndices, "MOVE THE FOG TOKEN TO ANY SPACE");
    if (pick == -1)
        pick = 0;

    Heroes *fogMarker = fogSpace->get_Fog();
    fogSpace->set_Fog(nullptr);
    board->get_spaces()[spaceIndices[pick]].set_Fog(fogMarker);
}



Heroes *Raylib::SelectComrade(Player *owner, Board *board) // Shows the player's living comrades in a menu and returns the one chosen.
{
    if (owner == nullptr || board == nullptr)
        return nullptr;

    std::vector<Heroes *> alive_comrades;
    std::vector<std::string> entries;

    for (Heroes *comrade : owner->get_comrade())
    {
        if (comrade == nullptr || !comrade->get_islive() || comrade->get_place() == nullptr)
            continue;

        alive_comrades.push_back(comrade);
        entries.push_back(comrade->get_name() + " (Space " + std::to_string(comrade->get_place()->get_number()) + ")");
    }

    if (alive_comrades.empty())
        return nullptr;

    int pick = PickFromListWithMap(*this, board, owner->get_name() + ", CHOOSE YOUR COMRADE", entries);
    if (pick == -1)
        return nullptr;

    return alive_comrades[pick];
}

Space *Raylib::SelectFogToken(Board *board)
{
    if (board == nullptr)
        return nullptr;

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

    int pick = PickFromListWithMap(*this, board, "CHOOSE FOG TOKEN", entries);
    if (pick == -1)
        return nullptr;

    return fogSpaces[pick];
}

void Raylib::MoveFogTokenDistance(Space *fogSpace, Board *board, int max_distance)
{
    if (fogSpace == nullptr || board == nullptr || max_distance <= 0)
        return;

    std::vector<Space *> available_spaces;
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
                available_spaces.push_back(next);
        }
    }

    if (available_spaces.empty())
        return;

    Space *spaceBase = &board->get_spaces()[0];
    std::vector<int> spaceIndices;
    spaceIndices.reserve(available_spaces.size());
    for (Space *s : available_spaces)
        spaceIndices.push_back((int)(s - spaceBase));

    int pick = PickSpaceOnMap(*this, board, spaceIndices, "MOVE FOG TOKEN TO...");
    if (pick == -1)
        pick = 0;

    Heroes *fogMarker = fogSpace->get_Fog();
    fogSpace->set_Fog(nullptr);
    available_spaces[pick]->set_Fog(fogMarker);
}

void Raylib::MoveHeroToFogSpace(Heroes *hero, Board *board) // for LURKING effect card
{
    if (hero == nullptr || board == nullptr)
        return;

    Space *spaceBase = &board->get_spaces()[0];
    std::vector<int> spaceIndices;

    for (Space &s : board->get_spaces())
        if (s.get_Fog() != nullptr && s.get_hero() == nullptr)
            spaceIndices.push_back((int)(&s - spaceBase));

    if (spaceIndices.empty())
        return;

    int pick = PickSpaceOnMap(*this, board, spaceIndices, "MOVE " + hero->get_name() + " TO A FOG TOKEN SPACE");
    if (pick == -1)
        pick = 0;

    Space *destination = &board->get_spaces()[spaceIndices[pick]];

    if (hero->get_place() != nullptr)
        hero->get_place()->set_hero(nullptr);

    destination->set_hero(hero);
    hero->set_place(destination);
}


bool Raylib::ChooseBetweenTwoEffects(std::string optionA, std::string optionB) // for LURKING effect card
{
    std::vector<std::string> entries = {optionA, optionB};

    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
    Color panelBg = Color{10, 9, 8, 255};

    auto clampf = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

    int selected = 0;
    bool confirmed = false;

    while (!WindowShouldClose())
    {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        int entryTextSize = (int)clampf(screenH * 0.026f, 15.0f, 28.0f);
        float entryLineH  = entryTextSize * 1.6f;
        float listH       = entries.size() * entryLineH;

        int titleSize  = (int)(entryTextSize * 1.1f);
        float titleGap = entryTextSize * 0.9f;

        float panelW = clampf(screenW * 0.36f, 340.0f, 560.0f);
        float pad    = panelW * 0.08f;
        float panelH = titleSize + titleGap + listH + pad * 2.2f;

        Rectangle panel = {screenW / 2.0f - panelW / 2.0f, screenH / 2.0f - panelH / 2.0f, panelW, panelH};

        float x      = panel.x + pad;
        float titleY = panel.y + pad;
        float listY  = titleY + titleSize + titleGap;

        std::vector<Rectangle> rowRects(entries.size());
        for (size_t i = 0; i < entries.size(); i++)
            rowRects[i] = {x, listY + entryLineH * i, panel.width - pad * 2, entryLineH};

        Vector2 mouse = GetMousePosition();
        int hoveredRow = -1;
        for (size_t i = 0; i < rowRects.size(); i++)
            if (CheckCollisionPointRec(mouse, rowRects[i]))
                hoveredRow = (int)i;

        SetMouseCursor(hoveredRow != -1 ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            selected = (selected + 1) % (int)entries.size();
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            selected = (selected - 1 + (int)entries.size()) % (int)entries.size();

        if (hoveredRow != -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            selected = hoveredRow;
            confirmed = true;
        }
        else if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
            confirmed = true;

        BeginDrawing();
        ClearBackground(Color{16, 15, 14, 255});

        DrawRectangleRounded(panel, 0.08f, 10, panelBg);
        DrawRectangleRoundedLines(panel, 0.08f, 10, Fade(gold, 0.6f));

        std::string title = "CHOOSE AN EFFECT";
        Vector2 titleDim = MeasureTextEx(GetGameFont(), title.c_str(), (float)titleSize, 2.0f);
        DrawTextEx(GetGameFont(), title.c_str(), {panel.x + panel.width / 2.0f - titleDim.x / 2.0f, titleY},
                   (float)titleSize, 2.0f, gold);

        for (size_t i = 0; i < entries.size(); i++)
        {
            bool isSelected = ((int)i == selected);
            Vector2 pos = {x + 10.0f, rowRects[i].y + (entryLineH - entryTextSize) / 2.0f};

            if (isSelected)
                DrawRectangleRounded(rowRects[i], 0.25f, 8, Fade(gold, 0.18f));
            if ((int)i == hoveredRow)
                DrawRectangleRoundedLines(rowRects[i], 0.25f, 8, Fade(gold, 0.5f));

            DrawTextEx(GetGameFont(), entries[i].c_str(), {pos.x + 1, pos.y + 1}, (float)entryTextSize, 1.0f, Fade(BLACK, 0.6f));
            DrawTextEx(GetGameFont(), entries[i].c_str(), pos, (float)entryTextSize, 1.0f, isSelected ? gold : textDim);
        }

        const char *footer = "CLICK an option, or use UP/DOWN + ENTER";
        int footSize = (int)clampf(screenH * 0.014f, 10.0f, 18.0f);
        Vector2 fDim = MeasureTextEx(GetGameFont(), footer, (float)footSize, 2.0f);
        DrawTextEx(GetGameFont(), footer, {screenW / 2.0f - fDim.x / 2.0f, screenH * 0.965f},
                   (float)footSize, 2.0f, Color{140, 135, 128, 255});

        EndDrawing();

        if (confirmed)
            break;
    }

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    return selected == 0; // true = optionA
}


void Raylib::MoveFogTokenAnywhere(Space *fogSpace, Board *board) // for ROLLING FOG effect card
{
    if (fogSpace == nullptr || board == nullptr)
        return;

    Space *spaceBase = &board->get_spaces()[0];
    std::vector<int> spaceIndices;

    for (Space &s : board->get_spaces())
        if (&s != fogSpace && s.get_Fog() == nullptr)
            spaceIndices.push_back((int)(&s - spaceBase));

    if (spaceIndices.empty())
        return;

    int pick = PickSpaceOnMap(*this, board, spaceIndices, "MOVE THE FOG TOKEN TO...");
    if (pick == -1)
        pick = 0;

    Heroes *fogMarker = fogSpace->get_Fog();
    fogSpace->set_Fog(nullptr);
    board->get_spaces()[spaceIndices[pick]].set_Fog(fogMarker);
}

Space *Raylib::MoveFogTokenToEmptySpace(Space *fogSpace, Board *board) // for SLIP AWAY effect card
{
    if (fogSpace == nullptr || board == nullptr)
        return nullptr;

    Space *spaceBase = &board->get_spaces()[0];
    std::vector<int> spaceIndices;

    for (Space &s : board->get_spaces())
        if (&s != fogSpace && s.get_hero() == nullptr && s.get_Fog() == nullptr)
            spaceIndices.push_back((int)(&s - spaceBase));

    if (spaceIndices.empty())
        return nullptr;

    int pick = PickSpaceOnMap(*this, board, spaceIndices, "MOVE THE FOG TOKEN TO AN EMPTY SPACE");
    if (pick == -1)
        return nullptr; // caller treats a null destination as "stay put"

    Heroes *fogMarker = fogSpace->get_Fog();
    fogSpace->set_Fog(nullptr);

    Space *destination = &board->get_spaces()[spaceIndices[pick]];
    destination->set_Fog(fogMarker);

    return destination;
}


Heroes *Raylib::SelectAdjacentHero(Heroes *center, Board *board) // for STEP LIGHTLY effect card
{
    if (center == nullptr || board == nullptr || center->get_place() == nullptr)
        return nullptr;

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

    int pick = PickFromListWithMap(*this, board, "CHOOSE ADJACENT FIGHTER", entries);
    if (pick == -1)
        return nullptr;

    return fighters[pick];
}

void Raylib::PlaceHeroOnBoard(Heroes *hero, Board *board) // for VANISH effect card
{
    if (hero == nullptr || board == nullptr)
        return;

    Space *spaceBase = &board->get_spaces()[0];
    std::vector<int> spaceIndices;

    for (Space &s : board->get_spaces())
        if (s.get_hero() == nullptr)
            spaceIndices.push_back((int)(&s - spaceBase));

    if (spaceIndices.empty())
        return;

    int pick = PickSpaceOnMap(*this, board, spaceIndices, "PLACE " + hero->get_name() + " ON ANY SPACE");
    if (pick == -1)
        return; // stays pending placement until the player is ready

    Space *destination = &board->get_spaces()[spaceIndices[pick]];

    hero->set_place(destination);
    destination->set_hero(hero);
}

int Raylib::SlotMenu(const std::vector<std::string> &slotLabels, const std::string &title)
{
    if (slotLabels.empty())
        return -1;

    Color gold    = Color{198, 170, 108, 255};
    Color textDim = Color{190, 185, 175, 255};
    Color panelBg = Color{10, 9, 8, 215};
    auto clampf = [](float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); };

    int selected = 0;
    bool confirmed = false;
    bool backOut = false;

    while (!WindowShouldClose())
    {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        int entryTextSize = (int)clampf(screenH * 0.028f, 16.0f, 30.0f);
        float entryLineH  = entryTextSize * 1.7f;
        float listH       = slotLabels.size() * entryLineH;

        int titleSize  = (int)(entryTextSize * 1.15f);
        float titleGap = entryTextSize * 1.0f;

        int btnTextSize = (int)(entryTextSize * 0.9f);
        float btnH      = btnTextSize * 2.2f;

        float panelW = clampf(screenW * 0.34f, 320.0f, 480.0f);
        float pad    = panelW * 0.08f;
        float panelH = titleSize + titleGap + listH + entryLineH * 0.6f + btnH + pad * 2.2f;

        Rectangle panel = {screenW / 2.0f - panelW / 2.0f, screenH / 2.0f - panelH / 2.0f, panelW, panelH};

        float x      = panel.x + pad;
        float titleY = panel.y + pad;
        float listY  = titleY + titleSize + titleGap;

        std::vector<Rectangle> rowRects(slotLabels.size());
        for (size_t i = 0; i < slotLabels.size(); i++)
            rowRects[i] = {x, listY + entryLineH * i, panel.width - pad * 2, entryLineH};

        float backY = listY + listH + entryLineH * 0.6f;
        Rectangle backBtn = {x, backY, panel.width - pad * 2, btnH};

        // ---------- input ----------
        Vector2 mouse = GetMousePosition();
        int hoveredRow = -1;
        for (size_t i = 0; i < rowRects.size(); i++)
            if (CheckCollisionPointRec(mouse, rowRects[i]))
                hoveredRow = (int)i;

        bool overBack = CheckCollisionPointRec(mouse, backBtn);

        SetMouseCursor((hoveredRow != -1 || overBack) ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            selected = (selected + 1) % (int)slotLabels.size();
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            selected = (selected - 1 + (int)slotLabels.size()) % (int)slotLabels.size();
        if (hoveredRow != -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            selected = hoveredRow;
            confirmed = true;
        }

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
            confirmed = true;
        else if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && overBack) || IsKeyPressed(KEY_ESCAPE))
            backOut = true;

        // ---------- draw ----------
        BeginDrawing();
        ClearBackground(Color{16, 15, 14, 255});

        DrawRectangleRounded(panel, 0.08f, 10, panelBg);
        DrawRectangleRoundedLines(panel, 0.08f, 10, Fade(gold, 0.6f));

        Vector2 titleDim = MeasureTextEx(GetGameFont(), title.c_str(), (float)titleSize, 2.0f);
        DrawTextEx(GetGameFont(), title.c_str(),
                   {panel.x + panel.width / 2.0f - titleDim.x / 2.0f, titleY},
                   (float)titleSize, 2.0f, gold);

        for (size_t i = 0; i < slotLabels.size(); i++)
        {
            bool isSelected = ((int)i == selected);
            std::string label = "Slot " + std::to_string(i + 1) + ": " + slotLabels[i];
            Vector2 pos = {x + 10.0f, rowRects[i].y + (entryLineH - entryTextSize) / 2.0f};

            if (isSelected)
                DrawRectangleRounded(rowRects[i], 0.2f, 8, Fade(gold, 0.18f));
            if ((int)i == hoveredRow)
                DrawRectangleRoundedLines(rowRects[i], 0.2f, 8, Fade(gold, 0.5f));

            DrawTextEx(GetGameFont(), label.c_str(), {pos.x + 1, pos.y + 1}, (float)entryTextSize, 1.0f, Fade(BLACK, 0.6f));
            DrawTextEx(GetGameFont(), label.c_str(), pos, (float)entryTextSize, 1.0f, isSelected ? gold : textDim);
        }

        DrawRectangleRounded(backBtn, 0.2f, 8, overBack ? Fade(gold, 0.3f) : panelBg);
        DrawRectangleRoundedLines(backBtn, 0.2f, 8, Fade(gold, 0.6f));
        Vector2 backDim = MeasureTextEx(GetGameFont(), "Back", (float)btnTextSize, 1.0f);
        DrawTextEx(GetGameFont(), "Back",
                   {backBtn.x + backBtn.width / 2.0f - backDim.x / 2.0f, backBtn.y + backBtn.height / 2.0f - backDim.y / 2.0f},
                   (float)btnTextSize, 1.0f, overBack ? WHITE : textDim);

        const char *footer = "CLICK a slot, or use UP/DOWN + ENTER   -   ESC to go back";
        int footSize = (int)clampf(screenH * 0.014f, 10.0f, 18.0f);
        Vector2 fDim = MeasureTextEx(GetGameFont(), footer, (float)footSize, 2.0f);
        DrawTextEx(GetGameFont(), footer, {screenW / 2.0f - fDim.x / 2.0f, screenH * 0.965f},
                   (float)footSize, 2.0f, Color{140, 135, 128, 255});

        EndDrawing();

        if (confirmed || backOut)
            break;
    }

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (!confirmed)
        return -1;

    return selected;
}