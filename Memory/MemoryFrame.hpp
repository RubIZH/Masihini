#pragma once
#include <stdio.h>
#include <map>
#include <string>
#include "MemoryDispatcher.hpp"
#include "../Semantics/Type.hpp"

using namespace std;

class MemoryFrame{
    private:
    MemoryDispatcher <int>* integerMemoryDispatcher = new MemoryDispatcher<int>(0, 1999); //from 0 to 1999 are integers
    MemoryDispatcher <float>* floatMemoryDispatcher = new MemoryDispatcher<float>(2000,1999); //from 200 to 3999 are float
    MemoryDispatcher <std::string>* stringMemoryDispatcher = new MemoryDispatcher<string>(4000, 1999); //from 4000 to 5999 are string
    MemoryDispatcher <bool>* booleanMemoryDispatcher = new MemoryDispatcher<bool> (6000,2000);//from 6000 to 8000 are boolean
 
    
    
    public:
    MemoryFrame(){
    }


//Registering values
    int registerValue(int value){
        int res =  integerMemoryDispatcher->insert(value);
        return res;
    }

    int registerValue(float value){
        int res =  floatMemoryDispatcher->insert(value);
        return res;
    }

    int registerValue(string value){
        int res =  stringMemoryDispatcher->insert(value);
        return res;
    }

    int registerValue(bool value){
        int res =  booleanMemoryDispatcher->insert(value);
        return res;
    }


//Declaring values


    int declareValue(Type type){

        switch(type){

            //set default values for a new variable
            case INTEGER_ : return integerMemoryDispatcher->insert(0);
            case FLOAT_ : return floatMemoryDispatcher->insert(0.0f); 
            case STRING_ : return stringMemoryDispatcher->insert("");
            case BOOLEAN_: return booleanMemoryDispatcher->insert(true);
            default : return -1;

        }

    }

//Get type from memory dir

Type getType(int memDir){


    if(memDir >=0 && memDir <=1999) return INTEGER_;
    if(memDir >=2000 && memDir <=3999) return FLOAT_;
    if(memDir >=4000 && memDir <=5999) return STRING_;
    if(memDir >=6000 && memDir <=8000) return BOOLEAN_;
    
    return VOID_;


}

//Get value from memory  direction    
    template <typename T>
    inline T getValue(int memDir, Type type){
        switch(type){

            case INTEGER_ : return integerMemoryDispatcher->getValue(memDir);
            case FLOAT_ : return floatMemoryDispatcher->getValue(memDir); 
            case STRING_ : return stringMemoryDispatcher->getValue(memDir);
            case BOOLEAN_: return booleanMemoryDispatcher->getValue(memDir);
            default : return -1;

        }
    }




    ~MemoryFrame(){
        
    }

};

