//
// Created by Airam on 27/09/2025.
//

#ifndef BLACKJACK_PLAYER_H
#define BLACKJACK_PLAYER_H
#include "Card.h"
#include "Comun.h"

class Player {

private:
    char Name[20];
    vector<Card> hand;
    int bet;
public:

    Player(char n[20]);
    int getHandValue() const;
    void hit();
    void showHand(bool showAll) const;
};


#endif //BLACKJACK_PLAYER_H