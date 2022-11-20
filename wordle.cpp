// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    std::cout << in << std::endl;
    std::cout << floating << std::endl;
    wordleHelper(in, dict);

}

// Define any helper functions here
std::set<str::string> wordleHelper(const std::string& in, const std::string& floating, const std::set<std::string>& dict) {
    return
}
