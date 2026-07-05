
#include "Dracula.h"
#include <random>
using namespace std;

Dracula::Dracula()
    : Heroes("DRACULA", "MELEE", 13, 2)
{
  deck.push_back(Card(CardType::Feedingfrenzy, CardTiming::During, "Attack", 2, 3));
  deck.push_back(Card(CardType::Feedingfrenzy, CardTiming::During, "Attack", 2, 3));

  deck.push_back(Card(CardType::Mistform, CardTiming::Immediate, "Event", 0, 2));
  deck.push_back(Card(CardType::Mistform, CardTiming::Immediate, "Event", 0, 2));

  deck.push_back(Card(CardType::Ambush, CardTiming::During, "Attack", 2, 3));
  deck.push_back(Card(CardType::Ambush, CardTiming::During, "Attack", 2, 3));

  deck.push_back(Card(CardType::Baptism_of_blood, CardTiming::Immediate, "Event", 0, 2));
  deck.push_back(Card(CardType::Baptism_of_blood, CardTiming::Immediate, "Event", 0, 2));

  deck.push_back(Card(CardType::Beastform, CardTiming::During, "Attack", 6, 4));
  deck.push_back(Card(CardType::Beastform, CardTiming::During, "Attack", 6, 4));

  deck.push_back(Card(CardType::Dash, CardTiming::After, "Both", 3, 1));
  deck.push_back(Card(CardType::Dash, CardTiming::After, "Both", 3, 1));
  deck.push_back(Card(CardType::Dash, CardTiming::After, "Both", 3, 1));

  deck.push_back(Card(CardType::Exploit, CardTiming::After, "Both", 4, 1));
  deck.push_back(Card(CardType::Exploit, CardTiming::After, "Both", 4, 1));
  deck.push_back(Card(CardType::Exploit, CardTiming::After, "Both", 4, 1));

  deck.push_back(Card(CardType::Look_into_my_eyes, CardTiming::During, "Defense", 1, 2));
  deck.push_back(Card(CardType::Look_into_my_eyes, CardTiming::During, "Defense", 1, 2));
  deck.push_back(Card(CardType::Look_into_my_eyes, CardTiming::During, "Defense", 1, 2));

  deck.push_back(Card(CardType::Prey_upon, CardTiming::Immediate, "Event", 0, 4));
  deck.push_back(Card(CardType::Prey_upon, CardTiming::Immediate, "Event", 0, 4));

  deck.push_back(Card(CardType::Ravening_Seduction, CardTiming::Immediate, "Event", 0, 2));
  deck.push_back(Card(CardType::Ravening_Seduction, CardTiming::Immediate, "Event", 0, 2));
  deck.push_back(Card(CardType::Ravening_Seduction, CardTiming::Immediate, "Event", 0, 2));

  deck.push_back(Card(CardType::Thirst_for_sustenance, CardTiming::After, "Attack", 3, 3));
  deck.push_back(Card(CardType::Thirst_for_sustenance, CardTiming::After, "Attack", 3, 3));
  deck.push_back(Card(CardType::Thirst_for_sustenance, CardTiming::After, "Attack", 3, 3));

  deck.push_back(Card(CardType::Feint, CardTiming::Before, "Both", 2, 2));
  deck.push_back(Card(CardType::Feint, CardTiming::Before, "Both", 2, 2));
  deck.push_back(Card(CardType::Feint, CardTiming::Before, "Both", 2, 2));
}

void Dracula::set_sisters(Sisters &s1, Sisters &s2, Sisters &s3)
{
  sisters[0] = &s1;
  sisters[1] = &s2;
  sisters[2] = &s3;
}

vector<Sisters *> Dracula::get_sisters()
{
  return sisters;
}
