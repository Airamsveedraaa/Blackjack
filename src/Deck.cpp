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
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(),cards.end(),g);
    curr_indx=0;
}


Card Deck::dealCard() {

    if (curr_indx< (int)cards.size()) {
        return cards[curr_indx++];
    }throw std::out_of_range("No more cards in Deck");
}


bool Deck::isEmpty() const {

    return curr_indx >= (int)cards.size();

}


void Deck::stackTop(const std::vector<Card>& top) {
    //las cartas indicadas salen primero; detras va un mazo completo barajado
    //para que startRound no rebaraje (lo hace con 10 o menos cartas)
    Deck rest;
    rest.shuffle();

    cards = top;
    cards.insert(cards.end(), rest.cards.begin(), rest.cards.end());
    curr_indx=0;
}