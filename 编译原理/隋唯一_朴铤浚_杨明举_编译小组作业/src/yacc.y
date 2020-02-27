%{
#include "prepare.h"
#include <stdarg.h>
#include <stdlib.h>
using namespace std;

#define UNIT_INDENT 4
FILE* target;

typeASTNode *_type(typeEnum value);
intASTNode *_int(int value);
stringASTNode *_string(string value);
doubleASTNode *_double(double value);
exprASTNode *_expr(int childNum,int _operator, ...);
exprListASTNode *_exprList(int childNum, ...);
statementASTNode *_statement(int childNum,int _statement, ...);
statementListASTNode *_statementList(int childNum, ...);
functionASTNode *_function(int childNum, ...);
functionListASTNode *_functionList(int childNum, ...);
parameterASTNode *_parameter(int childNum, ...);
parameterListASTNode *_parameterList(int childNum, ...);
idASTNode *_id(string value);
int getChildNum(ASTNode *p);
void freeNode(ASTNode *p);

void codeGenerator(ASTNode *p);

string codeType(ASTNode *p);
string codeInteger(ASTNode *p);
string codeString(ASTNode *p);
string codeIdentifier(ASTNode *p);
string codeFunction(ASTNode *p);
string codeStatement(ASTNode* p, int indent_level);
string codeStatementList(ASTNode* p, int indent_level);
string codeExpr(ASTNode* p);
string codeExprList(ASTNode* p);

void test(const char* sFile, const char* tFile);

%}

%token<intValue>INTEGER
%token<dblValue>DOUBLE_NUM
%token<type>INT CHAR DOUBLE
%token<strValue>IDENTIFIER STRING

%token INC_OP DEC_OP INC_OP_LEFT INC_OP_RIGHT DEC_OP_LEFT DEC_OP_RIGHT GE_OP LE_OP EQ_OP NE_OP AND_OP OR_OP
%token DECLARE DECLARE_ARRAY ARRAY
%token FOR WHILE BREAK CONTINUE IF ELSE RETURN PRINTF STRLEN

%nonassoc IFX
%nonassoc ELSE

%left EQ_OP NE_OP '>' '<'
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%type<ptr>functionList function parameterList parameter typeName statementList statement exprList expr
%start program

%%

program:
        functionList                                                                {codeGenerator($1);freeNode($1);}
       ;

functionList:
        function                                                                    {$$ = _functionList(1,$1);}
       |function functionList                                                       {$$ = _functionList(1 + getChildNum($2),$1,$2);}
       ;

function:
        typeName IDENTIFIER '(' parameterList ')' '{' statementList '}'             {$$ = _function(4,$1,_id($2),$4,$7);}
       |typeName IDENTIFIER '('')' '{' statementList '}'                            {$$ = _function(3,$1,_id($2),$6);}
       ;

parameterList:
        parameter                                                                   {$$ = _parameterList(1,$1);}
       |parameter ',' parameterList                                                 {$$ = _parameterList(1 + getChildNum($3),$1,$3);}
       ;

parameter:
        typeName IDENTIFIER                                                         {$$ = _parameter(2,$1,_id($2));}
       |typeName IDENTIFIER '[' ']'                                                 {$$ = _parameter(2,$1,_id($2));}
       ;

typeName:
        INT                                                                         {$$ = _type($1);}
       |CHAR                                                                        {$$ = _type($1);}
       |DOUBLE                                                                      {$$ = _type($1);}
       ;

statementList:
        statement                                                                   {$$ = _statementList(1,$1);}
       |statement statementList                                                     {$$ = _statementList(1 + getChildNum($2),$1,$2);}
       ;

