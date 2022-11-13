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

#include "game.h"

/* The default constructor for the Game class. Asks user whether they would like to load a save file, attempts to load savefile if user
confirms, otherwise prompts user for the number of AI players and completes the necessary setup (including memory allocations for the
 players) */
Game::Game(): m_pot(0), m_highestBet(0)
{
    // The variables that will store relevant game properties if a save file is read
    int humanChips = 0;
    vector<int> AIChips;
    // num_AIs will be changed by either reading a savefile or by prompting the user (and will be used under any circumstance)
    int num_AIs = 0;
    
    cout << "Welcome to Poker++! Your table is waiting for you..." << endl << endl;
    sleep(2e8);
    
    // Asks user whether they would like to load a save file
    bool saveRead = false;
    string answer;
    do
    {
        cout << "Would you like to load a save file? (yes, no) ";
        cin >> answer;
    }
    while (!(answer == "y" || answer == "yes" || answer == "Yes" || answer == "YES" || answer == "n" || answer == "no" || answer == "No" || answer == "NO"));
    
    // Creates a save file if the user said yes
    if (answer == "y" || answer == "yes" || answer == "Yes" || answer == "YES")
    {
        string name;
        do
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter the name of the save file: ";
            cin >> name;
        }
        while (name.length() == 0 || name.length() > 15);
        
        // Reads the save file (readSaveFile returns a bool indicating whether the save file was successfully read)
        saveRead = readSaveFile(name, humanChips, num_AIs, AIChips);

    }
    // Else, constructs the game class regularly
    if (!saveRead)
    {
        // Prompts user for number of AI players
        do
        {
            cout << "How many computer players would you like to play against? (1-4) ";
            cin >> num_AIs;
        }
        while (!(1 <= num_AIs && num_AIs <= 4));
        
        cout << endl;
    }
    
    // Dynamically allocates a human player
    this->m_human = new Player;
    
    // Dynamically allocates AI players and adds them to the computers vector
    for (int i = 0; i < num_AIs; i++)
    {
        AI *AIPlayer = new AI;
        m_computers.push_back(AIPlayer);
    }
    
    // Sets number of players (AI players and one human player)
    numPlayers = num_AIs + 1;
    
    // If a save file was read, stores the data that was read into the relevant data members stored within the Game class
    if (saveRead)
    {
        // Sets the chips of the AI players
        for (int i = 0; i < num_AIs; i++)
        {
            // If the player was eliminated in the game (indicates by 0 chips), sets their pointer to nullptr
            if (AIChips[i] == 0)
            {
                m_computers[i] = nullptr;
                continue;
            }
            
            m_computers[i]->setChips(AIChips[i]);
        }
        
        // Sets the chips of the human player
        m_human->setChips(humanChips);
    }
    
    // Sets the interval length for the timespec struct, used within the sleep method
    interval.tv_sec = 0;
    interval.tv_nsec = 5e8; // 0.5 seconds
    
    cout << endl;
}

/* The destructor for the Game class, handles deallocation for any remaining dynamically allocated memory blocks */
Game::~Game()
{
    // Deallocates the memory allocated for any AI player remaining
    for (AI *computer : m_computers)
    {
        if (computer != nullptr)
        {
            delete computer;
            computer = nullptr;
        }
    }
    
    // Deallocates the memory allocated for the human player (if not already done)
    if (m_human != nullptr)
    {
        delete m_human;
        m_human = nullptr;
    }
}

/* The main Game method that allows the user to start the game and play the game until the user either wins or
 is eliminated (by running out of chips) */
void Game::playGame()
{
    while (true)
    {
        // Checks if the game is over
        if (gameOver())
            break;
        
        // Plays a hand
        playHand();
        
        // Evaluates each player's hand after the hand is over
        evaluateHands();
        
        // Distributes the chips in the pot to the winner(s)
        distributeChips();
        
        // Resets table
        resetTable();
        
        // Eliminates players that have run out of chips
        eliminatePlayers();
        
        // Asks the player whether they want to save the game
        promptSaveGame();
    }
        
}

