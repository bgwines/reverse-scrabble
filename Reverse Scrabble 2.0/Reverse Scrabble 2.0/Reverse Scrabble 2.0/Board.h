//
//  Board.h
//  Reverse Scrabble 2.0
//
//  Created by Brett Wines on 11/25/12.
//  Copyright (c) 2012 Brett Wines. All rights reserved.
//

#ifndef __Reverse_Scrabble_2_0__Board__
#define __Reverse_Scrabble_2_0__Board__

#include <iostream>
#include <vector>

using namespace std;

class Board
{
public:
    int DIM;
    
    Board();
    
    void set_up();
    
    void fill_board(const string &filename);
    
    void print_board();
    
    bool in_bounds(size_t x, size_t y);
    
    char** board;
private:
    void file_read(string filename, vector<string> &lines);
    
    void allocate_grid();
};


#endif /* defined(__Reverse_Scrabble_2_0__Board__) */