statement:
        FOR '(' statement expr ';' expr ')' '{' statementList '}'                   {$$ = _statement(4,FOR,$3,$4,$6,$9);}
       |WHILE '(' expr ')' '{' statementList '}'                                    {$$ = _statement(2,WHILE,$3,$6);}
       |BREAK ';'                                                                   {$$ = _statement(0,BREAK);}
       |CONTINUE ';'                                                                {$$ = _statement(0,CONTINUE);}
       |IF '(' expr ')' '{' statementList '}' %prec IFX                             {$$ = _statement(2,IF,$3,$6);}
       |IF '(' expr ')' '{' statementList '}' ELSE '{' statementList '}'            {$$ = _statement(3,ELSE,$3,$6,$10);}
       |RETURN expr ';'                                                             {$$ = _statement(1,RETURN,$2);}
       |PRINTF '(' exprList ')' ';'                                                 {$$ = _statement(1,PRINTF,$3);}
       |IDENTIFIER '(' exprList ')' ';'                                             {$$ = _statement(2,IDENTIFIER,_id($1),$3);}
       |IDENTIFIER '=' expr ';'                                                     {$$ = _statement(2,'=',_id($1),$3);}
       |IDENTIFIER '[' expr ']' '=' expr ';'                                        {$$ = _statement(3,'=',_id($1),$3,$6);}
       |typeName IDENTIFIER '=' expr ';'                                            {$$ = _statement(3,DECLARE,$1,_id($2),$4);}
       |typeName IDENTIFIER '[' ']' '=' expr ';'                                    {$$ = _statement(3,DECLARE_ARRAY,$1,_id($2),$6);}
       |typeName IDENTIFIER '[' ']' '=' '{' exprList '}' ';'                        {$$ = _statement(3,ARRAY,$1,_id($2),$7);}
       |typeName IDENTIFIER '[' INTEGER ']' ';'                                     {$$ = _statement(3,DECLARE_ARRAY,$1,_id($2),_int($4));}
       |typeName IDENTIFIER '[' INTEGER ']' '=' expr ';'                            {$$ = _statement(4,DECLARE_ARRAY,$1,_id($2),_int($4),$7);}
       |typeName IDENTIFIER ';'                                                     {$$ = _statement(2,DECLARE,$1,_id($2));}
       |INC_OP expr ';'                                                             {$$ = _statement(1,INC_OP_LEFT,$2);}
       |DEC_OP expr ';'                                                             {$$ = _statement(1,DEC_OP_LEFT,$2);}
       |expr INC_OP ';'                                                             {$$ = _statement(1,INC_OP_RIGHT,$1);}
       |expr DEC_OP ';'                                                             {$$ = _statement(1,DEC_OP_RIGHT,$1);}
       ;

exprList:
        expr                                                                        {$$ = _exprList(1,$1);}
       |expr ',' exprList                                                           {$$ = _exprList(1 + getChildNum($3),$1,$3);}
       ;

expr:
        INTEGER                                                                     {$$ = _int($1);}
       |STRING                                                                      {$$ = _string($1);}
       |IDENTIFIER                                                                  {$$ = _id($1);}
       |DOUBLE                                                                      {$$ = _double($1);}
       |'-' expr %prec UMINUS                                                       {$$ = _expr(1,UMINUS,$2);}
       |STRLEN '(' IDENTIFIER ')'                                                   {$$ = _expr(1,STRLEN,_id($3));}
       |IDENTIFIER '(' exprList ')'                                                 {$$ = _expr(2,IDENTIFIER,_id($1),$3);}
       |IDENTIFIER '[' expr ']'                                                     {$$ = _expr(2,'[',_id($1),$3);}
       |expr '+' expr                                                               {$$ = _expr(2,'+',$1,$3);}
       |expr '-' expr                                                               {$$ = _expr(2,'-',$1,$3);}
       |expr '*' expr                                                               {$$ = _expr(2,'*',$1,$3);}
       |expr '/' expr                                                               {$$ = _expr(2,'/',$1,$3);}
       |expr '<' expr                                                               {$$ = _expr(2,'<',$1,$3);}
       |expr '>' expr                                                               {$$ = _expr(2,'>',$1,$3);}
       |expr NE_OP expr                                                             {$$ = _expr(2,NE_OP,$1,$3);}
       |expr EQ_OP expr                                                             {$$ = _expr(2,EQ_OP,$1,$3);}
       |expr OR_OP expr                                                             {$$ = _expr(2,OR_OP,$1,$3);}
       |expr AND_OP expr                                                            {$$ = _expr(2,AND_OP,$1,$3);}
       |expr LE_OP expr                                                             {$$ = _expr(2,LE_OP,$1,$3);}
       |expr GE_OP expr                                                             {$$ = _expr(2,GE_OP,$1,$3);}
       |'!' expr                                                                    {$$ = _expr(1,'!',$2);}
       |'(' expr ')'                                                                {$$ = _expr(1,'(',$2);}
       |INC_OP expr                                                                 {$$ = _expr(1,INC_OP_LEFT,$2);}
       |DEC_OP expr                                                                 {$$ = _expr(1,DEC_OP_LEFT,$2);}
       |expr INC_OP                                                                 {$$ = _expr(1,INC_OP_RIGHT,$1);}
       |expr DEC_OP                                                                 {$$ = _expr(1,DEC_OP_RIGHT,$1);}
       ;

