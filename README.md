scrabble-history
================

DFS to computer all possible histories of a Scrabble board.

Note: to run it, you'll have to put the dictionary file 
and the board file in specific places. Next update should 
fix that.


==================
   board format
==================
Example:

    ...cite
    .tilt..
    ...e...
    .planes
    ...n...
    .......
    .......
    %3
    %10
    %19
    %4
    #comment

Any lines starting with # will be ignored, so feel free to use them for comments.

The first of the lines starting with a '%' character is the number of players. Each 
subsequent i^th line is the score of the i^th player.