# Hold'em Poker Simulator

## Project information

Author: Cem Kupeli

Dates worked on: July 2020

Purpose: Simulate a text-based version of Texas Hold'em Poker

## Summary

This is a text-based Texas Hold'em Poker simulator that I wrote using C++ in July 2020. The game directs the user for the most part, so being familiar with the different moves you can make is enough to understand the flow of the game:
- Check: When your current bet is the same as the highest bet on the table, you can check to let the turn pass without any transaction of chips between you and the pot (the collection of chips that represent the sum of all players' bets)
- Call: When you current bet is less than the highest bet on the table, you can call to increase your current bet to that amount (covering the difference from the chips you have in your hand.) This move is mutually exclusive with check.
- Raise: When your current bet is less than the highest bet on the table and you have enough chips in your hand to increase your current bet beyond the highest bet on the table, you can choose to raise your bet to any amount greater than the current highest bet.
- Fold: You can fold at any time to indicate that you will no longer participate in this hand, losing the chips that you have betted so far.
- All-in: This occurs when you raise your bet to a certain amount (either by calling or raising) that results in you having no chips left in your hand. You are effectively removed from the betting rounds as you can take no further actions. Once you go all-in, losing means being eliminated from the game, as you have no remaining chips. While going all-in via raising will restrictively mean that you are increasing the highest bet on the table (due to the nature of raising,) going all-in via calling does not require to match the current highest bet on the table. Instead, you simply bet all of your remaining chips.

For more information, you can refer to [Texas Holdem](https://en.wikipedia.org/wiki/Texas_hold_%27em)
