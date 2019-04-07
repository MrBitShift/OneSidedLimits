#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>

char *InputErr = NULL;

#define InputLastErr() (InputErr != NULL) ? InputErr : "None"
#define check(A, B, ...) if (!(A)) { InputErr = (B); goto error; }

char* ReadLine()
{
	char *result = calloc(2, sizeof(char));
	size_t len = 1;

	char last_char = '\0';
	while (last_char != '\n')
	{
		last_char = fgetc(stdin);
		result[len - 1] = last_char;
		len++;
		result = realloc(result, len * sizeof(char));
	}
	result[len - 1] = '\0';

	return result;
}


#undef check
#endif //INPUT_H