#include "checker.h"

// The array of handNames used to print hands
string handNames[] = {"Pair", "Two Pairs", "Three of a Kind", "Straight", "Flush", "Full House", "Four of a Kind", "Straight Flush", "Royal Flush"};

// The methods for the Checker class
/* Checks the hand and community data members and returns the score that the current hand would receive */
int Checker::checkHand() const
{
    if (royalFlush())
        return 8;
    if (straightFlush())
        return 7;
    if (numOfAKind(4))
        return 6;
    if (fullHouse())
        return 5;
    if (flush())
        return 4;
    if (straight())
        return 3;
    if (numOfAKind(3))
        return 2;
    
    int numPairs;
    pairs(numPairs);
    
    if (numPairs)
    
    if (numPairs == 2)
        return 1;
    if (numPairs == 1)
        return 0;

    // Ace earns a score of -1, and Two earns a score of -13
    return highCard() - 14;
}

/* Returns the name of the hand that would receive the score taken as argument */
string Checker::displayHandName(int score) const
{
    if (score < 0)
        return "High Card";
    switch (score)
    {
        case 0:
            return "Pair";
        case 1:
            return "Two Pairs";
        case 2:
            return "Three of a Kind";
        case 3:
            return "Straight";
        case 4:
            return "Flush";
        case 5:
            return "Full House";
        case 6:
            return "Four of a Kind";
        case 7:
            return "Straight Flush";
        case 8:
            return "Royal Flush";
        default:
            return "Unknown";
    }
}

/* Checks for a royal flush */
bool Checker::royalFlush() const
{
    Rank startingRank = TWO;
    if (straightFlush() && straight(startingRank) && startingRank == TEN)
        return true;
    else
        return false;
}

/* Checks for a full house */
bool Checker::fullHouse() const
{
    const int NUM_CARDS = 7;
    Rank ranks[NUM_CARDS];
    
    // Stores the ranks of the cards in the community to the ranks array
    // Flop cards
    const int NUM_FLOP_CARDS = 3;
    for (int i = 0; i < NUM_FLOP_CARDS; i++)
    {
        ranks[i] = community.m_flop[i].getRank();
    }
    // The turn card
    ranks[3] = community.m_turn.getRank();
    // The river card
    ranks[4] = community.m_river.getRank();
    
    // Stores the ranks of the cards in the player's hand to the ranks array
    ranks[5] = hand.getCard1().getRank();
    ranks[6] = hand.getCard2().getRank();
    
    bool threes = false;
    bool twos = false;
    // Determine the card repeated three times (if any)
    for (Rank rank : ranks)
    {
        int occurence = 0;
        for (int i = 0; i < NUM_CARDS; i++)
        {
            if (ranks[i] == rank)
                occurence++;
        }
        
        if (occurence == 3)
            threes = true;
        else if (occurence == 2)
            twos = true;
    }
    
    if (threes && twos)
        return true;
    else
        return false;
}

/* Checks for three or four of a kind */
bool Checker::numOfAKind(const int NUMBER) const
{
    const int NUM_CARDS = 7;
    Rank ranks[NUM_CARDS];
    
    // Stores the ranks of the cards in the community to the ranks array
    // Flop cards
    const int NUM_FLOP_CARDS = 3;
    for (int i = 0; i < NUM_FLOP_CARDS; i++)
    {
        ranks[i] = community.m_flop[i].getRank();
    }
    // The turn card
    ranks[3] = community.m_turn.getRank();
    // The river card
    ranks[4] = community.m_river.getRank();
    
    // Stores the ranks of the cards in the player's hand to the ranks array
    ranks[5] = hand.getCard1().getRank();
    ranks[6] = hand.getCard2().getRank();
    
    
    for (Rank rank : ranks)
    {
        int occurence = 0;
        for (int i = 0; i < NUM_CARDS; i++)
        {
            if (ranks[i] == rank)
                occurence++;
        }
        
        if (occurence >= NUMBER)
            return true;
    }
    // If this point is reached, there is no four of a kind
    return false;
}

