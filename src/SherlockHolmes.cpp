
#include "SherlockHolmes.h"
#include <random>
using namespace std;

SherlockHolmes::SherlockHolmes()
    : Heroes("SHERLOCKHOLMES", "MELEE", 16, 2, 0)
{
    deck.push_back(Card(CardType::Administer_Aid, CardTiming::Immediate, "Event", 0, 2, "DR_WATSON"));
    deck.push_back(Card(CardType::Administer_Aid, CardTiming::Immediate, "Event", 0, 2, "DR_WATSON"));

    // deck.push_back(Card(CardType::Confirm_Suspicion, CardTiming::Immediate, "Event", 0, 1));
    // deck.push_back(Card(CardType::Confirm_Suspicion, CardTiming::Immediate, "Event", 0, 1));
    // deck.push_back(Card(CardType::Confirm_Suspicion, CardTiming::Immediate, "Event", 0, 1));

    deck.push_back(Card(CardType::Counterpunch, CardTiming::After, "Both", 3, 1, "SHERLOCKHOLMES"));
    deck.push_back(Card(CardType::Counterpunch, CardTiming::After, "Both", 3, 1, "SHERLOCKHOLMES"));
    deck.push_back(Card(CardType::Counterpunch, CardTiming::After, "Both", 3, 1, "SHERLOCKHOLMES"));

    deck.push_back(Card(CardType::Deduce_Strategy, CardTiming::During, "Both", 3, 1, "SHERLOCKHOLMES"));
    deck.push_back(Card(CardType::Deduce_Strategy, CardTiming::During, "Both", 3, 1, "SHERLOCKHOLMES"));
    deck.push_back(Card(CardType::Deduce_Strategy, CardTiming::During, "Both", 3, 1, "SHERLOCKHOLMES"));

    deck.push_back(Card(CardType::Education_Never_Ends, CardTiming::After, "Both", 3, 1, "ANY"));
    deck.push_back(Card(CardType::Education_Never_Ends, CardTiming::After, "Both", 3, 1, "ANY"));

    // deck.push_back(Card(CardType::Elementary, CardTiming::During, "Defense", 3, 3));
    // deck.push_back(Card(CardType::Elementary, CardTiming::During, "Defense", 3, 3));

    deck.push_back(Card(CardType::Eliminate_The_Impossible, CardTiming::Immediate, "Event", 0, 2, "SHERLOCKHOLMES"));
    deck.push_back(Card(CardType::Eliminate_The_Impossible, CardTiming::Immediate, "Event", 0, 2, "SHERLOCKHOLMES"));

    deck.push_back(Card(CardType::Feint, CardTiming::Before, "Both", 2, 1, "ANY"));
    deck.push_back(Card(CardType::Feint, CardTiming::Before, "Both", 2, 1, "ANY"));
    deck.push_back(Card(CardType::Feint, CardTiming::Before, "Both", 2, 1, "ANY"));

    deck.push_back(Card(CardType::Fixed_Point_in_a_Changing_Age, CardTiming::After, "Both", 3, 1, "DR_WATSON"));
    deck.push_back(Card(CardType::Fixed_Point_in_a_Changing_Age, CardTiming::After, "Both", 3, 1, "DR_WATSON"));

    deck.push_back(Card(CardType::Master_of_Disguise, CardTiming::Immediate, "Event", 0, 2, "SHERLOCKHOLMES"));
    deck.push_back(Card(CardType::Master_of_Disguise, CardTiming::Immediate, "Event", 0, 2, "SHERLOCKHOLMES"));

    deck.push_back(Card(CardType::The_Game_Is_Afoot, CardTiming::After, "Attack", 5, 2, "SHERLOCKHOLMES"));
    deck.push_back(Card(CardType::The_Game_Is_Afoot, CardTiming::After, "Attack", 5, 2, "SHERLOCKHOLMES"));

    deck.push_back(Card(CardType::Service_Revolver, CardTiming::Immediate, "Attack", 5, 2, "DR_WATSON"));
    deck.push_back(Card(CardType::Service_Revolver, CardTiming::Immediate, "Attack", 5, 2, "DR_WATSON"));

    deck.push_back(Card(CardType::Study_Methods, CardTiming::After, "Both", 3, 2, "ANY"));
    deck.push_back(Card(CardType::Study_Methods, CardTiming::After, "Both", 3, 2, "ANY"));

    std::mt19937 rng(std::random_device{}());
    std::shuffle(deck.begin(), deck.end(), rng);

    for (int i = 0; i < 5; i++)
        DrawnCard();
}

void SherlockHolmes::abiliti(Board *)
{
    // this abiliti implemented at Card Resolver class
}
