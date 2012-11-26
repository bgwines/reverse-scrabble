//
//  initialization_functions.h
//  Reverse Scrabble 2.0
//
//  Created by Brett Wines on 11/25/12.
//  Copyright (c) 2012 Brett Wines. All rights reserved.
//

#ifndef Reverse_Scrabble_2_0_initialization_functions_h
#define Reverse_Scrabble_2_0_initialization_functions_h

#include "globals_wrt_logic_files.h"

static string GetLine()
{
	string result;
	getline(cin, result);
	return result;
}

/* Fills board with file contents */
void initialize_board(Board &board)
{
    string filename;
    cout << "Enter the path to directory containing the \nboard file (or just hit enter to use \"sample_board.txt\":";
    filename = GetLine();
    if (filename == "")
    {
        filename = "/Users/brettwines/Library/Developer/Xcode/DerivedData/Reverse_Scrabble_2.0-heihxcyeadornsbxaxwmfxmmvdqj/Build/Products/Debug/sample_board.txt";
    }
    
    board.fill_board(filename);
    board.set_up();
    board.print_board();
}

void initialize_uncovered_tiles(list<point> &all_uncovered_tiles,
                                int DIM)
{
    for (size_t i=0; i<DIM; i++)
    {
        for (size_t j=0; j<DIM; j++)
        {
            if (board.board[i][j] != EMPTY_BLOCK)
            {
                all_uncovered_tiles.push_back(point(i,j));
            }
        }
    }
}

/* Initializes players' scores */
void initialize_players(vector<int> &player_final_scores,
                        vector<int> &player_ongoing_scores,
                        string filename = "/Users/brettwines/Library/Developer/Xcode/DerivedData/Reverse_Scrabble_2.0-heihxcyeadornsbxaxwmfxmmvdqj/Build/Products/Debug/sample_board.txt")
{
    bool initialized_N_PLAYERS = false;
    string line;
    ifstream myfile (filename);
    if (myfile.is_open())
    {
        while (myfile.good())
        {
            getline (myfile,line);
            if (line.length() >= 1)
            {
                if (line[0] == '%')
                {
                    if (!initialized_N_PLAYERS)
                    {
                        *get_n_players() = atoi(line.substr(1).c_str());
                        initialized_N_PLAYERS = true;
                    }
                    else
                        player_final_scores.push_back(atoi(line.substr(1).c_str()));
                }
            }
		}
		myfile.close();
    }
    
    for (int i=0; i<*get_n_players(); i++)
    {
        player_ongoing_scores.push_back(0);
    }
}

void initialize_letter_counts(map<char, int> &letter_counts)
{
    letter_counts['a'] = 9;
	letter_counts['b'] = 2;
	letter_counts['c'] = 2;
	letter_counts['d'] = 4;
	letter_counts['e'] = 12;
	letter_counts['f'] = 2;
	letter_counts['g'] = 3;
	letter_counts['h'] = 2;
	letter_counts['i'] = 9;
	letter_counts['j'] = 1;
	letter_counts['k'] = 1;
	letter_counts['l'] = 4;
	letter_counts['m'] = 2;
	letter_counts['n'] = 6;
	letter_counts['o'] = 8;
	letter_counts['p'] = 2;
	letter_counts['q'] = 1;
	letter_counts['r'] = 6;
	letter_counts['s'] = 4;
	letter_counts['t'] = 6;
	letter_counts['u'] = 4;
	letter_counts['v'] = 2;
	letter_counts['w'] = 2;
	letter_counts['x'] = 1;
	letter_counts['y'] = 2;
	letter_counts['z'] = 1;
}


void score_letters(map<char, int> &letter_scores) {
	letter_scores['a'] = 1;
	letter_scores['b'] = 3;
	letter_scores['c'] = 3;
	letter_scores['d'] = 2;
	letter_scores['e'] = 1;
	letter_scores['f'] = 4;
	letter_scores['g'] = 2;
	letter_scores['h'] = 4;
	letter_scores['i'] = 1;
	letter_scores['j'] = 8;
	letter_scores['k'] = 5;
	letter_scores['l'] = 1;
	letter_scores['m'] = 3;
	letter_scores['n'] = 1;
	letter_scores['o'] = 1;
	letter_scores['p'] = 3;
	letter_scores['q'] = 10;
	letter_scores['r'] = 1;
	letter_scores['s'] = 1;
	letter_scores['t'] = 1;
	letter_scores['u'] = 1;
	letter_scores['v'] = 4;
	letter_scores['w'] = 4;
	letter_scores['x'] = 8;
	letter_scores['y'] = 4;
	letter_scores['z'] = 10;
}

#endif
