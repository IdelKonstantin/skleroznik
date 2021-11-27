#include "great_code.h"

/****** "Privates, LOL" **********************************************************/

static bool isForbibddenSymbol(const char symbol) {
    
    return strchr("DFGJMQV0", toupper(symbol));
}

static bool checkForForbidenSymbolsAbsence(const char* text, const size_t size) {

	while(*forbiddenSymbols) {

		for(size_t i = 0; i < size; i++) {

			if(toupper(text[i]) == *forbiddenSymbols) {
				return false;
			}
		}
		++forbiddenSymbols;
	}
	return true;
}

static void errorMsgPrintAndExit(const char* errorMsg) {

	fprintf(stderr, "%s\n", errorMsg);
	exit(1);
}

static void convertTokensToStructs() {

	size_t i = 0;

	while(tokens != NULL) {

		idsHolder[i].letterCode = toupper(tokens[0]);
		idsHolder[i].numberCode = toupper(tokens[1]);
		tokens = strtok(NULL, "-");
		i++;
	}
}

static void printStructComponents() {

	for(size_t i = 0; i < MAX_ID_SIZE; i++) {

		if(idsHolder[i].letterCode == EMPTY || idsHolder[i].numberCode == EMPTY) {

			break;
		}
		//printf("Index = %d, First -> %d, Second -> %d\n", i, idsHolder[i].letterCode, idsHolder[i].numberCode);
		//printf("Index = %d, First -> %c, Second -> %c\n", i, idsHolder[i].letterCode, idsHolder[i].numberCode);
	}
}

static void makeTokensAndConvert() {

	tokens = strtok(readBuffer, "-");
	convertTokensToStructs();
}

/****** AKA "Publics" **********************************************************/

void inputRead() {

	puts("Введите идентификатор (по 2 символа, разделяя '-'):");
	scanf("%s", readBuffer);
}

char* makeNextID() {

	size_t readLength = strlen(readBuffer);

	if(readLength == 0) {
		errorMsgPrintAndExit("Введен пустой идентификатор. Выходим...");
	}

	if(!checkForForbidenSymbolsAbsence(readBuffer, readLength)) {
		errorMsgPrintAndExit("Строка содержит запрещеные символы. Выходим...");
	}

	makeTokensAndConvert();
	printStructComponents();

	size_t enteredPairs = 0;

	for(size_t i = 0; i < MAX_ID_SIZE; i++) {

		if(idsHolder[i].letterCode == EMPTY || idsHolder[i].numberCode == EMPTY) {

			break;
		}
		enteredPairs++;
	}

	size_t maxIndex = enteredPairs - 1;
	size_t currentBuffIndex = 0;

	for(size_t i = 0; i <= maxIndex; i++) {

		if(i != maxIndex) {

			//заполняем выходной буфер
			writeBuffer[currentBuffIndex] = idsHolder[i].letterCode;
			writeBuffer[currentBuffIndex + 1] = idsHolder[i].numberCode;
			writeBuffer[currentBuffIndex + 2] = '-';
			currentBuffIndex += 3;
		}
		else {

			idsHolder[i].numberCode++;

			if(idsHolder[i].numberCode > MAX_ASCII_NUM_NUM) {

			idsHolder[i].numberCode = MIN_ASCII_NUM_NUM;
			
				while(true) {

					idsHolder[i].letterCode += 1;

					if(idsHolder[i].letterCode > MAX_ASCII_CHAR_NUM && i != 0) {

						idsHolder[i].letterCode = MIN_ASCII_CHAR_NUM;
						writeBuffer[currentBuffIndex - 2] += 1;
					}

					if(isForbibddenSymbol(idsHolder[i].letterCode)) {

						continue;
					}
					break;
				}
			}

			writeBuffer[currentBuffIndex] = idsHolder[i].letterCode;
			writeBuffer[currentBuffIndex + 1] = idsHolder[i].numberCode;
			writeBuffer[currentBuffIndex + 2] = '\0';
		}
	}
	
	return writeBuffer;
}
