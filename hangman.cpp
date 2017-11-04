/********************
* COMP11
* Homework WORDS_PER_TURN
* Eberechukwu Justin
*********************/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <set>
#include <cctype>
#include "print.h"


using namespace std;

//populates an array with WORDS_PER_TURN ordered, unique random numbers
//Parameters: x is the array of size WORDS_PER_TURN, y is the number of words in dictionary
void generate_rand_lines(int x[], int y);

//populates an array with WORDS_PER_TURN random words from the dictionary
//Parameters: x is the array of size WORDS_PER_TURN for the words,
//y is an array of WORDS_PER_TURN random numbers
//dict is the dictionary file
void generate_rand_words(string x[], int y[]);

//contains the hang_man game play code
//takes a one-word string as argument
//returns true if user won, and false if user failed
bool game_play(string word);

//number of words in the dictionary
const int DICT_LENGTH = 19912;
const int MAX_FAIL_COUNT = 9;
const int WORDS_PER_TURN = 5;
int main()
{
    int play_time = 1,  //tracks how many times the game has been played
        win_count =0 , play_again;

    cout << "Welcome to Hangman!!!\n"
         << "Guess all letters corectly or little Mr. Hangie gets hanged\n"
         << "Goodluck! His life depends on it\n\n";

    //seeds the random variable
    srand(time(NULL));
    do
    {
        int rand_lines[WORDS_PER_TURN];
        bool won_game;
       
        generate_rand_lines(rand_lines, DICT_LENGTH);

        string hang_words[WORDS_PER_TURN];
        
        generate_rand_words(hang_words, rand_lines);

        int hang_index = 0;
        do
        {
            string hang_word = hang_words[hang_index];
            won_game = game_play(hang_word);

            if(won_game)
            {
                cout << "You guessed it!!  " << hang_word << endl;
                win_count++;
            }
            else
                cout << "You lose!\n"
                     << "The word was " << hang_word << endl;

            cout << "You have won " << win_count << " games\n\n";
            hang_index++;
            play_time++;
            cout << "Enter 1 for Next word or 0 to end game: ";
            cin >> play_again;
        }
        //generates new random words after every WORDS_PER_TURN plays
        while(play_again == 1 && play_time % WORDS_PER_TURN != 1);
    }
    while (play_time % WORDS_PER_TURN == 1 && play_again == 1); 
    return 0;
}

//generates an array of WORDS_PER_TURN ordered random numbers between 1 and y
//Parameters: an EMPTY array of size WORDS_PER_TURN. 
//int y shows the value  upper boundary of random numbers
//Result:Array x now contains WORDS_PER_TURN ordered random numbers
void generate_rand_lines(int x[], int y)
{
    set<int> random_lines;
    for (int i = 0; i < WORDS_PER_TURN; ++i)
    {
        int word_line = rand()%y + 1;
        random_lines.insert(word_line);
    }

    set<int>::iterator iter;
    int arr_index = 0;
    for (iter = random_lines.begin(); iter != random_lines.end(); ++iter)
    {
        x[arr_index] = *iter;
        arr_index++;
    }
}

//generates an array of WORDS_PER_TURN random dictionary words
//Parameters: x =>an EMPTY array of strings of size WORDS_PER_TURN. 
//y => an array of WORDS_PER_TURN random numbers
//Result:Array x now contains WORDS_PER_TURN random dictionary words
void generate_rand_words(string x[], int y[])
{
    //open dictionary for checking
    ifstream dictionary;
    dictionary.open("dictionary.txt");

    int file_count = 0, //tracks words checked in the file
        word_count = 0; //tracks words added to hang_words array
    
    //add random words to array
    if(dictionary.is_open())
    {
        while (!dictionary.eof() && word_count < WORDS_PER_TURN)
        {
            string temp;
            dictionary >> temp;

            if(file_count == y[word_count])
            {
                x[word_count] = temp;
                word_count++;
            }
            file_count++;
        }
    }
    dictionary.close();
}

//code for taking user guesses and checking if its correct or not
//Parameters: A string to be checked against user input
//Result: Keeps prompting user for guesses till he guesses the word or fails 9 
//times. Returns true if user won, false if he lost
bool game_play(string word)
{
    int false_count = 0, true_count = 0;
    string display = "_ ";
    for (size_t i = 1; i < word.size(); ++i)
    {
        display+="_ ";
    }
    
    cout << "Guess a letter: " ;
    do
    {
        char guess;

        do
        {
            cout << display << endl;
            cin >> guess;
        }
        while (!isalpha(guess));
        
        bool is_correct = false;
        for (size_t i = 0; i < word.size(); ++i)
        {
            if (tolower(guess) == word[i])
            {
                is_correct = true;
                //display[i*2] to account for the spaces in the string
                if(display[i*2] != word[i])
                {
                    true_count++;
                    display[i*2] = word[i];
                }
            }
        }

        if (!is_correct)
        {
            print_body_parts(false_count);
            cout << "The letter is not in the word\n"
                 << "You've guessed " << false_count + 1<< " letters wrongly\n";
            false_count++;
            if (false_count == MAX_FAIL_COUNT)
                return false;
        }
        else
            cout << "Correct letter!\n";
        if (true_count == (int)word.size())
            return true;

    }
    while(true);

}