/* Plays out a hand consisting of 1-4 betting rounds */
void Game::playHand()
{
    // The maximum number of betting rounds that can be played
    const int NUM_BETTING_ROUNDS = 4;
    
    // The stage of the community card (the cards on the table) reveal
    Stage currentStage = FLOP;
    
    // The bool that keeps track of whether the hand is over
    bool handOver = false;
    
    // Sets the community cards to random cards
    dealCommunityCards();
    
    // Reveals cards and plays betting rounds before and after each card reveal until hand is over
    for (int i = 0; i < NUM_BETTING_ROUNDS; i++)
    {
        
        // The first betting round takes place before the flop (first three cards) is revealed
        if (i == 0)
        {
            /* playBettingRound returns true if the hand is over (either one unfolded player remaining or all but one players
             have gone all-in) */
            handOver = playBettingRound();
            
            // Prints a separator
            printSeparator();
            
            continue;
        }
        
        // Reveals the community cards (for the current stage) and plays a betting round
        revealCommunityCards(currentStage);
        playBettingRound();
        
        // Prints a separator
        printSeparator();
        
        // Sets currentStage to the next stage
        if (currentStage == FLOP)
            currentStage = TURN;
        else if (currentStage == TURN)
            currentStage = RIVER;
        
        // If the hand is over before the maximum number of rounds are reached
        if (handOver)
        {
            // Prints the remaining community cards
            if (currentStage == TURN)
                revealCommunityCards(ALL);
            else
                revealCommunityCards(RIVER);
            
            // Breaks in order to move on to evaluating the hands
            break;
        }
            
    }
}

/* Plays a betting round, returns true if the hand is over by either all but one of the players folding or all but one of the
 players going all-in, else returns false */
bool Game::playBettingRound()
{
    // Checks if the hand is over, returns true if so
    if (handOver())
        return true;
    
    sleep();
    // Allows the human player to play the initial betting turn (if not folded or all-in)
    humanTurn(INITIAL_BETTING);
    
    // Allows the AI players to play the initial betting turn (if not folded or all-in)
    AITurns(INITIAL_BETTING);
    
    sleep();
    // Allows the human player to play the calling raises turn (if not folded, all-in, or below the highest bet)
    humanTurn(CALLING_RAISES);
    
    // Allows the AI players to play the calling raises turn (if not folded, all-in, or below the highest bet)
    AITurns(CALLING_RAISES);
    
    // Returns false to indicate that the hand is not over
    return false;
}

/* Prompts the human player to make a choice in the current turn. Turns: 0 -> initial betting turn, 1 -> calling raises turn */
Move Game::promptPlayer(const int turn)
{
    /* If it is the calling raises turn and the user isn't prompted (and humanMove isn't mutated,) returning PASS
     ensures that no action is taken, which is what is expected to happen if the user is skipped over for matching
     the current highest bet with their bet */
    Move humanMove = PASS;
    
    if (turn == 0)
    {
        // Presents information about the current state of the table
        presentTableInfo();
        
        // Informs player about their possible moves and receives a move
        receiveMove(humanMove, turn);
    }
    else if (turn == 1)
    {
        // Informs player about their possible moves and receives a move (if player not skipped over)
        receiveMove(humanMove, turn);
    }
    cout << endl;
    
    // Returns the move made by the human
    return humanMove;
}

/* Plays the turn of the human player in the current game state (initial betting or calling raises) by checking whether
 they are skipped over (for having folded or gone all-in.) If they aren't, prompts them for a move and performs the move
 received by the human player. */
