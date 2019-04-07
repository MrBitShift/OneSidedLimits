#include <stdio.h>
#include <math.h>
#include <string.h>
#include "MathInput.h"
#include "LibInput.h"

#define check(A, B, ...) if(!(A)) { printf(B "\n", ##__VA_ARGS__); goto error; }

int main(int argc, char *argv[])
{
	double point;
	char *pointString;
	char *lastCharRead;

	Term **tmpPtr = calloc(3, sizeof(Term*));
	Term *equationLess;
	Term *equationEqual;
	Term *equationGreater;
	size_t *equationLessLen;
	size_t *equationEqualLen;
	size_t *equationGreaterLen;

	printf("Enter point to find limit of: ");
	pointString = ReadLine();
	point = strtod(pointString, &lastCharRead);
	check(*lastCharRead == '\n' && pointString[0] != '\n', "Point was not a valid number.");

	printf("Enter the equation when x > %lf: ", point);
	check(GetEquation(tmpPtr, &equationGreaterLen) == 0, "%s", mathinputErr());
	equationGreater = *tmpPtr;
	tmpPtr++;

	printf("Enter the equation when x = %lf: ", point);
	check(GetEquation(tmpPtr, &equationEqualLen) == 0, "%s", mathinputErr());
	equationEqual = *tmpPtr;
	tmpPtr++;

	printf("Enter the equation when x < %lf: ", point);
	check(GetEquation(tmpPtr, &equationLessLen) == 0, "%s", mathinputErr());
	equationLess = *tmpPtr;

	double greaterResult;
	double lessResult;
	double equalResult;

	// find x->point+
	FindY(equationGreater, equationGreaterLen, point, &greaterResult);
	printf("x->%lf+ = %lf\n", point, greaterResult);

	// find x->point-
	FindY(equationLess, equationLessLen, point, &lessResult);
	printf("x->%lf- = %lf\n", point, lessResult);

	// find x->point
	if (lessResult == greaterResult)
	{
		printf("x->%lf = %lf\n", point, lessResult);
	}
	else
	{
		printf("x->%lf = DNE\n", point);
	}
	
	// find f(point)
	FindY(equationEqual, equationEqualLen, point, &equalResult);
	printf("f(%lf) = %lf\n", point, equalResult);
	
	return 0;

error:
	return 1;
}
