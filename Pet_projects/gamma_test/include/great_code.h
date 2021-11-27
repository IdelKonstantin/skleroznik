#ifndef _GREAT_CODE_H_
#define _GREAT_CODE_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_BUFFER_SIZE 	0xFF
#define MAX_ID_SIZE			9
#define EMPTY				0

#define MIN_ASCII_CHAR_NUM	65
#define MAX_ASCII_CHAR_NUM	90

#define MIN_ASCII_NUM_NUM	49
#define MAX_ASCII_NUM_NUM	57

/****** Justaaa "fields" *********************************************************/
const char* forbiddenSymbols = "DFGJMQV0";
char readBuffer[MAX_BUFFER_SIZE];
char writeBuffer[MAX_BUFFER_SIZE];
char * tokens;

typedef struct IdentifierPortion {

	int letterCode;
	int numberCode;
} identifierPortion;

identifierPortion idsHolder[MAX_ID_SIZE];

size_t currentCharIndex;
size_t currentNumberIndex;

/****** Kindaa "methods" *********************************************************/
/****** "Privates, LOL" **********************************************************/
static bool isForbibddenSymbol(const char symbol);

static bool checkForForbidenSymbolsAbsence(const char* text, const size_t size);

static void errorMsgPrintAndExit(const char* errorMsg);

static void printStructComponents();

static void makeTokensAndConvert();

/****** AKA "Publics" **********************************************************/
void inputRead();

char* makeNextID();

#endif /* _GREAT_CODE_H_ */