void Game::humanTurn(const int GAME_STATE)
{
    // The enum that will store the move made by the human player
    Move humanMove;
    
    // If the human player has folded or gone all in, notifies them and skips their turn, else prompts them for a move
    if (m_human->hasFolded())
    {
        // If it is currently the initial betting turn
        if (GAME_STATE == 0)
        {
            sleep();
            cout << endl << "You have folded." << endl;
        }
    }
    else if (m_human->getAllIn())
    {
        // If it is currently the initial betting turn
        if (GAME_STATE == 0)
        {
            sleep();
            cout << endl << "You have gone all-in." << endl;
        }
    }
    else
    {
        humanMove = promptPlayer(GAME_STATE);
        performMove(humanMove, m_human, false);
    }
}

/* Plays the turn of each AI player in the current game state (initial betting or calling raises) by checking whether
they are skipped over (for having folded or gone all-in.) If they aren't, instructs them to perform their next move */
void Game::AITurns(const int GAME_STATE)
{
    for (int i = 0; i < numPlayers - 1; i++)
    {
        // Initializes a pointer to the current AI player
        AI *computer = m_computers[i];
        
        // Checks if the player is eliminated
        if (computer == nullptr)
            return;
        
        // If the AI player has folded or gone all in, notifies the human player that this AI player's turn is skipped
        if (computer->hasFolded())
        {
            // If it is currently the initial betting turn
            if (GAME_STATE == 0)
            {
                sleep();
                cout << computer->name << " has folded." << endl;
            }
                
        }
        else if (computer->getAllIn())
        {
            // If it is currently the initial betting turn
            if (GAME_STATE == 0)
            {
                sleep();
                cout << computer->name <<" has gone all-in." << endl;
            }
                
        }
        // Else, instructs the AI player to pseudorandomly decide and perform their next move
        else
        {
            Move AIMove = computer->decideMove(m_pot, m_highestBet, GAME_STATE);
            performMove(AIMove, computer, true, computer);
        }
    }
}

/* Checks if the move that the human player has made is possible and returns the result as a bool */
bool Game::checkMove(Move humanMove)
{
    // Stores relevant data member values into local variables for easier access
    int currentChips = m_human->getChips();
    int currentBet = m_human->getCurrentBet();
    int raisedAmount = m_human->getRaisedAmount();
    int highestBet = m_highestBet;
    
    // The boolean values that will determine whether an action can be performed
    const bool CAN_CHECK = currentBet == highestBet;
    const bool CAN_CALL = !CAN_CHECK;
    const bool CAN_RAISE = (raisedAmount <= currentChips + currentBet) && (raisedAmount > highestBet);
    const bool CAN_FOLD = true;
    

    switch (humanMove)
    {
        case CHECK:
            if (CAN_CHECK)
                return true;
            else
                return false;
        case CALL:
            if (CAN_CALL)
                return true;
            else
                return false;
            
        case RAISE:
            if (CAN_RAISE)
                return true;
            else
                return false;
            
        case FOLD:
            if (CAN_FOLD)
                return true;
            else
                return false;
            
        // Cannot be reached manually by player (used for skipping during the calling raises game state)
        case PASS:
            return true;
    }
}

