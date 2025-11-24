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
            suitStr = "\u2665";
            break;
        case Suit::Spades:
            suitStr = "\u2660";
            break;
        case Suit::Clubs:
            suitStr = "\u2663";
            break;
        case Suit::Diamonds:
            suitStr = "\u2666";
            break;
    }

    return rankStr + suitStr;
}


string Card::getCardString() const {

    switch (r) {
        case Rank::TWO:   return "2";
        case Rank::THREE: return "3";
        case Rank::FOUR:  return "4";
        case Rank::FIVE:  return "5";
        case Rank::SIX:   return "6";
        case Rank::SEVEN: return "7";
        case Rank::EIGHT: return "8";
        case Rank::NINE:  return "9";
        case Rank::TEN:   return "10";
        case Rank::JACK:  return "J";
        case Rank::QUEEN: return "Q";
        case Rank::KING:  return "K";
        case Rank::ACE:   return "A";
        default: return "?";
    }

}


string Card::getSuitString() const {

    switch (s) {
        case Suit::Hearts:   return "\u2665";  // ♥
        case Suit::Spades:   return "\u2660";  // ♠
        case Suit::Clubs:    return "\u2663";  // ♣
        case Suit::Diamonds: return "\u2666";  // ♦
        default: return "?";
    }

}