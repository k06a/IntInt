#include <assert.h>
#include <iostream>

#include "IntInt.hpp"

using namespace intint;

int main(int argc, char *argv[]) {

    typedef IntInt<uint8_t> u16;
    typedef IntInt<uint16_t> u32;

    for (int _i = 0; _i < 1000; _i++) {
        std::cout << _i << std::endl;
        for (int _j = 0; _j < 65536; _j++) {
            unsigned short i = _i;
            unsigned short j = _j;

            u16 a = i;
            u16 b = j;
            // std::cout << i << ' ' << j << ' '
            //          << (int)((a << j).low) << ' ' << (int)((a << j).high) 
            //          << ' ' << (int)(u16(i << j).low) << ' ' << (int)(u16(i << j).high)
            //          << ' ' << (unsigned short)((unsigned short)i << (unsigned short)j) << std::endl;

            // (IntInt + IntInt) == (int + int)
            assert((a | b) == (i | j));
            assert((a & b) == (i & j));
            assert((a + b) == (i + j));

            // (IntInt + int) == (int + IntInt)
            assert((a | j) == (i | b));
            assert((a & j) == (i & b));
            assert((a + j) == (i + b));

            // (int + int) == (IntInt + IntInt)
            assert((i | j) == (a | b));
            assert((i & j) == (a & b));
            assert((i + j) == (a + b));
            
            if (j < sizeof(int) * CHAR_BIT) {
                assert((a << j) == (i << j));
                assert((a >> j) == (i >> j));

                assert((i << j) == (a << j));
                assert((i >> j) == (a >> j));
            }
        }
    }
}