/* Performs a move for a player, either the human player or one of the AIPlayers (in which case AIMove takes
true as argument and AIPlayer takes a pointer to the current AI Player in order to have access to the
data members of the child class */
void Game::performMove(Move move, Player *currentPlayer, bool AIMove, AI *AIPlayer)
{
    // Stores relevant data member values into local variables for easier access
    int currentChips = currentPlayer->getChips();
    int raisedAmount = currentPlayer->getRaisedAmount();
    int currentBet = currentPlayer->getCurrentBet();
    int highestBet = m_highestBet;
    
    // Calculates the minimum additional bet that will have to be made to stay in the game
    int additionalBet = highestBet - currentBet;

    switch (move)
    {
        // Where additionalBet is zero
        case CHECK:
            break;
            
        case CALL:
            // If not enough (or exactly enough) chips to call fully, goes all-in
            if (additionalBet >= currentChips)
            {
                currentPlayer->setAllIn();
                m_pot += currentChips;
                currentPlayer->setCurrentBet(currentBet + currentChips);
                currentPlayer->setChips(0);
            }
            // Else, calls normally
            else
            {
                m_pot += additionalBet;
                currentPlayer->setCurrentBet(highestBet);
                currentPlayer->setChips(currentChips - additionalBet);
            }
            break;
            
        case RAISE:
            // For RAISE, the additional bet is the raised amount (by the player) minus the current bet
            additionalBet = raisedAmount - currentBet;
            
            m_pot += additionalBet;
            currentPlayer->setCurrentBet(raisedAmount);
            currentPlayer->setChips(currentChips - additionalBet);
            
            // If no remaining chips, sets the all-in data member to true
            if (currentPlayer->getChips() == 0)
                currentPlayer->setAllIn();
            
            // Sets the highestBet data member of the Game object to the raised amount
            m_highestBet = raisedAmount;
            
            break;
            
        case FOLD:
            currentPlayer->setFolded();
            break;
            
        case PASS:
            break;
            
    }
    
    // Prints out the move if it an AI move (and is not the automatically decided PASS move)
    if (AIMove && move != PASS)
    {
        sleep(9e8);
        printAIMove(move, AIPlayer);
    }
    
    // Resets the raised amount for the player (done after printing the potential AI player's move to avoid printing zeros)
    currentPlayer->setRaisedAmount(0);
}

/* Prints the move made by an AI player */
void Game::printAIMove(Move move, AI *computer)
{
    // Initializes a pointer to the AI player
    cout << computer->name;

    switch (move)
    {
        case CHECK:
            cout << " checks.";
            break;
            
        case CALL:
            // If the player has gone all-in
            if (computer->getChips() == 0)
            {
                cout << " calls up to " << computer->getCurrentBet() << ", going all-in.";
            }
            // Else, for a normal call
            else
            {
                cout << " calls " << computer->getCurrentBet() << ".";
            }
            
            break;
            
        case RAISE:
            cout << " raises to " << computer->getRaisedAmount() << ".";
            break;
            
        case FOLD:
            cout << " folds.";
            break;
            
        // Unreachable (printAIMove isn't called when the move is PASS)
        case PASS:
            break;
    }
    cout << endl;
}

/* Checks if the current hand is over (in which case no more betting rounds will be played and the hands of the players
 will be evaluated) prints the appropriate message and returns true if so, else returns false */
bool Game::handOver()
{
    if (oneUnfolded())
    {
        cout << "Only one unfolded player remaining, skipping bets." << endl;
        return true;
    }
        
    if (tableAllIn())
    {
        cout << "Only one player hasn't gone all in, skipping bets." << endl;
        return true;
    }
    
    return false;
}

/* If there is only one unfolded player remaining, returns true and adds the unfolded player to the winners vector,
 else returns false */
bool Game::oneUnfolded()
{
    // The variable that will be used to keep track of the number of folded players
    int numFolded = 0;
    
    // Checks the AI players
    for (AI *computer : m_computers)
    {
        // Checks if the current player is eliminated
        if (computer == nullptr)
        {
            // Increments numFolded as changing numPlayers would have unintended results on any save files that are created
            numFolded++;
            continue;
        }
        
        if (computer->hasFolded())
        numFolded++;
    }
    // Checks the human player
    if (m_human->hasFolded())
        numFolded++;
    
    // If there is exactly one unfolded player
    if (numPlayers - numFolded == 1)
    {
        // Adds the human player to the winners vector if the unfolded player is the human player
        if (!m_human->hasFolded())
        {
            winners.push_back(m_human);
        }
        // If not, adds the AI player that hasn't folded yet to the winners vector
        else
        {
            for (AI *computer : m_computers)
            {
                // Checks if the current player is eliminated
                if (computer == nullptr)
                    continue;
                
                if (!computer->hasFolded())
                {
                    winners.push_back(computer);
                }
            }
        }
        // Returns true to indicate that the hand is over
        return true;
    }
    // If this point is reached, the hand is not over yet (at least in terms of the number of folded players)
    return false;
}

