#pragma once
#include <stdio.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <functional>
#include <numeric>
#include "../Quadruples/Quadruple.hpp"
#include "../Semantics/FuncDir.hpp"
#include "../Semantics/Dimension.hpp"
#include "OperationHelper.hpp"


/**
 * This class contains the Virtual Machine
 * It manages the information from the generated quadruples and executed as designed.
 * It sends the operations to the operation helper and when finished it exits the program
 * and outputs the result of code.
 */

using namespace std;

class VirtualMachine{
private:

    //Execution Environment
    vector<Quadruple*>* quadrupleSet;
    vector<FuncNode*>* functionDirectory;
    int execPointer = 0;
    stack<int> execPointerPositionStack;
    stack <MemoryFrame*> memoryStack;
    MemoryFrame* globalMemoryFrame;
    MemoryFrame* currentFrame;
    MemoryFrame* calledFrame;
    FuncNode* calledFunction;
    int globalMemoryOffset;

    void init(){
        FuncNode* mainFuncNode = functionDirectory->at(functionDirectory->size()-1);
        memoryStack.push(mainFuncNode->getMemoryFrame());
        currentFrame = memoryStack.top();
    }

    void goto_(Quadruple* quad){

        int result = quad->getResult();
        execPointer = result;

    }

    void gotoF_(Quadruple* quad){

        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        int res = helper.gotoFOperation(quad);

        if(res != -1){
            execPointer = res;
        }else{
            execPointer++;
        }

    }

    void exitExec(){

        //cout<<"Execution Sucessful!"<<endl;
        exit (0);

    }


    void printQuads(){

      int count = 0;
      for ( auto &i : *quadrupleSet ) {

            cout <<count<< ".-\t";
            i->print();
            count++;
      }

    }

    void callForArrayReturnMismatch(string id, string id2){
        cout<<"RUNTIME ERROR: Return value \"" + id + "\" and var \"" + id2 + "\" don't have the number dimensions"<<endl;
        exit(0);
    }

    Quadruple* getReturnValueFromFunc( Quadruple* quad){

            if (quad->getLeftOperand()>0 && quad->getRightOperand()>0){
                return quad;
            }else{

                Operator op = quad->getOperator();
                int left = quad->getLeftOperand();
                int right = quad->getRightOperand();
                int result = quad->getResult();

                
                if(quad->getLeftOperand()<=0 && quad->getRightOperand()<=0){
                    int indexL = quad->getLeftOperand()*-1;
                    FuncNode* funcL = functionDirectory->at(indexL);
                    Dimension* dimL = funcL->getReturnValue();
                    int memDirL = dimL->getR();

                    int indexR = quad->getRightOperand()*-1;
                    FuncNode* funcR = functionDirectory->at(indexR);
                    Dimension* dimR = funcR->getReturnValue();
                    int memDirR = dimR->getR();

                    return new Quadruple(op, memDirL, memDirR, result);

                }

                if(quad->getLeftOperand()<=0 ){

                    int indexL = quad->getLeftOperand()*-1;
                    FuncNode* funcL = functionDirectory->at(indexL);
                    Dimension* dimL = funcL->getReturnValue();
                    int memDirL = dimL->getR();


                    return new Quadruple(op, memDirL, right, result);

                }

                if(quad->getRightOperand()<=0 ){

                    int indexR = quad->getRightOperand()*-1;
                    FuncNode* funcR = functionDirectory->at(indexR);
                    Dimension* dimR = funcR->getReturnValue();
                    int memDirR = dimR->getR();


                    return new Quadruple(op, left, memDirR, result);

                }
            }
    }


    void assignment(Quadruple* quad){
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
         if (quad->getLeftOperand()>0){
            helper.assignmentOperation(quad);
         }else{
            int index = quad->getLeftOperand()*-1;
            FuncNode* func = functionDirectory->at(index);
            Dimension* returnDim = func->getReturnValue();
            int memDir = returnDim->getR();
            int numberOfDimensions = func->getNumberOfDimensions();

            if(numberOfDimensions > 0){
                //Get return value dimensions
                vector<int> returnDimensions = returnDim->getDimensionSize();
                int returnDimNum = returnDimensions.size();
                int memOffset = accumulate (returnDimensions.begin(), returnDimensions.end(), 1, multiplies<int>());
                Quadruple* tempQuad = new Quadruple(EQ_,memDir, memOffset, quad->getResult());
                helper.assignmentOperation(tempQuad);
                delete tempQuad;
                
            }
            else{
                Quadruple* tempQuad = new Quadruple(EQ_,memDir, -1, quad->getResult());
                helper.assignmentOperation(tempQuad);
                delete tempQuad;
            }
         }

    }

    void addition(Quadruple* quad){

            quad = getReturnValueFromFunc(quad);
            OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
            helper.additionOperation(quad);


    }

    void substraction(Quadruple* quad){

        quad = getReturnValueFromFunc(quad);
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.substractionOperation(quad);

    }

    void multiplication(Quadruple* quad){

        quad = getReturnValueFromFunc(quad);
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.multiplicationOperation(quad);

    }

    void division(Quadruple* quad){

        quad = getReturnValueFromFunc(quad);
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.divisionOperation(quad);

    }

    void greaterThan(Quadruple* quad){

        quad = getReturnValueFromFunc(quad);
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.greaterThanOperation(quad);

    }

    void lessThan(Quadruple* quad){

        quad = getReturnValueFromFunc(quad);
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.lessThanOperation(quad);

    }

