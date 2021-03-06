#pragma once
#include <stdio.h>
#include <iostream>
#include "../Semantics/Type.hpp"
#include "../Semantics/FuncDir.hpp"
#include "../Memory/MemoryFrame.hpp"
#include "../Quadruples/Quadruple.hpp"

using namespace std;

/**
 * This class contains the Operation helper.
 * It manages the operations that are executed by the VM.
 * It also handles the runtime errors.
 */

class OperationHelper{
private:

    int globalMemoryOffset;
    

    MemoryFrame* globalFrame;
    MemoryFrame* currentFrame;

    void callForDivisionbyCeroError(){
        cout<<"RUNTIME ERROR: Division by 0"<<endl;
        exit(0);
    }

    void callForIndexOutOfBounds(){
        cout<<"RUNTIME ERROR: Index Out Of Bounds"<<endl;
        exit(0);
    }

    Type getTypeFromContext(int value){

        Type type;

    

      if (value > 0 ){

          if (!isReference(value)){
                if(value<globalMemoryOffset){
                     return globalFrame->getType(value);
                }else{
                    return currentFrame->getType(value);
                }
          }else{

              return getTypeFromContext(retrieveReferenceValueFromContext(value));

          }

      }
    }

    bool isReference(int value){

        Type type;


        if (value > 0){

            if(value<globalMemoryOffset){
                    type = globalFrame->getType(value);

            }else{
                
                    type = currentFrame->getType(value);

            }

        }

        if(type == REFERENCE_){
         return true;
        }else{
          return false;
        }
    }




    //Retrieve values

    int retrieveIntegerValueFromContext(int value){

        if(isReference(value)){

            return retrieveIntegerValueFromContext(retrieveReferenceValueFromContext(value));
            
        }else{

            if(getScope(value) == GLOBAL_){
                return globalFrame->getIntegerValue(value);
            }else{
                return currentFrame->getIntegerValue(value);
            }
        }



    }

    float retrieveFloatValueFromContext(int value){


        if(isReference(value)){

            return retrieveFloatValueFromContext(retrieveReferenceValueFromContext(value));
            
        }else{
            if(getScope(value) == GLOBAL_){
                return globalFrame->getFloatValue(value);
            }else{
                return currentFrame->getFloatValue(value);
            }
        }

    }

    string retrieveStringValueFromContext(int value){

        if(isReference(value)){

            return retrieveStringValueFromContext(retrieveReferenceValueFromContext(value));
            
        }else{

            if(getScope(value) == GLOBAL_){
                return globalFrame->getStringValue(value);
            }else{
                return currentFrame->getStringValue(value);
            }
        }

    }

    bool retrieveBooleanValueFromContext(int value){

        if(isReference(value)){

            return retrieveBooleanValueFromContext(retrieveReferenceValueFromContext(value));
            
        }else{
            if(getScope(value) == GLOBAL_){
                return globalFrame->getBooleanValue(value);
            }else{
                return currentFrame->getBooleanValue(value);
            }
        }

    }


    int retrieveReferenceValueFromContext(int value){

        if(getScope(value) == GLOBAL_){
            return globalFrame->getReferenceValue(value);
        }else{
            return currentFrame->getReferenceValue(value);
        }

    }


    //Set Values

    void setReferenceValueFromContext(int memDir, int store){
        
        if(getScope(memDir) == GLOBAL_){
                globalFrame->setValue(memDir,store,0);
        }else{
                currentFrame->setValue(memDir,store,0);
        }


    }

    void setValueFromContext(int memDir, int store){

        if (isReference(memDir)){

            
            int memDirValue = retrieveReferenceValueFromContext(memDir);
            setValueFromContext(memDirValue, store);

        }    else{
            if(getScope(memDir) == GLOBAL_){
                globalFrame->setValue(memDir,store);
            }else{
                currentFrame->setValue(memDir,store);
            }
        }    

      


    }