/* Returns true if all but one of the players have gone all in */
bool Game::tableAllIn()
{
    // The variable that will be used to keep track of the number of players who have gone all-in
    int numAllIns = 0;
    
    // Checks the human player
    if (m_human->getAllIn())
        numAllIns++;
    
    // Checks the AI players
    for (AI *computer : m_computers)
    {
        // Checks if the current player is eliminated
        if (computer == nullptr)
        {
            // Increments numAllIns for the same reason numFolded is incremented in the oneUnfolded method
            numAllIns++;
            continue;
        }
        if (computer->getAllIn())
            numAllIns++;
    }
     
    // If there is exactly one player who hasn't gone all in
    if (numAllIns == numPlayers - 1)
        return true;
    // Else, the hand is not over yet (at least in terms of the number of players who have gone all-in)
    else
        return false;
}

/* Evaluates and scores the hand of each player, determines the winner(s), and adds them to the winners vector */
void Game::evaluateHands()
{
    // Sets the community data member of the handChecker object to the current community cards
    handChecker.community = m_community;
    
    // Keeps track of the highest score (-13 is the lowest score possible)
    int highestScore = -14;
    
    // Evaluates the hand of the human player first
    if (!m_human->hasFolded())
    {
        // Gets the human player's hand
        Hand humanPlayerHand = m_human->getHand();
        
        // Prints out the hand
        cout << "Your hand was: " << humanPlayerHand << endl;
        
        // Determines the score it receives
        handChecker.hand = humanPlayerHand;
        int humanScore = handChecker.checkHand();
        
        // If it has a greater score than the highest score, makes it the highest score
        if (humanScore > highestScore)
        {
            highestScore = humanScore;
        }
    }
    
    // Evaluates the hands of the AI players
    for (AI *computer : m_computers)
    {
        // Checks if the current player is eliminated
        if (computer == nullptr)
            continue;
        
        if (!computer->hasFolded())
        {
            // Gets the AI player's hand
            Hand AIHand = computer->getHand();
            
            // Prints out the hand
            cout << computer->name << "'s hand was: " << AIHand << endl;
            
            // Determines the score it receives
            handChecker.hand = AIHand;
            int AIScore = handChecker.checkHand();
            
            // If it has a greater score than the highest score, makes it the highest score
            if (AIScore > highestScore)
            {
                highestScore = AIScore;
            }
        }
    }
    
    // Checks each player again to determine if there are players that tie the greatest score
    // Evaluates the hand of the human player first
    // If the human player had not folded
    if (!m_human->hasFolded())
    {
        // Gets the human player's hand
        Hand humanPlayerHand = m_human->getHand();
        
        // Determines the score it receives
        handChecker.hand = humanPlayerHand;
        int humanScore = handChecker.checkHand();
        
        /* If it ties the highest score, adds player to the winners vector and prints the name of the hand
         they qualify for */
        if (humanScore == highestScore)
        {
            cout << "You won! You had: " << handChecker.displayHandName(humanScore) << endl;
            winners.push_back(m_human);
        }
    }
    // Else, if the human player had folded
    else
    {
        cout << "You had folded." << endl;
    }
    
    // Evaluates the hands of the AI players
    for (AI *computer : m_computers)
    {
        // Checks if the current player is eliminated
        if (computer == nullptr)
            continue;
        
        // If the AI player had not folded
        if (!computer->hasFolded())
        {
            // Gets the AI player's hand
            Hand AIHand = computer->getHand();
            
            // Determines the score it receives
            handChecker.hand = AIHand;
            int AIScore = handChecker.checkHand();
            
            /* If it ties the highest score, adds player to the winners vector and prints the name of
             the hand they qualify for */
            if (AIScore == highestScore)
            {
                cout << computer->name << " won, they had: " << handChecker.displayHandName(AIScore) << endl;
                winners.push_back(computer);
            }
        }
        // Else, if the AI player had folded
        else
        {
            cout << computer->name << " had folded." << endl;
        }
    }
    
    
}

