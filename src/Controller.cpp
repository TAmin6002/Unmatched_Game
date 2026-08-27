#include <fstream>
#include <filesystem>
#include <iostream>

#include "Controller.h"
#include "Ftxui_Front.h"
#include "Json.h"


using namespace std;

// int Controller::get_DraculaAction() const
// {
//     return DraculaAction;
// }

// void Controller::set_DraculaAction()
// {
//     DraculaAction++;
// }

// int Controller::get_SherlockAction() const
// {
//     return SherlockAction;
// }

// void Controller::set_SherlockAction()
// {
//     SherlockAction++;
// }

Dracula *Controller::get_Dracula()
{
    return &dracula;
}

SherlockHolmes *Controller::get_SherlockHolmes()
{
    return &sherlock;
}

Card *Controller::get_Attacker_selected_card()
{
    return Attacker_selected_card;
}

Card *Controller::get_Defender_selected_card(){

    return Defender_selected_card;
}

void Controller::set_players_character(int younger_choice, int older_choice)
{
    Player *Yanger_Player = (p1.get_age() <= p2.get_age() ? &p1 : &p2);
    Player *Older_Player  = (p1.get_age() <= p2.get_age() ? &p2 : &p1);

    auto assign_hero = [&](Player *player, int choice)
    {
        if (choice == 0) // DRACULA
        {
            player->set_character(&dracula);
            player->set_comrade(&s1);
            player->set_comrade(&s2);
            player->set_comrade(&s3);
        }
        else if (choice == 1) // SHERLOCK HOLMES
        {
            player->set_character(&sherlock);
            player->set_comrade(&Watson);
        }
        else if (choice == 2) // INVISIBLE MAN
        {
            player->set_character(&invisibleMan);
            player->set_comrade(&f1);
            player->set_comrade(&f2);
            player->set_comrade(&f3);
        }
    };

    assign_hero(Yanger_Player, younger_choice);
    assign_hero(Older_Player, older_choice);
}

// void Controller::Initial_characters_places()
// {
//     get_younger_player()->get_character()->set_place(&(board.get_spaces()[23])); // space 1
//     board.get_spaces()[23].set_hero(get_younger_player()->get_character());

//     get_older_player()->get_character()->set_place(&(board.get_spaces()[2])); // space 2
//     board.get_spaces()[24].set_hero(get_older_player()->get_character());
// }

Player *Controller::get_younger_player()
{
    if (p1.get_age() <= p2.get_age())
        return &p1;

    return &p2;
}

Player *Controller::get_older_player()
{
    if (p1.get_age() <= p2.get_age())
        return &p2;

    return &p1;
}

// bool Controller::can_Attack(Player *turn)
// {
//     if (!turn->get_character()->get_islive())
//         return false;
// }

void Controller::Initial_turn()
{
    if (p1.get_age() <= p2.get_age())
    {
        turn = &p1;
        not_turn = &p2;
    }

    else
    {
        turn = &p2;
        not_turn = &p1;
    }
}

void Controller::chane_turn()
{
    if (turn == &p1)
    {
        turn = &p2;
        not_turn = &p1;
    }

    else
    {
        turn = &p1;
        not_turn = &p2;
    }
}

std::array<Heroes *, 10> Controller::AllHeroes()
{
    return {&dracula, &s1, &s2, &s3, &sherlock, &Watson, &invisibleMan, &f1, &f2, &f3};
}

std::string Controller::HeroToId(Heroes *hero) const
{
    if (hero == nullptr)
        return "";

    return hero->get_name() + "#" + std::to_string(hero->get_number());
}

Heroes *Controller::FindHeroByKey(const std::string &key)
{
    if (key.empty())
        return nullptr;

    for (Heroes *hero : AllHeroes())
        if (HeroToId(hero) == key)
            return hero;

    return nullptr;
}

bool Controller::IsFogToken(Heroes *hero) const
{
    return hero == &f1 || hero == &f2 || hero == &f3;
}

// This method takes a card and converts its position into a JSON reference containing its owner hero's ID and index for serialization.
Json Controller::EncodeCardRef(Card *card)
{
    if (card == nullptr)
        return Json();

    for (Heroes *hero : AllHeroes())
    {
        std::vector<Card> &hand = hero->get_hand();

        // Checking the range using memory address.
        if (!hand.empty() && card >= &hand.front() && card <= &hand.back())
        {
            Json ref = Json::makeObject();
            ref.set("hero", Json(HeroToId(hero)));
            ref.set("index", Json(static_cast<int>(card - &hand.front())));
            return ref;
        }
    }

    return Json();
}

