
#include "Dracula.h"
#include "Ftxui_Front.h"
#include <random>
#include <algorithm>
using namespace std;

Dracula::Dracula()
    : Heroes("DRACULA", "MELEE", 13, 2, 0)
{
  deck.push_back(Card(CardType::Feedingfrenzy, CardTiming::During, "Attack", 2, 3, "DRACULA"));
  deck.push_back(Card(CardType::Feedingfrenzy, CardTiming::During, "Attack", 2, 3, "DRACULA"));

  deck.push_back(Card(CardType::Mistform, CardTiming::Immediate, "Event", 0, 2, "DRACULA"));
  deck.push_back(Card(CardType::Mistform, CardTiming::Immediate, "Event", 0, 2, "DRACULA"));

  deck.push_back(Card(CardType::Ambush, CardTiming::During, "Attack", 2, 3, "ANY"));
  deck.push_back(Card(CardType::Ambush, CardTiming::During, "Attack", 2, 3, "ANY"));

  deck.push_back(Card(CardType::Baptism_of_blood, CardTiming::Immediate, "Event", 0, 2, "DRACULA"));
  deck.push_back(Card(CardType::Baptism_of_blood, CardTiming::Immediate, "Event", 0, 2, "DRACULA"));

  deck.push_back(Card(CardType::Beastform, CardTiming::During, "Attack", 6, 4, "DRACULA"));
  deck.push_back(Card(CardType::Beastform, CardTiming::During, "Attack", 6, 4, "DRACULA"));

  deck.push_back(Card(CardType::Dash, CardTiming::After, "Both", 3, 1, "ANY"));
  deck.push_back(Card(CardType::Dash, CardTiming::After, "Both", 3, 1, "ANY"));
  deck.push_back(Card(CardType::Dash, CardTiming::After, "Both", 3, 1, "ANY"));

  deck.push_back(Card(CardType::Exploit, CardTiming::After, "Both", 4, 1, "ANY"));
  deck.push_back(Card(CardType::Exploit, CardTiming::After, "Both", 4, 1, "ANY"));
  deck.push_back(Card(CardType::Exploit, CardTiming::After, "Both", 4, 1, "ANY"));

  deck.push_back(Card(CardType::Look_into_my_eyes, CardTiming::During, "Defense", 1, 2, "DRACULA"));
  deck.push_back(Card(CardType::Look_into_my_eyes, CardTiming::During, "Defense", 1, 2, "DRACULA"));
  deck.push_back(Card(CardType::Look_into_my_eyes, CardTiming::During, "Defense", 1, 2, "DRACULA"));

  deck.push_back(Card(CardType::Prey_upon, CardTiming::Immediate, "Event", 0, 4, "DRACULA"));
  deck.push_back(Card(CardType::Prey_upon, CardTiming::Immediate, "Event", 0, 4, "DRACULA"));

  deck.push_back(Card(CardType::Ravening_Seduction, CardTiming::Immediate, "Event", 0, 2, "SISTERS"));
  deck.push_back(Card(CardType::Ravening_Seduction, CardTiming::Immediate, "Event", 0, 2, "SISTERS"));
  deck.push_back(Card(CardType::Ravening_Seduction, CardTiming::Immediate, "Event", 0, 2, "SISTERS"));
 
  deck.push_back(Card(CardType::Thirst_for_sustenance, CardTiming::After, "Attack", 3, 3, "SISTERS"));
  deck.push_back(Card(CardType::Thirst_for_sustenance, CardTiming::After, "Attack", 3, 3, "SISTERS"));
  deck.push_back(Card(CardType::Thirst_for_sustenance, CardTiming::After, "Attack", 3, 3, "SISTERS"));

  deck.push_back(Card(CardType::Feint, CardTiming::Before, "Both", 2, 2, "ANY"));
  deck.push_back(Card(CardType::Feint, CardTiming::Before, "Both", 2, 2, "ANY"));
  deck.push_back(Card(CardType::Feint, CardTiming::Before, "Both", 2, 2, "ANY"));

  std::mt19937 rng(std::random_device{}());
  std::shuffle(deck.begin(), deck.end(), rng);

  for (int i = 0; i < 5; i++)
    DrawnCard();
}

// void Dracula::set_sisters(Sisters &s1, Sisters &s2, Sisters &s3)
// {
//   sisters[0] = &s1;
//   sisters[1] = &s2;
//   sisters[2] = &s3;
// }

vector<Sisters *> Dracula::get_sisters()
{
  return sisters;
}

void Dracula::abiliti(Board *board)
{
  Ftxui_Front FF;

    if (!FF.AskUseSpecialAbility(this, board))
        return; 

    Heroes *target = FF.SelectHero(board, this);

    if (target == nullptr)
        return;

    target->Damage(1);

    if (this->DrawnCard() == 0)
        this->Damage(2);
}