/* Distributes the chips in the pot to the winner(s), dividing the pot equally if there are multiple winners */
void Game::distributeChips()
{
    // Divides the pot into the number of winners
    int receivedChips = m_pot / winners.size();
    
    // Add the divided share of the pot to each winner
    for (Player *winner : winners)
    {
        int currentChips = winner->getChips();
        winner->setChips(currentChips + receivedChips);
    }
}

/* Checks if the game is over (one player remaining,) displays the appropriate end message and returns true if so,
 else returns false */
bool Game::gameOver()
{
    // Checks if the human player has lost
    if (m_human == nullptr)
    {
        cout << "You are out of chips, better luck next time!" << endl << endl;
        return true;
    }
    
    // Checks if the human player has won
    bool humanWon = true;
    for (AI *computer : m_computers)
    {
        if (computer != nullptr)
            humanWon = false;
    }
    if (humanWon)
    {
        cout << "Congratulations, you have won the game!" << endl << endl;
        return true;
    }
    
    // If this point is reached, the game hasn't been won by anyone yet
    return false;
}

/* Randomly assings values to the cards in the community object */
void Game::dealCommunityCards()
{
    // Sets flop cards
    const int NUM_FLOP_CARDS = 3;
    for (int i = 0; i < NUM_FLOP_CARDS; i++)
    {
        Card randomCard1;
        m_community.m_flop[i] = randomCard1;
    }
    
    // Sets turn card
    Card randomCard2;
    m_community.m_turn = randomCard2;
    
    // Sets river card
    Card randomCard3;
    m_community.m_river = randomCard3;
}

/* Reveals a different number of community cards based on the stage value taken as an argument */
void Game::revealCommunityCards(Stage stage)
{
    const int NUM_FLOP_CARDS = 3;
    
    cout << endl << "Revealed community cards:" << endl;
    switch (stage)
    {
        case FLOP:
            for (int i = 0; i < NUM_FLOP_CARDS; i++)
            {
                sleep();
                cout << m_community.m_flop[i] << endl;
            }
            sleep();
            break;
            
        case TURN:
            for (int i = 0; i < NUM_FLOP_CARDS; i++)
            {
                cout << m_community.m_flop[i] << endl;
            }
            sleep();
            cout << m_community.m_turn << endl;
            sleep();
            break;
            
        case RIVER:
            for (int i = 0; i < NUM_FLOP_CARDS; i++)
            {
                cout << m_community.m_flop[i] << endl;
            }
            cout << m_community.m_turn << endl;
            sleep();
            cout << m_community.m_river << endl;
            sleep();
            break;
            
        // For when the hand ends before the turn card is revealed
        case ALL:
            for (int i = 0; i < NUM_FLOP_CARDS; i++)
            {
                cout << m_community.m_flop[i] << endl;
            }
            sleep();
            cout << m_community.m_turn << endl;
            sleep();
            cout << m_community.m_river << endl;
            sleep();
            break;
    }
    sleep(9e8);
    cout << endl;
}

/* Eliminates players that have zero chips left by deallocating the memory allocated for them and setting their pointers
 to nullptr */
void Game::eliminatePlayers()
{
    // Checks AI players
    for (int i = 0; i < numPlayers - 1; i++)
    {
        // If the AI player hasn't already been deallocated
        if (m_computers[i] != nullptr)
        {
            if (m_computers[i]->getChips() == 0)
            {
                delete m_computers[i];
                m_computers[i] = nullptr;
            }
        }
    }
    
    // Checks the human player
    if (m_human->getChips() == 0)
    {
        delete m_human;
        // Setting m_human to nullptr will result in the game ending after the gameOver method is called
        m_human = nullptr;
    }
}

