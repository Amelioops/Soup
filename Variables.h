#ifndef VARIABLES_H
#define VARIABLES_H

template <typename T>
class Variables{
public:
    map<int, string> varsOrder;
    unordered_map<string, Word<T>* > varsList;

    Word<T>* get(string name){
        return varsList.at(name);
    }

    bool contains(const string& name){
        return (varsList.find(name) != varsList.end());
    }

    void addVar(int lineNumber, const string& name, Word<T>*& w){
        if(this->contains(name)){
            cout << "\nVariable already defined: " << name << endl;
        }
        else{
            varsOrder[lineNumber] = name;
            varsList[name] = w;
        }
    }

    void clear(){
        varsOrder.clear();
        varsList.clear();
    }
};

#endif /*VARIABLES_H*/