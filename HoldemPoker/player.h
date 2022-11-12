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
