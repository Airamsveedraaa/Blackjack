//
// Created by Airam on 27/09/2025.
//
#include "../include/Dealer.h"

Dealer::Dealer():Player("Dealer") {}

bool Dealer::shouldHit() const {
    return getHandValue() < 17;
}

void Dealer::showHand(bool hideFirstCard) const {

    cout << name << "'s hand: " << endl;

    for (size_t i=0; i<hand.size(); i++) {
        if (i==0 && hideFirstCard) {
            cout << "[HIDDEN]" << endl;
        }
        else {
            cout << hand[i].toString() << endl;
        }
    }

    if (!hideFirstCard) {
        cout << "Total:" << getHandValue() << endl;
    }
    cout << endl;
}


