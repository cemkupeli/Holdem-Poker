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
