#ifndef checker_h
#define checker_h

#include "cards.h"
#include <vector>

/* The class used to evaluate hands after the betting rounds are over */
class Checker
{
private:
    Hand hand;
    Community community;
    
public:
    int checkHand() const;
    bool flush() const;
    bool flush(Suit & flushSuit) const;
    bool straight() const;
    bool straight(Rank &startingRank) const;
    bool straight(Suit flushSuit) const;
    bool straightFlush() const;
    bool royalFlush() const;
    bool fullHouse() const;
    bool numOfAKind(const int NUMBER) const;
    bool pairs(int &numPairs) const;
    string displayHandName(int score) const;
    Rank highCard() const;
    
    void test();
    
    friend class Game;
    
};

// External functions relevant to the Checker class
bool contains(const vector<int> &v, int number);
bool contains(const vector<Card> &v, int number);
bool compareRanks(Card card1, Card card2);
void sortCards(Card * const cards, int numCards);
void swapCards(Card *card1, Card *card2);

#endif
