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

#define MOVE

struct xray
{
	std::string str;
    xray(const std::string &s) : str(s)
    {
        std::cout << "[" << str << "] " << "ctor: " << this << ' ' << __FUNCTION__ << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl;
    }
    xray(const xray& x)
    {
        std::cout << "copy ctor: " << this << " from " << &x << ' ' << __FUNCTION__ << std::endl;
    }
#ifdef MOVE
    xray(xray&& x) noexcept
    {
        std::cout << "copy ctor: " << this << " from " << &x << ' ' << __FUNCTION__ << std::endl;
        x.destroy_by_move();
    }
#endif
    struct xray &operator=(const struct xray& t) {
        std::cout << "[" << str << "] <- [" << t.str << "] " << "copy: " << this << ' ' << __FUNCTION__ << std::endl;
        return *this;
    }
#ifdef MOVE
    struct xray &operator=(struct xray&& x) noexcept {
        std::cout << "[" << str << "] <- [" << x.str << "] " << "move: " << this << ' ' << __FUNCTION__ << std::endl;
        str.swap(x.str);
        std::cout << "\t swapped [" << str << "] <- [" << x.str << "] " << "move: " << this << ' ' << __FUNCTION__ << std::endl;
        // x.destroy_by_move();
        return *this;
    }
#endif
    ~xray()
    {
        std::cout << "[" << str << "] " << "dtor: " << this << ' ' << __FUNCTION__ << std::endl;
    }
    void operator()()
    {
        std::cout << "[" << str << "] " << "tick" << std::endl;
    }
    void destroy_by_move() {
        std::cout << "[" << str << "] " << "destroy_by_move" << std::endl;
    }
};

void foo(const xray &)
{
	std::cout <<  "=ref" << std::endl;
}

void foo(xray &&)
{
	std::cout <<  "=rvalue" << std::endl;
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

	std::cout << std::endl << std::endl;
//=================== break ===================

	{
		xray x("x");
		xray y("y");

		x = xray("z");

		std::cout << "~~~~~" << std::endl;

		std::cout << "foo(x)" << std::endl;
		foo(x);

		std::cout << "foo(xray(q))" << std::endl;
		foo(xray("q"));

		std::cout << "~~~~~" << std::endl;

		std::cout << "foo(xray(z))" << std::endl;
		foo(xray("z"));

		std::cout << "scope end" << std::endl;
	}
	std::cout << std::endl << std::endl;

	{
		std::cout << "~~~~~" << std::endl;

		xray x("x");
		xray y("y");

		std::cout << "x = y" << std::endl;
		x = y;

		std::cout << "x = xray(z)" << std::endl;
		x = xray("z");

		std::cout << "scope end" << std::endl;
	}
	std::cout << std::endl << std::endl;

	{
		std::cout << "~~~~~" << std::endl;

		xray x("x");
		xray y("y");

		std::cout << "x = static_cast<xray &&>(y)" << std::endl;
		x = static_cast<xray &&>(y);
		std::cout << "x = std::move(y)" << std::endl;
		x = std::move(y);

		std::cout << "scope end" << std::endl;

		std::cout << "type name (i3): " << type_name<decltype(i3)>() << std::endl;
		std::cout << "demangle type name (i3): " << demangle_type_name<decltype(i3)>() << std::endl;
	}
	std::cout << std::endl << std::endl;
//=================== end ===================

	console->info("Goodbye!");

	return 0;
}