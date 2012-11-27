//
//  util.h
//  Reverse Scrabble 2.0
//
//  Created by Brett Wines on 11/25/12.
//  Copyright (c) 2012 Brett Wines. All rights reserved.
//

#ifndef Reverse_Scrabble_2_0_util_h
#define Reverse_Scrabble_2_0_util_h

#include<algorithm>
#include<vector>

void PopulateDictionary(set<string> &dictionary) {
	std::ifstream myfile;
    myfile.open (dictionary_file_name);
    if (!myfile.is_open())
    {
        cout << "Unable to open dictionary file: " << dictionary_file_name << endl;
        cout.flush();
        exit(0);
    }
    
	insert_iterator<set<string> > itr(dictionary, dictionary.begin());
	copy(istream_iterator<string>(myfile), istream_iterator<string>(), itr);
	
	myfile.close();
	return;
}

template<typename T>
bool contains(const vector<T> &v, const T &elem)
{
    for (size_t i=0; i<v.size(); i++)
        if (v[i] == elem)
            return true;
    
    return false;
}

static void print_vector(const vector<word_t> &v)
{
    cout << "number of starting words found: " << v.size() << endl;
    for (int i=0; i<v.size(); i++)
        cout << v[i].to_string() << endl;
}

void print_history(const vector<turn_t> &history,
                   bool write_to_file)
{
    write_to_file = false;
    ofstream myfile;
    
    for (int i=0; i<history.size(); i++)
    {
        cout << "\tTurn " << i+1 << ": ";
        cout << history[i].to_string() << endl;
    }
}

void print_end_messages(const vector< vector<turn_t> > &histories)
{
    size_t size = histories.size();
    if (size != 0)
    {
        cout << "Success! " << size << " possible histories found.\n";
        
        for (size_t i=0; i<size; i++)
        {
            cout << "\nPossible history #" << i+1 << "/" << size
                 << " (size " << histories[i].size() << "):\n";
            print_history(histories[i], true);
        }
    }
    else
    {
        cout << "No possible history found.\n";
    }
}

#endif