%%

void yyerror(const char *s)
{
	cerr<<s<<endl;
}

typeASTNode *_type(typeEnum value)
{
    typeASTNode *p = new typeASTNode();
    p->intValue = value;
    p->type = nodeEnum::typeType;
    return p;
}

intASTNode *_int(int value)
{
    intASTNode *p = new intASTNode();
    p->intValue = value;
    p->type = nodeEnum::typeInt;
    return p;
}

stringASTNode *_string(string value)
{
    stringASTNode *p = new stringASTNode();
    p->strValue = value;
    p->type = nodeEnum::typeString;
    return p;
}

doubleASTNode *_double(double value)
{
    doubleASTNode *p = new doubleASTNode;
    p->dblValue = value;
    p->type = nodeEnum::typeDouble;
    return p;
}

exprASTNode *_expr(int childNum,int _operator,...)
{
    va_list para;
    exprASTNode *p = new exprASTNode();
    p->type = nodeEnum::typeExpr;
    p->intValue = _operator;
    p->childNum = childNum;
    va_start(para,_operator);
    for(int i = 0;i < childNum;i++)
        p->child.push_back(va_arg(para,ASTNodeNonTerminal*));
    va_end(para);
    return p;
}

exprListASTNode *_exprList(int childNum,...)
{
    va_list para;
    exprListASTNode *p = new exprListASTNode();
    p->type = nodeEnum::typeExprList;
    p->childNum = childNum;
    va_start(para,childNum);
    p->child.push_back(va_arg(para,ASTNodeNonTerminal*));
    if(childNum > 1)
    {
        exprListASTNode *_p = va_arg(para,exprListASTNode*);
        for(int i = 1;i < childNum;i++)
            p->child.push_back(_p->child[i-1]);
    }
    va_end(para);
    return p;
}

statementASTNode *_statement(int childNum,int _statement,...)
{
    va_list para;
    statementASTNode *p = new statementASTNode();
    p->type = nodeEnum::typeStatement;
    p->intValue = _statement;
    p->childNum = childNum;
    va_start(para,_statement);
    for(int i = 0;i < childNum;i++)
        p->child.push_back(va_arg(para,ASTNodeNonTerminal*));
    va_end(para);
    return p;
}

statementListASTNode *_statementList(int childNum,...)
{
    va_list para;
    statementListASTNode *p = new statementListASTNode();
    p->type = nodeEnum::typeStatementList;
    p->childNum = childNum;
    va_start(para,childNum);
    p->child.push_back(va_arg(para,ASTNodeNonTerminal*));
    if(childNum > 1)
    {
        statementListASTNode *_p = va_arg(para,statementListASTNode*);
        for(int i = 1;i < childNum;i++)
            p->child.push_back(_p->child[i-1]);
    }
    va_end(para);
    return p;

}

functionASTNode *_function(int childNum,...)
{
    va_list para;
    functionASTNode *p = new functionASTNode();
    p->type = nodeEnum::typeFunction;
    p->childNum = childNum;
    va_start(para,childNum);
    for(int i = 0;i < childNum;i++)
        p->child.push_back(va_arg(para,ASTNodeNonTerminal*));
    va_end(para);
    return p;
}

functionListASTNode *_functionList(int childNum,...)
{
    va_list para;
    functionListASTNode *p = new functionListASTNode();
    p->type = nodeEnum::typeFunctionList;
    p->childNum = childNum;
    va_start(para,childNum);
    p->child.push_back(va_arg(para,ASTNodeNonTerminal*));
    if(childNum > 1)
    {
        functionListASTNode *_p = va_arg(para,functionListASTNode*);
        for(int i = 1;i < childNum;i++)
            p->child.push_back(_p->child[i-1]);
    }
    va_end(para);
    return p;
}

