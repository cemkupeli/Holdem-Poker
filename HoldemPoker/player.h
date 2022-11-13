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

#ifndef player_h
#define player_h

#include "cards.h"
#include <vector>

// The enum that represents the moves a player can make
enum Move {CHECK=1, CALL=2, RAISE=3, FOLD=4, PASS};

// The parent class of the Human and AI classes (representing two different types of Players)
class Player
{
private:
//    bool m_eliminated;
    bool m_allIn;
    bool m_folded;
    int m_chips;
    int m_currentBet;
    int m_raisedAmount;
    Hand m_hand;
public:
    Player();
    
    bool getAllIn() const;
    void setAllIn(bool allIn=true);
    bool hasFolded() const;
    void setFolded(bool folded=true);
    int getChips() const;
    void setChips(int amount);
    int getCurrentBet() const;
    void setCurrentBet(int bet);
    Hand & getHand();
    int getRaisedAmount() const;
    void setRaisedAmount(int amount);

};

class AI: public Player
{
public:
    AI();
    static int namesCount;
    string name;
    Move decideMove(int pot, int highestBet, int gameState);
};



#endif
