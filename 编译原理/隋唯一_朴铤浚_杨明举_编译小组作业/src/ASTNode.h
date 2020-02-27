#pragma once

#include<iostream>
using namespace std;

typedef enum {intType, charType, doubleType} typeEnum;
typedef enum {typeType, typeInt, typeChar, typeString, typeDouble,typeId, typeExpr, typeExprList, typeStatement,
    typeStatementList, typeFunction, typeFunctionList, typeParameter, typeParameterList } nodeEnum;

class ASTNode
{
public:
    nodeEnum type;
    int intValue;
    string strValue;
    double dblValue;
};

class ASTNodeTerminal:public ASTNode{};

class ASTNodeNonTerminal:public ASTNode
{
public:
    int childNum;
    vector<ASTNode*> child;
};

class typeASTNode:public ASTNodeTerminal{};
class intASTNode:public ASTNodeTerminal{};
class charASTNode:public ASTNodeTerminal{};
class doubleASTNode:public ASTNodeTerminal{};
class stringASTNode:public ASTNodeTerminal{};
class idASTNode:public ASTNodeTerminal{};

class exprASTNode:public ASTNodeNonTerminal{};
class exprListASTNode:public ASTNodeNonTerminal{};
class statementASTNode:public ASTNodeNonTerminal{};
class statementListASTNode:public ASTNodeNonTerminal{};
class functionASTNode:public ASTNodeNonTerminal{};
class functionListASTNode:public ASTNodeNonTerminal{};
class parameterASTNode:public ASTNodeNonTerminal{};
class parameterListASTNode:public ASTNodeNonTerminal{};



