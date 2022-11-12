#include "main.h"

int main()
{
    // Seed the random number generator
    srand(static_cast<int>(time(0)));
    
    // Uncomment for testing
    // test();
    
    Game gameObj;
    gameObj.playGame();
}


