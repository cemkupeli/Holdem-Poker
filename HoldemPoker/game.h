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

#ifndef game_h
#define game_h

#include "cards.h"
#include "player.h"
#include "checker.h"

#include <vector>
#include <time.h>
#include <fstream>

// The enum that represents which stage of the community card reveal it is
enum Stage {FLOP, TURN, RIVER, ALL};

class Game
{
private:
    Player *m_human;
    vector<AI *> m_computers;
    int numPlayers;
    int m_pot;
    int m_highestBet;
    vector<Player *> winners;
    Community m_community;
    Checker handChecker;
    struct timespec interval;
    
    const int INITIAL_BETTING = 0;
    const int CALLING_RAISES = 1;
    
    void playHand();
    bool playBettingRound();
    void humanTurn(const int turn);
    void AITurns(const int turn);
    Move promptPlayer(const int turn);
    bool checkMove(Move humanMove);
    void performMove(Move move, Player *currentPlayer, bool AIMove, AI *AIPlayer=nullptr);
    void presentTableInfo();
    void receiveMove(Move &humanMove, const int turn);
    bool handOver();
    bool oneUnfolded();
    bool tableAllIn();
    void evaluateHands();
    void distributeChips();
    bool gameOver();
    void dealCommunityCards();
    void revealCommunityCards(Stage stage);
    void eliminatePlayers();
    void resetTable();
    void printAIMove(Move move, AI *computer);
    void promptSaveGame();
    void createSaveFile(string name);
    bool readSaveFile(string name, int &humanChips, int &numAIs, vector<int> &AIChips);
    void sleep(long nanoseconds=5e8);
    void printSeparator();
public:
    Game();
    ~Game();
    void playGame();
    
};

#endif