    void setValueFromContext(int memDir, float store){
        
        if(getScope(memDir) == GLOBAL_){
            globalFrame->setValue(memDir,store);
        }else{
            currentFrame->setValue(memDir,store);
        }
    }

    void setValueFromContext(int memDir, string store){
        
        if(getScope(memDir) == GLOBAL_){
            globalFrame->setValue(memDir,store);
        }else{
            currentFrame->setValue(memDir,store);
        }
    }

    void setValueFromContext(int memDir, bool store ){
        
        if(getScope(memDir) == GLOBAL_){
            globalFrame->setValue(memDir,store);
        }else{
            currentFrame->setValue(memDir,store);
        }
    }
    

    DeclarationState getScope(int value){
        DeclarationState scope;

        if(value<globalMemoryOffset){
                return GLOBAL_;

        }else{
            return LOCAL_;

        }

    }
  
    
public:

    OperationHelper(int offset, MemoryFrame* globalFrame_, MemoryFrame* currentFrame_){
        globalMemoryOffset = offset;
        currentFrame = currentFrame_;
        globalFrame = globalFrame_;
    }

    void additionOperation(Quadruple* quad){

        int leftOperand = quad->getLeftOperand();
        int rightOperand = quad->getRightOperand();
        int memDir = quad->getResult();

        Type leftType = getTypeFromContext(leftOperand);
        Type rightType = getTypeFromContext(rightOperand);


        if(leftType==INTEGER_ && rightType == INTEGER_){
            int result = retrieveIntegerValueFromContext(leftOperand) + retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==INTEGER_ && rightType == FLOAT_){
            float result = retrieveIntegerValueFromContext(leftOperand) + retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == INTEGER_){
            float result = retrieveFloatValueFromContext(leftOperand) + retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == FLOAT_){
            float result = retrieveFloatValueFromContext(leftOperand) + retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==STRING_ && rightType == STRING_){
            string result = retrieveStringValueFromContext(leftOperand) + retrieveStringValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }
    }

    void substractionOperation(Quadruple* quad){

        int leftOperand = quad->getLeftOperand();
        int rightOperand = quad->getRightOperand();
        int memDir = quad->getResult();

        Type leftType = getTypeFromContext(leftOperand);
        Type rightType = getTypeFromContext(rightOperand);


        if(leftType==INTEGER_ && rightType == INTEGER_){
            int result = retrieveIntegerValueFromContext(leftOperand) - retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==INTEGER_ && rightType == FLOAT_){
            float result = retrieveIntegerValueFromContext(leftOperand) - retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == INTEGER_){
            float result = retrieveFloatValueFromContext(leftOperand) - retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == FLOAT_){
            float result = retrieveFloatValueFromContext(leftOperand) - retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }
    }

    void multiplicationOperation(Quadruple* quad){

        int leftOperand = quad->getLeftOperand();
        int rightOperand = quad->getRightOperand();
        int memDir = quad->getResult();

        Type leftType = getTypeFromContext(leftOperand);
        Type rightType = getTypeFromContext(rightOperand);


        if(leftType==INTEGER_ && rightType == INTEGER_){
            int result = retrieveIntegerValueFromContext(leftOperand) * retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==INTEGER_ && rightType == FLOAT_){
            float result = retrieveIntegerValueFromContext(leftOperand) * retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == INTEGER_){
            float result = retrieveFloatValueFromContext(leftOperand) * retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == FLOAT_){
            float result = retrieveFloatValueFromContext(leftOperand) * retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }
    }

