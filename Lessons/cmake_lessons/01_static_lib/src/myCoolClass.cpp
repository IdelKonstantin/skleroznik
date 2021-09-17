#include <myCoolClass.h>

void myCoolClass::showThreadID() {

	std::cout << "Привет из потока с ID: " << std::this_thread::get_id() << std::endl;
}