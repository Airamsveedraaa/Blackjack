//
// Created by Airam on 27/09/2025.
//

#ifndef BLACKJACK_DEALER_H
#define BLACKJACK_DEALER_H
#include "Comun.h"
#include "Player.h"

class Dealer :public Player{
public:
    Dealer();
    bool shouldHit()const;
    void showHand(bool hideFirstCard=true)const;
};


#endif //BLACKJACK_DEALER_H