#ifndef _BC_KEY_WORKER_H_
#define _BC_KEY_WORKER_H_

#include <Keypad.h>

#define KEY_ROWS 5
#define KEY_COLS 1

namespace key {
	
	const byte ROWS = 5;
	const byte COLS = 1;
	
	char keys[ROWS][COLS] = {
		{'L'},
		{'R'},
		{'U'},
		{'D'},
		{'O'}
	};

	byte rowPins[ROWS] = {27, 16, 17, 25, 26};
	byte colPins[COLS] = {14};
}

#endif /* _BC_KEY_WORKER_H_ */

/************************************************************************************ 

#include "key_worker.h"

Keypad keypad(makeKeymap(key::keys), key::rowPins, key::colPins, KEY_ROWS, KEY_COLS);

auto key = keypad.getKey();

if (key == ...) {
	...
}

************************************************************************************/