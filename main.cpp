
/* 
 * File:   main.cpp
 * Author: Adrianqm
 *
 * Created on November 1, 2018, 8:43 PM
 */

#include <cstdlib>
#include <cstdarg>

#include <unordered_set>
#include <unordered_map>
#include <string>

#include <iostream>
#include <iomanip>

#include <fstream>
#include <map>
#include <vector>
#include <tuple>

#include <ios>

using namespace std;

//typedef unordered_set<char> Alphabet;

//Deprecated?
/*enum TokenType{
    Junk,  //junk - so D isn't same as (int)NULL

    Ignore_Delimeter,
    Delimeter,

    Sentence_Delimeter_Default,

    Symbol123, 
    Operator,  

    Pair
};*/

struct AlphabetGen{
    /*void generate(vector<char> &letters, Alphabet &alphabet){
        for(int i = 0; i < letters.size(); i++){
            alphabet[letters[i]] = static_cast<int>(letters[i]);
        }
    }*/
};


#include "Alphabet.h"

#include "RuleSet.h"

//*********************
//Semantics

/*enum Attribute{
    INT,
    STRING,
    FLOAT,

    VAR,
    FUNC

};

//Defines what needs to surround this symbol
struct Phrase{

};

//
struct Word{
    //From children
    Attribute synth;
    //From parent/sibling
    Attribute inher;

    string name;

    //Use template magic instead
    void* value;
};*/



void simpleLexer(string line, Alphabet alphabet, RuleSet test){
    typedef string::iterator sIt;
    sIt beg = line.begin();
    sIt ends = line.end();
    //Alphabet. ::iterator letterId;
    //TokenType lastToken = Junk;
    //SentenceType st = Init;
    while(beg < ends){
        //cout << "\"" << *beg << "\"" << endl;
        while(alphabet.exists(*beg) || *beg != ' ' && *beg != '\0'){
	    string out = "";
            
            try{
                RuleSet currentRuleSet = test;
                
                //Init with starting rule
                bool letterId = alphabet.exists(*beg);
                if(!letterId){throw out_of_range("Letter not in alphabet");}
                //cout << "S: " << *letterId << endl;
                int nextRule = currentRuleSet.ruleset.at(0)->rule.at(*beg);
                out += *beg;

                //Go to next rule
                while(nextRule != -1){
                    beg++;
                    
                    letterId = alphabet.exists(*beg);
                    if(!letterId){throw out_of_range("Letter not in alphabet");}
                    //cout << *letterId << " in " << nextRule << endl;
                    nextRule = currentRuleSet.ruleset.at(nextRule)->rule.at(*beg);
                    if(nextRule != -1){
                        out += *beg;
                    }
                }
                
                cout << "A: ";
                
            }catch(out_of_range e){
                cout << "R: ";// << "\'" << static_cast<int>(*beg) << "\' ";
                
                //Go to next token - not end of string && (include unknown value || non-delimeter)
                while(beg < ends && (!alphabet.exists(*beg) || *beg != ' ')){
                    out += *beg;
                    beg++;
                }
            }
            
            //Print token that was accepted or rejected
            cout << "\"" << out << "\"" << endl;
	}
        //cout << "E: " << *beg << endl;
        //Skip delimeters
        beg++;
    }
}

