//
//  main.cpp
//  Reverse Scrabble 2.0
//
//  Created by Brett Wines on 11/25/12.
//  Copyright (c) 2012 Brett Wines. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <list>
#include <assert.h>

#include "Board.h"
#include "boolgrid_functions.h"
#include "datastructures.h"
#include "initialization_functions.h"
#include "globals_wrt_logic_files.h"
#include "tile_functions.h"
#include "scoring_functions.h"
#include "util.h"
using namespace std;

/* --- static instance variables --- */

/* players' final scores (set at start) */
static vector<int> player_final_scores;

/* used for storing history if success */
static vector< vector<turn_t> > successful_histories;

/* valid English words */
static set<string> dictionary;

/* --- functions ---*/

string extend_word_based_on_state(const string &word_contents,
                                  bool **state,
                                  size_t X,
                                  size_t Y,
                                  bool vertical)
{
    if (vertical)
    {
        size_t extended_left_x = X;
        while (board.in_bounds(extended_left_x-1, Y)
               && board.board[extended_left_x-1][Y] != EMPTY_BLOCK
               && state[extended_left_x-1][Y])
            extended_left_x--;
        
        size_t extended_right_x = X + word_contents.length()-1;
        while (board.in_bounds(extended_right_x+1, Y)
               && board.board[extended_right_x+1][Y] != EMPTY_BLOCK
               && state[extended_right_x+1][Y])
            extended_right_x++;
        
        string extended_word_contents;
        for (size_t x=extended_left_x; x<=extended_right_x; x++)
            extended_word_contents += board.board[x][Y];
        return extended_word_contents;
    }
    else //horizontal
    {
        size_t extended_up_y = Y;
        while (board.in_bounds(X, extended_up_y-1)
               && board.board[X][extended_up_y-1] != EMPTY_BLOCK
               && state[X][extended_up_y-1])
            extended_up_y--;
        
        size_t extended_down_y = Y + word_contents.length()-1;
        while (board.in_bounds(X, extended_down_y+1)
               && board.board[X][extended_down_y+1] != EMPTY_BLOCK
               && state[X][extended_down_y+1])
            extended_down_y++;
        
        string extended_word_contents;
        for (size_t y=extended_up_y; y<=extended_down_y; y++)
            extended_word_contents += board.board[X][y];
        return extended_word_contents;
    }
}

bool xy_borders_played_square(const size_t &x,
                              const size_t &y,
                              bool** state)
{
    if (board.in_bounds(x-1, y  ))
        if (state      [x-1][y  ])
            return true;
    if (board.in_bounds(x+1, y  ))
        if (state      [x+1][y  ])
            return true;
    if (board.in_bounds(x  , y-1))
        if (state      [x  ][y-1])
            return true;
    if (board.in_bounds(x  , y+1))
        if (state      [x  ][y+1])
            return true;
    return false;
}

/* Note: this could share a lot of code, but for sake of
 * keeping things not too messy, we'll keep it separate, here.
 */
void get_all_words_from_location_with_direction_for_side_effects(const size_t X,
                                                                 const size_t Y,
                                                                 bool vertical,
                                                                 vector<word_t> &words,
                                                                 bool** state)
{
    if (vertical)
    {
        for (size_t left_x=X; left_x!=(size_t)(-1); left_x--)
        {
            if (board.board[left_x][Y] == EMPTY_BLOCK)
                break;
            if (!state[left_x][Y] && left_x!=X)
                break;
            for (size_t right_x=X; right_x<=board.DIM-1; right_x++)
            {
                if (board.board[right_x][Y] == EMPTY_BLOCK)
                    break;
                if (!state[right_x][Y] && right_x!=X)
                    break;
                if (left_x==X && right_x==X)
                    continue;
                
                string word_contents;
                for (size_t i=left_x; i<=right_x; i++)
                    word_contents += board.board[i][Y];
                
                vector<int> letters_played;
                for (size_t i=left_x; i<=right_x; i++)
                {
                    letters_played.push_back(!state[i][Y]);
                }
                word_t word(left_x, Y, vertical, word_contents, letters_played);
                words.push_back(word);
            }
        }
    }
    else //horizontal
    {
        for (size_t up_y=Y; up_y!=(size_t)(-1); up_y--)
        {
            if (board.board[X][up_y] == EMPTY_BLOCK)
                break;
            if (!state[X][up_y] && up_y!=Y)
                break;
            for (size_t down_y=Y; down_y<=board.DIM-1; down_y++)
            {
                if (board.board[X][down_y] == EMPTY_BLOCK)
                    break;
                if (!state[X][down_y] && down_y!=Y)
                    break;
                if (up_y==Y && down_y==Y)
                    continue;
                
                string word_contents;
                for (size_t j=up_y; j<=down_y; j++)
                    word_contents += board.board[X][j];
                
                vector<int> letters_played;
                for (size_t j=up_y; j<=down_y; j++)
                {
                    letters_played.push_back(!state[X][j]);
                }
                word_t word(X, up_y, vertical, word_contents, letters_played);
                words.push_back(word);
            }
        }
    }
}

