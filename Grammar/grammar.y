%{
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string>
    #include <iostream>
    #include <stack>
    #include <vector>
    #include "Semantics/FuncDir.hpp"
    #include "Memory/MemoryFrame.hpp"
    #include "Semantics/SemanticRuleSet.hpp"
    #include "Quadruples/Quadruple.hpp"

    using namespace std;

    int yylex ();
    void yyerror (char const *);
    extern int yylineno;

    //Semantic functions
    void performSemantics();
    void performSemanticsNot();
    void performSemanticsAssignment();
    void manageMemoryVarCte(Type type, char value);
    void printQuads();


    //Functions that handle errors

    void callForLocalRedefinitionError(string message);
    void callForGlobalRedefinitionError(string message);
    void callForNonDeclaredVariableError(string message);
    void callForTypeMismatchError(string message);

      //Parameters used to store values in Func Directory
      DeclarationState declarationState = GLOBAL_;
      Type currentDeclaredtype = VOID_;
      FuncNode *currentDeclaredFunction ;
      VarTable *globalSymbolTable = new VarTable();
      FuncDir *functionDirectory = new FuncDir();

      //Parameters used to assign memory to items;

      MemoryFrame *globalMemoryFrame = new MemoryFrame();

      //Stack used for Code Generation

      stack <int> stackOperand;
      stack <Operator> stackOperator;
      stack <int> pendingJumps;

      //Set of global semantic considerations
      SemanticRuleSet *semantics ;

      //Global Quadruple Vector
      vector<Quadruple*> quadrupleSet;

      
      

%}

/*UNIONS DEFINITION*/

%union
{
    int intValue;
    float floatValue;
    char *stringValue;
}


/* TOKENS */

%token <floatValue>   FLOAT
%token <intValue>     INT
%token <stringValue>  ID
%token <stringValue>  STRING
%token TRUE
%token FALSE
%token IF
%token ELSE
%token WHILE
%token VAR
%token FUNC
%token TYPE_INT
%token TYPE_FLOAT
%token TYPE_BOOLEAN
%token TYPE_STRING
%token L_BRACE
%token R_BRACE
%token L_PARENTHESIS
%token R_PARENTHESIS
%token L_BRACKET
%token R_BRACKET
%token ADD
%token SUBS
%token MULT
%token DIV
%token EQ
%token GT
%token LT
%token LE
%token GE
%token EE
%token AND
%token OR
%token NOT
%token SEMICOLON
%token COLON
%token COMMA
%token RETURN
%token STATIC
%token RUN
%token VOID
%token LOCAL
%token SYSTEM_PREFIX


%start global_declaration


%%

/* Grammar Rules */


global_declaration : STATIC declaration global_declaration  {declarationState = GLOBAL_;}
                    |  func_declaration
                    ;

declaration : VAR ID COLON type array SEMICOLON 
                                                { 
                                                      if(declarationState == GLOBAL_){
                                                            int memDir = globalMemoryFrame->declareValue(currentDeclaredtype);
                                                            callForLocalRedefinitionError(globalSymbolTable->insertNode(new VarNode($2, currentDeclaredtype, memDir))); 
                                                      }else{
                                                            int memDir = globalMemoryFrame->declareValue(currentDeclaredtype);
                                                            VarTable *symbolTable = currentDeclaredFunction->getSymbolTable();
                                                            callForLocalRedefinitionError(symbolTable->insertNode(new VarNode($2, currentDeclaredtype, memDir))); 
                                                            callForGlobalRedefinitionError(globalSymbolTable->isContained($2, currentDeclaredtype));
                                                      }
                                                }
            ;

func_declaration : {declarationState = LOCAL_;} func func_declaration
                 | run
                 ;

func : FUNC VOID {currentDeclaredtype = VOID_;}  func_0 
     | FUNC type func_0
     ;

func_0 :    ID    {
                        //Function definition
                        currentDeclaredFunction = new FuncNode($1, currentDeclaredtype, new VarTable(), new MemoryFrame());
                        callForLocalRedefinitionError(functionDirectory->insertNode(currentDeclaredFunction));
                  }


            L_PARENTHESIS func_1 R_PARENTHESIS local_declaration 
       ;

