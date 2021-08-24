#include "catch2/catch_all.hpp"
#include <String.h>
#include <cstring>

TEST_CASE("Test custom string", "[Custom string class]") {

	SECTION("Test of overloaded operators") {

		String s1{"Hello world"};
		String s2;

		REQUIRE(s1.length() == std::strlen("Hello world"));
		REQUIRE(s1[0] == s1.at(0));
		REQUIRE(s1[0] == 'H');

		REQUIRE(s2.empty());

		s2 = "Foo!!!";
		REQUIRE(!s2.empty());
		REQUIRE(s2.length() == std::strlen("Foo!!!"));

		s2 = s1;
		REQUIRE(std::strcmp(s1.c_str(), s2.c_str()) == 0);

		s1.clear();
		s2.clear();

		REQUIRE(s1.empty());
		REQUIRE(s2.empty());

		s1 += "Hello";
		s2 += "world";

		REQUIRE(std::strcmp(s1.c_str(), "Hello") == 0);
		REQUIRE(std::strcmp(s2.c_str(), "world") == 0);

		s1.clear();
		s2.clear();

		String s3{"Fuuuu"};

		s1 += s3;		
		s2 += s3;
		
		REQUIRE(std::strcmp(s1.c_str(), s2.c_str()) == 0);
		
		auto i = (s1 == s3);
		REQUIRE(i);

		i = (s1 == s3);
		REQUIRE(i);

		i = (s1 == "Fuuuu");
		REQUIRE(i);

		i = (s2 == "Fuuuu");
		REQUIRE(i);
	}
	
	SECTION("Test of routine methods") {

		String s1{"Hello world"};
		String s2{s1};

		REQUIRE(s1.length() == std::strlen("Hello world"));
		REQUIRE(s1.c_str() != nullptr);
		
		s1.clear();

		REQUIRE(s1.empty() == true);
		REQUIRE(s1.length() == Str::EMPTY);

		s1.append("Bye all");

		REQUIRE(s1.length() != 0);
		REQUIRE(s1.at(0) == 'B');

		s1.clear();
		s1.append("Foo!!!");

		REQUIRE(s1.at(0) == 'F');
		REQUIRE(s1.at(1) == 'o');

		s1.append(s2);

		REQUIRE(s1.length() > s2.length());
		REQUIRE(s1.contains("Foo!!!Hello world"));
		REQUIRE(s1.contains('H'));

		String s3{"Foo!!!Hello world"};

		REQUIRE(s1.contains(s3));
	}
}

int main (int argc, char* argv[]) { //https://github.com/catchorg/Catch2/blob/devel/docs/own-main.md#top

	auto numFailed = Catch::Session().run();
	
	return numFailed;
}