Card *Controller::DecodeCardRef(const Json &j)
{
    if (j.getType() != Type::Object)
        return nullptr;

    Heroes *hero = FindHeroByKey(j["hero"].asString());

    if (hero == nullptr)
        return nullptr;

    int index = j["index"].asInt(-1);
    std::vector<Card> &hand = hero->get_hand();

    if (index < 0 || index >= static_cast<int>(hand.size()))
        return nullptr;

    return &hand[index];
}

std::string Controller::SaveFilePath(int slot) const
{
    return "saves/slot_" + std::to_string(slot) + ".json";
}

bool Controller::SaveGame(int slot)
{
    if (slot < 1 || slot > MaxSaveSlots)
        return false;

    std::filesystem::create_directories("saves");

    Json root = Json::makeObject();

    root.set("round", Json(round));
    root.set("turn", Json(turn == &p1 ? std::string("p1") : std::string("p2")));

    root.set("attackValue", Json(Attack_Value));
    root.set("defenseValue", Json(Defense_Value));
    root.set("attackLocked", Json(Attack_Locked));
    root.set("defenseLocked", Json(Defense_Locked));

    root.set("attacker", Json(HeroToId(Attacker)));
    root.set("defender", Json(HeroToId(Defender)));

    root.set("attackerSelectedCard", EncodeCardRef(Attacker_selected_card));
    root.set("defenderSelectedCard", EncodeCardRef(Defender_selected_card));

    Json heroes = Json::makeObject();

    for (Heroes *hero : AllHeroes())
        heroes.set(HeroToId(hero), hero->toJson());

    root.set("heroes", heroes);

    

    Json occupancy = Json::makeArray();
    for (Space &space : board.get_spaces())
    {
        Heroes *occupant = space.get_hero();
        if (occupant != nullptr)
        {
            Json entry = Json::makeObject();
            entry.set("hero", Json(HeroToId(occupant)));
            entry.set("space", Json(space.get_number()));
            entry.set("slot", Json(std::string("hero")));
            occupancy.push_back(entry);
        }

        Heroes *fogToken = space.get_Fog();
        if (fogToken != nullptr)
        {
            Json entry = Json::makeObject();
            entry.set("hero", Json(HeroToId(fogToken)));
            entry.set("space", Json(space.get_number()));
            entry.set("slot", Json(std::string("fog")));
            occupancy.push_back(entry);
        }
    }
    root.set("occupancy", occupancy);

    auto playerToJson = [this](Player &p) -> Json
    {
        Json pj = Json::makeObject();

        pj.set("age", Json(p.get_age()));
        pj.set("name", Json(p.get_name()));
        pj.set("count", Json(p.get_count()));
        pj.set("isAttacker", Json(p.get_isAttacker()));
        pj.set("character", Json(HeroToId(p.get_character())));

        Json comradeArr = Json::makeArray();

        for (Heroes *c : p.get_comrade())
            comradeArr.push_back(Json(HeroToId(c)));

        pj.set("comrade", comradeArr);

        pj.set("selectedCard", EncodeCardRef(p.get_selected_card()));

        return pj;
    };

    root.set("p1", playerToJson(p1));
    root.set("p2", playerToJson(p2));

        std::ofstream out(SaveFilePath(slot));
        
        if (!out.is_open())
            return false;

        out << root.dump();
        return true;
}

