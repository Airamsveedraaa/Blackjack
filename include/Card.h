//
// Created by Airam on 27/09/2025.
//

#ifndef BLACKJACK_CARD_H
#define BLACKJACK_CARD_H

using namespace std;

enum class Suit{Hearts,Spades,Clubs,Diamonds};
enum class Rank{TWO=2,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,TEN,JACK=10,QUEEN=10,KING=10,ACE=11};

class Card {
private:
    Suit s;
    Rank r;

public:
    Card(Suit s, Rank r); //constructor
    Suit getsuit() const; //returns suit of the card
    Rank getrank() const; //returns rank of the card

};


#endif //BLACKJACK_CARD_H