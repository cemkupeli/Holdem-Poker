/*
 MIT License

 Copyright (c) [2022] [Cem Kupeli]

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

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
