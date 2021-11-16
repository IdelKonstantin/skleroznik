#include <iostream>

class OnlyOne {

public:

	void vyak() {

		std::cout << "Превед!!!" << std::endl;
	}

    static OnlyOne& Instance() {

        static OnlyOne theSingleInstance;
        return theSingleInstance;
    }

private:        
    OnlyOne(){}
    OnlyOne(const OnlyOne& root) = delete;
    OnlyOne& operator=(const OnlyOne&) = delete;
};

int main () {

	OnlyOne::Instance().vyak();
}