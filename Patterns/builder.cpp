#include <iostream>
#include <memory>

/* Not used yet */
class Product {

public:

	Product() = default;	
	void sayHi() {

		std::cout << "Hi from Product" << std::endl;
	}
};


class Builder{

public:
    
    virtual ~Builder(){}
    virtual void ProducePartA() const = 0;
    virtual void ProducePartB() const = 0;
    virtual void ProducePartC() const = 0;
};


class ConcreteBuilder : public Builder{

private:
    /* Not used yet */
    std::unique_ptr<Product> product{};

public:
    ConcreteBuilder() = default;

    void ProducePartA() const override{
        std::cout << "PartA" << std::endl;
    }

    void ProducePartB() const override{
        std::cout << "PartB" << std::endl;
    }

    void ProducePartC() const override{
        std::cout << "PartC" << std::endl;
    }

    /* Not used yet */
    std::unique_ptr<Product>&& GetProduct() {

        return std::move(this->product);
    }
};


class Director {

private:
	const Builder& builder{};

public:
	Director(const Builder& builder) : builder(builder) {};
	
	void buildConfigBase() {

		std::cout << "buildConfigBase" << std::endl;
		builder.ProducePartA();
	}

	void buildConfigWide() {
		
		std::cout << "buildConfigWide" << std::endl;
		builder.ProducePartA();
		builder.ProducePartB();
	}

	void buildConfigAll() {

        std::cout << "buildConfigAll" << std::endl;
		builder.ProducePartA();
		builder.ProducePartB();
		builder.ProducePartC();
	}
};


int main() {

	ConcreteBuilder cb;
	Director dir{cb};

	dir.buildConfigBase();
	dir.buildConfigAll();
}