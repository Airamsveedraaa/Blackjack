//
// Created by Airam on 27/09/2025.
//
#include "../include/Dealer.h"

Dealer::Dealer():Player("Dealer") {}

bool Dealer::shouldHit() const {
    return getHandValue() < 17;
}

void Dealer::showHand(bool hideFirstCard) const {
    std::cout << name << "'s hand: " << std::endl;

    for (size_t i=0; i<hand.size(); i++) {
        if (i==0 && hideFirstCard) {
            std::cout << "[HIDDEN]" << std::endl;
        }
        else {
            std::cout << hand[i].toString() << std::endl;
        }
    }

    if (!hideFirstCard) {
        std::cout << "Total:" << getHandValue() << std::endl;
    }
    std::cout << std::endl;
}


