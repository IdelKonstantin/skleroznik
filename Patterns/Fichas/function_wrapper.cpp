#include <functional>
#include <iostream>
#include <string>

template <typename T> class Func {
  public:
      Func() {}
      Func(std::function<T> func) : m_func(func) {}

      void operator=(std::function<T> func) {
          
          m_func = func;
      }
      
      void operator=(std::nullptr_t func) {

          m_func = func;
      }

      operator bool() {
      
          return bool(m_func);
      }

      template <typename... Args> void operator()(Args &&... args) {

          auto backup = m_func;
          backup(std::forward<Args>(args)...);
      }

  private:
      std::function<T> m_func;
};

void saySomething(std::string text) {

  std::cout << text << std::endl;
}

int main() {

  Func<decltype(saySomething)> f1{saySomething};

  f1("Func #1");
}