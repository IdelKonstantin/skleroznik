#include <iostream>
#include <memory>

class Warrior {

public:
	virtual ~Warrior(){};
	virtual void fight() = 0; 
};

class Infantryman: public Warrior {

public:
	void fight() override { 
		
		std::cout << "Infantryman attaks!" << std::endl;
	};
};

class Horseman: public Warrior {

public:
	void fight() override { 
		
		std::cout << "Horseman attaks!" << std::endl;
	};
};

/***********************************************************/

class Factory {

public:
	virtual ~Factory() {};
	virtual std::unique_ptr<Warrior> createWarrior() = 0;
};

class InfantryFactory: public Factory {

public:
	std::unique_ptr<Warrior> createWarrior() override {
		
		return std::unique_ptr<Warrior>{new Infantryman};
	}
};

class HorsemanFactory: public Factory {

public:
	std::unique_ptr<Warrior> createWarrior() override { 
		
		return std::unique_ptr<Warrior>{new Horseman};
	}
};

/***********************************************************/

int main() {

	std::unique_ptr<Factory> infantry_factory{new InfantryFactory};
	std::unique_ptr<Factory> horseman_factory{new HorsemanFactory};

	infantry_factory->createWarrior()->fight();
	horseman_factory->createWarrior()->fight();
}