void get_all_words_from_location_with_direction(const size_t X,
                                                const size_t Y,
                                                bool vertical,
                                                vector<word_t> &words,
                                                bool** state)
{
    if (vertical)
    {
        for (size_t left_x=X; left_x!=(size_t)(-1); left_x--)
        {
            if (board.board[left_x][Y] == EMPTY_BLOCK)
                break;
            //don't try to make a word if it doesn't end a word
            if (board.in_bounds(left_x-1, Y))
                if (state[left_x-1][Y])
                    continue;
            for (size_t right_x=X; right_x<=board.DIM-1; right_x++)
            {
                if (board.board[right_x][Y] == EMPTY_BLOCK)
                    break;
                //don't try to make a word if it doesn't end a word
                if (board.in_bounds(right_x+1, Y))
                    if (state[right_x+1][Y])
                        continue;
                
                string word_contents;
                vector<int> letters_played_this_turn;
                for (size_t i=left_x; i<=right_x; i++)
                {
                    word_contents += board.board[i][Y];
                    letters_played_this_turn.push_back(!state[i][Y]);
                }
                
                if (dictionary.count(word_contents))
                {
                    word_t word(left_x, Y, vertical, word_contents, letters_played_this_turn);
                    words.push_back(word);
                }
            }
        }
    }
    else //horizontal
    {
        for (size_t up_y=Y; up_y!=(size_t)(-1); up_y--)
        {
            if (board.board[X][up_y] == EMPTY_BLOCK)
                break;
            //don't try to make a word if it doesn't end a word
            if (board.in_bounds(X, up_y-1))
                if (state[X][up_y-1])
                    continue;
            for (size_t down_y=Y; down_y<=board.DIM-1; down_y++)
            {
                if (board.board[X][down_y] == EMPTY_BLOCK)
                    break;
                //don't try to make a word if it doesn't end a word
                if (board.in_bounds(X, down_y+1))
                    if (state[X][down_y+1])
                        continue;
                
                string word_contents;
                vector<int> letters_played_this_turn;
                for (size_t j=up_y; j<=down_y; j++)
                {
                    word_contents += board.board[X][j];
                    letters_played_this_turn.push_back(!state[X][j]);
                }
                
                if (dictionary.count(word_contents))
                {
                    word_t word(X, up_y, vertical, word_contents, letters_played_this_turn);
                    words.push_back(word);
                }
            }
        }
    }
}

void get_all_words_from_location(size_t loc_x,
                                 size_t loc_y,
                                 vector<word_t> &words,
                                 bool** state)
{
    get_all_words_from_location_with_direction(loc_x, loc_y, false, words, state); //horizontal
    get_all_words_from_location_with_direction(loc_x, loc_y, true , words, state); //vertical
}

void get_all_possible_words_from_board_state(vector<word_t> &words_from_this_state,
                                             bool** state,
                                             const list<point> &tiles_to_be_covered)
{
    for (auto itr=tiles_to_be_covered.begin(); itr!=tiles_to_be_covered.end(); itr++)
    {
        size_t i = itr->x;
        size_t j = itr->y;
        
        if (!state[i][j] && board.board[i][j]!=EMPTY_BLOCK)
        {
            if (!xy_borders_played_square(i, j, state))
                continue;
            
            vector<word_t> possible_words_from_here;
            get_all_words_from_location(i, j, possible_words_from_here, state);
            
            for (size_t index=0; index<possible_words_from_here.size(); index++)
            {
                word_t word = possible_words_from_here[index];
                words_from_this_state.push_back(word);
            }
        }
    }
}

/* Logic:
 for each char in word
 if this char is a new placement
 go left/right (up/down) as far as can and add that word
 
 then, look lengthwise along word (extending it)
 */
