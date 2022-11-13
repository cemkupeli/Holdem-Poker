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

#include "player.h"


// The methods of the Player class

Player::Player(): m_folded(false), m_chips(1000), m_currentBet(0)
{}

bool Player::getAllIn() const
{
    return m_allIn;
}

void Player::setAllIn(bool allIn)
{
    m_allIn = allIn;
}

bool Player::hasFolded() const
{
    return m_folded;
}

void Player::setFolded(bool folded)
{
    m_folded = folded;
}

int Player::getChips() const
{
    return m_chips;
}

void Player::setChips(int amount)
{
    m_chips = amount;
}

int Player::getCurrentBet() const
{
    return m_currentBet;
}

void Player::setCurrentBet(int bet)
{
    m_currentBet = bet;
}

Hand & Player::getHand()
{
    return m_hand;
}

int Player::getRaisedAmount() const
{
    return m_raisedAmount;
}

void Player::setRaisedAmount(int amount)
{
    m_raisedAmount = amount;
}

// The methods of the AI Class, a specialization of Player
/* The default constructor for the AI Class (static data member namesCount is initialized as 0) */
int AI::namesCount = 0;
AI::AI(): Player()
{
    string AINames[] = {"John", "Liz", "Michael", "Jane"};
    name = AINames[namesCount];
    namesCount++;
}

Move AI::decideMove(int pot, int highestBet, int gameState)
{
    Move randomMove = FOLD;
    int amountToRaise = (rand() % 4 + 1) * (rand() % 4 + 1) * 10;
    
    const bool CAN_CHECK = !(highestBet > getCurrentBet());
    const bool CAN_CALL = !CAN_CHECK;
    const bool CAN_RAISE = ((highestBet - getCurrentBet() + amountToRaise) <= getChips()) && (getCurrentBet() + amountToRaise > highestBet);
    const bool CAN_FOLD = true;
    
    vector<Move> possibleMoves;
    
    if (CAN_CHECK)
        possibleMoves.push_back(CHECK);
    if (CAN_CALL)
        possibleMoves.push_back(CALL);
    if (CAN_RAISE)
        possibleMoves.push_back(RAISE);
    if (CAN_FOLD)
        possibleMoves.push_back(FOLD);
    
    if (gameState == 0)
    {
        // Last element in vector will always be fold
        int random = rand() % 19;
        switch (possibleMoves.size())
        {
            case 2:
                if (random < 18)
                    randomMove = possibleMoves[0];
                else
                    randomMove = possibleMoves[1];
                break;
            
            case 3:
                if (random < 9)
                    randomMove = possibleMoves[0];
                else if (random < 18)
                    randomMove = possibleMoves[1];
                else
                    randomMove = possibleMoves[2];
        }
            
    }
    else if (gameState == 1)
    {
        // Can only call or fold when gameState is 1  (in the calling raises turn) and AI player's bet is less than the highest bet
        if (getCurrentBet() < highestBet)
        {
            int random = rand() % 20;
            if (random < 19)
                randomMove = CALL;
            else
                randomMove = FOLD;
        }
        // Else, sets randomMove to PASS in order to skip player
        else
        {
            randomMove = PASS;
        }
    }
    
    // Special case for RAISE
    if (randomMove == RAISE)
    {
        setRaisedAmount(getCurrentBet() + amountToRaise);
    }
    
    // Returns the random move that the AI player generates
    return randomMove;
}


