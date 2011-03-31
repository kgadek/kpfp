#ifndef FUNKCJE_H_
#define FUNKCJE_H_

#ifndef NULL
#define NULL 0
#endif

enum CompResult {
	EQUAL = 0, DIFFERENT, NOTEQUAL = 1
};

unsigned int CopyString(char[], char[]);

enum CompResult eCompareString(char[], char[]);

void AppendString(char[], char[]);

void ReplaceCharactersInString(char[], char, char);

#endif

