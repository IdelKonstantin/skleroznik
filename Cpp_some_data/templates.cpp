//========================================================================
//Простой шаблон функции (необходимо конкретизировать тип):

template<typename T, class U>
T foo(const T& a, const U& b) {

	//Some code here
}

template<typename T, class U>
inline T foo(const T& a, const U& b) {

	//Some code here
}

template<typename T, class U>
constexpr T foo(const T& a, const U& b) {

	//Some code here
}

//Шаблон со специализацией
template<>
bool foo(const bool& a, const int& b) {

	//Some code here
	return a;
}

//Вызов:
foo<>(false, 42); 
foo(false, 42);


//========================================================================
//Параметры значения шаблона функции (необходимо конкретизировать значение):

template<std::size_t N, std::size_t M>
int compare(const char* a[N], const char* b[M]) {

	//Some code here	
}


//========================================================================
//Простой шаблон класса

template<typename T>
class foo {

	T m_a;
	void bar(int i) const;

public:
	foo(const T& a) : m_a(a) {}
	//Some another code here
};

//Определение методов за пределами шаблонного класса
void foo<T>::bar(int i) const {

	//Some code here
}

foo<T>::foo(const T& a) : m_a(a) {};

//Дружественный тип класса
template<typename Type> class foo {
friend Type; //Дружественный тип, т.е. для foo<bar> - bar это друг

	//....

}


//========================================================================
//Шаблоный метод не шаблонного класса

class foo {

	T m_a;
	template <class T> void bar(T i) const;

public:
	foo() = default;
	//Some another code here
};


//========================================================================
//Псевдонимы типа шаблона

typedef foo<bar> fooBar;
template<typename T> using myTwin = pair<T,T>;

myTwins<std::string> это - pair<std::string, std::string>


//========================================================================
//Объявления (но не определения) шаблонов:

template<typename T, class U> T foo(const T& a, const U& b);
template<typename T> class foo;

//также (для избегания раскрытия и раздельной компиляции в каждом объектном файле):

extern template<typename T, class U> T foo(const T& a, const U& b);
extern template<typename T> class foo;


//========================================================================
//Аргументы по-умолчанию:

template<typename T, typename F = std::less<T>>
T foo(const T& a, const T& b, F func = F()) {

	//Some code here
}

template<typename T = int> //создается вот так: foo<>
class foo {

	T m_a;
	void bar(int i) const;

public:
	foo(const T& a) : m_a(a) {}
	//Some another code here
};


//========================================================================
//Автовыведение возвращаемого типа с помощью замыкающего типа возвращаемого значения:

template<typename T>
auto foo(T beg, T end) -> decltype(*beg) {

	//Some code here
	return *beg; //возврат ссылки на элемент диапазона
}


//========================================================================
//Стандартные шаблоны трансформации типа -> см. type_traits.h

template<typename T> 
auto foo(T beg, T end) -> typename std::remove_reference<decltype(*beg)>::type {

	//Some code here
	return *beg; //возврат элемента из диапазона
}


//========================================================================
//Пересылающая ссылка и сохранение типа агрумента при дедукции (utillity.h)

template<typename T> 
void foo(T&& arg) {

	//Some code here
}

foo<my_type>(std::forward(myType));


template <class T>
class bar {
    
private:

	T* worker{};

public:

	bar (T* obj) : worker{std::forward<T*>(obj)} {

		//do some at construction
	}

	~bar () {

		//do some at destruction
	}
};


//========================================================================
//Вариадический шаблон

template<typename T, typename... Args>
void foo(const T& a, const Args& ... b) {

	std::cout << std::sizeof...(Args) << std::endl;
	//Some code here
}

foo("Hi", 52, 5.0F);
