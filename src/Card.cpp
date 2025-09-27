//
// Created by Airam on 27/09/2025.
//
#include "Card.h"


Card::Card(Suit s, Rank r) {
    this->s=s;
    this->r=r;
}

Suit Card::getsuit() const {
    return s;
}

Rank Card::getrank() const {
    return r;
}
