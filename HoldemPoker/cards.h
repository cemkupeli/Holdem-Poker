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

#ifndef cards_h
#define cards_h

#include <ctime>
#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

enum Suit {CLUBS, DIAMONDS, HEARTS, SPADES};
enum Rank {ACE=1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};

// Define copy constructors

class Card
{
// Change to private after done with testing
private:
    Suit m_suit;
    Rank m_rank;
public:
    Card();
    Card(const Card &);
    void operator = (const Card  & cardObj) noexcept;
    
    Suit getSuit() const;
    Rank getRank() const;
    void setSuit(Suit suit);
    void setRank(Rank rank);
};

// External functions related to the Card class
ostream & operator << (ostream & out, Card & card);

class Hand
{
// Change to private after done with testing
private:
    Card m_card1;
    Card m_card2;
public:
    Hand();
    Hand(const Hand &);
    
    Card getCard1() const;
    Card getCard2() const;
    
    friend class Checker;
};

// External functions related to the Hand class
ostream & operator << (ostream &, Hand &);

class Community
{
// Change to private after done with testing
private:
    // The fourth card that is revealed
    Card m_turn;
    // The fifth card that is revealed
    Card m_river;
    // The first three cards that are revealed
    Card m_flop[3];
public:
    Community();
    Community(const Community &);
    
    friend class Game;
    friend class Checker;
};

// External functions related to the Community class


#endif
