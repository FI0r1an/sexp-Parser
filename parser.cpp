#include "parser.h"

#define notline(c) (c != '\r' && c != '\n')
#define notws(c) (c != '\t' && c != ' ')
#define isline(c) (c == '\r' || c == '\n')
#define isws(c) (c == '\t' || c == ' ')
#define isDigit(c) ('0' <= c && c <= '9')

token parseCode(parserState* ps);

token readString(parserState* ps)
{
	string result = "";
	ps->next();
	while (ps->current() != '\"')
	{
		assert(!ps->iseof());
		result += ps->current();
		ps->next();
	}
	return token(TK_STRING, result);
}

token readNumber(parserState *ps)
{
	string result = "";
	if (ps->current() == '-')
	{
		result += '-';
		ps->next();
	}
	while (!ps->iseof() &&(ps->current() == '.' || isDigit(ps->current())))
	{
		result += ps->current();
		ps->next();
	}
	return token(TK_NUMBER, atof(result.c_str()));
}

token readName(parserState* ps)
{
	string result = "";
	while (ps->current() != ')' && notws(ps->current()) && notline(ps->current()))
	{
		result += ps->current();
		ps->next();
		if (ps->iseof()) break;
	}
	return token(TK_ID, result);
}

void skipWhitespace(parserState* ps)
{
	while (isws(ps->current()) || isline(ps->current()))
	{
		if (notline(ps->current()))
		{
			ps->next();
		}
		else
		{
			char old = ps->current();
			ps->next();
			if (ps->current() != old)
			{
				ps->next();
			}
			ps->line++;
			ps->lptr = 1;
		}
	}
}

token readList(parserState* ps)
{
	ps->next();
	token result[32];
	int index = 0;
	while (ps->current() != ')')
	{
		skipWhitespace(ps);
		assert(!ps->iseof());
		if (ps->current() == ')') break;
		token tk = parseCode(ps);
		if (ps->current() == ')') break;
		result[index++] = tk;
		assert(isws(ps->current()) || isline(ps->current()));
	}
	return token(TK_LIST, result);
}

token parseCode(parserState* ps)
{
	char c = ps->current();

	if (isDigit(c) || c == '.' || c == '-') { return readNumber(ps); }
	else if (c == '\"') { return readString(ps); }
	else if (c == '(') { return readList(ps); }
	return readName(ps);
}
#include <time.h>

int main()
{
	time_t begin_t = clock();
	parserState* ps = new parserState("(( 1 2\r\n( a\ta ( a a ) ) ))");
	parseCode(ps);
	time_t finish_t = clock();
	printf("%f", (double)(finish_t - begin_t) / CLOCKS_PER_SEC);
}