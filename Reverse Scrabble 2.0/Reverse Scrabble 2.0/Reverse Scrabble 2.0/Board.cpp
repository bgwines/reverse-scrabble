//
//  Board.cpp
//  Reverse Scrabble 2.0
//
//  Created by Brett Wines on 11/25/12.
//  Copyright (c) 2012 Brett Wines. All rights reserved.
//

#include <fstream>
#include <sstream>
#include <assert.h>

#include "Board.h"
#include "constants.h"

Board::Board()
{
    allocate_grid();
}

void Board::set_up()
{
    if (board == NULL)
        allocate_grid();
}

void Board::allocate_grid()
{
    this->board = new char*[this->DIM];
    for (size_t i=0; i<this->DIM; i++)
    {
        this->board[i] = new char[this->DIM];
    }
}

bool Board::in_bounds(size_t x, size_t y)
{
    return (int)x >= 0
        && (int)y >= 0
        && (int)x < this->DIM
        && (int)y < this->DIM;
}

void Board::file_read(string filename,
                      vector<string> &lines)
{
    string line;
    ifstream myfile (filename);
    if (myfile.is_open())
    {
        while (myfile.good())
        {
            getline (myfile,line);
            if (line.length() >= 1)
            {
                if (line[0] != '#' &&
                    line[0] != '%')
                {
                    lines.push_back(line);
                    this->DIM = (int)line.length();
                }
                if (line[0] == '%')
                {
                    break;
                }
            }
        }
		myfile.close();
    }
    else
    {
        cout << "Unable to open file";
        assert(false);
    }
}

void Board::fill_board(const string &filename)
{
    vector<string> lines;
    file_read(filename, lines);
    allocate_grid();
    
    for (size_t i=0; i<this->DIM; i++)
    {
        for (size_t j=0; j<this->DIM; j++)
        {
            this->board[i][j] = lines[i][j];
        }
    }
}

void Board::print_board()
{
    for (size_t i=0; i<this->DIM; i++)
    {
        for (size_t j=0; j<this->DIM; j++)
        {
            cout << this->board[i][j];
        }
        cout << endl;
    }
}
