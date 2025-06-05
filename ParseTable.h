/* 
 * File:   ParseTable.h
 * Author: Adrianqm
 *
 * Created on November 16, 2018, 6:47 PM
 */

#ifndef PARSETABLE_H
#define PARSETABLE_H

#include<vector>
#include<map>
#include<unordered_set>
#include<string>

using namespace std;

//LR
class ParseTable{
    enum ACTIONS{
        SHIFT,
        REDUCE,
        GOTO,
        ACCEPT
    };
    
    //action, state
    typedef std::tuple<ACTIONS, int> Action;
    
    //map<string, int> symbolTable;
    
    //state, <letter, action>
    map<int, map<char, action> > actionTable;
    //state, <productionRuleName, gotoAction>
    map<int, map<string, action> > gotoTable;
    
    //
    vector<int> stack;
};

#endif /* PARSETABLE_H */

