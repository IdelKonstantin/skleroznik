#include <vector>
#include <iostream>
#include <memory>

class foo {
private:

    int m_prop1;
    bool m_prop2;
    bool m_prop3;
    std::vector<int> m_prop4;

public:
    class builder;

    foo(int prop1, bool prop2, bool prop3, std::vector<int> prop4) : 
    m_prop1(prop1), m_prop2(prop2), m_prop3(prop3), m_prop4(prop4) {}

    void showParams() {

        std::cout << "====================================" << std::endl;
        std::cout << "m_prop1 -> " << m_prop1 << std::endl;
        std::cout << "m_prop2 -> " << std::boolalpha << m_prop2 << std::endl;
        std::cout << "m_prop3 -> " << std::boolalpha << m_prop3 << std::endl;

        for(const auto& i : m_prop4) {

            std::cout << "p_prop4 -> " << i << std::endl;
        }
    }
};

class foo::builder {

public:
    builder& set_prop1(int value) {m_prop1 = value; return *this;};
    builder& set_prop2(bool value) {m_prop2 = value; return *this;};
    builder& set_prop3(bool value) {m_prop3 = value; return *this;};
    builder& set_prop4(std::vector<int> value) {m_prop4 = value; return *this;};

    foo build() const {
        
        return std::move(foo{m_prop1, m_prop2, m_prop3, m_prop4});
    }

private:

    int m_prop1{0};
    bool m_prop2{false};
    bool m_prop3{false};
    std::vector<int> m_prop4{5, 2, 42};
};

foo::builder makeBuilder() {

  return foo::builder{};
}

int main() {

  foo obj_1 = makeBuilder()
          .set_prop1(5)
          .set_prop3(true)
          .set_prop4({1,2,3})
          .build();
  
  obj_1.showParams();

  foo obj_2 = makeBuilder().build();
  obj_2.showParams();
}