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


int main() {

	/* Создаем поведение */
	auto flyWithWings = std::make_shared<FlyWithWings>();
	auto flyNoWay = std::make_shared<FlyNoWay>();

	auto quack = std::make_shared<Quack>();
	auto squeak = std::make_shared<Squeak>();
	auto mute = std::make_shared<muteQuack>();

	// /* Создаем уточек */
	duck usualDuck;
	usualDuck.setFlyBehavior(flyWithWings);
	usualDuck.setQuackBehavior(quack);

	duck rubberDuck;
	rubberDuck.setFlyBehavior(flyNoWay);
	rubberDuck.setQuackBehavior(squeak);	

	duck woodenDuck;
	woodenDuck.setFlyBehavior(flyNoWay);
	woodenDuck.setQuackBehavior(mute);

	duck ironDuck;
	ironDuck.setFlyBehavior(flyNoWay);
	ironDuck.setQuackBehavior(mute);

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

	std::cout << "\nIron duck: " << std::endl;
	ironDuck.fly();
	ironDuck.quack();	
}