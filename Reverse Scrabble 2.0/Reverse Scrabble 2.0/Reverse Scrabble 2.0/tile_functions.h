//
//  tile_functions.h
//  Reverse Scrabble 2.0
//
//  Created by Brett Wines on 11/25/12.
//  Copyright (c) 2012 Brett Wines. All rights reserved.
//

#ifndef Reverse_Scrabble_2_0_tile_functions_h
#define Reverse_Scrabble_2_0_tile_functions_h

static void fix_letter_counts(const string &word,
                              const size_t &breaking_index,
                              map<char, int> &letter_counts)
{
    for (size_t i=0; i<breaking_index; i++)
    {
        letter_counts[word[i]]++;
    }
}

/* Returns true if successful, false otherwise. */
bool update_letter_counts_from_turn(map<char, int> &letter_counts,
                                    const turn_t &turn)
{
    string word = turn.word_played.word;;
    for (size_t i=0; i<word.length(); i++)
    {
        if (letter_counts[word[i]] == 0)
        {
            fix_letter_counts(word, i, letter_counts);
            return false;
        }
        
        letter_counts[word[i]]--;
    }
    
    return true;
}

void cover_tiles(list<point> &tiles_to_be_covered,
                 const turn_t &turn)
{
    vector<point> locations = turn.word_played.get_locations();
    vector<int> letters_played = turn.word_played.letters_played;
    for (size_t i=0; i<locations.size(); i++)
    {
        if (letters_played[i])
        {
            tiles_to_be_covered.remove(locations[i]);
        }
    }
}


#endif
