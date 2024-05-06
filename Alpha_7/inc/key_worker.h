#ifndef _BC_KEY_WORKER_H_
#define _BC_KEY_WORKER_H_

#include <Keypad.h>
#include "keys_names.h"

#define KEY_ROWS 5
#define KEY_COLS 1

namespace key {
	
	char keys[KEY_ROWS][KEY_COLS] = {
		
		{LEFT_KEY},
		{RIGHT_KEY},
		{UP_KEY},
		{DOWN_KEY},
		{OK_KEY}
	};

	byte rowPins[KEY_ROWS] = {27, 16, 17, 25, 26};
	byte colPins[KEY_COLS] = {14};
}

#endif /* _BC_KEY_WORKER_H_ */