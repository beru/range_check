#pragma once

#include <stdint.h>

struct ValueBase;

struct Value
{
	union {
		char c;
		unsigned char uc;
		unsigned short us;
		unsigned int ui;
		unsigned long ul;
		unsigned long long ull;

		signed char sc;
		signed short ss;
		signed int si;
		signed long sl;
		signed long long sll;

//		float f;
//		double d;
	};
	
	enum class Type : int {
		uc = 0,
		us = 2,
		ui = 4,
		ul = 8,
		ull= 16,

		sc = 1,
		ss = 3,
		si = 5,
		sl = 9,
		sll = 17,

//		fl = 32,
//		db = 64,
	} type;

	Value()
		:
		type(Value::Type::si)
	{
	}

	Value(Type t)
		:
		type(t)
	{
	}

	Value(const Value& v)
		:
		type(v.type),
		ull(v.ull)
	{}

	Value& operator = (uint64_t rhs);
	Value& operator = (uint32_t rhs);
	Value& operator = (uint16_t rhs);
	Value& operator = (uint8_t rhs);
	Value& operator = (int64_t rhs);
	Value& operator = (int32_t rhs);
	Value& operator = (int16_t rhs);
	Value& operator = (int8_t rhs);

	Value Cast(Type nt);

	Value& operator = (Value rhs);		// a = b
	Value operator - (void);			// -a

	Value& operator += (Value rhs);		// a += b
	Value& operator -= (Value rhs);		// a -= b
	Value& operator *= (Value rhs);		// a *= b
	Value& operator /= (Value rhs);		// a /= b
	Value& operator %= (Value rhs);		// a %= b
	Value& operator |= (Value rhs);		// a |= b
	Value& operator &= (Value rhs);		// a &= b
	Value& operator ^= (Value rhs);		// a ^= b
	Value& operator <<= (Value rhs);	// a <<= b
	Value& operator >>= (Value rhs);	// a >>= b
	Value& operator ++ (void);			// ++a
	Value operator ++ (int);			// a++
	Value& operator -- (void);			// --a
	Value operator -- (int);			// a--

	Value operator + (Value rhs);		// a + b
	Value operator - (Value rhs);		// a - b
	Value operator * (Value rhs);		// a * b
	Value operator / (Value rhs);		// a / b
	Value operator % (Value rhs);		// a % b
	Value operator | (Value rhs);		// a | b
	Value operator & (Value rhs);		// a & b
	Value operator ^ (Value rhs);		// a ^ b
	Value operator << (Value rhs);		// a << b
	Value operator >> (Value rhs);		// a >> b

	bool operator == (Value rhs);		// a == b
	bool operator != (Value rhs);		// a != b
	bool operator < (Value rhs);		// a < b
	bool operator > (Value rhs);		// a > b
	bool operator <= (Value rhs);		// a <= b
	bool operator >= (Value rhs);		// a >= b

};

