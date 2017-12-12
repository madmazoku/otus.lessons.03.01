#pragma once

template<typename... Args>
void iterate(Args... args)
{
    std::cout << "iterate: " << sizeof...(args) << std::endl;
    // printf("printf: %d %d %d\n", args...);

    std::cout << "{";
    int a[sizeof...(args)] = {(std::cout << args, 0)...}; // {(cout << arg1, 0), (cout << arg2, 0), (cout << arg3, 0)}
    std::cout << "}" << std::endl;
}

void iterate2() {
    std::cout << std::endl;
}

template<typename T, typename... Args>
void iterate2(T t, Args... args)
{
    std::cout << "{" << sizeof...(args) << ", " << t << "} ";
    iterate2(args...);
}
