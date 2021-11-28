#include <iostream>

class iPrepareRecipe {

protected:
	void boilWatter() {

		std::cout << "Boil water" << std::endl;
	}
	void pourInCup() {

		std::cout << "Pour in cup" << std::endl;
	}

	virtual void brew() = 0;
	virtual void addCondiments() = 0;

public:
	iPrepareRecipe(){};
	virtual ~iPrepareRecipe(){};

	void prepareAll() {

		boilWatter();
		brew();
		pourInCup();
		addCondiments();
	}

};

class prepareTea : public iPrepareRecipe {

private:
	
	void brew() override {

		std::cout << "Steep tee bag" << std::endl;
	}
	
	void addCondiments() override {

		std::cout << "Add lemon" << std::endl;	
	}
};


class prepareCoffee : public iPrepareRecipe {

private:
	
	void brew() override {

		std::cout << "Brew coffee grinds" << std::endl;
	}
	
	void addCondiments() override {

		std::cout << "Add sugar and milk" << std::endl;	
	}
};


int main () {

	std::cout << "Prepairing tea..." << std::endl;
	prepareTea{}.prepareAll();

	std::cout << "\nPrepairing coffee..." << std::endl;
	prepareCoffee{}.prepareAll();
}