// read json file and load informations .
bool Controller::LoadGame(int slot)
{
    if (slot < 1 || slot > MaxSaveSlots)
        return false;

    std::ifstream in(SaveFilePath(slot));
    if (!in.is_open())
        return false;

    // This line of code reads the entire content of a file from start to finish using iterators and stores it all at once into a string variable.
    std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    Json root = Json::parse(text);

    // reset everything to a clean state.
    board = Board();

    p1 = Player();
    p2 = Player();

    dracula = Dracula();
    s1 = Sisters{1};
    s2 = Sisters{2};
    s3 = Sisters{3};

    sherlock = SherlockHolmes();
    Watson = Dr_Watson();

    invisibleMan = InvisibleMan{};
    f1 = Fog{1};
    f2 = Fog{2};
    f3 = Fog{3};

    Attacker = nullptr;
    Defender = nullptr;
    Attacker_selected_card = nullptr;
    Defender_selected_card = nullptr;

    auto resolver = [this](const std::string &key) -> Heroes *
    { return FindHeroByKey(key); };

    Json heroesJson = root["heroes"];
    for (Heroes *hero : AllHeroes())
        hero->loadFromJson(heroesJson[HeroToId(hero)], board, resolver);

        // Loading information for each hero.

    Json occupancy = root["occupancy"];
    for (size_t i = 0; i < occupancy.size(); i++)
    {
        Heroes *hero = FindHeroByKey(occupancy[i]["hero"].asString());
        int spaceNumber = occupancy[i]["space"].asInt(-1);

        if (hero == nullptr || spaceNumber < 1)
            continue;

        Space *space = &board.get_spaces()[spaceNumber - 1];

        std::string slot = occupancy[i]["slot"].asString(IsFogToken(hero) ? "fog" : "hero");

        if (slot == "fog")
            space->set_Fog(hero);
        else
            space->set_hero(hero);
    }

    auto playerFromJson = [&resolver](Player &p, const Json &pj)
    {
        p.set_age(pj["age"].asInt());
        p.set_name(pj["name"].asString());
        p.set_count(pj["count"].asInt());
        p.set_isAttacker(pj["isAttacker"].asBool(false));
        p.set_character(resolver(pj["character"].asString()));

        Json comradeArr = pj["comrade"];
        for (size_t i = 0; i < comradeArr.size(); i++)
        {
            Heroes *comrade = resolver(comradeArr[i].asString());
            if (comrade != nullptr)
                p.set_comrade(comrade);
        }
    };

    playerFromJson(p1, root["p1"]);
    playerFromJson(p2, root["p2"]);

    // selected-card pointers can only be resolved once every hand is loaded
    p1.set_selected_card(DecodeCardRef(root["p1"]["selectedCard"]));
    p2.set_selected_card(DecodeCardRef(root["p2"]["selectedCard"]));

    round = root["round"].asInt(1);
    turn = (root["turn"].asString("p1") == "p1") ? &p1 : &p2;
    not_turn = (turn == &p1) ? &p2 : &p1;

    Attack_Value = root["attackValue"].asInt(0);
    Defense_Value = root["defenseValue"].asInt(0);
    Attack_Locked = root["attackLocked"].asBool(false);
    Defense_Locked = root["defenseLocked"].asBool(false);

    Attacker = FindHeroByKey(root["attacker"].asString());
    Defender = FindHeroByKey(root["defender"].asString());

    Attacker_selected_card = DecodeCardRef(root["attackerSelectedCard"]);
    Defender_selected_card = DecodeCardRef(root["defenderSelectedCard"]);

    return true;
}

bool Controller::HasSave(int slot) const
{
    std::ifstream in(SaveFilePath(slot));
    return in.good();
}

std::string Controller::GetSaveSummary(int slot) const
{
    std::ifstream in(SaveFilePath(slot));
    if (!in.is_open())
        return "Empty Slot";

    std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    Json root = Json::parse(text);

    std::string p1Name = root["p1"]["name"].asString("Player 1");
    std::string p2Name = root["p2"]["name"].asString("Player 2");
    int roundNum = root["round"].asInt(1);

    return p1Name + " vs " + p2Name + " - Round " + std::to_string(roundNum);
}

bool check_winner(Heroes *hero, Player *p1, Player *p2)
{
    if (p1->get_character() != hero && p2->get_character() != hero)
        return false;

    Player *hero_player = (p1->get_character() == hero) ? p1 : p2;
    Player *opponent_player = (hero_player == p1) ? p2 : p1;

    if (!hero->get_islive())
        return false;

    if (opponent_player->get_character()->get_islive())
        return false;

    for (Heroes *comrade : opponent_player->get_comrade())
    {
        if (comrade->get_name() == "FOG")
            continue;

        if (comrade->get_islive())
            return false;
    }

    return true;
}