parameterASTNode *_parameter(int childNum,...)
{
    va_list para;
    parameterASTNode *p = new parameterASTNode();
    p->type = nodeEnum::typeParameter;
    p->childNum = childNum;
    va_start(para,childNum);
    for(int i = 0;i < childNum;i++)
        p->child.push_back(va_arg(para,ASTNodeNonTerminal*));
    va_end(para);
    return p;
}

parameterListASTNode *_parameterList(int childNum,...)
{
    va_list para;
    parameterListASTNode *p = new parameterListASTNode();
    p->type = nodeEnum::typeParameterList;
    p->childNum = childNum;
    va_start(para,childNum);
    p->child.push_back(va_arg(para,ASTNodeNonTerminal*));
    if(childNum > 1)
    {
        parameterListASTNode *_p = va_arg(para,parameterListASTNode*);
        for(int i = 1;i < childNum;i++)
            p->child.push_back(_p->child[i-1]);
    }
    va_end(para);
    return p;
}

idASTNode *_id(string value)
{
    idASTNode *p = new idASTNode();
    p->type = nodeEnum::typeId;
    p->strValue = value;
    return p;
}

int getChildNum(ASTNode *p)
{
    return ((ASTNodeNonTerminal*)p)->childNum;
}

void freeNode(ASTNode *p)
{
    if (!p) return;
    switch (p->type)
    {
        case typeExpr:
        case typeExprList:
        case typeStatement:
        case typeStatementList:
        case typeFunction:
        case typeFunctionList:
        case typeParameter:
        case typeParameterList:
            {
                ASTNodeNonTerminal* ptr = (ASTNodeNonTerminal*)p;
                for (int i = 0; i < ptr->childNum; i++)
                    freeNode(ptr->child[i]);
            }
            break;
    }
    delete p;
}

string codeParameter(ASTNode* p)
{
    if (p->type != typeParameter){
        cerr << "Not Parameter type!" << endl;
    }

    parameterASTNode* pa = (parameterASTNode*)p;
    return codeIdentifier(pa->child[1]);
}

string codeParameterList(ASTNode* p)
{
    if (p->type != typeParameterList){
        cerr << "Not ParameterList type!" << endl;
    }

    parameterListASTNode* pl = (parameterListASTNode*)p;
    string code = "";
    for (int i = 0; i < pl->childNum; i++) {
        code += codeParameter(pl->child[i]);
        if (i != pl->childNum - 1) {
            code += ", ";
        }
    }
    return code;
}


string codeType(ASTNode* p)
{
    if (p->type != typeType){
        cerr << "Not Type type!" << endl;
    }

    return "let";
}

string codeInteger(ASTNode* p) {
    if (p->type != typeInt) {
        cerr << "Not Integer type!" << endl;
    }
    intASTNode* i = (intASTNode*)p;
    stringstream ss;
    ss << i->intValue;
    return ss.str();
}

string codeChar(ASTNode* p) {
    if (p->type != typeChar) {
        cerr << "Not Char type!" << endl;
    }
    charASTNode* c = (charASTNode*)p;
    return "#";
}

string codeString(ASTNode* p) {
    if (p->type != typeString) {
        cerr << "Not String type!" << endl;
    }
    stringASTNode* s = (stringASTNode*)p;
    stringstream ss;
    ss << s->strValue;
    return ss.str();
}

string codeIdentifier(ASTNode* p) {
    if (p->type != typeId) {
        cerr << "Not Identifier type!" << endl;
    }
    idASTNode* i = (idASTNode*)p;
    stringstream ss;
    ss << i->strValue;
    return ss.str();
}

