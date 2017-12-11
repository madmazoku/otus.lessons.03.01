#include <iostream>
#include <spdlog/spdlog.h>

#include "../bin/version.h"
#include "type_name.h"

void example_lambda() {
	int x = 5;
	auto lambda = [x](int i){return x*i;};
	std::cout << "lambda: " << lambda(10) << std::endl;
}

void example_tuple() {

	auto lambda = [=](){ return std::make_tuple(std::string("name"), std::string("surname"), 25); };

    std::string name, surname;
    unsigned int age;

    std::tie(name, surname, age) = lambda();

    std::cout << "tuple: name: " << name << "; surname: " << surname << "; age: " << age << std::endl;
}

int bar() {
	std::cout << "bar called" << std::endl;
}

auto foo(int x) -> decltype(bar())
{
	return x;
}

int x = 33;
decltype(auto) foo2()
{
	const int &y = x;
	return y;
}

auto foo3(std::string s) -> decltype(s)
{
	return "42";
}

int main(int argc, char** argv) {
	auto console = spdlog::stdout_logger_st("console");
	console->info("Welcome to spdlog!");

	example_lambda();
	example_tuple();

	std::cout << "decltype foo call: " << foo(10) << std::endl;

	decltype(auto) i = foo2();
	std::cout << "decltype foo2 call: " << i << " type: " << typeid(i).name() << std::endl;

	auto i3 = foo3("");
	std::cout << "decltype foo2 call: " << i3 << " type: " << typeid(i3).name() << std::endl;

	std::cout << "type_name of i3: " << type_name<decltype(i3)>() << std::endl;

	console->info("Goodbye!");

	return 0;
}