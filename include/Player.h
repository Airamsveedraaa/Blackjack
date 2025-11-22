//
// Created by Airam on 27/09/2025.
//

#ifndef BLACKJACK_PLAYER_H
#define BLACKJACK_PLAYER_H
#include "Card.h"
#include "Comun.h"

class Player {

protected:
    char* name;
    vector<Card> hand;
    int bet;
public:
    size_t getHandSize() {return hand.size();};
    Player(const char* Name);
    ~Player();
    int getHandValue() const;
    void hit();
    void showHand(bool showAll=true) const;
    void clearHand();
    void addCard(Card card);
};


#endif //BLACKJACK_PLAYER_H