string codeExpr(ASTNode* p)
{
    string code = "";
    switch (p->type) {
        case typeInt:
            code = codeInteger(p);
            break;
        case typeString:
            code = codeString(p);
            break;
        case typeId:
            code = codeIdentifier(p);
            break;
        case typeExpr:
            exprASTNode* e = (exprASTNode*)p;
            switch (e->intValue) {
                case '+':
                case '-':
                case '*':
                case '/':
                case '<':
                case '>':
                    code = codeExpr(e->child[0]) + " ";
                    code += e->intValue;
                    code += " " + codeExpr(e->child[1]);
                    break;
                case INC_OP_LEFT:
                    code = "++" + codeExpr(e->child[0]);
                    break;
                case DEC_OP_LEFT:
                    code = "--" + codeExpr(e->child[0]);
                    break;
                case INC_OP_RIGHT:
                    code = codeExpr(e->child[0]) + "++";
                    break;
                case DEC_OP_RIGHT:
                    code = codeExpr(e->child[0]) + "--";
                    break;
                case LE_OP:
                    code = codeExpr(e->child[0]) + " <= " + codeExpr(e->child[1]);
                    break;
                case GE_OP:
                    code = codeExpr(e->child[0]) + " >= " + codeExpr(e->child[1]);
                    break;
                case EQ_OP:
                    code = codeExpr(e->child[0]) + " == " + codeExpr(e->child[1]);
                    break;
                case NE_OP:
                    code = codeExpr(e->child[0]) + " != " + codeExpr(e->child[1]);
                    break;
                case AND_OP:
                    code = codeExpr(e->child[0]) + " && " + codeExpr(e->child[1]);
                    break;
                case OR_OP:
                    code = codeExpr(e->child[0]) + " || " + codeExpr(e->child[1]);
                    break;
                case '!':
                    code = "!" + codeExpr(e->child[0]);
                    break;
                case '[':
                    code = codeIdentifier(e->child[0]) + "[" + codeExpr(e->child[1]) + "]";
                    break;
                case '(':
                    code = "(" + codeExpr(e->child[0]) + ")";
                    break;
                case STRLEN:
                    code = codeIdentifier(e->child[0]) + ".length";
                    break;
                case UMINUS:
                    code = "-" + codeExpr(e->child[0]);
                    break;
                case IDENTIFIER:
                    code = codeIdentifier(e->child[0]) + "(" + codeExprList(e->child[1]) + ")";
                    break;
            }
            break;
    }
    return code;
}

string codeExprList(ASTNode* p)
{
    if (p->type != typeExprList) {
        cerr << "Not exprList type!" << endl;
    }

    exprListASTNode* el = (exprListASTNode*)p;
    string code = "";
    for (int i = 0; i < el->childNum; i++) {
        code += codeExpr(el->child[i]);
        if (i != el->childNum - 1) {
            code += ", ";
        }
    }
    return code;
}

string codeStatement(ASTNode* p, int indent_level)
{
    if (p->type != typeStatement) {
        cerr << "Not Statement type!" << endl;
    }

    statementASTNode* s = (statementASTNode*)p;
    string code = "";
    switch (s->intValue) {
        case FOR:
            code = "for (" + codeStatement(s->child[0], 0) + codeExpr(s->child[1]) + "; " + codeExpr(s->child[2]) + ") " + codeStatementList(s->child[3], indent_level + 1);
            break;
        case WHILE:
            code = "while (" + codeExpr(s->child[0]) + ") " + codeStatementList(s->child[1], indent_level + 1);
            break;
        case BREAK:
            code = "break;";
            break;
        case CONTINUE:
            code = "continue;";
            break;
        case IF:
            code = "if (" + codeExpr(s->child[0]) + ") " + codeStatementList(s->child[1], indent_level + 1);
            break;
        case ELSE:
            code = "if (" + codeExpr(s->child[0]) + ") " + codeStatementList(s->child[1], indent_level + 1) + " else " + codeStatementList(s->child[2], indent_level + 1);
            break;
        case RETURN:
            code = "return " + codeExpr(s->child[0]) + ";";
            break;
        case PRINTF:
            code = "process.stdout.write(printf(" + codeExprList(s->child[0]) + "));";
            break;
        case IDENTIFIER:
            code = codeIdentifier(s->child[0]) + "(" + codeExprList(s->child[1]) + ");";
            break;
        case '=':
            if (s->childNum == 3) {
                code = codeIdentifier(s->child[0]) + "[" + codeExpr(s->child[1]) + "] = " + codeExpr(s->child[2]) + ";";
            } else {
                code = codeIdentifier(s->child[0]) + " = " + codeExpr(s->child[1]) + ";";
            }
            break;
        case DECLARE:
            if (s->childNum == 3) {
                code = codeType(s->child[0]) + " " + codeIdentifier(s->child[1]) + " = " + codeExpr(s->child[2]) + ";";
            } else {
                code = codeType(s->child[0]) + " " + codeIdentifier(s->child[1]) + " = undefined;";
            }
            break;
        case DECLARE_ARRAY:
            if(s->childNum == 3)
            {
                if (s->child[2]->type == typeString) {
                    code = codeType(s->child[0]) + " " + codeIdentifier(s->child[1]) + " = " + codeString(s->child[2]) + ";";
                } else {
                    code = codeType(s->child[0]) + " " + codeIdentifier(s->child[1]) + " = " + "new Array(" + codeInteger(s->child[2]) + ");";
                }
            }
            else
            {
                code = codeType(s->child[0]) + " " + codeIdentifier(s->child[1]) + " = " + codeString(s->child[3]) + ".split('');";
            }
            break;
        case ARRAY:
            code = codeType(s->child[0]) + " " + codeIdentifier(s->child[1]) + " = " + '[' + codeExprList(s->child[2]) + ']' + ";";
            break;
        case INC_OP_LEFT:
            code = "++" + codeExpr(s->child[0]) + ";";
            break;
        case DEC_OP_LEFT:
            code = "--" + codeExpr(s->child[0]) + ";";
            break;
        case INC_OP_RIGHT:
            code = codeExpr(s->child[0]) + "++;";
            break;
        case DEC_OP_RIGHT:
            code = codeExpr(s->child[0]) + "--;";
            break;

    }
    code.insert(0, indent_level * UNIT_INDENT, ' ');
    return code;
}

