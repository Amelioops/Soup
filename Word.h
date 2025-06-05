#ifndef WORD_H
#define WORD_H

enum TYPE{
    INT,

    CHAR,
    STRING,
    
    SYMBOL,
    
    ALPHABET,
    WORD,
    SEMANTIC
};

#include <string>

using namespace std;

//Attribute container
template <typename T> 
class Word{
public:
    int scope;
    string name;
    
    //Modifier mod;
    TYPE type;
    T value;
    
    Word(){
        scope = 0;
        name = "";
        type = (TYPE)0;
        value = T();
    }
    
    //
    void varType(char mod){
        switch(mod){
            case '!':
                type = WORD;
                break;
            case '#':
                type = ALPHABET;
                break;
            case '~':
                type = SEMANTIC;
                break;
        }
    }
};

#endif /*WORD_H*/