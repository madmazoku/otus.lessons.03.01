#pragma once

#include <type_traits>
#include <typeinfo>
#include <cxxabi.h>
#include <memory>
#include <string>
#include <cstdlib>


template <class T>
std::string demangle_type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
                abi::__cxa_demangle(typeid(TR).name(), nullptr, nullptr, nullptr),
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}

class static_string
{
    const char* const p_;
    const std::size_t sz_;

public:
    typedef const char* const_iterator;

    template <std::size_t N>
    constexpr static_string(const char(&a)[N]) noexcept
        : p_(a)
        , sz_(N-1)
        {}

    constexpr static_string(const char* p, std::size_t N) noexcept
        : p_(p)
        , sz_(N)
        {}

    constexpr const char* data() const noexcept {return p_;}
    constexpr std::size_t size() const noexcept {return sz_;}

    constexpr const_iterator begin() const noexcept {return p_;}
    constexpr const_iterator end()   const noexcept {return p_ + sz_;}

    constexpr char operator[](std::size_t n) const
    {
        return n < sz_ ? p_[n] : throw std::out_of_range("static_string");
    }
};

inline std::ostream& operator<<(std::ostream& os, static_string const& s)
{
    return os.write(s.data(), s.size());
}

template <class T>
constexpr static_string type_name()
{
    static_string p = __PRETTY_FUNCTION__;
    return static_string(p.data() + 46, p.size() - 46 - 1);
}
