#include <iostream>
#include <memory>

/*************** Полет ***************/
/* Интерфейс полета */
class iFlyBehavior {

public:
	iFlyBehavior(){};
	virtual void fly() = 0;
	virtual ~iFlyBehavior(){}; 
};

/* Реализация полета (может летать) */
class FlyWithWings : public iFlyBehavior {

public:
	void fly() override {

		std::cout << "I am flying..." << std::endl;
	}
};

/* Реализация полета (не может летать) */
class FlyNoWay : public iFlyBehavior {

public:
	void fly() override {

		std::cout << "I can't fly..." << std::endl;
	}
};
/****************************************/

/*************** Кваканье ***************/
/* Интерфейс кваканья */
class iQuackBehavior {

public:
	iQuackBehavior(){};
	virtual void quack() = 0;
	virtual ~iQuackBehavior(){};
};

/* Реализация кваканья (может квакать) */
class Quack : public iQuackBehavior {

public:
	void quack() override {

		std::cout << "I am quacking..." << std::endl;	
	}
};

/* Реализация кваканья (может пищать) */
class Squeak : public iQuackBehavior {

public:
	void quack() override {

		std::cout << "I am squeaking..." << std::endl;	
	}
};

/* Реализация кваканья (не может издавать звуки) */
class muteQuack : public iQuackBehavior {

public:
	void quack() override {

		std::cout << "I am silent..." << std::endl;	
	}
};
/****************************************/

/*************** Утка ***************/
/* Интерфейс утки */
class iDuck {

protected:
	std::shared_ptr<iFlyBehavior> flyBehavior{nullptr};
	std::shared_ptr<iQuackBehavior> quackBehavior{nullptr};

public:

	iDuck(){};
	virtual void swim() = 0;
	virtual void fly() const = 0;
	virtual void quack() const = 0;

	virtual void setFlyBehavior(std::shared_ptr<iFlyBehavior>) = 0;
	virtual void setQuackBehavior(std::shared_ptr<iQuackBehavior>) = 0;

	virtual ~iDuck(){}; 
};

/* Реализация (обычная утка) */
class duck: protected iDuck {

public:

	duck() {};
	void swim() override {

		std::cout << "I swim as usual..." << std::endl;
	}
	void fly() const override {

		flyBehavior->fly();
	}
	void quack() const override {

		quackBehavior->quack();
	}

	void setFlyBehavior(std::shared_ptr<iFlyBehavior> flyBehavior_) override {

		flyBehavior = flyBehavior_;
	}
	void setQuackBehavior(std::shared_ptr<iQuackBehavior> quackBehavior_) override {

		quackBehavior = quackBehavior_;
	}
};

namespace duck_type {

	const int usual = 1;
	const int rubber = 2;
	const int wooden = 3;
}

class fabrique {

private:

	std::shared_ptr<FlyWithWings> flyWithWings;
	std::shared_ptr<FlyNoWay> flyNoWay;

	std::shared_ptr<Quack> quack;
	std::shared_ptr<Squeak> squeak;
	std::shared_ptr<muteQuack> mute;

public:
	fabrique() : flyWithWings(new FlyWithWings), flyNoWay(new FlyNoWay), quack(new Quack), squeak(new Squeak), mute(new muteQuack) {}
	
	duck produceDuck(const int duckType) {

		duck newDuck;

		switch(duckType) {

			case duck_type::usual:

				newDuck.setFlyBehavior(flyWithWings);
				newDuck.setQuackBehavior(quack);			
				break;

			case duck_type::rubber:
				newDuck.setFlyBehavior(flyNoWay);
				newDuck.setQuackBehavior(squeak);
				break;

			case duck_type::wooden:

				newDuck.setFlyBehavior(flyNoWay);
				newDuck.setQuackBehavior(mute);
				break;
				
			return newDuck;
		}
	}
};


int main() {

	fabrique f{};

	auto usualDuck = f.produceDuck(duck_type::usual);
	auto rubberDuck = f.produceDuck(duck_type::rubber);
	auto woodenDuck = f.produceDuck(duck_type::wooden);

	/* Проверяем, что все работает */
	std::cout << "Usual duck: " << std::endl;
	usualDuck.fly();
	usualDuck.quack();

	std::cout << "\nRubber duck: " << std::endl;
	rubberDuck.fly();
	rubberDuck.quack();

	std::cout << "\nWooden duck: " << std::endl;
	woodenDuck.fly();
	woodenDuck.quack();
}