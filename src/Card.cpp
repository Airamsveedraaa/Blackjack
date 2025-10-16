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
        case Rank::TWO: rankStr=  '2'; break;
        case Rank::THREE: rankStr= '3'; break;
        case Rank::FOUR: rankStr= '4'; break;
        case Rank::FIVE: rankStr= '5'; break;
        case Rank::SIX:  rankStr= '6'; break;
        case Rank::SEVEN: rankStr= '7'; break;
        case Rank::EIGHT: rankStr= '8'; break;
        case Rank::NINE: rankStr= '9'; break;
        case Rank::TEN:  rankStr= '10'; break;
        case Rank::JACK: rankStr= 'J'; break;
        case Rank::QUEEN: rankStr= 'Q'; break;
        case Rank::KING: rankStr= 'K'; break;
        case Rank::ACE:  rankStr= 'A'; break;
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
