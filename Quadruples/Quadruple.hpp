#pragma once
#include <stdio.h>
#include <iostream>
#include "../Semantics/Type.hpp"

using namespace std;

/**
 * This class contains the structure of a Quadruple
 * where it has a Operator, a left operand, a right operand and a result
 * The case handles which operator to print in the quadruples output.
 */

class Quadruple{
private:
   
    Operator operator_;
    int leftOperand;
    int rightOperand;
    int result;
    
    
public:

    Quadruple(Operator oper, int leftOper, int rightOper, int res){

        operator_ = oper;
        leftOperand = leftOper;
        rightOperand = rightOper;
        result = res;

    }

    Operator getOperator(){
        return operator_;
    }

    int getLeftOperand(){
        return leftOperand;
    }

    int getRightOperand(){
        return rightOperand;
    }

    int getResult(){
        return result;
    }

    void setResult(int res){
        result = res;
    }


    void print(){

        switch (operator_){

            case ADD_: cout<< "+ "; break;
            case SUBS_: cout<<"- "; break;
            case MULT_: cout<<"* "; break;
            case DIV_: cout<<"/ "; break;
            case GT_: cout<<"> "; break;
            case LT_: cout<<"< "; break;
            case LE_: cout<< "<= "; break;
            case GE_: cout<< ">= "; break;
            case EE_: cout<< "== "; break;
            case NOT_:cout<<"! "; break;
            case AND_: cout<<"& "; break;
            case OR_: cout<< "| "; break;
            case EQ_: cout<< "= "; break;
            case GOTOF_: cout << "GOTOF "; break;
            case GOTO_: cout << "GOTO "; break;
            case ENDPROC_: cout << "ENDPROC "; break;
            case RETURN_: cout << "RETURN "; break;
            case ERA_: cout << "ERA_ "; break;
            case PARAMETER_: cout << "PARAMETER_ "; break;
            case GOSUB_: cout << "GOSUB_ "; break;
            case ENDPROG_: cout << "ENDPROG_ "; break;
            case SPEAK_: cout << "SPEAK_ "; break;
            case ACCEL_: cout << "ACCEL_ "; break;
            case ROT_: cout << "ROT_ "; break;
            case STOP_: cout << "STOP_ "; break;
            case JUMP_: cout << "JUMP_ "; break;
            case POPRES_: cout << "POPRES_"; break;
            case VER_: cout << "VER_"; break;
            case DIR_: cout << "DIR_"; break;
        }

        cout<<"\t\t";

        cout <<leftOperand<<"\t"<<rightOperand<<"\t"<<result<<endl;
        
    }
    

    
};
