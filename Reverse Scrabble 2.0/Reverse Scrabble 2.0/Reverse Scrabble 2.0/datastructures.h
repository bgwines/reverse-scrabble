//
//  datastructures.h
//  Reverse Scrabble 2.0
//
//  Created by Brett Wines on 11/25/12.
//  Copyright (c) 2012 Brett Wines. All rights reserved.
//

#ifndef Reverse_Scrabble_2_0_datastructures_h
#define Reverse_Scrabble_2_0_datastructures_h

#include <sstream>
#include <vector>

#include "constants.h"

using namespace std;

static size_t N_PLAYERS;

enum PLAYER
{
    ONE,
    TWO,
    THREE,
    FOUR
};

size_t player_to_sizet (PLAYER player)
{
    switch (player) {
        case ONE   : return 0;
        case TWO   : return 1;
        case THREE : return 2;
        case FOUR  : return 3;
        default    : return 0;
    }
}

PLAYER sizet_to_player (size_t sizet)
{
    switch (sizet) {
        case 0 : return ONE;
        case 1 : return TWO;
        case 2 : return THREE;
        case 3 : return FOUR;
        default: return ONE;
    }
}

PLAYER next_player(PLAYER player)
{
    return sizet_to_player((player_to_sizet(player)+1)%N_PLAYERS);
}

size_t* get_n_players()
{
    return &N_PLAYERS;
}

struct point
{
    size_t x;
    size_t y;
    
    point(size_t x,
          size_t y)
    : x (x)
    , y (y)
    {}
    
    bool operator== (const point &other)
    {
        return other.x == this->x
        && other.y == this->y;
    }
    
    bool operator< (const point &other)
    {
        return other.x < this->x;
    }
};

struct word_t {
    string word;
    size_t x;
    size_t y;
    bool vertical;
    vector<int> letters_played; // \in {0,1}*
    
    word_t(size_t       x,
           size_t       y,
           bool         vertical,
           string       word,
           vector<int> letters_played)
    : x              (x)
    , y              (y)
    , vertical       (vertical)
    , word           (word)
    , letters_played (letters_played)
    {}
    
    word_t(const word_t& other)
    : x              (other.x)
    , y              (other.y)
    , vertical       (other.vertical)
    , word           (other.word)
    , letters_played (other.letters_played)
    {}
    
    string to_string() const
    {
        stringstream sx, sy;
        
        string str = "{(";
        
        sx << this->x;
        str += sx.str();
        str += ",";
        sy << this->y;
        str += sy.str();
        str += "), ";
        
        if (this->vertical)
            str += "v., ";
        else
            str += "h., ";
        
        str += this->word + "}";
        
        return str;
    }
    
    vector<point> get_locations() const
    {
        vector<point> locations;
        for (int i=0; i<word.length(); i++)
        {
            point p(x + i*vertical, y + i*!vertical);
            locations.push_back(p);
        }
        return locations;
    }
    
    bool operator== (const struct word_t &other) const
    {
        bool equals_ignoring_verticality =
        other.x              == this->x
        && other.y              == this->y
        && other.word           == this->word
        && other.letters_played == this->letters_played;
        
        if ((this->word.length()==1 &&
             other.word.length()==1))
        {
            return equals_ignoring_verticality;
        } else {
            return equals_ignoring_verticality
            && other.vertical == this->vertical;
        }
    }
    
    bool operator!= (const struct word_t &other) const
    {
        return !(*this == other);
    }
};
    
struct turn_t {
    vector<word_t> words_played_by_side_effect;
    
    word_t word_played;
    
    int score;
    PLAYER player;
    
    turn_t(word_t word_played,
           vector<word_t> words_played_by_side_effect)
    : word_played                 (word_played)
    , words_played_by_side_effect (words_played_by_side_effect)
    {}
    
    string to_string() const
    {
        string str;
        
        stringstream s1;
        s1 << (player_to_sizet(player) + 1);
        str += "\n\t\tPlayer: \t\t\t\t" + s1.str();
        
        stringstream s;
        s << score;
        str += "\n\t\tScore: \t\t\t\t" + s.str();
        
        str += "\n\t\tLetters played: \t\t\[";
        for (size_t i=0; i<word_played.word.length(); i++)
        {
            if (word_played.letters_played[i])
            {
                str += word_played.word[i];
                str += ",";
            }
        }
        str += "]\n\t\t";
        
        str += "Main word played: \t\t" + word_played.to_string() + "";
        
        if (words_played_by_side_effect.size() != 0)
        {
            str += "\n\t\tSide-effect words played:\t";
            //str += "\n\t\tWords played by side-effect:";
            for (size_t i=0; i<words_played_by_side_effect.size(); i++)
                str += words_played_by_side_effect[i].to_string() + "\n\t\t\t\t\t\t\t";
        }
        
        return str;
    }
    
    bool operator== (const struct turn_t &other) const
    {
        return other.player                      == this->player
        && other.words_played_by_side_effect == this->words_played_by_side_effect
        && this->word_played                 == other.word_played;
        
    }
};

#endif
