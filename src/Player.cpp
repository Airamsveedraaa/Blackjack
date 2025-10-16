//
// Created by Airam on 27/09/2025.
//

#include "../include/Player.h"

Player::Player(const char* name) {
    name=new char[strlen(name)+1];
    strcpy(this->name,name);
    bet=0;
}

Player::~Player() {
    delete[] name;
}

void Player::addCard(Card card) {
    hand.push_back(card);
}

void Player::clearHand() {
    hand.clear();
}

int Player::getHandValue() const {

    int aces=0;
    int value=0;

    for (auto &card  : hand) {
        Rank cardRank=card.getrank();
        if (cardRank==Rank::ACE) {
            value+=11;
            aces++;
        }
        else if (cardRank>= Rank::JACK && cardRank<= Rank::KING) {
            value+=10;
        }
        else {
            value+= static_cast<int>(cardRank);
        }
    }

    while (value > 21 && aces > 0) {
        value-=10;
        aces--;
    }

    return value;
}


void Player::showHand(bool showAll) const {

    cout << name << "'s hand: " << endl;
    for (size_t i = 0; i < hand.size(); i++) {
        if (!showAll && i==0) {
            cout << "[HIDDEN]" << endl;
        }
        else {
            cout << hand[i].toString() << " " <<  endl;
        }
    }

    cout << "(" << getHandValue() << ")\n" << endl;
}