void Controller::run()
{
    
    //ool Exit = false;

    int monitor = 0;

    int screenW = GetMonitorWidth(monitor);
    int screenH = GetMonitorHeight(monitor);

    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(screenW, screenH, "Unmatched");
    InitAudioDevice();

    SetTargetFPS(60);

    
    
     while (true)
    {
        if (WindowShouldClose())
            break;

        switch (RF.Menu_())
        {

        case e_Menu::Play:
        {
            board = Board();

            p1 = Player();
            p2 = Player();

            dracula = Dracula();
            s1 = Sisters{1};
            s2 = Sisters{2};
            s3 = Sisters{3};

            sherlock = SherlockHolmes();
            Watson = Dr_Watson();

            invisibleMan = InvisibleMan{};
            f1 = Fog{1};
            f2 = Fog{2};
            f3 = Fog{3};

            RF.Players_Info_List(&p1, &p2);

            std::vector<int> hero_choices = RF.Det_characters(&p1, &p2);
            set_players_character(hero_choices[0], hero_choices[1]);

            RF.catch_place(&p1, &p2, &board);

            RF.choose_comrad_place(&p1, &p2, &board);

            if (round == 1)
                Initial_turn();

            GameLoop();
        }
        break;

        case e_Menu::Continue:
        {
            std::vector<std::string> labels = {
                GetSaveSummary(1),
                GetSaveSummary(2),
                GetSaveSummary(3)};

            int chosen = RF.SlotMenu(labels, "Continue Game");

            if (chosen != -1 && HasSave(chosen + 1) && LoadGame(chosen + 1))
                GameLoop();
        }
        break;

        case e_Menu::Help:
        {
            RF.Show_Help();
        }
        break;

        case e_Menu::Exit:
        {
            Exit = true;
        }
        break;
        }

        if (Exit)
        {
            Exit = false;
            break;
        }
    }
    RF.UnloadGameFont();
    CloseAudioDevice();
    CloseWindow();

    
}

    // while (true)
    // {
        
    //     switch (RF.Menu_())
    //     {
            
    //         case e_Menu::Play:
    //         {
                   
    //         board = Board();

    //         p1 = Player();
    //         p2 = Player();
            
    //         dracula = Dracula();
    //         s1 = Sisters{1};
    //         s2 = Sisters{2};
    //         s3 = Sisters{3};
            
    //         sherlock = SherlockHolmes();
    //         Watson = Dr_Watson();

    //         invisibleMan = InvisibleMan{};
    //         f1 = Fog{1};
    //         f2 = Fog{2};
    //         f3 = Fog{3};


            

    //         RF.Players_Info_List(&p1, &p2);
            
    //         // RF.catch_place(&p1, &p2, &board);

    //         std::vector<int> hero_choices = RF.Det_characters(&p1, &p2);
    //         set_players_character(hero_choices[0], hero_choices[1]);

    //         RF.catch_place(&p1, &p2, &board);

    //         RF.choose_comrad_place(&p1, &p2, &board);
            
    //         if(round == 1)
    //         Initial_turn();
            
    //         // std::vector<int> choices = RF.Det_characters(&p1, &p2);
    //         // set_players_character(choices[0], choices[1]);

            
    //         // dracula.set_place(&board.get_spaces()[2]);
    //         // s1.set_place(&board.get_spaces()[0]);
    //         // s2.set_place(&board.get_spaces()[1]);
    //         // s3.set_place(&board.get_spaces()[3]);
            
    //         // board.get_spaces()[2].set_hero(&dracula);
    //         // board.get_spaces()[0].set_hero(&s1);
    //         // board.get_spaces()[1].set_hero(&s2);
    //         // board.get_spaces()[3].set_hero(&s3);
            
    //         // sherlock.set_place(&board.get_spaces()[5]);
    //         // Watson.set_place(&board.get_spaces()[31]);
            
    //         // board.get_spaces()[5].set_hero(&sherlock);
    //         // board.get_spaces()[31].set_hero(&Watson);
            



    //         while (true)
    //         {
    //             if(check_winner(&dracula, &p1, &p2))
    //             {
    //                 RF.DeclareWinner(&dracula);
    //                 Exit = true;
    //                 break;
    //             }
    //             else if(check_winner(&sherlock, &p1, &p2))
    //             {
    //                 RF.DeclareWinner(&sherlock);
    //                 Exit = true;
    //                 break;
    //             }
    //              else if(check_winner(&invisibleMan, &p1, &p2))
    //             {
    //                 RF.DeclareWinner(&invisibleMan);
    //                 Exit = true;
    //                 break;
    //             }
                

    //              if (turn->get_count() == 0 && turn->get_character()->get_name() == "DRACULA")
    //                 {
    //                     try
    //                     {
    //                         cout << "before dracula.abiliti()\n";
    //                         dracula.abiliti(&board);
    //                         cout << "after dracula.abiliti()\n";
    //                     }
    //                     catch (const std::exception &e)
    //                     {
    //                         RF.get_msg().push_back(e.what());
    //                     }
    //                 }

    //               if (turn->get_character()->get_name() == "InvisibleMan")
    //                 {
    //                     if (turn->get_character()->get_PendingPlacement())
    //                     {
    //                         RF.PlaceHeroOnBoard(turn->get_character(), &board);

    //                         if (turn->get_character()->get_place() != nullptr)
    //                             turn->get_character()->set_PendingPlacement(false);
    //                     }
    //                     else if (turn->get_count() == 0)
    //                     {
    //                         bool onFog = (turn->get_character()->get_place() != nullptr &&
    //                                     turn->get_character()->get_place()->get_Fog() != nullptr);
                                        
    //                         turn->get_character()->set_StartedTurnOnFog(onFog);
    //                     }
    //                 }
                    
    //             RF.main_map(&p1, &p2, &board, turn);
                

    //             switch (RF.get_number_of_choose())
    //             {

    //                 case 0: // Attack
    //                 {
                    //     if (turn->get_count() < 2)
                    //     {
                    //         Defender = nullptr;

                    //         try
                    //         {

                    //             if (!RF.Attakcer_Heroes_Menu(turn, &board, Attacker) || Attacker == nullptr)
                    //                 break; 
                    //             cout << "1\n";
                                
                    //             if (!RF.Defender_Heroes_Menu(not_turn, &board, Defender, Attacker) || Defender == nullptr)
                    //                 break; 
                    //             cout << "2\n";
                                
                    //             if (!RF.Attacker_selected_card(Attacker, Defender, &p1, &p2, &board, Attacker_selected_card) || Attacker_selected_card == nullptr)
                    //                 break; 
                    //             cout << "3\n";

                    //             if (!RF.Defender_selected_card(Attacker, Defender, &p1, &p2, &board, Defender_selected_card))
                    //                 break; 
                    //             cout << "4\n";
                                
                    //             RF.Reveal_Combat(Attacker, Defender, Attacker_selected_card, Defender_selected_card); // show tow v&s cards
                    //             cout << "5\n";

                    //             Attack_Value = Attacker_selected_card->get_amount();
                                
                    //             if(Defender_selected_card == nullptr) {Defense_Value = 0;}
                    //             else
                    //                 Defense_Value = Defender_selected_card->get_amount();


                    //             if (Defender->get_name() == "InvisibleMan" && Defender->get_place() != nullptr && Defender->get_place()->get_Fog() != nullptr)
                    //                 {
                    //                     Defense_Value += 1;
                    //                 }

                    //         }
                    //         catch (const std::exception &e)
                    //         {
                    //             RF.get_msg().push_back(e.what());
                    //             // cout << e.what() << endl;
                    //         }

                    //         // ------------------------ start combat -----------------------------------.
                            
                    //         if(!(Attacker_selected_card == nullptr or Defender_selected_card == nullptr)){
                    //         try
                    //         {

                    //             // -------------------------- Befor Calculation --------------------------.
                    //             cout << "6\n";
                                
                    //             if (Defender_selected_card != nullptr && Defender_selected_card->get_CardTiming() == CardTiming::Before)
                    //             {
                    //                 card_resolver.excute(Defender_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                    //             }

                    //             if (Attacker_selected_card->get_CardTiming() == CardTiming::Before)
                    //             {
                    //                 card_resolver.excute(Attacker_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                    //             }
                                
                                
                    //             // -------------------------- During Calculation --------------------------.
                    //             cout << "7\n";

                                
                    //             if (Defender_selected_card != nullptr && Defender_selected_card->get_CardTiming() == CardTiming::During)
                    //             {
                    //                 card_resolver.excute(Defender_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                    //             }
                                
                    //             if (Attacker_selected_card != nullptr && Attacker_selected_card->get_CardTiming() == CardTiming::During)
                    //             {
                    //                 card_resolver.excute(Attacker_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                    //             }
                               
                                
                                
                    //             // --------------------------- Damage Calculation ------------------------.
                                
                    //             cout << "8\n";
                                
                    //             int Damage = Attack_Value - Defense_Value;
                                
                    //             if (Damage >= 1)
                    //             Defender->Damage(Damage);
                                
                    //             // Heroes *Winner = (Attack_Value > Defense_Value) ? Attacker : Defender;
                                
                    //             //---------------------------- After Calculation ---------------------------.
                    //             cout << "9\n";
                                
                    //             if (Defender_selected_card != nullptr && Defender_selected_card->get_CardTiming() == CardTiming::After)
                    //             {
                    //                 card_resolver.excute(Defender_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                    //             }

                    //            if (Attacker_selected_card != nullptr && Attacker_selected_card->get_CardTiming() == CardTiming::After)
                    //             {
                    //                 card_resolver.excute(Attacker_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                    //             }
                                
                                
                    //             // ------------------------- Transfer Cards to discard --------------------------.
                    //             cout << "10\n";
                                
                    //            if (Attacker_selected_card != nullptr)
                    //             {
                    //                 Attacker->Discard_Card(Attacker_selected_card);
                    //                 Attacker_selected_card->set_user_card(nullptr);
                    //             }

                    //             if (Defender_selected_card != nullptr)
                    //             {
                    //                 Defender->Discard_Card(Defender_selected_card);
                    //                 Defender_selected_card->set_user_card(nullptr);
                    //             }

                    //             Attacker_selected_card = nullptr;
                    //             Defender_selected_card = nullptr;
                                
                    //             Player *Attacker_Player = (p1.get_character()->get_name() == Attacker->get_name() ? &p1 : &p2);
                    //             Player *Defender_Player = (p1.get_character()->get_name() == Defender->get_name() ? &p1 : &p2);
                                
                    //             Attacker_Player->set_selected_card(nullptr);
                    //             Defender_Player->set_selected_card(nullptr);
                                
                    //             Attack_Value = 0;
                    //             Defense_Value = 0;

                    //             Attack_Locked = false;
                    //             Defense_Locked = false;
                    //         }
                            
                    //         catch (const std::exception &e)
                    //         {
                    //             RF.get_msg().push_back(e.what());
                    //             // std::cerr << e.what() << '\n';
                    //         }}
                    //         cout << "11\n";

                    //         turn->add_count();

                    //     } 
                    //     Attacker->set_Movement((Attacker->get_Movement() - 1 >= 0 ) ? Attacker->get_Movement() - 1 : 0);
                    // }

                    //     // exeption ...

                    //     break;

    //                 case 1: // Maneuver
                        // if (turn->get_count() < 2)
                        // {
                        //       Heroes* selected = nullptr;
                        //     RF.Attakcer_Heroes_Menu(turn, &board, selected);

                        //     if (selected != nullptr)
                        //     {
                        //         Heroes *cardHolder = turn->get_character(); 

                        //         if (cardHolder->DrawnCard() == 0)
                        //             cardHolder->Damage(2);

                        //         int extraMove = 0;

                        //         if (!cardHolder->get_hand().empty() && RF.AskBurnCardForMove(selected, &board))
                        //         {
                        //             Card *burned = RF.ChooseCardFromHand(turn, &p1, &p2, &board);
                        //             extraMove = burned->get_Boost();
                        //             cardHolder->Discard_Card(burned);
                        //         }

                        //         RF.MoveHero(selected, &board, selected->get_Movement() + extraMove, &p1, &p2);

                        //         selected->set_Movement((selected->get_Movement() - 1 >= 0) ? selected->get_Movement() - 1 : 0);
                        //     }
                        //     turn->add_count();
                        // }

                        // // exeption ...
                        // break;

                    
    //                     case 2: // Event
    //                 {
    //                     if (turn->get_count() < 2)
    //                     {
    //                         int temp1, temp2 ;
    //                         bool actionUsed = false; // only true if an Event card was actually drawn & played

    //                         try{
    //                                 Card *selected_Card = nullptr;
    //                                 Heroes * selectedHero = nullptr;
        
    //                                 RF.Attakcer_Heroes_Menu(turn, &board, selectedHero);
        
    //                                 if(selectedHero != nullptr)
    //                                 {
    //                                     bool cardDrawn = RF.Event_Selected_Card(selectedHero, turn, &p1, &p2, &board, selected_Card);

    //                                     if(cardDrawn && selected_Card != nullptr)
    //                                     {
    //                                         card_resolver.excute(selected_Card, &p1, &p2, selectedHero, nullptr, &board, temp1, temp2, Attack_Locked, Defense_Locked);

    //                                         selectedHero->Discard_Card(selected_Card);
    //                                         selected_Card->set_user_card(nullptr);

    //                                         actionUsed = true;
    //                                     }
    //                                 }

    //                                 turn->set_selected_card(nullptr);

    //                             }
    //                             catch (const std::exception &e)
    //                             {
    //                                 RF.get_msg().push_back(e.what());
    //                                 // std::cerr << e.what() << '\n';
    //                             }

    //                         if (actionUsed)
    //                             turn->add_count();
    //                     }

    //                     // exeption ...
    //                     break;
    //                 }

    //                 case 3: // Back
    //                 {
    //                     Exit = true;
    //                     break;
    //                 }

    //             }



    //                       if (Exit)
    //                 {
    //                     Exit = false;
    //                     break;
    //                 }

    //                 if (turn->get_count() >= 2)
    //                 {
    //                     turn->set_count(0);
    //                     chane_turn();
    //                     round++;
    //                 }
                    
    //                 round++;    
                    
    //             }
    //     }
    //     break;

    //     case e_Menu::Help:
    //     {
    //         RF.Show_Help();
    //     }
    //     break;

    //     case e_Menu::Exit:
    //     {
    //         Exit = true;
    //     }
    //     break;
    //     }

    //     if (Exit)
    //     {
    //         Exit = false;
    //         break;
    //     }
    // }


    void Controller::GameLoop()
{
    while (true)
    {
        if (WindowShouldClose())
        {
            Exit = true;
            break;
        }
        
        if (check_winner(&dracula, &p1, &p2))
        {
            RF.DeclareWinner(&dracula);
            Exit = true;
            break;
        }
        else if (check_winner(&sherlock, &p1, &p2))
        {
            RF.DeclareWinner(&sherlock);
            Exit = true;
            break;
        }
        else if (check_winner(&invisibleMan, &p1, &p2))
        {
            RF.DeclareWinner(&invisibleMan);
            Exit = true;
            break;
        }

        if (turn->get_count() == 0 && turn->get_character()->get_name() == "DRACULA")
        {
            try
            {
                dracula.abiliti(&board);
            }
            catch (const std::exception &e)
            {
                RF.get_msg().push_back(e.what());
            }
        }

        if (turn->get_character()->get_name() == "InvisibleMan")
        {
            if (turn->get_character()->get_PendingPlacement())
            {
                RF.PlaceHeroOnBoard(turn->get_character(), &board);

                if (turn->get_character()->get_place() != nullptr)
                    turn->get_character()->set_PendingPlacement(false);
            }
            else if (turn->get_count() == 0)
            {
                bool onFog = (turn->get_character()->get_place() != nullptr &&
                              turn->get_character()->get_place()->get_Fog() != nullptr);

                turn->get_character()->set_StartedTurnOnFog(onFog);
            }
        }

        RF.main_map(&p1, &p2, &board, turn);

        switch (RF.get_number_of_choose())
        {

        case 0: // Attack
        {
            if (turn->get_count() < 2)
                {
                    Defender = nullptr;

                    try
                    {

                        if (!RF.Attakcer_Heroes_Menu(turn, &board, Attacker) || Attacker == nullptr)
                            break; 
                        cout << "1\n";
                        
                        if (!RF.Defender_Heroes_Menu(not_turn, &board, Defender, Attacker) || Defender == nullptr)
                            break; 
                        cout << "2\n";
                        
                        if (!RF.Attacker_selected_card(Attacker, Defender, &p1, &p2, &board, Attacker_selected_card) || Attacker_selected_card == nullptr)
                            break; 
                        cout << "3\n";

                        if (!RF.Defender_selected_card(Attacker, Defender, &p1, &p2, &board, Defender_selected_card))
                            break; 
                        cout << "4\n";
                        
                        RF.Reveal_Combat(Attacker, Defender, Attacker_selected_card, Defender_selected_card); // show tow v&s cards
                        cout << "5\n";

                        Attack_Value = Attacker_selected_card->get_amount();
                        
                        if(Defender_selected_card == nullptr) {Defense_Value = 0;}
                        else
                            Defense_Value = Defender_selected_card->get_amount();


                        if (Defender->get_name() == "InvisibleMan" && Defender->get_place() != nullptr && Defender->get_place()->get_Fog() != nullptr)
                            {
                                Defense_Value += 1;
                            }

                    }
                    catch (const std::exception &e)
                    {
                        RF.get_msg().push_back(e.what());
                        // cout << e.what() << endl;
                    }

                    // ------------------------ start combat -----------------------------------.
                    
                    if(Attacker_selected_card != nullptr){
                    try
                    {

                        // -------------------------- Befor Calculation --------------------------.
                        cout << "6\n";
                        
                        if (Defender_selected_card != nullptr && Defender_selected_card->get_CardTiming() == CardTiming::Before)
                        {
                            card_resolver.excute(Defender_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                        }

                        if (Attacker_selected_card->get_CardTiming() == CardTiming::Before)
                        {
                            card_resolver.excute(Attacker_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                        }
                        
                        
                        // -------------------------- During Calculation --------------------------.
                        cout << "7\n";

                        
                        if (Defender_selected_card != nullptr && Defender_selected_card->get_CardTiming() == CardTiming::During)
                        {
                            card_resolver.excute(Defender_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                        }
                        
                        if (Attacker_selected_card != nullptr && Attacker_selected_card->get_CardTiming() == CardTiming::During)
                        {
                            card_resolver.excute(Attacker_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                        }
                        
                        
                        
                        // --------------------------- Damage Calculation ------------------------.
                        
                        cout << "8\n";
                        
                        int Damage = Attack_Value - Defense_Value;
                        
                        if (Damage >= 1)
                        Defender->Damage(Damage);
                        
                        // Heroes *Winner = (Attack_Value > Defense_Value) ? Attacker : Defender;
                        
                        //---------------------------- After Calculation ---------------------------.
                        cout << "9\n";
                        
                        if (Defender_selected_card != nullptr && Defender_selected_card->get_CardTiming() == CardTiming::After)
                        {
                            card_resolver.excute(Defender_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                        }

                        if (Attacker_selected_card != nullptr && Attacker_selected_card->get_CardTiming() == CardTiming::After)
                        {
                            card_resolver.excute(Attacker_selected_card, &p1, &p2, Attacker, Defender, &board, Attack_Value, Defense_Value, Attack_Locked, Defense_Locked);
                        }
                        
                        
                       // ------------------------- Transfer Cards to discard --------------------------.
                            cout << "10\n";

                    auto isOwnerOf = [](Player &p, Heroes *hero)
                    {
                        if (p.get_character() == hero)
                            return true;

                        for (Heroes *c : p.get_comrade())
                            if (c == hero)
                                return true;

                        return false;
                    };

                    Player *Attacker_Player = isOwnerOf(p1, Attacker) ? &p1 : &p2;
                    Player *Defender_Player = isOwnerOf(p1, Defender) ? &p1 : &p2;
                    
                    if (Attacker_selected_card != nullptr)
                    {
                        Attacker_Player->get_character()->Discard_Card(Attacker_selected_card);
                        Attacker_selected_card->set_user_card(nullptr);
                    }

                    if (Defender_selected_card != nullptr)
                    {
                        Defender_Player->get_character()->Discard_Card(Defender_selected_card);
                        Defender_selected_card->set_user_card(nullptr);
                    }

                    Attacker_selected_card = nullptr;
                    Defender_selected_card = nullptr;

                    Attacker_Player->set_selected_card(nullptr);
                    Defender_Player->set_selected_card(nullptr);

                    Attack_Value = 0;
                    Defense_Value = 0;

                    Attack_Locked = false;
                    Defense_Locked = false;
                                        }
                    
                    catch (const std::exception &e)
                    {
                        RF.get_msg().push_back(e.what());
                        // std::cerr << e.what() << '\n';
                    }}
                    cout << "11\n";

                    turn->add_count();

                } 
            }

    // exeption ...

        
        break;

        case 1: // Maneuver
        {
            if (turn->get_count() < 2)
            {
                try
                {
                    Heroes* selected = nullptr;
                    RF.Attakcer_Heroes_Menu(turn, &board, selected);

                    if (selected != nullptr)
                    {
                        Heroes *cardHolder = turn->get_character(); 

                        if (cardHolder->DrawnCard() == 0)
                            cardHolder->Damage(2);

                        int extraMove = 0;

                        if (!cardHolder->get_hand().empty() && RF.AskBurnCardForMove(selected, &board))
                        {
                            Card *burned = RF.ChooseCardFromHand(turn, &p1, &p2, &board);
                            extraMove = burned->get_Boost();
                            cardHolder->Discard_Card(burned);
                        }

                        RF.MoveHero(selected, &board, selected->get_Movement() + extraMove, &p1, &p2);

                        turn->add_count();
                    }
                }
                catch (const std::exception &e)
                {
                    RF.get_msg().push_back(e.what());
                }
            }

                // exeption ...
        }
            break;

        case 2: // Event
        {
            if (turn->get_count() < 2)
            {
                int temp1, temp2;
                bool actionUsed = false;

                try{
                        Card *selected_Card = nullptr;
                        Heroes * selectedHero = nullptr;

                        RF.Attakcer_Heroes_Menu(turn, &board, selectedHero);

                        if(selectedHero != nullptr)
                        {
                            bool cardDrawn = RF.Event_Selected_Card(selectedHero, turn, &p1, &p2, &board, selected_Card);

                            if(cardDrawn && selected_Card != nullptr)
                            {
                         
                                turn->get_character()->Discard_Card(selected_Card);

                                Card &playedCard = turn->get_character()->get_discard().back();

                                card_resolver.excute(&playedCard, &p1, &p2, selectedHero, nullptr, &board, temp1, temp2, Attack_Locked, Defense_Locked);

                                playedCard.set_user_card(nullptr);

                                actionUsed = true;
                            }
                        }

                        turn->set_selected_card(nullptr);

                    }
                    catch (const std::exception &e)
                    {
                        RF.get_msg().push_back(e.what());
                        // std::cerr << e.what() << '\n';
                    }

                if (actionUsed)
                    turn->add_count();
            }
        }
        break;

        case 3: // Save 
        {
            std::vector<std::string> labels = {
                GetSaveSummary(1),
                GetSaveSummary(2),
                GetSaveSummary(3)};

            int chosen = RF.SlotMenu(labels, "Save Game");

            if (chosen != -1)
            {
                if (SaveGame(chosen + 1))
                    RF.get_msg().push_back("Game saved to slot " + std::to_string(chosen + 1) + ".");
                else
                    RF.get_msg().push_back("Failed to save the game.");
            }
        }
        break;

        case 4: // Back (was case 3)
        {
            Exit = true;
        }
        break;
        }

        if (Exit)
        {
            Exit = false;
            break;
        }

        if (turn->get_count() >= 2)
        {
            turn->set_count(0);
            chane_turn();
            round++;
        }

        // round++;
    }
}