void runSimpleLexerTest(){
    ifstream ifile(string("test-data.txt").c_str());
  
    string line = "";
    getline(ifile, line);   
    ifile.close();
    
    //unordered_set
    Alphabet alphabet;
    alphabet.letters = {'a', 'b', 'c', ';', ' ', '\0'};

    //int end = *alphabet.find(' ');
    char end = ' ';
    RuleSet test = RuleSet();
    test = test + 
    //ac | bc
    RuleMacro::productionTuple(0, new _Rule(
        (RuleMacro() +
            RuleMacro::ruleTuple('a', 3) +
            RuleMacro::ruleTuple('b', 2)
        ).rules)) +
    //bc
    RuleMacro::productionTuple(1, new _Rule(
        (RuleMacro() +
            RuleMacro::ruleTuple('b', 7) +
            RuleMacro::ruleTuple(end, -1)
        ).rules)) +  
    RuleMacro::productionTuple(7, new _Rule(
        (RuleMacro() +
            RuleMacro::ruleTuple('c', 8) +
            RuleMacro::ruleTuple(end, -1)
        ).rules)) +  
    RuleMacro::productionTuple(8, new _Rule(
        (RuleMacro() +
            RuleMacro::ruleTuple('b', 2) +
            RuleMacro::ruleTuple('c', 7) +
            RuleMacro::ruleTuple(end, -1)
        ).rules)) +  
    //cb
    RuleMacro::productionTuple(2, new _Rule(
        (RuleMacro() +
            RuleMacro::ruleTuple('c', 1) +
            RuleMacro::ruleTuple(end, -1)
        ).rules)) +     
    //cc
    RuleMacro::productionTuple(3, new _Rule(
        (RuleMacro() +
            RuleMacro::ruleTuple('c', 3) +
            RuleMacro::ruleTuple(end, -1)
        ).rules));
    
    //;     
    RuleSet endSet = RuleSet();
    endSet = endSet + RuleMacro::productionTuple(0, new _Rule((RuleMacro() + RuleMacro::ruleTuple(';', -1)).rules));

    simpleLexer(line, alphabet, test);
}

#include"Word.h"
#include"Variables.h"

#include"ugly_regTokenizer.h"

#include"ProductionRule.h"

#include"SubRule.h"

struct SemanticFunction{
    vector< vector<string> > statements;
};

