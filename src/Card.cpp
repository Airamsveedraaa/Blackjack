//
// Created by Airam on 27/09/2025.
//
#include "Card.h"


Card::Card(Suit s, Rank r) {
    this->s=s;
    this->r=r;
}

Suit Card::getsuit() const {
    return this->s;
}

Rank Card::getrank() const {
    return this->r;
}

int Card::getcard() const {
    return static_cast<int> (r);
}

string Card::toString() const {
    string rankStr;
    switch (r) {
        case Rank::TWO:   '2'; break;
        case Rank::THREE: '3'; break;
        case Rank::FOUR:  '4'; break;
        case Rank::FIVE:  '5'; break;
        case Rank::SIX:   '6'; break;
        case Rank::SEVEN: '7'; break;
        case Rank::EIGHT: '8'; break;
        case Rank::NINE:  '9'; break;
        case Rank::TEN:   '10'; break;
        case Rank::JACK:
        case Rank::QUEEN:
        case Rank::KING:  '10'; break;
        case Rank::ACE:   '11'; break;
    }

    string suitStr;
    switch (s) {
        case Suit::Hearts:
            suitStr = "♥";
            break;
        case Suit::Spades:
            suitStr = "♠";
            break;
        case Suit::Clubs:
            suitStr = "♣";
            break;
        case Suit::Diamonds:
            suitStr = "♦";
            break;
    }

    return rankStr + suitStr;
}
