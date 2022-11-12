Hello,
This is my version of Texas Holdem Poker, a popular version of Poker that consists of two-card hands for each player, 5 community cards that are revealed sequentially on the table, betting via chips, and a hierarchy of poker hands wherein each hand has a precedence over the next (which decides who wins each hand). Although this version is text-based, it represents most features that the actual game has. The game directs the user for the most part, so being familiar with the different moves one can make in the game is enough to understand the flow of the game:
- Check: When your current bet is the same as the highest bet on the table, you can check to let the turn pass without any transaction of chips between you and the pot (the collection of chips that represent the sum of all players' bets)
- Call: When you current bet is less than the highest bet on the table, you can call to increase your current bet to that amount (covering the difference from the chips you have in your hand.) As expected, this move is mutually exclusive with check.
- Raise: When you current bet is less than the highest bet on the table and you also have enough chips in your hand to increment your current bet enough to make it exceed the current highest bet, you can choose to raise your bet (and, therefore, the highest bet on the table) to any amount greater than the current highest bet. There are usually restrictions on raising in different variations of the game, though this version doesn't implement any restriction.
- Fold: You can fold at any time to indicate that you will no longer participate in this hand, effectively losing the chips you have betted in the current hand so far (though avoiding further betting.)

One additional terminology that is useful to be familiar with (if not already) is going all-in. This occurs when you raise your bet to a certain amount (either by calling or raising) that results in you having no chips left in your hand. You are effectively removed from the betting rounds as you can take no further actions. Once you go all-in, the result of the current hand decides your future in the game, as losing the hand means having no chips to participate in the next and thereby being eliminated from the game. While going all-in via raising will restrictively mean that you are increasing the highest bet on the table (due to the nature of raising,) going all-in via calling does not require to match the current highest bet on the table. Instead, you simply bet all of your remaining chips away and into the pot.

For more information, the Wikipedia article on Texas Holdem is a very helpful source:
https://en.wikipedia.org/wiki/Texas_hold_%27em

There were some features on my mind that I unfortunately did not have enough time to get to. These include:
- Unique cards (no repetitions)
- Considering high cards when evaluating pairs (as in real life,) so that not all pairs are equal in value
- "Blinds," constraints that dictate the minimum amount certain players have to bet each round, rotating amongst players
- Currently, there is some collision between checking if all but one of the players have folded and if all but one of the players have gone all in. It doesn't seem to impact the flow of the game, but it is slightly confusing for the user as some messages are unintentionally printed. I hope to be able to solve that with some in-depth look into the order in which those are called and the scenarios in which one might overlap with the other.

Hopefully I will be able to implement some or all of these in the near future.