func_1 : ID COLON type {
                              int memDir = globalMemoryFrame->declareValue(currentDeclaredtype);
                              VarTable *symbolTable = currentDeclaredFunction->getSymbolTable();
                              callForLocalRedefinitionError( symbolTable->insertNode(new VarNode($1, currentDeclaredtype,memDir))); 
                              callForGlobalRedefinitionError(globalSymbolTable->isContained($1, currentDeclaredtype));
                        }
         func_2 
       |
       ;

func_2 : COMMA ID COLON type {
                                    int memDir = globalMemoryFrame->declareValue(currentDeclaredtype);
                                    VarTable *symbolTable = currentDeclaredFunction->getSymbolTable();
                                    callForLocalRedefinitionError(symbolTable->insertNode(new VarNode($2, currentDeclaredtype,memDir)));   
                              } 
      func_2 
       |
       ;

local_declaration : declaration local_declaration
                  | block
                  ;

run : STATIC FUNC VOID RUN L_PARENTHESIS R_PARENTHESIS      {
                                                                  currentDeclaredFunction = new FuncNode("run", VOID_, new VarTable(), new MemoryFrame());
                                                                  callForLocalRedefinitionError(functionDirectory->insertNode(currentDeclaredFunction));
                                                            } 
      local_declaration
    ;

block : L_BRACE block_1
      ;

block_1 : statement block_1
        | block_2
        ;

block_2 : RETURN expression SEMICOLON block_2
        | R_BRACE
        ;


statement : assignment
          | cycle
          | condition
          | func_call SEMICOLON
          | system_func
          ;

assignment : ID 
                  {
                        VarTable *symbolTable = currentDeclaredFunction->getSymbolTable();
                        int memDir = symbolTable->search($1);
                        if(memDir == -1){
                              memDir = globalSymbolTable->search($1);
                              if(memDir==-1){
                                    string id ($1);
                                    callForNonDeclaredVariableError("Variable \"" +id+ "\" has not been declared");
                              }
                        }
                        stackOperand.push(memDir);
                  } 
             array EQ {stackOperator.push(EQ_);} expression
                                                            {
                                                                  if(stackOperator.empty() == false && stackOperator.top() == EQ_ ){
                                                                        
                                                                        performSemanticsAssignment();
                                                                        
                                                                  }
                                                             }
             SEMICOLON
           ;

condition : IF L_PARENTHESIS expression R_PARENTHESIS {
                                                            MemoryFrame *memFrame = currentDeclaredFunction->getMemoryFrame();

                                                            int expressionResult = stackOperand.top();
                                                            Type type = memFrame->getType(expressionResult);
                                                            if(type == BOOLEAN_){

                                                                  stackOperand.pop();
                                                                  quadrupleSet.push_back(new Quadruple(GOTOF_,expressionResult, -1, -1));
                                                                  pendingJumps.push(quadrupleSet.size()-1);                    
                                                            }else{                        
                                                                 
                                                                 cout<<"my type is "<<type<<endl;
                                                                   callForTypeMismatchError("Mismatch error, cannot perform operation");  
                                                            }

                                                      } block condition_1
          ;

condition_1 : ELSE
                  {
                        int index = pendingJumps.top();
                        pendingJumps.pop();

                        quadrupleSet.at(index)->setResult(quadrupleSet.size()+1);

                        quadrupleSet.push_back(new Quadruple(GOTO_,-1, -1, -1));

                        pendingJumps.push(quadrupleSet.size()-1);

                  } 
                  block
                  {
                        int index = pendingJumps.top();
                        pendingJumps.pop();

                        quadrupleSet.at(index)->setResult(quadrupleSet.size());


                  }
            | 
            {
                  int index = pendingJumps.top();
                  pendingJumps.pop();
                  quadrupleSet.at(index)->setResult(quadrupleSet.size());

            }
            ;

func_call : ID L_PARENTHESIS func_call_1 R_PARENTHESIS
          ;
func_call_1 :  expression func_call_2
            |
            ;

func_call_2 : COMMA expression func_call_2
            |
            ;

system_func : SYSTEM_PREFIX L_PARENTHESIS system_func_1 R_PARENTHESIS SEMICOLON
            ;

system_func_1 : expression 
              |
              ;