/* Checks for pairs (one or two) */
bool Checker::pairs(int &numPairs) const
{
    const int NUM_CARDS = 7;
    Rank ranks[NUM_CARDS];
    
    // Stores the ranks of the cards in the community to the ranks array
    // Flop cards
    const int NUM_FLOP_CARDS = 3;
    for (int i = 0; i < NUM_FLOP_CARDS; i++)
    {
        ranks[i] = community.m_flop[i].getRank();
    }
    // The turn card
    ranks[3] = community.m_turn.getRank();
    // The river card
    ranks[4] = community.m_river.getRank();
    
    // Stores the ranks of the cards in the player's hand to the ranks array
    ranks[5] = hand.getCard1().getRank();
    ranks[6] = hand.getCard2().getRank();
    
    int pairs = 0;
    vector<Rank> foundRanks;
    for (Rank rank : ranks)
    {
        // Checks if this rank was already found in a pair, continues to the next rank if so
        bool rankAlreadyFound = false;
        for (Rank foundRank : foundRanks)
        {
            if (foundRank == rank)
            {
                rankAlreadyFound = true;
            }
        }
        if (rankAlreadyFound)
            continue;
        
        // Records the occurences of that rank
        int occurence = 0;
        for (int i = 0; i < NUM_CARDS; i++)
        {
            if (ranks[i] == rank)
                occurence++;
        }
        
        // If there are exactly two occurences, increments pairs by one and adds the rank to the foundRanks vector
        if (occurence == 2)
        {
            pairs++;
            
            // Adds rank to the found ranks vector
            foundRanks.push_back(rank);
        }
            
    }
    numPairs = pairs;
    
    if (pairs)
        return true;
    else
        return false;
}

/* Checks for a flush */
bool Checker::flush() const
{
    int suitNumbers[4] = {0, 0, 0, 0};
    
    // Checks the cards in the community
    // Flop cards
    const int NUM_FLOP_CARDS = 3;
    for (int i = 0; i < NUM_FLOP_CARDS; i++)
    {
        suitNumbers[community.m_flop[i].getSuit()]++;
    }
    // The turn card
    suitNumbers[community.m_turn.getSuit()]++;
    // The river card
    suitNumbers[community.m_river.getSuit()]++;
    
    // Checks the player's hand
    suitNumbers[hand.getCard1().getSuit()]++;
    suitNumbers[hand.getCard2().getSuit()]++;
    
    // Checks if there are 5 or more for any suit (condition for a flush)
    for (int suitNumber : suitNumbers)
    {
        if (suitNumber >= 5)
            return true;
    }
    // If this point is reached, the hand is not a flush
    return false;
}

/* Overloaded version of flush that sets the Suit reference taken as an argument equal to the
 flush suit if a flush is found */
bool Checker::flush(Suit & flushSuit) const
{
    const int NUM_SUITS = 4;
    int suitNumbers[NUM_SUITS] = {0, 0, 0, 0};
    
    // Checks the cards in the community
    // Flop cards
    const int NUM_FLOP_CARDS = 3;
    for (int i = 0; i < NUM_FLOP_CARDS; i++)
    {
        suitNumbers[community.m_flop[i].getSuit()]++;
    }
    // The turn card
    suitNumbers[community.m_turn.getSuit()]++;
    // The river card
    suitNumbers[community.m_river.getSuit()]++;
    
    // Checks the player's hand
    suitNumbers[hand.getCard1().getSuit()]++;
    suitNumbers[hand.getCard2().getSuit()]++;
    
    // Checks if there are 5 or more for any suit (condition for a flush)
    for (int i = 0; i < NUM_SUITS; i++)
    {
        if (suitNumbers[i] >= 5)
        {
            // Sets the flushSuit reference to the suit in which the flush was found
            flushSuit = static_cast<Suit>(i);
            
            return true;
        }
    }
    // If this point is reached, the hand is not a flush
    return false;
}

/* Checks for a straight */
bool Checker::straight() const
{
    const int NUM_CARDS = 7;
    Rank ranks[NUM_CARDS];
    
    // Stores the ranks of the cards in the community to the ranks array
    // Flop cards
    const int NUM_FLOP_CARDS = 3;
    for (int i = 0; i < NUM_FLOP_CARDS; i++)
    {
        ranks[i] = community.m_flop[i].getRank();
    }
    // The turn card
    ranks[3] = community.m_turn.getRank();
    // The river card
    ranks[4] = community.m_river.getRank();
    
    // Stores the ranks of the cards in the player's hand to the ranks array
    ranks[5] = hand.getCard1().getRank();
    ranks[6] = hand.getCard2().getRank();
    
    // Sorts the ranks array
    sort(ranks, ranks + NUM_CARDS);
    
    // Checks for a straight (sequence of 5 cards in increasing value, where 1 can follow 13)
    // The sequence vector
    vector<int> sequence;
    sequence.push_back(ranks[0]);
    for (int i = 1; i < 7; i++)
    {
        int nextNumber = ranks[i];
        if (!contains(sequence, nextNumber))
            sequence.push_back(nextNumber);
    }
    
    // Checks if the sequence is at least 5 numbers in length (condition for a straight)
    if (sequence.size() < 5)
        return false;
    
    // Checks to see if the sequence has a common difference of 1 throughout (barring repeated terms)
    for (int i = 0, len = static_cast<int>(sequence.size()); i < len - 1; i++)
    {
        bool foundStraight = true;
        
        for (int j = 0; j < 4; j++)
        {
            
            // Checks if the next element and the current element have a difference of 1
            if (sequence[i + j + 1 < 7 ? i + j + 1: i + j - 5] - sequence[i + j < 7 ? i + j : i + j - 6] != 1)
            {
                foundStraight = false;
                break;
            }
        }
        
        if (foundStraight)
            return true;
    }
    
    // If this point is reached, a straight was not found
    return false;
};

