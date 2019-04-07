#ifndef MATHINPUT_H
#define MATHINPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "LibInput.h"

char *mathinputLastErr = NULL;

#define mathinputErr() (mathinputLastErr == NULL) ? ("None") : (mathinputLastErr)
#define check(A, B) if (!(A)) {\
	mathinputLastErr = B;\
	goto error;\
}

typedef struct Term
{
	double Coefficient;
	double Exponent;
} Term;

char *CombineStrings(char *string1, char *string2)
{
	check(string1 != NULL, "string1 can't be null.");
	check(string2 != NULL, "string2 can't be null.");

	char *result = calloc(strlen(string1) + strlen(string2) + 1, sizeof(char)); // + 1 for null!
	strcat(result, string1);
	strcat(result, string2);

	return result;

error:
	return NULL;
}

int PrintEquation(Term *equation, size_t equationLen)
{
	for (size_t i = 0; i < equationLen - 1; i++)
	{
		printf("%lfx^%lf + ", equation[i].Coefficient, equation[i].Exponent);
	}
	printf("%lfx^%lf\n", equation[equationLen - 1].Coefficient, equation[equationLen - 1].Exponent);
}

int StringToTerm(char *string, Term *out)
{
	char *lastReadChar;

	check(string != NULL, "string can't be null.");
	check(out != NULL, "out can't be null.");

	// string possibilites: (X) indicates current model works with it
	// x (X)
	// 5 (X)
	// 5x (X)
	// 5x^3 (X)
	// x^3 (X)
	// -x (X)
	// -x^3 (X)

	// handle coefficient
	if (string[0] == 'x')
	{
		out->Coefficient = 1;
		lastReadChar = string;
	}
	else if (string[0] == '-' && string[1] == 'x')
	{
		out->Coefficient = -1;
		lastReadChar = string + 1;
	}
	else
	{
		out->Coefficient = strtod(string, &lastReadChar);
	}

	// handle exponent
	if (*lastReadChar == '\0')
	{
		// ie 5*x^0
		out->Exponent = 0;
	}
	else if (*lastReadChar == 'x')
	{
		// ie 5*x
		switch (*(lastReadChar + 1))
		{
			case '\0':
				out->Exponent = 1;
				break;
			case '^':
				out->Exponent = strtod(lastReadChar + 2, &lastReadChar);
				check(*lastReadChar == '\0', "Invalid format.");
				break;
			default:
				check(0, "Invalid format.");
		}
	}
	else
	{
		check(0, "Invalid format.");
	}

	return 0;

error:
	if (out != NULL);
	{
		out->Coefficient = 0;
		out->Exponent = 0;
	}
	return 1;
}

int FindY(Term *equation, size_t equationLen, double x, double *out)
{
	check(equation != NULL, "equation can't be null.");

	*out = 0;
	for (size_t i = 0; i < equationLen; i++)
	{
		*out += equation[i].Coefficient * pow(x, equation[i].Exponent);
	}

	return 0;

error:
	return 1;
}

int GetEquation(Term **out, size_t *outLen)
{
	char *entered; // taken from user console input
	char *token; // stores tokens from seperation and processing
	char *rest; // used for strtok_s

	check(out != NULL, "out can't be null.");
	check(outLen != NULL, "len can't be null.");
	
	entered = ReadLine();
	check(entered != NULL, 
		CombineStrings("Failure in GetEquation. Error thrown in ReadLine: ", mathinputErr()));

	check(strlen(entered) > 1, "You must enter an equation.");

	rest = entered;

	*outLen = 1;

	int nextNegative = 0;
	int prevOperator = 1; // is true the first time since you don't write "+ x + 3"
	while ((token = strtok_s(rest, " \n", &rest)))
	{
		*out = realloc(*out, *outLen * sizeof(Term));

		if (token[0] == '+' && token[1] == '\0')
		{
			prevOperator = 1;
			continue;
		}
		else if (token[0] == '-' && token[1] == '\0')
		{
			nextNegative = 1;
			prevOperator = 1;
			continue;
		}

		check(prevOperator == 1, "Invalid format. Make sure that + and - are seperated by spaces.");
		check(StringToTerm(token, &(*out)[*outLen - 1]) == 0, 
			"Invalid format. Make sure that + and - are seperated by spaces.");

		if (nextNegative)
		{
			(*out)[*outLen - 1].Coefficient *= -1;
		}

		nextNegative = 0;
		prevOperator = 0;
		*outLen += 1;
	}

	// it will always make outLen one greater so remove one
	(*outLen)--;

	return 0;

error:
	return 1;
}

#undef check
#endif //MATHINPUT_H
