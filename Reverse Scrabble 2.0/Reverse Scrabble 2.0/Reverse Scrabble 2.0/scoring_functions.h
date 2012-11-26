//
//  scoring_functions.h
//  Reverse Scrabble 2.0
//
//  Created by Brett Wines on 11/25/12.
//  Copyright (c) 2012 Brett Wines. All rights reserved.
//

#ifndef Reverse_Scrabble_2_0_scoring_functions_h
#define Reverse_Scrabble_2_0_scoring_functions_h

#include "datastructures.h"
#include "initialization_functions.h"

static map<char, int> letter_scores;

static int determine_word_score(const word_t &word)
{
    int score = 0;
    int word_multipier = 1;
    for (size_t i=0; i<word.word.length(); i++)
    {
        int tile_score = letter_scores[word.word[i]];
        if (word.letters_played[i])
        {
            tile_score *= 1; //multiplier[]
            word_multipier *= 1; //multiplier[]
        }
        score += tile_score;
    }
    return score * word_multipier;
}

int determine_turn_score(const turn_t &turn)
{
    int score = 0;
    score += determine_word_score(turn.word_played);
    for (size_t i=0; i<turn.words_played_by_side_effect.size(); i++)
    {
        score += determine_word_score(turn.words_played_by_side_effect[i]);
    }
    return score;
}

void fill_tile_score_map()
{
    score_letters(letter_scores);
}

#endif
