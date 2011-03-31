#include "funkcje.h"

unsigned int CopyString(char cSource[], char cDestination[]) {
	unsigned int i;
	for(i=0; cSource[i] != NULL; ++i)
		cDestination[i] = cSource[i];
	cDestination[i] = 0;
	return i;
}

enum CompResult eCompareString(char cStr1[], char cStr2[]) {
	for(; *cStr1 != NULL; ++cStr1, ++cStr2)
		if(*cStr1 != *cStr2)
			return DIFFERENT;
	return *cStr1 == *cStr2 ? EQUAL : DIFFERENT;
}

void AppendString(char cSourceStr[], char cDestinationStr[]) {
	for(; *cDestinationStr != NULL; ++cDestinationStr) ;
	for(; *cSourceStr != NULL; ++cDestinationStr, ++cSourceStr)
		*cDestinationStr = *cSourceStr;
}

void ReplaceCharactersInString(char cString[], char OldChar, char NewChar) {
	for(; *cString != NULL; ++cString)
		if(*cString == OldChar)
			*cString = NewChar;
}

