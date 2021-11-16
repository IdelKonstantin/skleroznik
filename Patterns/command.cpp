#include <iostream>
#include <vector>
#include <memory>

class iCommand {

public:

	iCommand(){};
	virtual void execute() = 0;
	virtual ~iCommand(){}
};

class Light {

public:
	void liteOn() {

		std::cout << "Включаю свет" << std::endl;
	}
};

class WaterCooler {

public:
	void safeEnergy() {

		std::cout << "Сохраняю энергию" << std::endl;
	}

	void collTheWater() {

		std::cout << "Охлаждаю воду" << std::endl;
	}
};

class lightSwitch : public iCommand, public Light {

public:

	void execute() override {

		liteOn();
	}
};

class waterCollerManipulator : public iCommand, public WaterCooler {

public:

	void execute() override {

		safeEnergy();
		collTheWater();
	}
};


int main() {

	std::vector<std::unique_ptr<iCommand>> switches;

	switches.push_back(std::make_unique<waterCollerManipulator>());
	switches.push_back(std::make_unique<lightSwitch>());

	for(const auto& currentSwitch : switches) {

		currentSwitch->execute();
	}

	return 0;
}