    void lessOrEquals(Quadruple* quad){

        quad = getReturnValueFromFunc(quad);
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.lessOrEqualsOperation(quad);

    }

    void greaterOrEquals(Quadruple* quad){

        quad = getReturnValueFromFunc(quad);
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.greaterOrEqualsOperation(quad);

    }

    void equalsEquals(Quadruple* quad){

        quad = getReturnValueFromFunc(quad);
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.equalsEqualsOperation(quad);

    }

    void notOp(Quadruple* quad){

        quad = getReturnValueFromFunc(quad);
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.notOperation(quad);

    }

    void andOp(Quadruple* quad){

        quad = getReturnValueFromFunc(quad);
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.andOperation(quad);

    }

    void orOp(Quadruple* quad){

        quad = getReturnValueFromFunc(quad);
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.orOperation(quad);

    }

    void speak(Quadruple* quad){
        quad = getReturnValueFromFunc(quad);


        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.speakOperation(quad);

    }

    void accel(Quadruple* quad){
        quad = getReturnValueFromFunc(quad);
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.accelOperation(quad);

    }

    void rot(Quadruple* quad){
        quad = getReturnValueFromFunc(quad);
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.rotOperation(quad);

    }

    void stopOp(){

        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.stopOperation();

    }

    void jump(){

        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.jumpOperation();

    }

    void eraOp(Quadruple* quad){
        int index = quad->getResult();
        calledFunction = functionDirectory->at(index);
        calledFrame = new MemoryFrame(calledFunction->getMemoryFrame());
    }

    void parameterOp(Quadruple* quad){
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.assignParameter(calledFunction, calledFrame, quad);
    }

    void gosubOp(){
        memoryStack.push(calledFrame);
        currentFrame = memoryStack.top();
        execPointerPositionStack.push(execPointer);
        execPointer = calledFunction->getStartingInstruction();
        
    }

    void endProc(){


        

        MemoryFrame* oldMem = memoryStack.top();
        memoryStack.pop();

        delete oldMem;

        if(!memoryStack.empty()){

            currentFrame = memoryStack.top();



            execPointer = execPointerPositionStack.top();
            execPointerPositionStack.pop();
            execPointer++;

        }else{
            exitExec();
        }

        
    }

    void retOp(Quadruple* quad){
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        int memDir = helper.returnOperation(quad);
        int index = quad->getRightOperand();

        FuncNode* func = functionDirectory->at(index);

        int returnDir = quad->getResult();
        VarTable* symbolTable = func->getSymbolTable();
        string returnID = symbolTable->getIdFromMemoryContext(returnDir);
        Dimension returnDimension = symbolTable->getDimensionInformation(returnID);
        vector<int> returnDimensions = returnDimension.getDimensionSize();

        func->setReturnValue(memDir, returnDimensions);
        endProc();
    }

    void popRes(Quadruple* quad){

        int index = quad->getResult();
        FuncNode* func = functionDirectory->at(index);
        func->getReturnValue();

    }

    void verify(Quadruple* quad){
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.verifyOperation(quad);
    }

    void dirArray(Quadruple* quad){
        OperationHelper helper(globalMemoryOffset, globalMemoryFrame, currentFrame);
        helper.directionArrayOperation(quad);
    }


    
public:
    
    VirtualMachine(vector <Quadruple*>* quadSet, vector<FuncNode*>* funcDir, MemoryFrame* globalMemFrame, int offset ){
       
            quadrupleSet = quadSet;
            functionDirectory = funcDir;
            globalMemoryFrame = globalMemFrame;
            globalMemoryOffset = offset;

            //cout<<"------------------------ Exec output---------------------"<<endl;
            //printQuads();
            init();
            run();

    }


    void run(){

        while (true) {

            Quadruple* quad = quadrupleSet->at(execPointer);
            Operator operator_ = quad->getOperator();

            //Any action that alters exec pointer does not add to the counter
            switch (operator_){
                case ADD_: addition(quad); execPointer++; break; 
                case SUBS_: substraction(quad); execPointer++; break;
                case MULT_: multiplication(quad); execPointer++; break;
                case DIV_: division(quad); execPointer++; break;
                case GT_: greaterThan(quad); execPointer++; break;
                case LT_: lessThan(quad); execPointer++; break;
                case LE_: lessOrEquals(quad); execPointer++; break;
                case GE_: greaterOrEquals(quad); execPointer++; break;
                case EE_: equalsEquals(quad); execPointer++; break;
                case NOT_: notOp(quad); execPointer++; break;
                case AND_: andOp(quad); execPointer++; break;
                case OR_: orOp(quad); execPointer++; break;
                case SPEAK_: speak(quad); execPointer++; break;
                case ACCEL_: accel(quad); execPointer++; break;
                case ROT_: rot(quad); execPointer++; break;
                case JUMP_: jump(); execPointer++; break;
                case STOP_: stopOp(); execPointer++; break;
                case EQ_: assignment(quad); execPointer++; break;
                case ERA_: eraOp(quad); execPointer++; break;
                case POPRES_: popRes(quad); execPointer++; break;
                case PARAMETER_: parameterOp(quad); execPointer++; break;
                case GOSUB_: gosubOp(); break;
                case GOTOF_: gotoF_(quad); break; 
                case GOTO_: goto_(quad); break; 
                case RETURN_: retOp(quad); break; 
                case VER_: verify(quad); execPointer++; break;
                case DIR_: dirArray(quad); execPointer++; break;
                case ENDPROC_: endProc(); break;
                case ENDPROG_: exitExec(); break;
            }


        }

    }


};