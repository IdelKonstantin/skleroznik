#include <iostream>

class keyLock {

public:
	void keyLockTurnOn() {

		std::cout << "Turn on ignition lock" << std::endl;
	}

	void keyLockTurnOff() {

		std::cout << "Turn off ignition lock" << std::endl;
	}
};

class ignition {

public:
	void setIgnition() {

		std::cout << "Start ignition" << std::endl;
	}

	void unsetIgnition() {

		std::cout << "Stop ignition" << std::endl;
	}	
};

class fuelPump {

public:
	void startFuelPump() {

		std::cout << "Start fuel pump" << std::endl;
	}

	void stopFuelPump() {

		std::cout << "Stop fuel pump" << std::endl;
	}	
};

class motoEngine {

public:
	void startEngine() {

		std::cout << "Start engine" << std::endl;
	}

	void warmUpEngine() {

		std::cout << "Warm engine" << std::endl;
	}

	void steadyRide() {

		std::cout << "Doing stady ride" << std::endl;
	}

	void stopEngine() {

		std::cout << "Stop engine" << std::endl;
	}
};

/******************************************************/

class motoFacade {

public:
	/* ВНИМАНИЕ: Классы, методы которых предполагается использовать,
	можно делать публичными, если требуется сохранность доступа к
	их методам. Если не требуется, убираем в приват. Так же, для 
	более слабой связанности (агрегация), сюда можно через конструктор 
	передать ссылки на объекты, а не встраивать (композиция) их */

	keyLock m_keyLock;
	ignition m_ignition;
	fuelPump m_fuelPumpl;
	motoEngine m_motoEngine;

	motoFacade() = default;

	void startUpAndWarmEngine() {

		m_keyLock.keyLockTurnOn();
		m_fuelPumpl.startFuelPump();
		m_ignition.setIgnition();
		m_motoEngine.startEngine();
		m_motoEngine.warmUpEngine();
	}

	void coldStartAndRide() {

		startUpAndWarmEngine();
		m_motoEngine.steadyRide();
	}

	void startOfWarmedEngine() {

		m_keyLock.keyLockTurnOn();
		m_fuelPumpl.startFuelPump();
		m_ignition.setIgnition();
		m_motoEngine.startEngine();	
		m_motoEngine.steadyRide();
	}

	void endOfRide() {

		m_keyLock.keyLockTurnOff();
		m_ignition.unsetIgnition();
		m_fuelPumpl.stopFuelPump();
		m_motoEngine.stopEngine();
	}
};

int main() {

	motoFacade mf;
	mf.startUpAndWarmEngine();
}