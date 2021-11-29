#include <iostream>

/* УТКА **********************************************************************/
/* Интерфейс утки */
class iDuck {

public:

	iDuck(){};
	virtual void fly() const = 0;
	virtual void quack() const = 0;
	virtual ~iDuck(){}; 
};

/* Реализация (обычная утка) */
class duck : public iDuck {

public:

	duck() {};
	void fly() const override {

		std::cout << "I am flying as usual..." << std::endl;
	}
	void quack() const override {

		std::cout << "I am quacking just as planned..." << std::endl;
	}
};

/* ИНДЮШКА *******************************************************************/
/* Интерфейс индейки */
class iTurkey {

public:

	iTurkey(){};
	virtual void fly() const = 0;
	virtual void gobble() const = 0;
	virtual ~iTurkey(){}; 
};

/* Реализация индейки */
class turkey : public iTurkey {

public:

	turkey() {};

	void fly() const override {

		std::cout << "I am flying but on short distance only..." << std::endl;
	}
	void gobble() const override {

		std::cout << "I am gobbling. Wooooaahh..." << std::endl;
	}
};

/* Адаптер ИНДЮШКИ для имитации поведения УТКИ *******************************/
class turkeyAdapter : public iDuck {

private:
	turkey m_theTurkey;
public:

	turkeyAdapter(const turkey& theTurkey) : m_theTurkey(theTurkey) {}
	
	void fly() const override {

		m_theTurkey.fly();
	}
	void quack() const override {

		m_theTurkey.gobble();
	}
};

/*****************************************************************************/

void testDuck(const iDuck& duck) {

	duck.quack();
	duck.fly();
}

int main () {

	duck usualDuck;
	turkey wildTurkey;

	turkeyAdapter fakeDuck(wildTurkey);

	std::cout << "The Turkey says..." << std::endl;

	wildTurkey.gobble();
	wildTurkey.fly();

	std::cout << "\nThe Duck says..." << std::endl;
	testDuck(usualDuck);

	std::cout << "\nThe fake Duck (Turkey adapter) says..." << std::endl;
	testDuck(fakeDuck);
}