    void divisionOperation(Quadruple* quad){

        int leftOperand = quad->getLeftOperand();
        int rightOperand = quad->getRightOperand();
        int memDir = quad->getResult();

        Type leftType = getTypeFromContext(leftOperand);
        Type rightType = getTypeFromContext(rightOperand);


        if(leftType==INTEGER_ && rightType == INTEGER_){

            if (retrieveIntegerValueFromContext(rightOperand) == 0)
            callForDivisionbyCeroError();

            float result = retrieveIntegerValueFromContext(leftOperand) / retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==INTEGER_ && rightType == FLOAT_){

            if (retrieveFloatValueFromContext(rightOperand) == 0.0f)
            callForDivisionbyCeroError();

            float result = retrieveIntegerValueFromContext(leftOperand) / retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == INTEGER_){

            if (retrieveIntegerValueFromContext(rightOperand) == 0)
            callForDivisionbyCeroError();
            float result = retrieveFloatValueFromContext(leftOperand) / retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == FLOAT_){
            if (retrieveFloatValueFromContext(rightOperand) == 0.0f)
            callForDivisionbyCeroError();
            float result = retrieveFloatValueFromContext(leftOperand) / retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }
    }

    void assignmentOperation(Quadruple* quad){

        int leftOperand = quad->getLeftOperand();
        int memDir = quad->getResult();

        Type leftType = getTypeFromContext(leftOperand);

        int memOffset = quad->getRightOperand();
        if(memOffset == -1 ){ memOffset = 1; }

        int i = 0;
        while(i<memOffset){

            if(leftType==INTEGER_){
                setValueFromContext(memDir , retrieveIntegerValueFromContext(leftOperand));
            }

            if(leftType==FLOAT_){
                setValueFromContext(memDir , retrieveFloatValueFromContext(leftOperand));
            }

            if(leftType==BOOLEAN_){
                setValueFromContext(memDir , retrieveBooleanValueFromContext(leftOperand));
            }

            if(leftType==STRING_){
                setValueFromContext(memDir , retrieveStringValueFromContext(leftOperand));
            }

            leftOperand++;
            memDir++;
            i++;
        }

    }


    void greaterThanOperation(Quadruple* quad){

        int leftOperand = quad->getLeftOperand();
        int rightOperand = quad->getRightOperand();
        int memDir = quad->getResult();

        Type leftType = getTypeFromContext(leftOperand);
        Type rightType = getTypeFromContext(rightOperand);


        if(leftType==INTEGER_ && rightType == INTEGER_){
            bool result = retrieveIntegerValueFromContext(leftOperand) > retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==INTEGER_ && rightType == FLOAT_){
            bool result = retrieveIntegerValueFromContext(leftOperand) > retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == INTEGER_){
            bool result = retrieveFloatValueFromContext(leftOperand) > retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == FLOAT_){
            bool result = retrieveFloatValueFromContext(leftOperand) > retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }
    }

      void lessThanOperation(Quadruple* quad){

        int leftOperand = quad->getLeftOperand();
        int rightOperand = quad->getRightOperand();
        int memDir = quad->getResult();

        Type leftType = getTypeFromContext(leftOperand);
        Type rightType = getTypeFromContext(rightOperand);

        if(leftType==INTEGER_ && rightType == INTEGER_){
            bool result = retrieveIntegerValueFromContext(leftOperand) < retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==INTEGER_ && rightType == FLOAT_){
            bool result = retrieveIntegerValueFromContext(leftOperand) < retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == INTEGER_){
            bool result = retrieveFloatValueFromContext(leftOperand) < retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == FLOAT_){
            bool result = retrieveFloatValueFromContext(leftOperand) < retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }
    }

     void lessOrEqualsOperation(Quadruple* quad){

        int leftOperand = quad->getLeftOperand();
        int rightOperand = quad->getRightOperand();
        int memDir = quad->getResult();

        Type leftType = getTypeFromContext(leftOperand);
        Type rightType = getTypeFromContext(rightOperand);


        if(leftType==INTEGER_ && rightType == INTEGER_){
            bool result = retrieveIntegerValueFromContext(leftOperand) <= retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==INTEGER_ && rightType == FLOAT_){
            bool result = retrieveIntegerValueFromContext(leftOperand) <= retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == INTEGER_){
            bool result = retrieveFloatValueFromContext(leftOperand) <= retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == FLOAT_){
            bool result = retrieveFloatValueFromContext(leftOperand) <= retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }
    }

