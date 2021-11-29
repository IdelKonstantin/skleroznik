#include <iostream>
#include <memory>

class Cat;
class Dog;

class PetVisitor {

public:
	virtual void visit(Cat* cat) = 0;
	virtual void visit(Dog* dog) = 0;
};

class Pet {

private:

	const char* m_nick;

public:
	virtual ~Pet(){}
	
	Pet(const char* nick) : m_nick(nick) {}
	const char* getNickname() {

		return m_nick;
	}
	virtual void acceptVisitor(PetVisitor& v) = 0;
};

class Cat : public Pet {

public:
	Cat(const char* nick) : Pet(nick){}
	void acceptVisitor(PetVisitor& v) override {
		v.visit(this);
	}
};

class Dog : public Pet {

public:
	Dog(const char* nick) : Pet(nick){}
	void acceptVisitor(PetVisitor& v) override {
		v.visit(this);
	}
};

class feedingVisitor : public PetVisitor {

public:
	void visit(Cat* cat) override { 

		std::cout << "Покормить тунцом кошку с кличкой -> " << cat->getNickname() << std::endl;
	}
	
	void visit(Dog* dog) override {

		std::cout << "Покормить стейком собаку с кличкой -> " << dog->getNickname() << std::endl;
	}
};

class playingVisitor : public PetVisitor {

public:
	void visit(Cat* cat) override { 

		std::cout << "Поиграть в перышко с кошкой -> " << cat->getNickname() << std::endl;
	}
	
	void visit(Dog* dog) override {

		std::cout << "Поиграть в апорт с собакой -> " << dog->getNickname() << std::endl;
	}
};

/* Диспетчер... */

void dispatch(const std::shared_ptr<Pet>& pet, const std::shared_ptr<PetVisitor>& visitor) {

	pet->acceptVisitor(*visitor);
}


/*******************************************************/

int main () {

	std::shared_ptr<Pet> cat(new Cat("Мурка"));
	std::shared_ptr<Pet> dog(new Dog("Жучка"));

	std::shared_ptr<PetVisitor> fw(new feedingVisitor);
	std::shared_ptr<PetVisitor> pw(new playingVisitor);

	dispatch(cat, fw);
	dispatch(cat, pw);
	dispatch(dog, fw);
	dispatch(dog, pw);

	return 0;
}