//
// Created by Airam on 27/09/2025.
//

#include "../include/Player.h"

Player::Player(char n[20]) {
    strcpy(Name,n);
    bet=0;
}

int Player::getHandValue() const {

    int aces=0;
    int value=0;

    for (auto &card  : hand) {
        value+=card.value;
        if (card.rank=='A') {
            aces++;
        }
    }

    while (value > 21 && aces > 0) {
        value-=10;
        aces--;
    }

    return value;
}


void Player::showHand(bool showAll) const {
    showAll=true;

    cout << Name << "'s hand: " << endl;
    for (size_t i = 0; i < hand.size(); i++) {
        if (!showAll & i==0) {
            cout << "[?]" << endl;
        }
        else {
            cout << hand[i].toString() << " " <<  endl;
        }
    }

    cout << "(" << getHandValue() << ")\n" << endl;
}
