/* 
 * File:   Alphabet.h
 * Author: Adrianqm
 *
 * Created on November 15, 2018, 9:40 PM
 */

#ifndef ALPHABET_H
#define ALPHABET_H

#include <unordered_set>
#include <map>
#include <string>

using namespace std;

struct Alphabet{
    
    string name;
    
    //Subset alphabets
    unordered_set<string> subsets;
    //[c,c] range
    map<char, char> ranges;
    unordered_set<char> letters;
    
    bool exists(char c){
        if(letters.find(c) != letters.end()){
            return true;
        }
        else{
            for(auto it = ranges.begin(); it != ranges.end(); it++){
                if(c < it->first){
                    return false;
                }
                else{
                    if(c >= it->first && c <= it->second){
                        return true;
                    }
                }
            }
        }
    }
    
    void add(char c){
        letters.insert(c);
    }
    
    void add(char start, char end){
        if(start > end){
            cerr << "Start must be greater than end" << endl;
        }
        else{
           ranges[start] = end;
        }
    }
};

#endif /* ALPHABET_H */

