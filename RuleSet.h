/* 
 * File:   RuleSet.h
 * Author: Adrianqm
 *
 * Created on November 15, 2018, 9:44 PM
 */

#ifndef RULESET_H
#define RULESET_H

//Letters, id to next rule in RuleSet
typedef map< char, int > Rule;
struct _Rule{
    Rule rule;

    _Rule(){
        
    }

    _Rule(Rule r){
        rule = r;
    }
};

//id of rule, rule
struct RuleSet{
    //TokenType type;
    map<int, _Rule*> ruleset;

    RuleSet(){
        //type = t;
    }

    /*void addRule(int id, _Rule *&r){
        ruleset[id] = r;
    }*/
    
    RuleSet operator+(tuple<int, _Rule*> r){
        ruleset[get<0>(r)] = get<1>(r);
        return *this;
    }
    
};

//Generates multiple rules
//Allows alphabet-groups & left-factoring
struct RuleMacro{
    Rule rules;

    RuleMacro* addRule(char c, int id){
        rules[c] = id;
    }

    /*RuleMacro* addRule(tuple<Alphabet, int>&... rs){
        vector<tuple<Alphabet, int> > rss(args);
        rs = rss[0];
            Alphabet alpha = get<0>(r);
            int id = get<1>(r);
            for(map< Alphabet, int >::iterator macro = alphas.begin(); macro != alphas.end(); macro++){
                for(Alphabet::const_iterator letter = macro->first.begin(); letter != macro->first.end(); letter++){
                    rules[*letter] = macro->second;
                }
            }
    }*/
    
    RuleMacro operator+(tuple<Alphabet, int> r){
        Alphabet alphas = get<0>(r);
        int id = get<1>(r);
        std::cout << "\nImplement pls\tRuleMacro operator+(tuple<Alphabet, int> r)" << endl;
        /*for(Alphabet::const_iterator letter = alphas.begin(); letter != alphas.end(); letter++){
            rules[*letter] = id;
        }*/
        
        return *this;
    }
    
    RuleMacro operator+(tuple<char, int> r){
        char letter = get<0>(r);
        int id = get<1>(r);
        rules[letter] = id;
        
        return *this;
    }
    
    static std::tuple<int, _Rule*> productionTuple(int id, _Rule *r){
    return make_tuple(id, r);
    }

    static std::tuple<Alphabet, int> ruleTuple(Alphabet a, int id){
        return make_tuple(a, id);
    }

    static std::tuple<char, int> ruleTuple(char letter, int id){
        return make_tuple(letter, id);
    }

};

#endif /* RULESET_H */