    void greaterOrEqualsOperation(Quadruple* quad){

        int leftOperand = quad->getLeftOperand();
        int rightOperand = quad->getRightOperand();
        int memDir = quad->getResult();

        Type leftType = getTypeFromContext(leftOperand);
        Type rightType = getTypeFromContext(rightOperand);


        if(leftType==INTEGER_ && rightType == INTEGER_){
            bool result = retrieveIntegerValueFromContext(leftOperand) >= retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==INTEGER_ && rightType == FLOAT_){
            bool result = retrieveIntegerValueFromContext(leftOperand) >= retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == INTEGER_){
            bool result = retrieveFloatValueFromContext(leftOperand) >= retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == FLOAT_){
            bool result = retrieveFloatValueFromContext(leftOperand) >= retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }
    }

    void equalsEqualsOperation(Quadruple* quad){

        int leftOperand = quad->getLeftOperand();
        int rightOperand = quad->getRightOperand();
        int memDir = quad->getResult();

        Type leftType = getTypeFromContext(leftOperand);
        Type rightType = getTypeFromContext(rightOperand);


        if(leftType==INTEGER_ && rightType == INTEGER_){
           
            bool result = retrieveIntegerValueFromContext(leftOperand) == retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==INTEGER_ && rightType == FLOAT_){
            bool result = retrieveIntegerValueFromContext(leftOperand) == retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == INTEGER_){
            bool result = retrieveFloatValueFromContext(leftOperand) == retrieveIntegerValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }

        if(leftType==FLOAT_ && rightType == FLOAT_){
            bool result = retrieveFloatValueFromContext(leftOperand) == retrieveFloatValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }
    }

    void notOperation(Quadruple* quad){

        int leftOperand = quad->getLeftOperand();
        int rightOperand = quad->getRightOperand();
        int memDir = quad->getResult();

        Type leftType = getTypeFromContext(leftOperand);
        Type rightType = getTypeFromContext(rightOperand);


        if(rightType==BOOLEAN_){
            bool result = !retrieveBooleanValueFromContext(rightOperand);
            setValueFromContext(memDir, result);
        }
    }

    void andOperation(Quadruple* quad){

        int leftOperand = quad->getLeftOperand();
        int rightOperand = quad->getRightOperand();
        int memDir = quad->getResult();

        Type leftType = getTypeFromContext(leftOperand);
        Type rightType = getTypeFromContext(rightOperand);

        if(leftType==BOOLEAN_ && rightType == BOOLEAN_){
            bool result = retrieveBooleanValueFromContext(leftOperand) && retrieveBooleanValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }
    }

    void orOperation(Quadruple* quad){

        int leftOperand = quad->getLeftOperand();
        int rightOperand = quad->getRightOperand();
        int memDir = quad->getResult();

        Type leftType = getTypeFromContext(leftOperand);
        Type rightType = getTypeFromContext(rightOperand);

        if(leftType==BOOLEAN_ || rightType == BOOLEAN_){
            bool result = retrieveBooleanValueFromContext(leftOperand) || retrieveBooleanValueFromContext(rightOperand);
            setValueFromContext(memDir,result );
        }
    }

    int gotoFOperation(Quadruple* quad){


        int leftOperand = quad->getLeftOperand();
        int result= quad->getResult();
        Type leftType = getTypeFromContext(leftOperand);

        if(leftType==BOOLEAN_){
            bool value = retrieveBooleanValueFromContext(leftOperand);
            if( value == false){

                return result;
            }
        }

        return -1;
    }

    void speakOperation(Quadruple* quad){

        int result= quad->getRightOperand();
        Type type = getTypeFromContext(result);

        if(type == STRING_)
        cout<<"SPEAK "<<retrieveStringValueFromContext(result)<<endl;

        if(type == FLOAT_)
        cout<<"SPEAK "<<retrieveFloatValueFromContext(result)<<endl;

        if(type == INTEGER_)
        cout<<"SPEAK "<<retrieveIntegerValueFromContext(result)<<endl;

        if(type == BOOLEAN_)
        cout<<"SPEAK "<<retrieveBooleanValueFromContext(result)<<endl;

    }


