
#ifndef UGLYREGTOKENIZER_H
#define UGLYREGTOKENIZER_H

#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>

#include "Alphabet.h"

using namespace std;
    
struct regState{
    bool lineComment = false;
    bool semanticLHS = false;
    
    enum EXPECT{
        lhs,
        assign,
        rhs,
        size
    };
    
    EXPECT expect = lhs;

    vector<char> input;
    vector<string> ops;
    
    vector<string> ids;
    
    const unordered_set<char> pairs1 = {'{', '[', '('};
    const unordered_set<char> pairs2 = {'}', ']', ')'};
    
    const unordered_set<char> assignMod = {'!', '#'};
    const unordered_set<char> assignOp = {'=', '~'};

    const unordered_set<char> opChars = {'{', '}', '[', ']', '(', ')',
    '+', '-',  ',', '*', 
    '!', '$', '/', '@', '#', ':', '\\', '~', '=', ';', '|', '&', '.'
    };
    //Special escape chars
    const unordered_set<char> specialBSlash = {'s'};

    const unordered_set<char> wSpace = {' ', '\n', '\t', '\r', '\f'};
    
    const char endStatementChar = ';';
    
    void nextExpect(){
        expect = (EXPECT)(((int)expect + 1) % (int)size);
    }
    
    void prevExpect(){
        expect = (EXPECT)(((int)expect + (int)size - 1) % (int)size);
    }
    
    void resetExpect(){
        expect = (EXPECT)0;
    }
    
    bool isLineComment(char c){
        if((c == '/' && lineComment) || (expect == lhs && c == '/')){
            lineComment = false;
            //Pop prev /
            if(ids.size() > 0){
                ids.pop_back();
            }
            if(ops.size() > 0){
                ops.pop_back();
            }
            return true;
        }
        else if(c == '/' && !lineComment){
            lineComment = true;
        }
        else if(c != '/' && lineComment){
            lineComment = false;
        }
        return false;
    }
    
    
    void addID(vector<string>& ids, vector<char>& in){
        if(in.size() > 0){
            string id = "";
            while(in.size() > 0){
                id = in.back() + id;
                in.pop_back();
            }
            ids.push_back(id);
        }
    }

    void tokenize(ifstream& ifile, Variables<vector<string> >& alphaVars, Variables<vector<string> >& ruleVars, Variables<vector<string> >& semVars){
        string line = "";
        int currentLine = -1;
        while(getline(ifile, line)){
            currentLine++;
            typedef string::iterator sIt;
            sIt beg = line.begin();
            sIt end = line.end();
            Word<vector<string> >* currentWord;
            while(beg != end){
                //Line comment
                if(isLineComment(*beg)){
                    break;
                }

                if(expect == lhs && *beg == '~'){
                    semanticLHS = true;
                }
                //Add operator
                else if(opChars.find(*beg) != opChars.end() && !semanticLHS){
                    //Add lhs name & look for assignment
                    if(expect == lhs){
                        addID(ids, input);
                        
                        Word<vector<string> >* var = new Word<vector<string> >();
                        
                        var->name = ids.back();
                        ids.pop_back();
                        var->scope = currentLine;
                        currentWord = var;
                        
                        nextExpect();
                    }
                    //Add rule id
                    else if(expect == rhs){
                        addID(ids, input);
                    }
                    
                    //Add operator to input
                    input.push_back(*beg);
                    cout << input.back();
                    
                    //
                    if(expect == assign){
                        if(assignMod.find(*beg) != assignMod.end()){
                            ops.push_back(string(1, *beg));
                            currentWord->varType(*beg);
                        }
                        else if(assignOp.find(*beg) != assignOp.end()){
                            ops.back() += string(1, *beg);
                            
                            switch((int)currentWord->type){
                                case (int)TYPE::ALPHABET:
                                    alphaVars.addVar(currentLine, currentWord->name, currentWord);
                                    break;
                                case (int)TYPE::WORD:
                                    ruleVars.addVar(currentLine, currentWord->name, currentWord);
                                    break;
                                case (int)TYPE::SEMANTIC:
                                    semVars.addVar(currentLine, currentWord->name, currentWord);
                                    break;
                            }

                            input.clear();
                            nextExpect();
                            currentWord->varType(*beg);
                        }
                        else{
                            cout << "\nInvalid assignment operator: " << *beg << endl;
                        }
                    }
                    else if(expect == rhs){
                        addID(ids, input);
                        ops.push_back(string(1, *beg));
                        
                        //Reset to lhs
                        if(*beg == endStatementChar){
                            currentWord->value = ids;
                            ids.clear();
                            nextExpect();
                        }
                    }
                    else{
                        cout << "??" << expect << "??" << endl;
                    }
                    
                }
                //Skip wSpace
                else if(wSpace.find(*beg) != wSpace.end()){
                    addID(ids, input);

                    if(semanticLHS){
                        Word<vector<string> >* var = new Word<vector<string> >();
                        
                        var->name = ids.back();
                        ids.pop_back();
                        var->scope = currentLine;
                        currentWord = var;
                        semVars.addVar(currentLine, currentWord->name, currentWord);

                        //goto rhs
                        nextExpect();
                        nextExpect();
                        semanticLHS = false;
                    }
                }
                //Add char
                else{
                    input.push_back(*beg);
                    cout << input.back();
                }
                
                
                beg++;
            }
        }
    }
    
} STATE;



#endif /*  UGLYREGTOKENIZER_H  */