#include "cards.h"

// The string arrays containing strings that represent Suit and Card enumerations
string suitNames[] = {"Clubs", "Diamonds", "Hearts", "Spades"};
string rankNames[] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};

// The methods of the Card class
/* The default constructor for the Card class. As all cards in the game are dealt and, therefore, random, each Card object
 is initialized pseudorandomly */
Card::Card()
{
    this->m_suit = static_cast<Suit>(rand() % 4);
    this->m_rank = static_cast<Rank>(rand() % 13 + 1);
}

/* The copy constructor for the Card class */
Card::Card(const Card & card)
{
    this->m_rank = card.m_rank;
    this->m_suit = card.m_suit;
}

void Card::operator = (const Card  & cardObj) noexcept
{
    this->m_suit = cardObj.m_suit;
    this->m_rank = cardObj.m_rank;
}

/* Accessor for the suit data member */
Suit Card::getSuit() const
{
    return this->m_suit;
}

/* Accessor for the rank data member */
Rank Card::getRank() const
{
    return this->m_rank;
}

/* Mutator for the suit data member */
void Card::setSuit(Suit suit)
{
    this->m_suit = suit;
}

/* Mutator for the rank data member */
void Card::setRank(Rank rank)
{
    this->m_rank = rank;
}

/* Prints a card hand (as rank of suit) to an output stream */
ostream & operator << (ostream & out, Card & card)
{
    // 1 is subtracted as the Rank enumeration starts at 1, whereas rankNames starts at 0 since it's an array
    cout << rankNames[card.getRank() - 1] << " of " << suitNames[card.getSuit()];
    
    // Returns the ostream reference to allow multiple outputs in one statement
    return out;
}

// The methods of the Hand class
/* The default constructor for the Hand class */
Hand::Hand()
{}

/* The copy constructor for the Hand class */
Hand::Hand(const Hand & hand)
{
    this->m_card1 = hand.m_card1;
    this->m_card2 = hand.m_card2;
}

/* The accessor for the card1 data member */
Card Hand::getCard1() const
{
    return this->m_card1;
}

/* The accessor for the card2 data member */
Card Hand::getCard2() const
{
    return this->m_card2;
}

/* Prints the hand (as two cards, where each card's rank and suit are stated) to an output stream */
ostream & operator << (ostream & out, Hand & hand)
{
    cout << rankNames[hand.getCard1().getRank() - 1] << " of " << suitNames[hand.getCard1().getSuit()]
        << ", " << rankNames[hand.getCard2().getRank() - 1] << " of " << suitNames[hand.getCard2().getSuit()];
    
    // Returns the ostream reference to allow multiple outputs in one statement
    return out;
}

// The methods of the Community class
/* The default constructor for the Community class */
Community::Community()
{}

/* The copy constructor for the Community class */
Community::Community(const Community & community)
{
    const int FLOP_CARDS = 3;
    for (int i = 0; i < FLOP_CARDS; i++)
    {
        this->m_flop[i] = community.m_flop[i];
    }
    this->m_turn = community.m_turn;
    this->m_river = community.m_river;
}