cycle : WHILE 
            {
                  pendingJumps.push(quadrupleSet.size());
            }
            L_PARENTHESIS expression R_PARENTHESIS
            {
                   MemoryFrame *memFrame = currentDeclaredFunction->getMemoryFrame();

                  int expressionResult = stackOperand.top();
                  Type type = memFrame->getType(expressionResult);
                  if(type == BOOLEAN_){

                        stackOperand.pop();
                        quadrupleSet.push_back(new Quadruple(GOTOF_,expressionResult, -1, -1));
                        pendingJumps.push(quadrupleSet.size()-1);                    
                  }else{                        
                        
                        cout<<"my type is "<<type<<endl;
                              callForTypeMismatchError("Mismatch error, cannot perform operation");  
                  }

            } 
            block
            {

                        int index = pendingJumps.top();
                        pendingJumps.pop();
                        quadrupleSet.at(index)->setResult(quadrupleSet.size()+1);

                        index = pendingJumps.top();
                        pendingJumps.pop();
                        quadrupleSet.push_back(new Quadruple(GOTO_,-1, -1, index));
                  
            }
      ;


expression : NOT {stackOperator.push(NOT_);} relation {performSemanticsNot();}
             expression_1
                        {
                              if(stackOperator.empty() == false && (stackOperator.top() == AND_ || stackOperator.top() == OR_) ){
                                          performSemantics();
                              }
                        } 

           | relation expression_1 
                        {
                              if(stackOperator.empty() == false && (stackOperator.top() == AND_ || stackOperator.top() == OR_)){
                                    performSemantics();   
                              }
                        } 
           ;

expression_1 : AND {stackOperator.push(AND_);} expression_2
             | OR {stackOperator.push(OR_);} expression_2
             |
             ;

expression_2 : NOT {stackOperator.push(NOT_);} relation {performSemanticsNot();}
             | relation
             ;

relation : exp relation_1 
            {
                  if(stackOperator.empty() == false){
                        if(stackOperator.top() == GT_ || stackOperator.top() == LT_ ||stackOperator.top() == LE_ || stackOperator.top() == GE_ || stackOperator.top() == EE_ ){
                              performSemantics();
                        }
                  }
            } 
           ;

relation_1  : GT {stackOperator.push(GT_);} exp
            | LT {stackOperator.push(LT_);} exp
            | LE {stackOperator.push(LE_);} exp
            | GE {stackOperator.push(GE_);} exp
            | EE {stackOperator.push(EE_);} exp
            |
            ;


exp : term  
            {
                  if(stackOperator.empty() == false && ( stackOperator.top() == ADD_ || stackOperator.top() == SUBS_ )){
                        performSemantics();
                  }
            } 
      exp_1
    ;

exp_1 : ADD {stackOperator.push(ADD_);} exp
      | SUBS {stackOperator.push(SUBS_);} exp
      |
      ;

term  : factor
            {
                  if(stackOperator.empty() == false && (stackOperator.top() == MULT_ || stackOperator.top() == DIV_  )){
                        performSemantics();
                  }
            } 
            term_1
      ;

term_1 : MULT {stackOperator.push(MULT_);} term
       | DIV {stackOperator.push(DIV_);} term
       |
       ;

factor : L_PARENTHESIS  expression R_PARENTHESIS 
       | var_cte 
       ;



var_cte : func_call
        | ID {
                  VarTable *symbolTable = currentDeclaredFunction->getSymbolTable();
                  int memDir = symbolTable->search($1);
                  if(memDir == -1){
                        memDir = globalSymbolTable->search($1);
                        if(memDir==-1){
                              string id ($1);
                              callForNonDeclaredVariableError("Variable \"" +id+ "\" has not been declared");
                        }
                  }

                  stackOperand.push(memDir);

             } array
        | INT     {manageMemoryVarCte(INTEGER_, $1);}
        | FLOAT   {manageMemoryVarCte(FLOAT_, $1);}
        | STRING  {
                        string literal($1);
                        MemoryFrame *memFrame = currentDeclaredFunction->getMemoryFrame();
                        int memDir = memFrame->registerValue(literal);
                        stackOperand.push(memDir);
                  }
        | TRUE    {manageMemoryVarCte(BOOLEAN_, 1);}
        | FALSE   {manageMemoryVarCte(BOOLEAN_, 0);}
        ;


array : L_BRACKET expression R_BRACKET array
      |
      ;


