#ifndef INTINT_HPP_
#define INTINT_HPP_

#include <limits.h>
#include <type_traits>
#include <iostream>
#include <iomanip>

namespace intint {

template<typename T>
class IntInt {
public:
    T low;
    T high;

    IntInt(T low, T high) 
        : low(low)
        , high(high)
    {
    }
    
    IntInt(int value = 0) 
        : low(value)
        , high(value >> (sizeof(T) * CHAR_BIT))
    {
        //std::cout << 'z' << (int)value << ':' << (int)low << ':' << (int)high << 'z';
    }

    template<typename T2, typename = typename std::enable_if<sizeof(T2)<sizeof(T)>::type>
    IntInt(const IntInt<T2> & value)  // Implicit cast to greater
        : low()
        , high()
    {
        unsigned offset = CHAR_BIT * (sizeof(T) - sizeof(T2));
        low = T(value.low | (value.high << (CHAR_BIT * sizeof(T2))));
        high = T(value.high >> offset);
    }

    template<typename T2>
    explicit IntInt(const IntInt<T2> & value, const char * = 0)
        : low()
        , high()
    {
        if (sizeof(T2) < sizeof(T)) {
            unsigned offset = CHAR_BIT * (sizeof(T) - sizeof(T2));
            low = T(value.low | (value.high << (CHAR_BIT * sizeof(T2))));
            high = T(value.high >> offset);
        }
        else {
            unsigned offset = CHAR_BIT * (sizeof(T2) - sizeof(T));
            low = T(value.low);
            high = T((value.high << offset) | (value.low >> offset));
        }
    }

    template<typename T2, typename = typename std::enable_if<std::is_integral<T2>::value>::type>
    explicit operator T2() const {
        return T2(low) | T2(high << (CHAR_BIT * sizeof(T)));
    }

    IntInt<T> operator >> (unsigned value) const {
        unsigned offset = sizeof(T) * CHAR_BIT;
        return IntInt<T>(
            (low >> value) | ((value < offset) ? (high << (offset - value)) : (high >> (value - offset))),
            high >> value
        );
    }

