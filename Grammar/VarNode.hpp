//
//  VarNode.hpp
//  CompilerTestProject
//
//  Created by Ruben Hidalgo on 3/8/18.
//  Copyright © 2018 Ruben Hidalgo. All rights reserved.
//

#ifndef VarNode_hpp
#define VarNode_hpp

#include <string>
#include <iostream>
#include "Type.hpp"
#endif /* VarNode_hpp */

using namespace std;




class VarNode{
   private:
        string id;
        Type type;
        int memDir;
    
    
    
    public:
    VarNode( string identifier, Type dataType){
        
        id = identifier;
        type = dataType;
        
    }

    VarNode( string identifier, Type dataType, int memoryDirection){
        
        id = identifier;
        type = dataType;
        memDir = memoryDirection;
        
    }
    
    string getId() const{
        return id;
    }

};


namespace std
{
    template<> struct hash<VarNode>
    {
        size_t
        operator()(const VarNode & obj) const
        {
            return hash<string>()(obj.getId());
        }
    };
    
}

struct VarNodeHasher
{
    size_t
    operator()(const VarNode & obj) const
    {
        return hash<string>()(obj.getId());
    }
};

struct VarNodeComparator
{

    bool
    operator()(const VarNode & obj1, const VarNode & obj2) const
    {
        if (obj1.getId() == obj2.getId())
            return true;
        return false;
    }
};


