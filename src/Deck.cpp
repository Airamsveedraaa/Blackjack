//
// Created by Airam on 27/09/2025.
//

#include "../include/Deck.h"

Deck::Deck(){

    cards.reserve(52);
    for (int s=(int) Suit::Hearts; s<=(int)Suit::Diamonds;s++) {
        for (int r=(int)Rank::TWO;r<=(int)Rank::ACE;r++) {
            cards.emplace_back(static_cast<Suit>(s), static_cast<Rank>(r));
        }
    }

    curr_indx=0;

}


void Deck::shuffle() {
    random_device rd;
    mt19937 g(rd());
    std::shuffle(cards.begin(),cards.end(),g);
    curr_indx=0;
}


Card Deck::dealCard() {

    if (curr_indx< (int)cards.size()) {
        return cards[curr_indx++];
    }throw out_of_range("No more cards in Deck");
}


bool Deck::isEmpty() const {

    return curr_indx >= (int)cards.size();

}