//
// Created by Airam on 27/09/2025.
//

#ifndef BLACKJACK_DECK_H
#define BLACKJACK_DECK_H
#include "Card.h"
#include "Comun.h"

using namespace std;

class Deck {
    private:
    int curr_indx; //current position on the deck, initially on 0
    vector<Card> cards; //vector of cards type object

public:
    Deck(); //constructor
    void shuffle(); //shuffles the deck
    Card dealCard(); //gives card
    bool isEmpty() const; //checks if the deck is empty or not
};


#endif //BLACKJACK_DECK_H