#include <stdio.h>
#include <inttypes.h>
#include <iostream>

using fx8q8_t = int16_t;

#define MAKE_FROM_FLOAT(a)  ((fx8q8_t)((a) * (1LL << 8)))
#define CONVERT_TO_FLOAT(a) ((a) / (float)(1LL<<8))

#define MULT(a, b) ((fx8q8_t)(((int32_t)(a) * (b)) >> 8))
#define DIV(a, b) ((fx8q8_t)(((int32_t)(a) << 8) / (b)))

class fix {

private:
	fx8q8_t m_num{0};

public:
	fix() = default;
	fix(float num) : m_num(MAKE_FROM_FLOAT(num)) {};
	fix(double num) : m_num(MAKE_FROM_FLOAT((float)num)) {};
	
	fix(fx8q8_t num) : m_num(num) {};

	fix(const fix& op) {
		m_num = op.m_num;
	}

	fix(fix&& op) {
		m_num = op.m_num;
	}
	
	fix operator=(const fix& op) {

		m_num = op.m_num;
		return *this;
	}

	fix operator=(fix&& op) {

		m_num = op.m_num;
		return *this;
	}

///////////////////////////////////////////////////////////

	fix operator+(const fix& op) const {
		return fix{fx8q8_t(m_num + op.m_num)};
	}

	fix operator-(const fix& op) const {
		return fix{fx8q8_t(m_num - op.m_num)};
	}

	fix operator*(const fix& op) const {
		return fix{MULT(m_num, op.m_num)};
	}

	fix operator/(const fix& op) const {
		return fix{DIV(m_num, op.m_num)};
	}

	bool operator==(const fix& op) const {
		return m_num == op.m_num;
	}

	bool operator!=(const fix& op) const {
		return m_num != op.m_num;
	}

	bool operator<(const fix& op) const {
		return m_num < op.m_num;
	}

	bool operator>(const fix& op) const {
		return m_num > op.m_num;
	}

	bool operator<=(const fix& op) const {
		return m_num <= op.m_num;
	}

	bool operator>=(const fix& op) const {
		return m_num >= op.m_num;
	}

///////////////////////////////////////////////////////////

	operator float() const {
		return CONVERT_TO_FLOAT(m_num);
	}

	operator int() const {
		return static_cast<int>(CONVERT_TO_FLOAT(m_num));
	}

	operator double() const {
		return CONVERT_TO_FLOAT(m_num);
	}

///////////////////////////////////////////////////////////

	friend std::ostream& operator<<(std::ostream& out, const fix& num);
};

std::ostream& operator<<(std::ostream& out, const fix& num) {

	out << CONVERT_TO_FLOAT(num.m_num);
	return out;
}

int main() {

	fix f1{2.0};
	fix f2{3.1};

	fix f5 = 456.223;

	std::cout << "f1 = " << f1 << std::endl;
	std::cout << "f2 = " << f2 << std::endl;

	std::cout << "f1 + f2 = " << (f1 + f2) << std::endl;
	std::cout << "f1 - f2 = " << (f1 - f2) << std::endl;
	std::cout << "f1 * f2 = " << (f1 * f2) << std::endl;
	std::cout << "f1 / f2 = " << (f1 / f2) << std::endl;

	auto f3 = f1 + f2;
	auto f4 = f3;

	std::cout << "f3 = " << f3 << std::endl;
	std::cout << "f4 == f3 = " << std::boolalpha << (f4 == f3) << std::endl;
	std::cout << "f4 != f3 = " << std::boolalpha << (f4 != f3) << std::endl;
	std::cout << "f4 >= f3 = " << std::boolalpha << (f4 >= f3) << std::endl;
	std::cout << "f4 <= f3 = " << std::boolalpha << (f4 <= f3) << std::endl;

	std::cout << "f5 = " << f5 << std::endl;

	std::cout << "f1 < f2 = " << std::boolalpha << (f1 < f2) << std::endl;
	std::cout << "f1 <= f2 = " << std::boolalpha << (f1 <= f2) << std::endl;

	std::cout << "f2 > f1 = " << std::boolalpha << (f2 > f1) << std::endl;
	std::cout << "f2 >= f1 = " << std::boolalpha << (f2 >= f1) << std::endl;

	std::cout << "f1 as int: " << int(f1) << std::endl;
	std::cout << "f2 as float: " << float(f2) << std::endl;

	int f1_ = f1;
	float f2_ = f2;

	std::cout << "f1_ as int: " << f1_ << std::endl;
	std::cout << "f2_ as float: " << f2_ << std::endl;

}