/* Overloaded version of straight that checks for a straight flush by taking a flushSuit as argument */
bool Checker::straight(Suit flushSuit) const
{
    const int NUM_CARDS = 7;
    Card cards[NUM_CARDS];
    
    // Stores the  cards in the community to the cards array
    // Flop cards
    const int NUM_FLOP_CARDS = 3;
    for (int i = 0; i < NUM_FLOP_CARDS; i++)
    {
        cards[i] = community.m_flop[i];
    }
    // The turn card
    cards[3] = community.m_turn;
    // The river card
    cards[4] = community.m_river;
    
    // Stores the ranks of the cards in the player's hand to the ranks array
    cards[5] = hand.m_card1;
    cards[6] = hand.m_card2;
    
    // Sorts the ranks array
    sortCards(cards, NUM_CARDS);
    
    // Checks for a straight (sequence of 5 cards in increasing value, where 1 can follow 13)
    // The sequence vector
    vector<Card> sequence;

    // Adds cards to the sequence vector if the number is unique and the suit is equal to the flush suit
    for (int i = 0; i < 7; i++)
    {
        Card nextCard = cards[i];
        if (!contains(sequence, nextCard.getRank()) && nextCard.getSuit() == flushSuit)
            sequence.push_back(cards[i]);
    }
    
    // Checks if the sequence is at least 5 numbers in length (condition for a straight)
    if (sequence.size() < 5)
        return false;
    
    // Checks to see if the sequence has a common difference of 1 throughout (barring repeated terms)
    for (int i = 0, len = static_cast<int>(sequence.size()); i < len - 1; i++)
    {
        bool foundStraight = true;
        
        for (int j = 0; j < 4; j++)
        {
            Card currentCard = sequence[i + j < 7 ? i + j : i + j - 6];
            Card nextCard = sequence[i + j + 1 < 7 ? i + j + 1: i + j - 5];
            
            // Checks if the next element and the current element have a difference of 1
            if ((nextCard.getRank() - currentCard.getRank() != 1))
            {
                foundStraight = false;
                break;
            }
        }
        
        if (foundStraight)
            return true;
    }
    // If this point is reached, a straight flush was not found
    return false;
};

/* Overloaded version of straight that sets the Rank reference taken as an argument to the starting rank of the
 straight (which will only be valid if true is returned) */
bool Checker::straight(Rank & startingRank) const
{
    const int NUM_CARDS = 7;
    Rank ranks[NUM_CARDS];
    
    // Stores the ranks of the cards in the community to the ranks array
    // Flop cards
    const int NUM_FLOP_CARDS = 3;
    for (int i = 0; i < NUM_FLOP_CARDS; i++)
    {
        ranks[i] = community.m_flop[i].getRank();
    }
    // The turn card
    ranks[3] = community.m_turn.getRank();
    // The river card
    ranks[4] = community.m_river.getRank();
    
    // Stores the ranks of the cards in the player's hand to the ranks array
    ranks[5] = hand.getCard1().getRank();
    ranks[6] = hand.getCard2().getRank();
    
    // Sorts the ranks array
    sort(ranks, ranks + NUM_CARDS);
    
    // Testing
    
    // Checks for a straight (sequence of 5 cards in increasing value, where 1 can follow 13)
    // The sequence vector
    vector<int> sequence;
    sequence.push_back(ranks[0]);
    for (int i = 1; i < 7; i++)
    {
        int nextNumber = ranks[i];
        if (!contains(sequence, nextNumber))
            sequence.push_back(nextNumber);
    }
    
    // Checks if the sequence is at least 5 numbers in length (condition for a straight)
    if (sequence.size() < 5)
        return false;
    
    // Checks to see if the sequence has a common difference of 1 throughout (barring repeated terms)
    for (int i = 0, len = static_cast<int>(sequence.size()); i < len - 1; i++)
    {
        bool foundStraight = true;
        
        for (int j = 0; j < 4; j++)
        {
            startingRank = static_cast<Rank>(sequence[i + j < 7 ? i + j : i + j - 6]);
            // Checks if the next element and the current element have a difference of 1
            if (sequence[i + j + 1 < 7 ? i + j + 1: i + j - 5] - sequence[i + j < 7 ? i + j : i + j - 6] != 1)
            {
                foundStraight = false;
                break;
            }
        }
        
        if (foundStraight)
            return true;
    }
    
    // If this point is reached, a straight was not found
    return false;
};