int main(int argc, char** argv) {
    Variables<vector<string> > alphaVars;
    Variables<vector<string> > ruleVars;
    Variables<vector<string> > semVars;
    
    
    //Tokenizer
    ifstream ifile(string("test-reg-simple.txt").c_str());
    STATE.tokenize(ifile, alphaVars, ruleVars, semVars);
    ifile.close();
    cout << endl;

    Variables<Alphabet> initializedAlphas;
    //Variables<> initializedProductions;
    //Variables<Alphabet> initializedSemantics;

    vector<ProductionRuleSet*> allProductionRules;
    
    //Parse-Table-Supplier
    for(auto it = alphaVars.varsOrder.begin(); it != alphaVars.varsOrder.end(); it++){
        cout << setw(3) << left << it->first << " Var: " << 
                setw(2) << left << alphaVars.get(it->second)->type << " " << 
                setw(16) << left << alphaVars.get(it->second)->name << " == ";
        for(int i = 0; i < alphaVars.get(it->second)->value.size(); i++){
            cout << left << alphaVars.get(it->second)->value[i] << " ";
        }
        cout << endl;
        
        if(alphaVars.get(it->second)->type == TYPE::ALPHABET){
            Word<Alphabet>* w = new Word<Alphabet>();
            w->scope = alphaVars.get(it->second)->scope;
            w->name = alphaVars.get(it->second)->name;
            w->type = alphaVars.get(it->second)->type;
            
            Alphabet a;
            a.name = w->name;
                       
            for(auto tok = alphaVars.get(it->second)->value.begin(); tok != alphaVars.get(it->second)->value.end(); tok++){
                if(alphaVars.contains(*tok)){
                    //alpha id
                    a.subsets.insert(initializedAlphas.get(*tok)->value.name);
                    ++tok;//#
                }
                else if(tok->compare("{") == 0){
                    char startC = (++tok)->c_str()[0];
                    ++tok;//,
                    char endC = (++tok)->c_str()[0];
                    a.add(startC, endC);
                    //++tok;//}
                }
                else{
                    a.add(tok->c_str()[0]);
                }
            }
            
            w->value = a;
            
            initializedAlphas.addVar(w->scope, w->name, w);
        }
        else{
            cout << "hwat0: " << it->second << endl;
        }
    }
    alphaVars.clear();

    //
    for(auto it = ruleVars.varsOrder.begin(); it != ruleVars.varsOrder.end(); it++){
        cout << "Generating production rules for:\t" << it->second << endl;
        if(ruleVars.get(it->second)->type == TYPE::WORD){
            
            //ProductionRule[0][0] and side-rules[0][n] and sub-rules[x][0]
            vector<vector< vector<SubRule> > > idRule;
            idRule.push_back(vector<vector<SubRule> >());
            idRule.at(0).push_back(vector<SubRule>());
            int subRule = 0;

            auto prods = &(ruleVars.get(it->second)->value);//shorthand

            vector<char> lastPair;
            vector<int> atSubRule = {0};
            vector<int> atSideRule = {0};

            bool semanticBody = false;

            for(int tok = 0; tok < prods->size(); tok++){
                char op = prods->at(tok).c_str()[0];
                
                if((op == '[' || op == '(') && !semanticBody){
                    lastPair.push_back(op);
                    //Add reference to subrule in this rule -- make wrapper for string
                    SubRule sub(++subRule, 0);
                    idRule.at(atSubRule.back()).at(atSideRule.back()).push_back(sub);
                    //Create sub-rule
                    atSubRule.push_back(subRule);//was ++ now done earlier
                    atSideRule.push_back(0);
                    idRule.push_back(vector<vector<SubRule> >());
                    idRule.at(subRule).push_back(vector<SubRule>());
                }
                else if((op == ']' || op == ')') && !semanticBody){
                    //prev pair
                    lastPair.pop_back();
                    //Goto prev rule -- Always pop in pairs
                    atSubRule.pop_back();
                    atSideRule.pop_back();
                }
                else if(op == '$'){
                    semanticBody = !semanticBody;
                    //inc subRule++?????
                }
                else if(op == '*' && !semanticBody){
                    //Get prev SubRule
                    SubRule prev = idRule.at(atSubRule.back()).at(atSideRule.back()).back();
                    //Pop
                    idRule.at(atSubRule.back()).at(atSideRule.back()).pop_back();
                    //Point to subrule to recurrence
                    SubRule sub(++subRule, 0);
                    idRule.at(atSubRule.back()).at(atSideRule.back()).push_back(sub);

                    //Create sub-rule
                    atSubRule.push_back(subRule);
                    atSideRule.push_back(0);
                    idRule.push_back(vector<vector<SubRule> >());
                    idRule.at(subRule).push_back(vector<SubRule>());

                    //Add prev-from-parent & recurrence to subRule
                    idRule.at(atSubRule.back()).at(atSideRule.back()).push_back(prev);//this x

                    //Create side-rule
                    atSubRule.push_back(atSubRule.back());
                    atSideRule.push_back(idRule.at(atSubRule.back()).size());
                    idRule.at(subRule).push_back(vector<SubRule>());

                    idRule.at(atSubRule.back()).at(atSideRule.back()).push_back(SubRule(""));//no loop

                    //Go back to recurrence
                    atSubRule.pop_back();
                    atSideRule.pop_back();

                    //Point to loop in orig-siderule
                    idRule.at(atSubRule.back()).at(atSideRule.back()).push_back(SubRule(++subRule, 0));//sub to (x | $)

                    //Create sub-rule
                    //atSubRule.push_back(subRule);
                    //atSideRule.push_back(0);
                    idRule.push_back(vector<vector<SubRule> >());
                    idRule.at(subRule).push_back(vector<SubRule>());

                    //Loop back to parent
                    idRule.at(subRule).at(0).push_back(SubRule(atSubRule.back(), atSideRule.back()));

                    //Create side-rule
                    atSubRule.push_back(atSubRule.back());
                    atSideRule.push_back(idRule.at(atSubRule.back()).size());
                    idRule.at(subRule).push_back(vector<SubRule>());

                    //End loop
                    idRule.at(subRule).at(0).push_back(SubRule(""));

                    //Go back to recurrence
                    atSubRule.pop_back();
                    atSideRule.pop_back();
                    //Go back to parentRule
                    atSubRule.pop_back();
                    atSideRule.pop_back();
                }
                else if(op == '+' && !semanticBody){
                    //Get prev SubRule
                    SubRule prev = idRule.at(atSubRule.back()).at(atSideRule.back()).back();
                    //Pop
                    idRule.at(atSubRule.back()).at(atSideRule.back()).pop_back();
                    //Point to subrule to recurrence
                    SubRule sub(++subRule, 0);
                    idRule.at(atSubRule.back()).at(atSideRule.back()).push_back(sub);

                    //Create sub-rule
                    atSubRule.push_back(subRule);
                    atSideRule.push_back(0);
                    idRule.push_back(vector<vector<SubRule> >());
                    idRule.at(subRule).push_back(vector<SubRule>());

                    //Add prev-from-parent & recurrence to subRule
                    idRule.at(atSubRule.back()).at(atSideRule.back()).push_back(prev);//this x
                    idRule.at(atSubRule.back()).at(atSideRule.back()).push_back(SubRule(++subRule, 0));//sub to (x | $)

                    //Create sub-rule
                    //atSubRule.push_back(subRule);
                    //atSideRule.push_back(0);
                    idRule.push_back(vector<vector<SubRule> >());
                    idRule.at(subRule).push_back(vector<SubRule>());

                    //Loop back to parent
                    idRule.at(subRule).at(0).push_back(SubRule(atSubRule.back(), atSideRule.back()));

                    //Create side-rule
                    atSubRule.push_back(atSubRule.back());
                    atSideRule.push_back(idRule.at(atSubRule.back()).size());
                    idRule.at(subRule).push_back(vector<SubRule>());

                    //End loop
                    idRule.at(subRule).at(0).push_back(SubRule(""));

                    //Go back to recurrence
                    atSubRule.pop_back();
                    atSideRule.pop_back();
                    //Go back to parentRule
                    atSubRule.pop_back();
                    atSideRule.pop_back();
                }
                else if(op == '|' && !semanticBody){
                    //Create side-rule (of root rule????nope)
                    atSubRule.push_back(atSubRule.back());
                    atSideRule.push_back(idRule.at(atSubRule.back()).size());
                    idRule.at(atSubRule.back()).push_back(vector<SubRule>());
                }
                else if(op == '?' && !semanticBody){
                    //Get parentRule
                    vector<SubRule> prev = idRule.at(atSubRule.back()).at(atSideRule.back());
                    //Create copy in side-rule without last SubRule - makes optional
                    atSubRule.push_back(atSubRule.back());
                    atSideRule.push_back(idRule.at(atSubRule.back()).size());
                    idRule.at(subRule).push_back(vector<SubRule>());
                    idRule.at(atSubRule.back()).at(atSideRule.back()) = prev;
                    idRule.at(atSubRule.back()).at(atSideRule.back()).pop_back();
                    //Go back to parentRule
                    atSubRule.pop_back();
                    atSideRule.pop_back();
                }
                else if(op == '!' && !semanticBody){
                    //Prev non-term has type WORD
                }
                else if(op == '#' && !semanticBody){
                    //Prev non-term has type ALPHABET
                }
                else if(semanticBody){
                    //Semantic stuff
                }
                else if(lastPair.size() == 0 || lastPair.back() == '('){
                    //Add non-term
                    SubRule nonTerm(prods->at(tok));
                    idRule.at(atSubRule.back()).at(atSideRule.back()).push_back(nonTerm);
                }
                else if(lastPair.back() == '['){
                    //Add non-term
                    SubRule nonTerm(prods->at(tok));
                    idRule.at(atSubRule.back()).at(atSideRule.back()).push_back(nonTerm);
                    //Create side-rule
                    atSubRule.push_back(atSubRule.back());
                    atSideRule.push_back(idRule.at(atSubRule.back()).size());
                    idRule.at(subRule).push_back(vector<SubRule>());
                }
                else{
                    cout << "Went wrong:" << (*prods)[tok] << endl;
                }
            }

            ProductionRuleSet *prodRule = new ProductionRuleSet(idRule);

            allProductionRules.push_back(prodRule);

        }
        else{
            cout << "Not a TYPE::WORD: " << it->second << endl;
        }
    }

    struct SemantStatement{

    };

    struct Semant{
        vector<SubRule> pattern;
        vector<SemantStatement> bodyStatements;
    };

    struct Tokenizer_State_Semantic{


    } SEMSTATE;

    cout << "Num Rules: " << allProductionRules.size() << endl;
    for(auto it = allProductionRules.begin(); it != allProductionRules.end(); it++){
        //cout << "SubRules: " << (*it)->getNumSubRules() << endl;
        cout << "Side&Sub: " << (*it)->getTotalNumRules() << endl;
    }


    for(auto it = semVars.varsOrder.begin(); it != semVars.varsOrder.end(); it++){
        cout << "Semantic func:\t"<< it->second << endl;
    }
    
    cout << "----end----" << endl;
    
    return 0;
}


