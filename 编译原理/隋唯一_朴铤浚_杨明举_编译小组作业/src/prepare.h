#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include "ASTNode.h"

using namespace std;

struct Node
{
	string strValue;
	union {
		typeEnum type;
		int intValue;
		double dblValue;
		ASTNode *ptr;
	};
};


extern "C"				
{						
	int yywrap(void);
	int yylex(void);
	void yyerror(const char *s);
}

#define YYSTYPE Node

