# IntInt
Extra efficient C++ BigInt implementation

# Why?
Using "Divide and Conquer" rule to construct big integer with smaller types recursively:
```
typedef IntInt<uint64_t> u128;
typedef IntInt<u128> u256;
typedef IntInt<u256> u512;
typedef IntInt<u512> u1024;
```
