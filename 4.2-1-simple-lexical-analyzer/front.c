#include <stdio.h>
#include <stdlib.h>

FILE *in_fp;
char nextChar;
int charClass;
int lexLen;
char lexeme[100];
int nextToken;

/* Character Classes */
#define LETTER   0
#define DIGIT    1
#define REAL_DOT 2
#define UNKNOWN  99

/* Token Codes */
#define INT_LIT     10
#define IDENT       11
#define REAL_LIT    12
#define ASSIGN_OP   20
#define ADD_OP      21
#define SUB_OP      22
#define MUL_OP      23
#define DIV_OP      24
#define LEFT_PAREN  25
#define RIGHT_PAREN 26


void getChar()
{
	if ((nextChar = fgetc(in_fp)) != EOF)
		if (isdigit(nextChar))
			charClass = DIGIT;
		else if (isalpha(nextChar))
			charClass = LETTER;
		else if (nextChar == '.')
			charClass = REAL_DOT;
		else
			charClass = UNKNOWN;
	else
		charClass = EOF;
}

void getNonBlank()
{
	while (isspace(nextChar))
		getChar();
}

void addChar()
{
	if (lexLen <= 98)
	{
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	}
	else
		printf("ERROR - lexeme is too long.\n");
}

int lookup(char c)
{
	addChar();
	switch (c)
	{
	case '(':
		nextToken = LEFT_PAREN;
		break;
	case ')':
		nextToken = RIGHT_PAREN;
		break;
	case '=':
		nextToken = ASSIGN_OP;
		break;
	case '+':
		nextToken = ADD_OP;
		break;
	case '-':
		nextToken = SUB_OP;
		break;
	case '*':
		nextToken = MUL_OP;
		break;
	case '/':
		nextToken = DIV_OP;
		break;
	default:
		nextToken = EOF;
		break;
	}
	return nextToken;
}

void lex()
{
	lexLen = 0;
	getNonBlank();

	switch (charClass)
	{
	case LETTER:
		addChar();
		getChar();
		while (charClass == DIGIT || charClass == LETTER)
		{
			addChar();
			getChar();
		}
		nextToken = IDENT;
		break;
	case DIGIT:
		addChar();
		getChar();
		if (charClass == REAL_DOT)
		{
			addChar();
			getChar();
			while (charClass == DIGIT)
			{
				addChar();
				getChar();
			}
			nextToken = REAL_LIT;
		}
		else
		{
			while (charClass == DIGIT)
			{
				addChar();
				getChar();
			}
			nextToken = INT_LIT;
		}
		break;
	default:
	case UNKNOWN:
		lookup(nextChar);
		getChar();
		break;
	case EOF:
		strcpy(lexeme, "EOF");
		nextToken = EOF;
		break;
	}

	printf("Next token is %d. Next lexeme is %s\n", nextToken, lexeme);
}

int main()
{
	if ((in_fp = fopen("front.txt", "r")) == NULL)
		printf("ERROR - cannot open front.txt.\n");
	else
	{
		getChar();
		do
		{
			lex();
		} while (nextToken != EOF);
	}
	return 0;
}