    void accelOperation(Quadruple* quad){

        int result= quad->getRightOperand();
        Type type = getTypeFromContext(result);

        if(type == FLOAT_)
        cout<<"MOVE "<<retrieveFloatValueFromContext(result)<<endl;

        if(type == INTEGER_)
        cout<<"MOVE "<<retrieveIntegerValueFromContext(result)<<endl;

    }

    void rotOperation(Quadruple* quad){

        int result= quad->getRightOperand();
        Type type = getTypeFromContext(result);

        if(type == FLOAT_)
        cout<<"ROT "<<retrieveFloatValueFromContext(result)<<endl;

        if(type == INTEGER_)
        cout<<"ROT "<<retrieveIntegerValueFromContext(result)<<endl;

    }

    void jumpOperation(){
        cout<<"JUMP "<<endl;
    }

    void stopOperation(){
        cout<<"STOP "<<endl;
    }


    void assignParameter(FuncNode* functionCalled, MemoryFrame* frameCalled, Quadruple* quad){

        int valueDir = quad->getRightOperand();
        Type typeValue = getTypeFromContext(valueDir);

        int index = quad->getResult();
        int paramDir = functionCalled->getParameterDirAt(index);

        int memOffset = quad->getLeftOperand();
        if(memOffset < 0){ memOffset = 1; }

        MemoryFrame* oldFrame = currentFrame;

        int i = 0;
        while(i<memOffset){
            if(typeValue == INTEGER_){
            int value = retrieveIntegerValueFromContext(valueDir);
            currentFrame = frameCalled;
            setValueFromContext(paramDir, value);
            }

            if(typeValue == FLOAT_){
                float value = retrieveFloatValueFromContext(valueDir);
                currentFrame = frameCalled;
                setValueFromContext(paramDir, value);
            }

            if(typeValue == STRING_){
                string value = retrieveStringValueFromContext(valueDir);
                currentFrame = frameCalled;
                setValueFromContext(paramDir, value);
            }

            if(typeValue == BOOLEAN_){
                bool value = retrieveBooleanValueFromContext(valueDir);
                currentFrame = frameCalled;
                setValueFromContext(paramDir, value);
            }

            currentFrame = oldFrame;
            valueDir++;
            paramDir++;
            i++;
        }


        

    }

    int returnOperation(Quadruple* quad){

        int result = quad->getResult();
        Type type = getTypeFromContext(result);

        int memOffset = quad->getLeftOperand();
        if(memOffset == -1){ memOffset = 1; }

        int returnValue = 0;

        int i = 0;
        while(i<memOffset){                
            if(type == INTEGER_){
                int value = retrieveIntegerValueFromContext(result);
                if(i == 0){ returnValue = globalFrame->registerValue(value); }
            }

            if(type == FLOAT_){
                float value = retrieveFloatValueFromContext(result);
                if(i == 0){ returnValue = globalFrame->registerValue(value); }
            }

            if(type == STRING_){
                string value = retrieveStringValueFromContext(result);
                if(i == 0){ returnValue = globalFrame->registerValue(value); }
            }

            if(type == BOOLEAN_){
                bool value = retrieveBooleanValueFromContext(result);
                if(i == 0){ returnValue = globalFrame->registerValue(value); }
            }
            result++;
            i++;
        }

        return returnValue;

    }

    void verifyOperation(Quadruple* quad){
        int value = quad->getLeftOperand();
        int index = retrieveIntegerValueFromContext(value);
        int size = quad->getResult();

        if(index >= size || index < 0 ){
            callForIndexOutOfBounds();
        }
        
    }

    void directionArrayOperation(Quadruple* quad){


        int value = retrieveIntegerValueFromContext(quad->getLeftOperand());
        int value2 = retrieveIntegerValueFromContext(quad->getRightOperand());
        setReferenceValueFromContext(quad->getResult(), value+value2);


    }

    
};