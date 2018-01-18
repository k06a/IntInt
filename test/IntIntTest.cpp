// Copyright 2017 <ggarber@github>

#include <iostream>

#include <gtest/gtest.h>

#include "IntInt.hpp"

using namespace intint;

//

typedef IntInt<uint8_t> u16;
typedef IntInt<uint16_t> u32;
typedef IntInt<u16> u16u16;

const uint64_t ShortLimit = 1000;
//const uint64_t ShortLimit = 65536;

TEST(IntIntTest, Eq) { 
    for (int _i = 0; _i < ShortLimit; _i++) {
        unsigned short i = _i;
        
        u16 a = i;
        u16 b = i;
        u32 c = i;
        u16u16 d = i;

        ASSERT_TRUE(i == i);
        ASSERT_TRUE(i == a);
        ASSERT_TRUE(i == b);
        ASSERT_TRUE(i == c);
        ASSERT_TRUE(i == d);

        ASSERT_TRUE(a == i);
        ASSERT_TRUE(a == a);
        ASSERT_TRUE(a == b);
        ASSERT_TRUE(a == c);
        ASSERT_TRUE(a == d);

        ASSERT_TRUE(b == i);
        ASSERT_TRUE(b == a);
        ASSERT_TRUE(b == b);
        ASSERT_TRUE(b == c);
        ASSERT_TRUE(b == d);

        ASSERT_TRUE(c == i);
        ASSERT_TRUE(c == a);
        ASSERT_TRUE(c == b);
        ASSERT_TRUE(c == c);
        ASSERT_TRUE(c == d);

        ASSERT_TRUE(d == i);
        ASSERT_TRUE(d == a);
        ASSERT_TRUE(d == b);
        ASSERT_TRUE(d == c);
        ASSERT_TRUE(d == d);
    }
}

TEST(IntIntTest, Lt) { 
    for (int _i = 0; _i < ShortLimit; _i++) {
        for (int _j = 0; _j < ShortLimit; _j++) {
            unsigned short i = _i;
            unsigned short j = _j;
        
            u16 a = i;
            u16 b = j;
            u32 c = j;
            u16u16 d = j;

            ASSERT_TRUE((a < b) == (i < j));
            ASSERT_TRUE((a < c) == (i < j));
            ASSERT_TRUE((a < d) == (i < j));
            
            ASSERT_TRUE((b < a) == (j < i));
            ASSERT_TRUE((c < a) == (j < i));
            ASSERT_TRUE((d < a) == (j < i));
        }
    }
}

TEST(IntIntTest, Gt) { 
    for (int _i = 0; _i < ShortLimit; _i++) {
        for (int _j = 0; _j < ShortLimit; _j++) {
            unsigned short i = _i;
            unsigned short j = _j;
        
            u16 a = i;
            u16 b = j;
            u32 c = j;
            u16u16 d = j;

            ASSERT_TRUE((a > b) == (i > j));
            ASSERT_TRUE((a > c) == (i > j));
            ASSERT_TRUE((a > d) == (i > j));
            
            ASSERT_TRUE((b > a) == (j > i));
            ASSERT_TRUE((c > a) == (j > i));
            ASSERT_TRUE((d > a) == (j > i));
        }
    }
}

TEST(IntIntTest, Shift) { 
    for (int _i = 0; _i < ShortLimit; _i++) {
        for (int _j = 0; _j < sizeof(int) * CHAR_BIT; _j++) {
            unsigned short i = _i;
            unsigned short j = _j;

            u16 a = i;
            u32 b = i;
            u16u16 c = i;
            
            ASSERT_TRUE((a << j) == (i << j));
            ASSERT_TRUE((a >> j) == (i >> j));

            ASSERT_TRUE((b << j) == (i << j));
            ASSERT_TRUE((b >> j) == (i >> j));

            ASSERT_TRUE((c << j) == (i << j));
            ASSERT_TRUE((c >> j) == (i >> j));
        }
    }
}

TEST(IntIntTest, Or) { 
    for (int _i = 0; _i < ShortLimit; _i++) {
        for (int _j = 0; _j < ShortLimit; _j++) {
            unsigned short i = _i;
            unsigned short j = _j;

            u16 a = i;
            u16 b = j;
            u32 c = j;
            u16u16 d = j;
            
            // (IntInt + IntInt) == (int + int)
            // (IntInt + int) == (int + IntInt)
            // (int + int) == (IntInt + IntInt)
            ASSERT_TRUE((a | b) == (i | j));
            ASSERT_TRUE((a | j) == (i | b));
            ASSERT_TRUE((i | j) == (a | b));

            ASSERT_TRUE((a | c) == (i | j));
            ASSERT_TRUE((c | a) == (i | j));
            ASSERT_TRUE((a | d) == (i | j));
            ASSERT_TRUE((d | a) == (i | j));
        }
    }
}

TEST(IntIntTest, And) { 
    for (int _i = 0; _i < ShortLimit; _i++) {
        for (int _j = 0; _j < ShortLimit; _j++) {
            unsigned short i = _i;
            unsigned short j = _j;

            u16 a = i;
            u16 b = j;
            u32 c = j;
            u16u16 d = j;
            
            // (IntInt + IntInt) == (int + int)
            // (IntInt + int) == (int + IntInt)
            // (int + int) == (IntInt + IntInt)
            ASSERT_TRUE((a & b) == (i & j));
            ASSERT_TRUE((a & j) == (i & b));
            ASSERT_TRUE((i & j) == (a & b));

            ASSERT_TRUE((a & c) == (i & j));
            ASSERT_TRUE((c & a) == (i & j));
            ASSERT_TRUE((a & d) == (i & j));
            ASSERT_TRUE((d & a) == (i & j));
        }
    }
}

TEST(IntIntTest, Add) { 
    for (int _i = 256; _i < ShortLimit; _i++) {
        for (int _j = 0; _j < ShortLimit; _j++) {
            unsigned short i = _i;
            unsigned short j = _j;

            u16 a = i;
            u16 b = j;
            u32 c = j;
            u16u16 d = j;

            // (IntInt + IntInt) == (int + int)
            // (IntInt + int) == (int + IntInt)
            // (int + int) == (IntInt + IntInt)
            ASSERT_TRUE((a + b) == (i + j));
            ASSERT_TRUE((a + j) == (i + b));
            ASSERT_TRUE((i + j) == (a + b));

            ASSERT_TRUE((a + c) == (i + j));
            ASSERT_TRUE((c + a) == (i + j));
            ASSERT_TRUE((a + d) == (i + j));
            ASSERT_TRUE((d + a) == (i + j));
        }
    }
}
