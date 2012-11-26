//
//  boolgrid_functions.h
//  Reverse Scrabble 2.0
//
//  Created by Brett Wines on 11/25/12.
//  Copyright (c) 2012 Brett Wines. All rights reserved.
//

#ifndef Reverse_Scrabble_2_0_boolgrid_functions_h
#define Reverse_Scrabble_2_0_boolgrid_functions_h

#include "Board.h"
#include "datastructures.h"

void initialize_bool_grid(bool** &grid,
                          bool init_value,
                          int DIM)
{
    grid = new bool*[DIM];
    for (size_t i=0; i<DIM; i++)
        grid[i] = new bool[DIM];
    
    for (size_t i=0; i<DIM; i++)
        for (size_t j=0; j<DIM; j++)
            grid[i][j] = init_value;
}

//TODO: should be able to make old_grid const
bool** copy_bool_grid(bool** old_grid,
                      int DIM)
{
    bool** new_grid = new bool*[DIM];
    for (int i=0; i<DIM; i++)
        new_grid[i] = new bool[DIM];
    
    for (size_t i=0; i<DIM; i++)
        for (size_t j=0; j<DIM; j++)
            new_grid[i][j] = old_grid[i][j];
    
    return new_grid;
}

void free_bool_grid(bool** grid,
                    int DIM)
{
    for (size_t i=0; i<DIM; i++)
        delete[] grid[i];
    
    delete[] grid;
}

void print_bool_grid(bool** grid,
                     size_t width,
                     size_t height);
void update_bool_grid_according_to_play(bool** grid,
                                        const word_t &play)
{
    vector<point> locations = play.get_locations();
    for (size_t i=0; i<locations.size(); i++)
    {
        point p(locations[i].x, locations[i].y);
        grid[p.x][p.y] = true;
    }
}

void print_bool_grid(bool** grid,
                     size_t width,
                     size_t height)
{
    for (size_t j=0; j<width; j++)
    {
        for (size_t i=0; i<height; i++)
        {
            cout << grid[i][j]; //\in {0,1}
        }
        cout << endl;
    }
}


#endif