/* Resets data members that were adjusted during the hand to their default values */
void Game::resetTable()
{
    // Sets folded to false for each player
    for (AI *computer : m_computers)
    {
        // Checks if the current player is eliminated
        if (computer == nullptr)
            continue;
        
        computer->setFolded(false);
    }
    
    m_human->setFolded(false);
    
    // Sets all-in to false for each player
    for (AI *computer : m_computers)
    {
        // Checks if the current player is eliminated
        if (computer == nullptr)
            continue;
        
        computer->setAllIn(false);
    }
        
    
    m_human->setAllIn(false);
    
    // Clears the winners vector
    winners.clear();
    
    // Sets bets and raised amounts to 0
    for (AI *computer : m_computers)
    {
        // Checks if the current player is eliminated
        if (computer == nullptr)
            continue;
        
        computer->setRaisedAmount(0);
    }
        
    
    m_human->setRaisedAmount(0);
    
    for (AI *computer : m_computers)
    {
        // Checks if the current player is eliminated
        if (computer == nullptr)
            continue;
        
        computer->setCurrentBet(0);
    }
        
    
    m_human->setCurrentBet(0);
    
    m_highestBet = 0;
    
    // Resets the pot
    m_pot = 0;
}

/* Presents information about the chips on the table (the pot, current bets, and remaining chips for each player) */
void Game::presentTableInfo()
{
    // Informs the human player about the AI players first
    for (AI *computer : m_computers)
    {
        // Checks if the current player is eliminated
        if (computer == nullptr)
            continue;

        if (computer->hasFolded())
        {
            cout << computer->name << " has folded. (Remainig chips: " << computer->getChips() << ")" << endl;
        }
        else
        {
            sleep();
            cout << computer->name << "'s current bet is " << computer->getCurrentBet() << ". (Remainig chips: "
            << computer->getChips() << ")" << endl;
        }
        
    }
    
    // Then, informs them about their current situation
    sleep();
    cout << endl << "Your cards are: " << m_human->getHand() << endl;
    sleep(4e8);
    cout << "There are " << m_pot << " chips in the pot." << endl;
    sleep(4e8);
    cout << "Current highest bet is " << m_highestBet << "." << endl;
    sleep(4e8);
    cout << "Your current bet is " << m_human->getCurrentBet() << "." << endl;
    sleep(4e8);
    cout << "You have " << m_human->getChips() << " chips remaining." << endl;

}

/* Prints the possible moves the user can make and receives input from them to determine their next move */
void Game::receiveMove(Move &humanMove, int turn)
{
    int moveNum = 0;
    int raisedAmount = 0;
    
    // If it is the initial betting turn
    if (turn == INITIAL_BETTING)
    {
        cout << endl << "Possible moves: check (1), call (2), raise (3), fold (4)" << endl;
        
        // Prompts human player for a move until they enter a valid move
        while (true)
        {
            cout << "Select move: ";
            cin >> moveNum;
            
            // Continues if moveNum isn't within the appropriate range
            if (!(1 <= moveNum && moveNum <= 4))
                continue;
            
            // Casts moveNum to a Move and checks whether the human player can make that move
            humanMove = static_cast<Move>(moveNum);
            
            // Special case for RAISE before validating move
            if (humanMove == RAISE)
            {
                cout << "Enter the amount you want to raise to: ";
                cin >> raisedAmount;
                m_human->setRaisedAmount(raisedAmount);
            }
            
            // If the move entered by human player cannot be made, informs them and prompts them again
            if (!checkMove(humanMove))
            {
                cout << "Cannot make that move." << endl;
                continue;
            }
            // If the move is valid, simply breaks (since the humanMove reference was already set to the correct value)
            else
            {
                break;
            }
        }
    }
    // Else, if it is the calling raises turn
    else if (turn == CALLING_RAISES)
    {
        // If the user's current bet is below the highest bet on the table, prompts them for a move
        if (m_human->getCurrentBet() < m_highestBet)
        {
            cout << endl <<"The new highest bet is now " << m_highestBet << ", you may call or fold." << endl;
            cout << "Possible moves: call (2), fold (4)" << endl;
            
            // Prompts user for a move until a valid move is entered
            while (true)
            {
                cout << "Select move: ";
                cin >> moveNum;
                
                if (!(moveNum == 2 || moveNum == 4))
                    continue;
                
                // Casts moveNum to a Move to set humanMove equal to it and validate it
                humanMove = static_cast<Move>(moveNum);
                
                // Should never evaluate to true, as calling (at the risk of going all-in) and folding are always possible
                if (!checkMove(humanMove))
                {
                    cout << "Cannot make that move" << endl;
                    continue;
                }
                // If the move is valid, breaks (as the humanMove reference was already set to the correct move)
                else
                {
                    break;
                }
            }
        }
    }
    
}