void identify_side_effect_words(const word_t &play,
                                const vector<bool> &letters_played,
                                bool** state,
                                vector< vector<word_t> > &possible_side_effects)
{
    vector<point> squares = play.get_locations();
    for (size_t xy=0; xy<squares.size(); xy++)
    {
        if (letters_played[xy])
        {
            vector<word_t> side_effects_from_this_xy;
            get_all_words_from_location_with_direction_for_side_effects
            (squares[xy].x, squares[xy].y, !play.vertical, side_effects_from_this_xy, state);
            
            if (side_effects_from_this_xy.size() != 0)
                possible_side_effects.push_back(side_effects_from_this_xy);
        }
    }
}

/* possible_intersection_side_effects [i] is the set of possible
 * intersections from a particular (x,y) from the played word.
 *
 * This function fills possible_sets_of_intersections, where [i] is
 * a set of possible extensions, as expected. Each set of possible
 * extensions is one extension per (x,y).
 */
void generate_sets_of_intersections(vector< vector<word_t> > &possible_sets_of_intersections,
                                    vector< vector<word_t> > source,
                                    vector<word_t> so_far = vector<word_t>())
{
    if (source.size() == 0)
    {
        if (so_far.size() != 0)
            possible_sets_of_intersections.push_back(so_far);
        return;
    }
    
    vector<word_t> intersections_at_xy = source[0]; //depth of recursion is i^th (x,y)
    source.erase(source.begin());
    
    for (size_t i=0; i<intersections_at_xy.size(); i++)
    {
        vector<word_t> so_far_copy = so_far;
        so_far_copy.push_back(intersections_at_xy[i]);
        
        generate_sets_of_intersections(possible_sets_of_intersections, source, so_far_copy);
    }
}

void generate_turns_from_sets_of_side_effects(vector< vector<word_t> > &sets_of_side_effects,
                                              vector<     turn_t     > &turns_from_play,
                                              const word_t &play,
                                              bool** state)
{
    //if it's extended as far as possible, it needs to be a word
    string ext = extend_word_based_on_state(play.word, state, play.x, play.y, play.vertical);
    if (ext == play.word)
        if (!dictionary.count(play.word))
            return;
    
    for (size_t set=0; set<sets_of_side_effects.size(); set++)
    {
        bool all_are_words = true;
        for (size_t side_word=0; side_word<sets_of_side_effects[set].size(); side_word++)
            if (!dictionary.count((sets_of_side_effects[set])[side_word].word))
                all_are_words = false;
        
        if (all_are_words)
            turns_from_play.push_back(turn_t(play, sets_of_side_effects[set]));
    }
    //if no side-effects, push back word itself
    if (sets_of_side_effects.size() == 0)
        turns_from_play.push_back(turn_t(play, vector<word_t>()));
}

void fill_turnts_with_side_effect_words(vector<turn_t> &turns_from_play,
                                        const word_t &play,
                                        bool** state,
                                        vector<bool> &letters_played)
{
    vector< vector<word_t> > intersection_side_effects;
    identify_side_effect_words(play, letters_played, state, intersection_side_effects);
    
    vector< vector<word_t> > sets_of_intersections;
    generate_sets_of_intersections(sets_of_intersections, intersection_side_effects);
    
    generate_turns_from_sets_of_side_effects(sets_of_intersections, turns_from_play, play, state);
}

vector<turn_t> generate_possible_turns_from_play(const word_t &play,
                                                 bool **state)
{
    vector<bool> letters_played;
    vector<point> squares_covered = play.get_locations();
    for (size_t i=0; i<squares_covered.size(); i++)
    {
        point p = squares_covered[i];
        letters_played.push_back(!state[p.x][p.y]);
    }
    
    vector<turn_t> possible_turns_from_play;
    fill_turnts_with_side_effect_words(possible_turns_from_play, play, state, letters_played);
    
    return possible_turns_from_play;
}

void get_all_possible_turns_from_board_state(vector<turn_t> &possible_turns,
                                             bool** state,
                                             const list<point> &tiles_to_be_covered)
{
    vector<word_t> possible_plays;
    get_all_possible_words_from_board_state(possible_plays, state, tiles_to_be_covered);
    for (size_t p=0; p<possible_plays.size(); p++)
    {
        vector<turn_t> possible_turns_for_this_play = generate_possible_turns_from_play(possible_plays[p], state);
        for (size_t t=0; t<possible_turns_for_this_play.size(); t++)
        {
            if (!contains(possible_turns, possible_turns_for_this_play[t]))
                possible_turns.push_back(possible_turns_for_this_play[t]);
        }
    }
}


