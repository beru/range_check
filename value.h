#pragma once

struct ValueProxy
{
	virtual bool operator < (const ValueProxy& rhs);
	virtual bool operator > (const ValueProxy& rhs);
};

template <typename T>
struct ValueTemplate : public ValueProxy
{
	T v;

	virtual bool operator < (const ValueProxy& rhs) { return rhs > v; }
	virtual bool operator > (const ValueProxy& rhs) { return rhs < v; }
};

struct Value {

	union {
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

		float f;
		double d;
	};
	enum class Type : int {
		UC = 0,
		US = 2,
		UI = 4,
		UL = 8,
		ULL= 16,
		SC = 1,
		SS = 3,
		SI = 5,
		SL = 9,
		SLL = 17,
	} type;

	bool isSigned() const {
		return (int)type & 1;
	}
	void setSigned() {
		type = (Type) ((int)type | 1);
	}

	template <typename T>
	void get(T& v) {
		switch (type) {
		case Type::UC:
			v = uc;
			break;
		case Type::US:
			v = us;
			break;
		case Type::UI:
			v = ui;
			break;
		case Type::UL:
			v = ul;
			break;
		case Type::ULL:
			v = ull;
			break;
		case Type::SC:
			v = sc;
			break;
		case Type::SS:
			v = ss;
			break;
		case Type::SI:
			v = si;
			break;
		case Type::SL:
			v = sl;
			break;
		case Type::SLL:
			v = sll;
			break;
		}
	}

	void promote() {
		if (isSigned()) {
			if (type < Type::SI) {
				get(si);
				type = Type::SI;
			}else {
				get(sll);
				type = Type::SLL;
			}
		}else {
			if (type < Type::UI) {
				get(ui);
				type = Type::UI;
			}else {
				get(ull);
				type = Type::ULL;
			}
		}
	}

	Value operator - () {
		Value ret = *this;
		ret.promote();
		ret.setSigned();
		switch (ret.type) {
		case Type::SI:
			ret.si = -ret.si;
			break;
		case Type::SLL:
			ret.sll = -ret.sll;
			break;
		}
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
