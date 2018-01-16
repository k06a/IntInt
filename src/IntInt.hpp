#ifndef INTINT_HPP_
#define INTINT_HPP_

#include <iostream>

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
    
    IntInt(int value) 
        : low(value)
        , high(value >> (sizeof(T) * CHAR_BIT))
    {
        //std::cout << 'z' << (int)value << ':' << (int)low << ':' << (int)high << 'z';
    }

    template<typename T2>
    IntInt(const IntInt<T2> & value, typename std::enable_if<sizeof(T2)<=sizeof(T)>::type * = 0)  // Implicit cast to greater
        : low()
        , high()
    {
        unsigned offset = CHAR_BIT*(sizeof(T) - sizeof(T2));
        low = value.low | (value.high << offset);
        high = value.high >> offset;
    }

    template<typename T2>
    explicit IntInt(const IntInt<T2> & value)
        : low()
        , high()
    {
        if (sizeof(T) > sizeof(T2)) {
            unsigned offset = CHAR_BIT*(sizeof(T) - sizeof(T2));
            low = value.low | (value.high << offset);
            high = value.high >> offset;
        }
        else {
            unsigned offset = CHAR_BIT*(sizeof(T2) - sizeof(T));
            low = value.low;
            high = (value.low >> offset) | (value.high << offset);
        }
    }

    IntInt<T> operator >> (unsigned value) {
        unsigned offset = sizeof(T) * CHAR_BIT;
        return IntInt<T>(
            (low >> value) | ((value < offset) ? (high << (offset - value)) : (high >> (value - offset))),
            high >> value
        );
    }

    IntInt<T> operator << (unsigned value) {
        unsigned offset = sizeof(T) * CHAR_BIT;
        return IntInt<T>(
            low << value,
            (high << value) | ((value < offset) ? (low >> (offset - value)) : (low << (value - offset)))
        );
    }
    
};

// Operator ==

template<typename T1, typename T2, typename T = typename std::conditional<sizeof(T1)<=sizeof(T2),T1,T2>::type>
bool operator == (const IntInt<T1> & first, const IntInt<T2> & second) {
    return first.low == second.low
        && first.high == second.high;
}

template<typename T1, typename T2>
bool operator == (const IntInt<T1> & first, const T2 & second) {
    return first == IntInt<T1>(second);
}

template<typename T1, typename T2>
bool operator == (const T1 & first, const IntInt<T2> & second) {
    return IntInt<T2>(first) == second;
}

// Operator |

template<typename T1, typename T2, typename T = typename std::conditional<sizeof(T1)<=sizeof(T2),T1,T2>::type>
IntInt<T> operator | (const IntInt<T1> & first, const IntInt<T2> & second) {
    return IntInt<T>(
        first.low | second.low,
        first.high | second.high
    );
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

template<typename T1, typename T2, typename T = typename std::conditional<sizeof(T1)<=sizeof(T2),T1,T2>::type>
IntInt<T> operator & (const IntInt<T1> & first, const IntInt<T2> & second) {
    return IntInt<T>(
        first.low & second.low,
        first.high & second.high
    );
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

template<typename T1, typename T2, typename T = typename std::conditional<sizeof(T1)<=sizeof(T2),T1,T2>::type>
IntInt<T> operator + (const IntInt<T1> & first, const IntInt<T2> & second) {
    T tmp = first.low + second.low;
    return IntInt<T>(
        tmp,
        first.high + second.high + (tmp < first.low)
    );
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
