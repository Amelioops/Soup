/* 
 * File:   ProductionRule.h
 * Author: Adrianqm
 *
 * Created on November 17, 2018, 1:57 PM
 */

#ifndef PRODUCTIONRULE_H
#define PRODUCTIONRULE_H

#include<vector>

#include "SubRule.h"

using namespace std;

class ProductionRuleSet{
    //name, <non-terms/terms>
    vector<vector< vector<SubRule> > > ruleSet;//rule[subrule][siderule]=id

    public:
        ProductionRuleSet(vector<vector< vector<SubRule> > > idRule){
            ruleSet = idRule;
        }


        //todo keep track over time -- not particulary important
        int getTotalNumRules(){
            int sum = 0;
            for(auto it = ruleSet.begin(); it != ruleSet.end(); it++){
                sum += getNumSideRules(*it);
            }
            return sum;
        }

        int getNumSubRules(){
            return ruleSet.size();
        }

        int getNumSideRules(vector< vector<SubRule> >& sideRules){
            return sideRules.size();
        }
    
};

#endif /* PRODUCTIONRULE_H */

