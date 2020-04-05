#ifndef _PARSER_
#define _PARSER_
#include <stdlib.h>
#include <assert.h>
#include <string>
using namespace std;
#define TK_UK 0
#define TK_NUMBER 1
#define TK_STRING 2
#define TK_ID 3
#define TK_LIST 4
typedef struct token
{
	int type;
	string stringValue;
	double numberValue;
	token* children[32];
	token(int t, string v) { type = t; stringValue = v; numberValue = NULL; };
	token(int t, double v) { type = t; numberValue = v; stringValue = ""; };
	token(int t, token c[]) { type = t; *children = c; numberValue = NULL; stringValue = ""; };
	token() { stringValue = ""; type = TK_UK; numberValue = NULL; };
} token;

typedef struct parserState
{
	size_t ptr;
	int lptr;
	int line;
	string source;
	parserState(string s)
	{
		ptr = 0;
		lptr = line = 1;
		source = s;
	};
	void next() { ptr++; lptr = 1; line++; };
	char current() { return source[ptr]; };
	char lookahead() { return source[ptr + 1]; };
	auto iseof() { return (ptr + 1) > source.length(); };
} parserState;
#endif