#pragma once

#include <iostream>


enum Type
{
    STRING_, 
    BOOLEAN_,
    INTEGER_,
    FLOAT_,
    VOID_
};

enum Operator
{
    ADD_,//Ready
    SUBS_,//Ready
    MULT_,//Ready
    DIV_,//Ready
    GT_,
    LT_,
    LE_,
    GE_,
    EE_,
    NOT_,
    AND_,
    OR_,
    EQ_,//Ready
    FAKE_BTTM_,
    //Non Expression-Related Operators
    GOTOF_,
    GOTO_,
    ENDPROC_,
    RETURN_,
    ERA_,
    PARAMETER_,
    GOSUB_,
    ENDPROG_
};

enum DeclarationState
{
    GLOBAL_,
    LOCAL_

};