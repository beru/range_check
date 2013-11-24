#pragma once

#include <boost/operators.hpp>

struct Value : private boost::operators<Value> {

	union {
		uint8_t u1;
		uint16_t u2;
		uint32_t u4;
		uint64_t u8;

		int8_t s1;
		int16_t s2;
		int32_t s4;
		int64_t s8;
	};

	Value operator - () {
		Value ret;
		ret.s8 = -s8;
		return ret;
	}

	bool operator < (const Value& rhs) {
		return s8 < rhs.s8;
	}

	bool operator == (const Value& rhs) {
		return s8 == rhs.s8;
	}

	Value& operator += (const Value& rhs) {
		s8 += rhs.s8;
		return *this;
	}

	Value& operator -= (const Value& rhs) {
		s8 -= rhs.s8;
		return *this;
	}

	Value& operator *= (const Value& rhs) {
		s8 *= rhs.s8;
		return *this;
	}

	Value& operator /= (const Value& rhs) {
		s8 /= rhs.s8;
		return *this;
	}

	Value& operator %= (const Value& rhs) {
		s8 %= rhs.s8;
		return *this;
	}

	Value& operator |= (const Value& rhs) {
		s8 |= rhs.s8;
		return *this;
	}

	Value& operator &= (const Value& rhs) {
		s8 &= rhs.s8;
		return *this;
	}

	Value& operator ^= (const Value& rhs) {
		s8 ^= rhs.s8;
		return *this;
	}

	Value& operator <<= (const Value& rhs) {
		s8 <<= rhs.s8;
		return *this;
	}

	Value& operator >>= (const Value& rhs) {
		s8 >>= rhs.s8;
		return *this;
	}

	Value& operator ++ () {
		++s8;
		return *this;
	}

	Value& operator -- () {
		--s8;
		return *this;
	}

};

Value operator << (const Value& lhs, const Value& rhs) {
	Value v = lhs;
	v <<= rhs;
	return v;
}

Value operator >> (const Value& lhs, const Value& rhs) {
	Value v = lhs;
	v >>= rhs;
	return v;
}
