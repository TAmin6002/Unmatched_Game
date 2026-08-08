#include "InvisibleMan.h"
#include "Card.h"


using namespace std;


InvisibleMan::InvisibleMan() :Heroes("InvisibleMan", "MELEE", 15, 2, 0){

deck.push_back(Card(CardType::Coded_Notes, CardTiming::After, "Defense", 3, 2, "InvisibleMan"));
deck.push_back(Card(CardType::Coded_Notes, CardTiming::After, "Defense", 3, 2, "InvisibleMan"));

deck.push_back(Card(CardType::Confound, CardTiming::After, "Both", 3, 2, "InvisibleMan"));
deck.push_back(Card(CardType::Confound, CardTiming::After, "Both", 3, 2, "InvisibleMan"));

deck.push_back(Card(CardType::Covert_Preparation, CardTiming::After, "Both", 2, 1, "InvisibleMan"));
deck.push_back(Card(CardType::Covert_Preparation, CardTiming::After, "Both", 2, 1, "InvisibleMan"));
deck.push_back(Card(CardType::Covert_Preparation, CardTiming::After, "Both", 2, 1, "InvisibleMan"));

deck.push_back(Card(CardType::Dreaming_of_Revenge, CardTiming::After, "Both", 3, 1, "InvisibleMan"));
deck.push_back(Card(CardType::Dreaming_of_Revenge, CardTiming::After, "Both", 3, 1, "InvisibleMan"));

deck.push_back(Card(CardType::Emerge_From_Mist, CardTiming::During, "Attack", 3, 2, "InvisibleMan"));
deck.push_back(Card(CardType::Emerge_From_Mist, CardTiming::During, "Attack", 3, 2, "InvisibleMan"));

deck.push_back(Card(CardType::Impossible_to_See, CardTiming::Before, "Both", 2, 2, "InvisibleMan"));
deck.push_back(Card(CardType::Impossible_to_See, CardTiming::Before, "Both", 2, 2, "InvisibleMan"));

deck.push_back(Card(CardType::Into_Thin_Air, CardTiming::After, "Defense", 4, 1, "InvisibleMan"));
deck.push_back(Card(CardType::Into_Thin_Air, CardTiming::After, "Defense", 4, 1, "InvisibleMan"));

deck.push_back(Card(CardType::Lurking, CardTiming::After, "Defense", 2, 2, "InvisibleMan"));
deck.push_back(Card(CardType::Lurking, CardTiming::After, "Defense", 2, 2, "InvisibleMan"));

deck.push_back(Card(CardType::Reign_Thrror, CardTiming::Immediate, "Event", 0, 1, "InvisibleMan"));
deck.push_back(Card(CardType::Reign_Thrror, CardTiming::Immediate, "Event", 0, 1, "InvisibleMan"));

deck.push_back(Card(CardType::Rolling_Fog, CardTiming::Immediate, "Event", 0, 1, "InvisibleMan"));
deck.push_back(Card(CardType::Rolling_Fog, CardTiming::Immediate, "Event", 0, 1, "InvisibleMan"));

deck.push_back(Card(CardType::Slip_Away, CardTiming::After, "Attack", 3, 2, "InvisibleMan"));
deck.push_back(Card(CardType::Slip_Away, CardTiming::After, "Attack", 3, 2, "InvisibleMan"));
deck.push_back(Card(CardType::Slip_Away, CardTiming::After, "Attack", 3, 2, "InvisibleMan"));

deck.push_back(Card(CardType::Step_Lightly, CardTiming::Immediate, "Event", 0, 1, "InvisibleMan"));
deck.push_back(Card(CardType::Step_Lightly, CardTiming::Immediate, "Event", 0, 1, "InvisibleMan"));

deck.push_back(Card(CardType::Vanish, CardTiming::Immediate, "Event", 0, 3, "InvisibleMan"));
deck.push_back(Card(CardType::Vanish, CardTiming::Immediate, "Event", 0, 3, "InvisibleMan"));



}