type :  TYPE_STRING     {currentDeclaredtype = STRING_;}
      | TYPE_INT        {currentDeclaredtype = INTEGER_;}  
      | TYPE_FLOAT      {currentDeclaredtype = FLOAT_;}
      | TYPE_BOOLEAN    {currentDeclaredtype = BOOLEAN_;}
      ;

%%

void performSemantics(){
      MemoryFrame *memFrame = currentDeclaredFunction->getMemoryFrame();

      int rightOperand = stackOperand.top();
      Type rightType = memFrame->getType(rightOperand);
      stackOperand.pop();
      
      int leftOperand = stackOperand.top();
      Type leftType = memFrame->getType(leftOperand);
      stackOperand.pop();
      
      Operator op = stackOperator.top();
      stackOperator.pop();

      Type resultType = semantics->isAllowed(rightType,leftType, op);
      if(resultType == VOID_){
            callForTypeMismatchError("Mismatch error, cannot perform operation");                      
      }else{                        
            int result = memFrame->declareValue(resultType);
            stackOperand.push(result);

            //Creating quadruple for action
            quadrupleSet.push_back(new Quadruple(op, leftOperand, rightOperand, result));
      }

}

void performSemanticsAssignment(){

      MemoryFrame *memFrame = currentDeclaredFunction->getMemoryFrame();

      int rightOperand = stackOperand.top();
      Type rightType = memFrame->getType(rightOperand);
      stackOperand.pop();
      
      int leftOperand = stackOperand.top();
      Type leftType = memFrame->getType(leftOperand);
      stackOperand.pop();
      
      Operator op = stackOperator.top();
      stackOperator.pop();

      Type resultType = semantics->isAllowed(rightType,leftType, op);
      if(resultType == VOID_){
            callForTypeMismatchError("Mismatch error, cannot perform operation");                      
      }else{                        
            //Creating quadruple for action
            quadrupleSet.push_back(new Quadruple(op, rightOperand, -1, leftOperand));
      }


}

void performSemanticsNot(){
      if(stackOperator.empty() == false){
            if(stackOperator.top() == NOT_){
                  MemoryFrame *memFrame = currentDeclaredFunction->getMemoryFrame();

                  int rightOperand = stackOperand.top();
                  Type rightType = memFrame->getType(rightOperand);
                  stackOperand.pop();
                                                                                   
                  int leftOperand = 0;
                  Type leftType = VOID_;

                  Operator op = stackOperator.top();
                  stackOperator.pop();

                  Type resultType = semantics->isAllowed(rightType,leftType, op);
                  if(resultType == VOID_){
                        callForTypeMismatchError("Mismatch error, cannot perform operation");
                  }else{
                        int result = memFrame->declareValue(resultType);
                        stackOperand.push(result);

                        //Creating quadruple for action
                        quadrupleSet.push_back(new Quadruple(op, leftOperand, rightOperand, result));
                  }
            }
      }
}


void printQuads(){

int count = 0;
      for ( auto &i : quadrupleSet ) {

            cout <<count<< ".- ";
            i->print();
            count++;
      }

}

void manageMemoryVarCte(Type type, char value){
      MemoryFrame *memFrame = currentDeclaredFunction->getMemoryFrame();
      int memDir;
      if(type == INTEGER_ || type == FLOAT_){
            memDir = memFrame->registerValue(value);
      }
      else if(type == BOOLEAN_){
            if(value == 1){
                  memDir = memFrame->registerValue(true);
            }else{
                  memDir = memFrame->registerValue(false);
            }
      }
      stackOperand.push(memDir);
}

void callForTypeMismatchError(string message){
      if(!message.empty()){
            cout<<yylineno<<" ERROR: "<<message << endl;
            exit (0);
      }
}

void callForNonDeclaredVariableError(string message){
      if(!message.empty()){
            cout<<yylineno<<" ERROR: "<<message << endl;
            exit (0);
      }
}


void callForLocalRedefinitionError(string message){
      if(!message.empty()){
            cout<<yylineno<<" ERROR: "<<message << endl;
            exit (0);
      }
}

void callForGlobalRedefinitionError(string message){
     
      if(!message.empty()){
                  cout<<yylineno<<" ERROR: "<<message << endl;
                  exit (0);
      }

}

void yyerror(char const *x)
{

    cout <<yylineno<<" ERROR: "<<x<<endl;
    exit (0);
}
