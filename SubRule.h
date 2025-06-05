#ifndef SUBRULE_H
#define SUBRULE_H

#include<string>
#include<iostream>

using namespace std;

class SubRule{
    //User-defined or generated
    bool generated = false;
    //When User-defined
    string name = "";
    //When generated {subRule, sideRule} in master rule{0,0}
    int pos[2] = {0, 0};

    public:
        SubRule(string name){
            this->name = name;
        }

        SubRule(int subRule, int sideRule){
            generated = true;
            pos[0] = subRule;
            pos[1] = sideRule;
        }

        string getName(){
            if(generated){
                cerr << "\nNo name for rule was generated" << endl;
                return name;
            }
            else{
                return name;
            }
        }

        bool isGenerated(){
            return generated;
        }

        int getSubRule(){
            return pos[0];
        }

        int getSideRule(){
            return pos[1];
        }

};



#endif /* SUBRULE_H */