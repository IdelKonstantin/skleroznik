#include <gtest/gtest.h>

class MyClassTest;

class MyClass {
 
private:
    int privateMethod(int a, int b);
    friend MyClassTest;
};

int MyClass::privateMethod(int a, int b) {
    return a + b;
}

/////////////////////////////////////////////////////////

class MyClassTest {

private:

   MyClass m_mcl; 

public:
    MyClassTest() = default;
    int testprivateMethod(int a, int b) {
        return m_mcl.privateMethod(a, b);
    }
};

/////////////////////////////////////////////////////////

TEST(TestGroupName, private_method_test) {

    MyClassTest m_mclt;
    ASSERT_EQ(m_mclt.testprivateMethod(1,3), 4);
}

int main(int argc, char **argv) {

  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}