/* Asks the user whether they want to save the game, calls the createSaveFile method if they confirm */
void Game::promptSaveGame()
{
    string answer = "";
    
    // Asks user whether they want to save the game until they provide a valid answer
    do
    {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl << "Would you like to save the game? (yes, no) ";
        cin >> answer;
    }
    while (!(answer == "y" || answer == "yes" || answer == "Yes" || answer == "YES" || answer == "n" || answer == "no" || answer == "No" || answer == "NO"));
    
    cout << endl;
    
    // Creates a save file if the user said yes
    if (answer == "y" || answer == "yes" || answer == "Yes" || answer == "YES")
    {
        // Prompts user for a save file name until they enter a valid name
        string name;
        do
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Give a name to your save file (max 15 characters): ";
            cin >> name;
        }
        while (name.length() == 0 || name.length() > 15);
        
        createSaveFile(name);
    }
        
    
}

/* Creates a save file following a set template */
void Game::createSaveFile(string name)
{
    // The txt file extension
    const string EXTENSION = ".txt";
    
    // Checks if the player is eliminated, informs user and returns if so
    if (m_human == nullptr)
    {
        cout << "Cannot create save file, you have no chips remaining!" << endl;
        return;
    }
    
    // Creates the file
    ofstream savefile(name + EXTENSION);
    
    /* Prints relevant information for the save file into the file */
    
    savefile << m_human->getChips() << endl; // number of chips the human player has
    savefile << numPlayers - 1 << endl; // number of AI players
    for (AI *computer : m_computers)
    {
        int chips;
        // Checks if the AI player is eliminated, sets chips to 0 to indicate it if so
        if (computer == nullptr)
            chips = 0;
        else
            chips = computer->getChips();
            
        savefile << chips << endl; // number of chips each AI player has
    }
        
}

/* Reads a save file if the user chooses to load a save file at the start of the game. Sets all the variables
 taken in by reference using the data read from the file */
bool Game::readSaveFile(string name, int &humanChips, int &numAIs, vector<int> &AIChips)
{
    ifstream savefile(name);
    
    // Checks if the file was successfully opened
    if (!savefile)
    {
        cout << "Could not open save file." << endl;
        return false;
    }

    string word;
    
    // Reads in the number of chips the human player has
    savefile >> word;
    humanChips = stoi(word);
    
    // Reads in the number of AI players in the game
    savefile >> word;
    numAIs = stoi(word);
    
    // Reads in the number of chips each AI player has
    for (int i = 0; i < numAIs; i++)
    {
        savefile >> word;
        AIChips.push_back(stoi(word));
    }
    
    return true;
}

/* Sleeps for a number of nanoseconds taken as argument, the parameter defaults to 5e8 (0.5 seconds) */
void Game::sleep(long nanoseconds)
{
    interval.tv_nsec = nanoseconds;
    nanosleep(&interval, &interval);
}

/* Prints a separator to help distinguish different betting rounds in the console */
void Game::printSeparator()
{
    cout << endl;
    for (int i = 0; i < 20; i++)
    {
        cout << "-";
    }
    cout << " Next betting round ";
    for (int i = 0; i < 20; i++)
    {
        cout << "-";
    }
    sleep(3e8);
    cout << endl;
}
