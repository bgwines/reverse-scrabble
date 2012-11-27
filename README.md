#scrabble-history

In order to run this, go to the following files and make the following changes:

* `constants.h:line 19` -- change the variable board_file_name to the location of the board file
* `constants.h:line 21` -- change the variable dictionary_file_name to the location of the dictionary file
* `constants.h:line 23` -- change the variable disregard_final_scores if you want to see all histories that lead to the final state, irrespective of the final scores of the players (this is useful if you're running it on a board that doesn't come from an actual game, in which case you wouldn't know the final scores).


##Assumptions
No error checking is currently performed on the input board. Specifically, the following assumptions are made:

* the board is a square (i.e. m x n for m == n)
* the board format is adhered to 

##board format
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