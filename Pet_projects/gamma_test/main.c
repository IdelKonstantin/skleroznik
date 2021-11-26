#include <stdio.h>

extern void inputRead();
extern char* makeNextID();

/****** Great "code" **********************************************************/
int main() {
	
	inputRead();
	printf("Следующий идентификатор:\n%s\n", makeNextID());
}
