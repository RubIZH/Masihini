#pragma once
#include <stdio.h>
#include <unordered_set>
#include <iostream>
#include <string>
#include <vector>
#include "./VarTable.hpp"
#include "./FuncNode.hpp"




using namespace std;

/**
 * This class contains the Function directory.
 * It is an unordered set containing FuncNodes, the hasher and comparator are handled by the id of the function.
 */

class FuncDir{
private:
    unordered_set<FuncNode, FuncNodeHasher, FuncNodeComparator> funcTable;
    vector <FuncNode*> funcList;
    
    
    
public:
    bool insertNode(FuncNode* node){
        
        auto res = funcTable.insert(*node);
        funcList.push_back(node);
        string message = "";
        

        return res.second;
    }

    FuncNode* getFunc(int index){
        return funcList.at(index);
    }

    vector <FuncNode*>* getFuncList(){
        return &funcList;
    }

    int search(string name){
        
        int count =0;
        for ( auto &i : funcList ) {    

            string id = i->getId();

            if(name.compare(id)==0){
                return count;
            }

            count++;

        }
      return -1;

    }

    
};