string codeStatementList(ASTNode* p, int indent_level)
{
    if (p->type != typeStatementList){
        cerr << "Not StatementList type!" << endl;
    }

    statementListASTNode* sl = (statementListASTNode*)p;
    string code = "";
    code += "{\n";
    for (int i = 0; i < sl->childNum; i++) {
        code += codeStatement(sl->child[i], indent_level) + "\n";
    }
    code.insert(code.length(), (indent_level - 1) * UNIT_INDENT, ' ');
    code += "}";
    return code;
}

string codeFunction(ASTNode *p)
{
    if (p->type != typeFunction){
        cerr << "Not Function type!" << endl;
    }

    functionASTNode* f = (functionASTNode*)p;
    string code = "";
    code += codeType(f->child[0]);
    code += " ";
    code += codeIdentifier(f->child[1]);
    code += " = ";
    if (f->childNum == 3) {
        code += "() => ";
        code += codeStatementList(f->child[2], 1);
    } else {
        code += "(" + codeParameterList(f->child[2]) + ") => ";
        code += codeStatementList(f->child[3], 1);
    }
    return code;
}

void codeGenerator(ASTNode *p)
{
    if (p->type != typeFunctionList){
        cerr << "Not FunctionList type!" << endl;
    }

    functionListASTNode* fl = (functionListASTNode*)p;
    string code = "";
    code += "'use strict'\n";
    code += "let printf = require('printf');\n";
    //code += "require('C2JS_header');\n";
    for (int i = 0; i < fl->childNum; i++) {
        code += codeFunction(fl->child[i]);
        code += "\n";
    }
    //code.insert(0, getModuleInfo());
    code += "main();";
    fwrite(code.c_str(), sizeof(char), code.length(), target);
}

void test(const char* sFile, const char* tFile)
{
	FILE* fp=fopen(sFile, "r");
	if(fp==NULL)
	{
		printf("cannot open %s\n", sFile);
		return;
	}
	extern FILE* yyin;
	yyin=fp;
    target = fopen(tFile, "w");

	printf("-----begin parsing %s\n", sFile);
	yyparse();
	puts("-----end parsing");

	fclose(fp);
    fclose(target);
}

int main()
{
    const char* level1 = "level1.c";
    const char* ans1 = "level1.js";
    test(level1, ans1);
    const char* level2 = "level2.c";
    const char* ans2 = "level2.js";
    test(level2, ans2);
    const char* level3 = "level3.c";
    const char* ans3 = "level3.js";
    test(level3, ans3);
    const char* level4 = "level4.c";
    const char* ans4 = "level4.js";
    test(level4, ans4);
	return 0;
}
