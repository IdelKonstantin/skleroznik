#include <iostream>
#include <memory>

/***********************************************************/
class iEmploee {

public:
	virtual ~iEmploee(){};
	virtual void doWork() const = 0;
};


/***********************************************************/
class Designer : public iEmploee {

public:

	void doWork() const override {

		std::cout << "I am making cool design, sir!" << std::endl;
	}
};

class Progger : public iEmploee {

public:

	void doWork() const override {

		std::cout << "I am producing a lot of bugs. Sorry..." << std::endl;
	}
};


/***********************************************************/
using iEploee_uptr_t = std::unique_ptr<iEmploee>;

int main () {

	iEploee_uptr_t designer{new Designer};
	iEploee_uptr_t progger{new Progger};

	designer->doWork();
	progger->doWork();
}