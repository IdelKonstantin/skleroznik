#ifndef _BC_KEY_WORKER_H_
#define _BC_KEY_WORKER_H_

#include <Keypad.h>

#define KEY_ROWS 5
#define KEY_COLS 1

#define LEFT_KEY	'L'
#define RIGHT_KEY	'R'
#define UP_KEY		'U'
#define DOWN_KEY	'D'
#define OK_KEY		'O'

namespace key {
	
	const byte ROWS = 5;
	const byte COLS = 1;
	
	char keys[ROWS][COLS] = {
		
		{LEFT_KEY},
		{RIGHT_KEY},
		{UP_KEY},
		{DOWN_KEY},
		{OK_KEY}
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