bool reconstruct_turns(vector<turn_t> history,
                       bool**         state, //explicitly copied, so "by value"
                       vector<int>    player_scores,
                       PLAYER         player,
                       list<point>    tiles_to_be_covered,
                       map<char, int> letter_tile_counts)
{
    //base case: board has been completed
    if (tiles_to_be_covered.size() == 0 &&
        player_scores == player_final_scores)
    {
        successful_histories.push_back(history);
        return true;
    }
    
    vector<turn_t> turns;
    get_all_possible_turns_from_board_state(turns, state, tiles_to_be_covered); //false is for it not being the first turn
    
    for (size_t t=0; t<turns.size(); t++)
    {
        word_t current_play = turns[t].word_played;
        
        int score = determine_turn_score(turns[t]);
        if (player_scores[player]+score > player_final_scores[player])
            continue;
        vector<int> updated_player_scores(player_scores);
        updated_player_scores[player] += score;
        
        map<char, int> updated_letter_tile_counts(letter_tile_counts);
        bool enough_letters_to_play = update_letter_counts_from_turn(updated_letter_tile_counts, turns[t]);
        if (!enough_letters_to_play)
            continue;
        
        turns[t].score = score;
        turns[t].player = player;
        
        list<point> updated_tiles_to_be_covered(tiles_to_be_covered);
        cover_tiles(updated_tiles_to_be_covered, turns[t]);
        
        vector<turn_t> updated_history(history);
        updated_history.push_back(turns[t]);
        
        bool** updated_state = copy_bool_grid(state, board.DIM);
        update_bool_grid_according_to_play(updated_state, current_play);
        
        bool success = reconstruct_turns(updated_history,
                                         updated_state,
                                         updated_player_scores,
                                         next_player(player),
                                         updated_tiles_to_be_covered,
                                         updated_letter_tile_counts);
        
        free_bool_grid(updated_state, board.DIM);
        
        if (success && false) //generate *all* histories
            return true;
    }
    return false; //implicit base case: 0 possible plays / no possible play returns true
}

void reconstruct_turns_wrapper(vector<int> zeroed_out_player_scores)
{
    bool **blank_state;
    initialize_bool_grid(blank_state, false, board.DIM);
    list<point> all_uncovered_tiles;
    initialize_uncovered_tiles(all_uncovered_tiles, board.DIM);
    
    map<char, int> initial_letter_tile_counts;
    initialize_letter_counts(initial_letter_tile_counts);
    
    vector<word_t> words;
    get_all_words_from_location(board.DIM/2, board.DIM/2, words, blank_state);
    vector<turn_t> turns;
    for (size_t w=0; w<words.size(); w++)
    {
        vector<turn_t> turns_from_word = generate_possible_turns_from_play(words[w], blank_state);
        turns_from_word[0].player = ONE;
        turns.push_back(turns_from_word[0]); //only one possible turn from starting word_t
    }
    
    vector<int> player_scores(zeroed_out_player_scores);
    
    for (size_t t=0; t<turns.size(); t++)
    {
        int score = determine_turn_score(turns[t]);
        if (score > player_final_scores[ONE])
            continue;
        turns[t].score = score;
        player_scores[ONE] = score; //don't need to make copy each time because only one index is modified
        
        map<char, int> letter_tile_counts(initial_letter_tile_counts);
        bool enough_letters_to_play = update_letter_counts_from_turn(letter_tile_counts, turns[t]);
        if (!enough_letters_to_play)
            continue;
        
        vector<turn_t> history;
        history.push_back(turns[t]);
        
        list<point> tiles_to_be_covered(all_uncovered_tiles);
        cover_tiles(tiles_to_be_covered, turns[t]);
        
        //reflect tiles as marked
        bool** state = copy_bool_grid(blank_state, board.DIM);
        update_bool_grid_according_to_play(state, turns[t].word_played);
        
        //search paths from this starting word
        bool success = reconstruct_turns(history,
                                         state,
                                         player_scores,
                                         next_player(ONE),
                                         tiles_to_be_covered,
                                         letter_tile_counts);
        
        free_bool_grid(state, board.DIM);
        
        if (success && false)
            return;
    }
    
    free_bool_grid(blank_state, board.DIM);
}

int main(int argc,
         const char* argv[])
{
    initialize_board(board);
    fill_tile_score_map();
    
    //TODO: verify board is valid, contains correct number of tiles
    
    vector<int> player_scores;
    initialize_players(player_final_scores, player_scores);
    
    PopulateDictionary(dictionary);
    
    reconstruct_turns_wrapper(player_scores);
    
    print_end_messages(successful_histories);
    return 0;
}