    IntInt<T> operator << (unsigned value) const {
        unsigned offset = sizeof(T) * CHAR_BIT;
        return IntInt<T>(
            low << value,
            (high << value) | ((value < offset) ? (low >> (offset - value)) : (low << (value - offset)))
        );
    }
    
};

// Stream operators

template<typename T>
std::ostream & operator << (std::ostream & stream, const IntInt<T> & value) {
    if (std::is_integral<T>::value) {
        return stream << std::hex << std::setfill('0')
                      << std::setw(sizeof(T)*2) << (uint64_t)(value.high) 
                      << std::setw(sizeof(T)*2) << (uint64_t)(value.low);
    }
    return stream << std::hex << std::setfill('0')
                  << std::setw(sizeof(T)*2) << value.high
                  << std::setw(sizeof(T)*2) << value.low;
}

template<typename T>
std::istream & operator >> (std::istream & stream, IntInt<T> & value) {
    return stream >> std::hex >> std::setw(sizeof(T)*2) >> value.high >> value.low;
}

// Operator ==

template<typename T1, typename T2>
bool operator == (const IntInt<T1> & first, const IntInt<T2> & second) {
    if (sizeof(T1) == sizeof(T2)) {
        return first.low == second.low
            && first.high == second.high;
    }

    // Corner case for example: IntInt<uint8_t> == IntInt<uint16_t>
    if (std::is_integral<T1>::value && std::is_integral<T2>::value) {
        return IntInt<uint64_t>(first, "") == IntInt<uint64_t>(second, "");
    }

    if (sizeof(T1) < sizeof(T2)) {
        return IntInt<T2>(first, "") == second;
    }
    else {
        return first == IntInt<T1>(second, "");
    }
}

template<typename T1, typename T2>
bool operator == (const IntInt<T1> & first, const T2 & second) {
    return first == IntInt<T1>(second);
}

template<typename T1, typename T2>
bool operator == (const T1 & first, const IntInt<T2> & second) {
    return IntInt<T2>(first) == second;
}

// Operator <

template<typename T1, typename T2>
bool operator < (const IntInt<T1> & first, const IntInt<T2> & second) {
    if (sizeof(T1) == sizeof(T2)) {
        return first.high < second.high 
            || (first.high == second.high && first.low < second.low);
    }

    // Corner case for example: IntInt<uint8_t> == IntInt<uint16_t>
    if (std::is_integral<T1>::value && std::is_integral<T2>::value) {
        return IntInt<uint64_t>(first, "") < IntInt<uint64_t>(second, "");
    }

    if (sizeof(T1) < sizeof(T2)) {
        return IntInt<T2>(first, "") < second;
    }
    else {
        return first < IntInt<T1>(second, "");
    }
}

template<typename T1, typename T2>
bool operator < (const IntInt<T1> & first, const T2 & second) {
    return first < IntInt<T1>(second);
}

template<typename T1, typename T2>
bool operator < (const T1 & first, const IntInt<T2> & second) {
    return IntInt<T2>(first) < second;
}

// Operator >

template<typename T1, typename T2>
bool operator > (const IntInt<T1> & first, const IntInt<T2> & second) {
    if (sizeof(T1) == sizeof(T2)) {
        return first.high > second.high 
            || (first.high == second.high && first.low > second.low);
    }

    // Corner case for example: IntInt<uint8_t> == IntInt<uint16_t>
    if (std::is_integral<T1>::value && std::is_integral<T2>::value) {
        return IntInt<uint64_t>(first, "") > IntInt<uint64_t>(second, "");
    }

    if (sizeof(T1) < sizeof(T2)) {
        return IntInt<T2>(first, "") > second;
    }
    else {
        return first > IntInt<T1>(second, "");
    }
}

template<typename T1, typename T2>
bool operator > (const IntInt<T1> & first, const T2 & second) {
    return first > IntInt<T1>(second);
}

template<typename T1, typename T2>
bool operator > (const T1 & first, const IntInt<T2> & second) {
    return IntInt<T2>(first) > second;
}

// Operator |

template<typename T1, typename T2, typename T = typename std::conditional<sizeof(T1)<sizeof(T2),T2,T1>::type>
IntInt<T> operator | (const IntInt<T1> & first, const IntInt<T2> & second) {
    if (sizeof(T1) == sizeof(T2)) {
        return IntInt<T>(
            first.low | second.low,
            first.high | second.high
        );
    }

    // Corner case for example: IntInt<uint8_t> == IntInt<uint16_t>
    if (std::is_integral<T1>::value && std::is_integral<T2>::value) {
        return IntInt<T>(IntInt<uint64_t>(first, "") | IntInt<uint64_t>(second, ""));
    }

    if (sizeof(T1) < sizeof(T2)) {
        return IntInt<T2>(first, "") | second;
    }
    else {
        return first | IntInt<T1>(second, "");
    }
}

template<typename T1, typename T2>
IntInt<T1> operator | (const IntInt<T1> & first, const T2 & second) {
    return first | IntInt<T1>(second);
}

template<typename T1, typename T2>
IntInt<T2> operator | (const T1 & first, const IntInt<T2> & second) {
    return IntInt<T2>(first) | second;
}

// Operator &

template<typename T1, typename T2, typename T = typename std::conditional<sizeof(T1)<sizeof(T2),T2,T1>::type>
IntInt<T> operator & (const IntInt<T1> & first, const IntInt<T2> & second) {
    if (sizeof(T1) == sizeof(T2)) {
        return IntInt<T>(
            first.low & second.low,
            first.high & second.high
        );
    }

    // Corner case for example: IntInt<uint8_t> == IntInt<uint16_t>
    if (std::is_integral<T1>::value && std::is_integral<T2>::value) {
        return IntInt<T>(IntInt<uint64_t>(first, "") & IntInt<uint64_t>(second, ""));
    }

    if (sizeof(T1) < sizeof(T2)) {
        return IntInt<T2>(first, "") & second;
    }
    else {
        return first & IntInt<T1>(second, "");
    }
}

template<typename T1, typename T2>
IntInt<T1> operator & (const IntInt<T1> & first, const T2 & second) {
    return first & IntInt<T1>(second);
}

template<typename T1, typename T2>
IntInt<T2> operator & (const T1 & first, const IntInt<T2> & second) {
    return IntInt<T2>(first) & second;
}

// Operator +

template<typename T1, typename T2, typename T = typename std::conditional<sizeof(T1)<sizeof(T2),T2,T1>::type>
IntInt<T> operator + (const IntInt<T1> & first, const IntInt<T2> & second) {
    if (sizeof(T1) == sizeof(T2)) {
        T low = first.low + second.low;
        return IntInt<T>(
            low,
            first.high + second.high + (low < first.low)
        );
    }

    // Corner case for example: IntInt<uint8_t> == IntInt<uint16_t>
    if (std::is_integral<T1>::value && std::is_integral<T2>::value) {
        return IntInt<T>(IntInt<uint64_t>(first, "") + IntInt<uint64_t>(second, ""));
    }

    if (sizeof(T1) < sizeof(T2)) {
        return IntInt<T2>(first, "") + second;
    }
    else {
        return first + IntInt<T1>(second, "");
    }
}

template<typename T1, typename T2>
IntInt<T1> operator + (const IntInt<T1> & first, const T2 & second) {
    return first + IntInt<T1>(second);
}

template<typename T1, typename T2>
IntInt<T2> operator + (const T1 & first, const IntInt<T2> & second) {
    return IntInt<T2>(first) + second;
}

}  // namespace bootstrap

#endif  // INTINT_HPP_
