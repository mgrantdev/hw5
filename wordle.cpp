// For debugging
#include <iostream>
// For std::remove
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// Add prototypes of helper functions here
std::set<std::string> wordleHelper(std::set<std::string> &guesses, std::vector<char> givenLetters, std::set<char> &floatingLetters);
std::string convertToString(std::vector<char> v);
std::set<std::string> setIntersection(std::set<std::string> &s1, std::set<std::string> &s2);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string &in,
    const std::string &floating,
    const std::set<std::string> &dict)
{

    // convert floating letters into set
    std::set<char> floatingLetters;
    for (unsigned int i = 0; i < floating.length(); i++)
    {
        floatingLetters.insert(floating[i]);
    }

    // convert floating letters into set
    std::vector<char> givenLetters;
    for (unsigned int i = 0; i < in.length(); i++)
    {
        givenLetters.push_back(in[i]);
    }

    // get combinations
    std::set<std::string> guesses;
    wordleHelper(guesses, givenLetters, floatingLetters);

    // check against dictionary
    std::set<std::string> dict2 = dict;
    std::set<std::string> validWords = setIntersection(guesses, dict2);
    return validWords;
}

// Define any helper functions here
std::set<std::string> wordleHelper(std::set<std::string> &guesses, std::vector<char> givenLetters, std::set<char> &floatingLetters)
{

    // @summary
    int index = 0;
    for (std::vector<char>::iterator it = givenLetters.begin(); it != givenLetters.end(); ++it)
    {
        // skip non blank character
        const char letter = *it;
        if (letter != '-')
        {
            index++;
            continue;
        }

        // try all combinations of floating letters at this index
        for (std::set<char>::iterator it2 = floatingLetters.begin(); it2 != floatingLetters.end(); ++it2)
        {
            const char floatingLetter = *it2;
            std::vector<char> givenLettersCopy = givenLetters;
            givenLettersCopy[index] = floatingLetter;
            wordleHelper(guesses, givenLettersCopy, floatingLetters);
        }

        index++;
    }

    // @condition If all letters
    std::string s = convertToString(givenLetters);
    //std::cout << s << std::endl;
    guesses.insert(s);

    return guesses;
}

std::string convertToString(std::vector<char> v)
{
    std::string s = "";
    for (std::vector<char>::iterator it = v.begin(); it != v.end(); ++it)
    {
        s += *it;
    }
    return s;
}

std::set<std::string> setIntersection(std::set<std::string> &s1, std::set<std::string> &s2)
{
    std::set<std::string>::iterator it;
    std::set<std::string> s3;
    std::string item;
    for (it = s1.begin(); it != s1.end(); ++it)
    {
        item = *it;
        // if item also exists in set2, add to new set
        if (s2.find(item) != s2.end())
        {
            s3.insert(item);
        }
    }

    for (it = s2.begin(); it != s2.end(); ++it)
    {
        item = *it;
        // if item also exists in set2, add to new set
        if (s1.find(item) != s1.end())
        {
            s3.insert(item);
        }
    }

    return s3;
}
