/******************************************************************************

  randwrite.cc                                                         
                                                                         
  Complete by: Oliver Keh                                                   
                                                                            
  April 7th, 2016

  This file takes a random seed (of length determined by user) from a source
  text and makes a list of the character that follows each occurance of the
  seed. A character is chosen at random from the list and is placed into a
  new text file. Then, that character is added to the end of the seed and 
  the first character of the seed is removed, restarting the process from
  the beginning.

******************************************************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "list.h"

using namespace std;

/*****************************************************************************

 This function checks to see if there are any occurances of the seed in the
 source text, and if the entire seed is found, then the immediately following
 character is added to a list containing other similar characters.

 ****************************************************************************/

void add_following(list & following_char, list & k_chars, list & ls, 
                   size_t count, int k) {
    
    for (size_t i = 0; i < (count - (size_t) k); i++) {
        // counter keeps track of seed index
        size_t character = 0;

	// while loop checks that the seed characters and source are equal
        while (ls.get(i) == k_chars.get(character)) {
            character++;
            i++;  
        }
	
	// if the full seed is found, the next character is added to a list
        if (character == (size_t) k) {
            following_char.add((char) ls.get(i), 0);
        }
    }
}

/*****************************************************************************

 This function chooses a random position in the source file and returns that
 position if it can fit the seed length.

 ****************************************************************************/

size_t random_seed (size_t count, int k) {
    size_t r = rand() % (count - k);
    return r;
}

int main (int argc, char ** argv) {

    srand(time(NULL));          // creates a random number every time used

    ifstream infile(argv[3]);   // input file data is taken from 4th command
    ofstream outfile(argv[4]);  // output destination is taken from 5th command
    int k = atoi(argv[1]);      // seed length is taken from 2nd command
    int length = atoi(argv[2]); // length of output is taken from 3rd command

    size_t count = 0;           // keeps track of length of source text
    char ch;                    // 
    list ls;                    // list of all source text characters
    list k_chars;               // list of seed characters

    int num_comlines = 5;       // number of command lines is set at 5

    // program fails if command line requirement hasnt been met
    if (argc != num_comlines) {
        cout << "Program failed as there weren't enough arguments." << endl;
        exit(1);
    }
    
    // seed length cannot be negative
    if (k < 0) { // 0 is neither positive nor negative, but it is non-negative
        cout << "Program failed because the seed length is negative." << endl;
        exit(1);
    }

    // length of output must be positive
    if (length < 0) {
        cout << "Program failed because the length of desired output is \
                 negative." << endl;
        exit(1);
    }

    // adds all characters in source text into a new list
    while ((ch = infile.get()) != EOF) {
        ls.add(ch, count);
        count++; // keeps track of number of characters in source text
    }
    
    // seed length must be less than source text length
    if (count < (size_t) k) {
        cout << "Program failed because seed length exceeds number of \
                 characters in input file." << endl;
        exit(1);
    }

    // checks to see if the source text can be opened
    if (!infile.is_open()) {
        cout << "Input file does not exist in directory." << endl;
        exit(1);
    }

    // checks to see if the output file can be opened
    if (!outfile.is_open()) {
        cout << "Output file does not exist in directory" << endl;
        exit(1);
    }
    
    size_t rand_seed = random_seed(count, k);

    size_t out_count = 0;       // counter keeps track of length of output file

    // if seed passes source file limit, a new seed is chosen
    while (ls.size() - k < rand_seed) {
        rand_seed = random_seed(count, k);
    }

    // adds series of characters at random position to a list of k length
    for (size_t i = 0; i < (size_t) k; i++) {
        k_chars.add((char) ls.get(rand_seed + i), i);
    }

    // loop fills the output file with the desired length of characters
    while (out_count != (size_t) length) {
        // creates an empty list for the new seed
        list following_char;

        add_following(following_char, k_chars, ls, count, k);

        while (following_char.size() == 0) {
	    // removes all elements from the seed list
	    for (int i = 0; i < k; i++) {
	        k_chars.remove_at(0);
	    }
	    // if the seed position exceed the source size a new seed is chosen
            while (rand_seed > ls.size() - k)
                rand_seed = random_seed(count, k);
	    // new seed characters are added to the list from correct position
            for (int i = 0; i < k; i++) {
                k_chars.add((char) ls.get(rand_seed + i), i);
            }
	    // adds the character following a full seed occurance
            add_following(following_char, k_chars, ls, count, k); 
        }

	// randomly chooses a character from character list
        size_t chosen_char = rand() % following_char.size();

	// adds the randomly chosen character to the output file
        outfile.put(following_char.get(chosen_char));

	// removes the first character of the seed
        k_chars.remove_at(0);

	// adds the randomly chosen character to the end of the seed list
        k_chars.add((char) following_char.get(chosen_char), k - 1);

	// increments the counter by 1
        out_count++;
    }
    
    // closes the output file
    outfile.close();
}