/* Checks for a straight flush */
bool Checker::straightFlush() const
{
    Suit flushSuit;
    // Checks if it is a flush and records the suit into flushSuit if it is
    bool isFlush = flush(flushSuit);
    if (!isFlush)
        return false;
    
    // Checks if it is a straight flush
    bool isStraightFlush = straight(flushSuit);
    
    if (isStraightFlush)
        return true;
    else
        return false;
}

/* Determines and returns the rank of the highest card */
Rank Checker::highCard() const
{
    const int NUM_CARDS = 7;
    Rank ranks[NUM_CARDS];
    
    // Stores the ranks of the cards in the community to the ranks array
    // Flop cards
    const int NUM_FLOP_CARDS = 3;
    for (int i = 0; i < NUM_FLOP_CARDS; i++)
    {
        ranks[i] = community.m_flop[i].getRank();
    }
    // The turn card
    ranks[3] = community.m_turn.getRank();
    // The river card
    ranks[4] = community.m_river.getRank();
    
    // Stores the ranks of the cards in the player's hand to the ranks array
    ranks[5] = hand.getCard1().getRank();
    ranks[6] = hand.getCard2().getRank();
    
    Rank highestRank = TWO;
    for (Rank rank : ranks)
    {
        // Ace is considered the highest rank when evaluating a high card hand
        if (rank == ACE)
        {
            highestRank = rank;
            break;
        }
        if (rank > highestRank)
            highestRank = rank;
    }
    
    // The ranks are decreased by 1 and Ace is moved to the top
    return highestRank == ACE ? KING : static_cast<Rank>(highestRank - ACE);
}

/* Used to test different hands */
void Checker::test()
{
    // Change rank and suit values as needed
    hand.m_card1.setRank(static_cast<Rank>(1));
    hand.m_card2.setRank(static_cast<Rank>(2));
    community.m_flop[0].setRank(static_cast<Rank>(11));
    community.m_flop[1].setRank(static_cast<Rank>(12));
    community.m_flop[2].setRank(static_cast<Rank>(13));
        
    hand.m_card1.setSuit(SPADES);
    hand.m_card2.setSuit(SPADES);
    community.m_flop[0].setSuit(CLUBS);
    community.m_flop[1].setSuit(DIAMONDS);
    community.m_flop[2].setSuit(SPADES);
    

    // Call any hand checker function here...
}


// Helper functions
/* Returns true if the vector passed by reference contains the number taken as argument, else false */
bool contains(const vector<int> &v, int number)
{
    for (int element : v)
    {
        if (element == number)
            return true;
    }
    return false;
}

/* Returns true if the vector passed by reference contains a Card whose rank matches the number taken as argument,
else false */
bool contains(const vector<Card> &v, int number)
{
    for (Card card : v)
    {
        if (card.getRank() == number)
            return true;
    }
    return false;
}

/* Returns true if the two cards taken as arguments have the same rank, else false */
bool compareRanks(Card card1, Card card2)
{
    return (card1.getRank() < card2.getRank());
}

/* Sorts an array of cards in ascending order */
void sortCards(Card * const cards, int numCards)
{
    for (int i = 0; i < numCards - 1; i++)
    {
        // Uses a temporary pointer to iterate through the array
        Card *tmp = cards;
        
        // A bool that keeps track of whether a swap is made for each complete iteration (inner loop)
        bool swapMade = false;
        
        for (int j = 0; j < numCards - 1 - i; j++)
        {
            if (tmp->getRank() > (tmp + 1)->getRank())
            {
                Card *next = tmp + 1;
                swapCards(tmp, next);
                swapMade = true;
            }
            tmp++;
        }
        
        // If no swap is made, breaks the loop as the names are alphabetically sorted
        if (!swapMade)
            break;
    }
}

/* Swaps two cards by taking pointers to them as arguments */
void swapCards(Card *card1, Card *card2)
{
    Card tmp = Card(*card1);
    *card1 = *card2;
    *card2 = tmp;
}
