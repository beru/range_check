
#include "value.h"

#include <type_traits>
#include <new>

namespace {

struct ValueBase
{
	virtual ValueBase& operator = (ValueBase& rhs) = 0;
	virtual Value operator - (void) = 0;

	virtual ValueBase& operator += (ValueBase& rhs) = 0;
	virtual ValueBase& operator -= (ValueBase& rhs) = 0;
	virtual ValueBase& operator *= (ValueBase& rhs) = 0;
	virtual ValueBase& operator /= (ValueBase& rhs) = 0;
	virtual ValueBase& operator %= (ValueBase& rhs) = 0;
	virtual ValueBase& operator |= (ValueBase& rhs) = 0;
	virtual ValueBase& operator &= (ValueBase& rhs) = 0;
	virtual ValueBase& operator ^= (ValueBase& rhs) = 0;
	virtual ValueBase& operator <<= (ValueBase& rhs) = 0;
	virtual ValueBase& operator >>= (ValueBase& rhs) = 0;
	virtual ValueBase& operator ++ (void) = 0;
	virtual Value operator ++ (int) = 0;
	virtual ValueBase& operator -- (void) = 0;
	virtual Value operator -- (int) = 0;

	virtual Value operator + (ValueBase& rhs) = 0;
	virtual Value operator - (ValueBase& rhs) = 0;
	virtual Value operator * (ValueBase& rhs) = 0;
	virtual Value operator / (ValueBase& rhs) = 0;
	virtual Value operator % (ValueBase& rhs) = 0;
	virtual Value operator | (ValueBase& rhs) = 0;
	virtual Value operator & (ValueBase& rhs) = 0;
	virtual Value operator ^ (ValueBase& rhs) = 0;
	virtual Value operator << (ValueBase& rhs) = 0;
	virtual Value operator >> (ValueBase& rhs) = 0;
	
	virtual bool operator == (ValueBase& rhs) = 0;
	virtual bool operator < (ValueBase& rhs) = 0;
	virtual bool operator > (ValueBase& rhs) = 0;
	virtual bool operator <= (ValueBase& rhs) = 0;
	virtual bool operator >= (ValueBase& rhs) = 0;

	virtual signed long long getSLL() const = 0;
	virtual unsigned long long getULL() const = 0;
	virtual bool isSignedType() const = 0;
};

template<typename T> Value makeValue(T v);
template<> Value makeValue<unsigned char>(unsigned char v) { Value ret(Value::Type::uc); ret.uc = v; return ret; }
template<> Value makeValue<signed char>(signed char v) { Value ret(Value::Type::sc); ret.sc = v; return ret; }
template<> Value makeValue<unsigned short>(unsigned short v) { Value ret(Value::Type::us); ret.us = v; return ret; }
template<> Value makeValue<signed short>(signed short v) { Value ret(Value::Type::ss); ret.ss = v; return ret; }
template<> Value makeValue<unsigned int>(unsigned int v) { Value ret(Value::Type::ui); ret.ui = v; return ret; }
template<> Value makeValue<signed int>(signed int v) { Value ret(Value::Type::si); ret.si = v; return ret; }
template<> Value makeValue<unsigned long>(unsigned long v) { Value ret(Value::Type::ul); ret.ul = v; return ret; }
template<> Value makeValue<signed long>(signed long v) { Value ret(Value::Type::sl); ret.sl = v; return ret; }
template<> Value makeValue<unsigned long long>(unsigned long long v) { Value ret(Value::Type::ull); ret.ull = v; return ret; }
template<> Value makeValue<signed long long>(signed long long v) { Value ret(Value::Type::sll); ret.sll = v; return ret; }

template <typename T>
struct ValueTemplate : public ValueBase
{
	T& v;
	ValueTemplate(T& v) : v(v) {}

	T toT(ValueBase& rhs) {
		return (T)(rhs.isSignedType() ? rhs.getSLL() : rhs.getULL());
	}

	virtual ValueBase& operator = (ValueBase& rhs) { v = toT(rhs); return *this; }
	virtual Value operator - (void) { Value ret = makeValue(-v); return ret; }

	virtual ValueBase& operator += (ValueBase& rhs) { v += toT(rhs); return *this; }
	virtual ValueBase& operator -= (ValueBase& rhs) { v -= toT(rhs); return *this; }
	virtual ValueBase& operator *= (ValueBase& rhs) { v *= toT(rhs); return *this; }
	virtual ValueBase& operator /= (ValueBase& rhs) { v /= toT(rhs); return *this; }
	virtual ValueBase& operator %= (ValueBase& rhs) { v %= toT(rhs); return *this; }
	virtual ValueBase& operator |= (ValueBase& rhs) { v |= toT(rhs); return *this; }
	virtual ValueBase& operator &= (ValueBase& rhs) { v &= toT(rhs); return *this; }
	virtual ValueBase& operator ^= (ValueBase& rhs) { v ^= toT(rhs); return *this; }
	virtual ValueBase& operator <<= (ValueBase& rhs) { v <<= toT(rhs); return *this; }
	virtual ValueBase& operator >>= (ValueBase& rhs) { v >>= toT(rhs); return *this; }
	virtual ValueBase& operator ++ (void) { ++v; return *this; }
	virtual Value operator ++ (int) { Value ret = makeValue(v); v++; return ret; }
	virtual ValueBase& operator -- (void) { --v; return *this; }
	virtual Value operator -- (int) { Value ret = makeValue(v); v--; return ret; }

	virtual Value operator + (ValueBase& rhs) { return makeValue(v + (T)rhs.getSLL()); }
	virtual Value operator - (ValueBase& rhs) { return makeValue(v - (T)rhs.getSLL()); }
	virtual Value operator * (ValueBase& rhs) { return makeValue(v * (T)rhs.getSLL()); }
	virtual Value operator / (ValueBase& rhs) { return makeValue(v / (T)rhs.getSLL()); }
	virtual Value operator % (ValueBase& rhs) { return makeValue(v % (T)rhs.getSLL()); }
	virtual Value operator | (ValueBase& rhs) { return makeValue(v | (T)rhs.getSLL()); }
	virtual Value operator & (ValueBase& rhs) { return makeValue(v & (T)rhs.getSLL()); }
	virtual Value operator ^ (ValueBase& rhs) { return makeValue(v ^ (T)rhs.getSLL()); }
	virtual Value operator << (ValueBase& rhs) { return makeValue(v << (T)rhs.getSLL()); }
	virtual Value operator >> (ValueBase& rhs) { return makeValue(v >> (T)rhs.getSLL()); }

	virtual bool operator == (ValueBase& rhs) { return v == toT(rhs); }
	virtual bool operator < (ValueBase& rhs) { return v < toT(rhs); }
	virtual bool operator > (ValueBase& rhs) { return v > toT(rhs); }
	virtual bool operator <= (ValueBase& rhs) { return v <= toT(rhs); }
	virtual bool operator >= (ValueBase& rhs) { return v >= toT(rhs); }


	virtual signed long long getSLL() const { return v; }
	virtual unsigned long long getULL() const { return v; }
	virtual bool isSignedType() const {
		return std::is_signed<T>();
	}
};

template <typename T>
ValueTemplate<T>& newRef(int idx, const T& v)
{
	static char buff[2][256];
	void* addr = buff[idx];
	return * ::new (addr) ValueTemplate<T>(const_cast<T&>(v));
}

ValueBase& newBase(int idx, const Value& v)
{
	switch (v.type) {
	case Value::Type::uc: return newRef(idx, v.uc);
	case Value::Type::us: return newRef(idx, v.us);
	case Value::Type::ui: return newRef(idx, v.ui);
	case Value::Type::ul: return newRef(idx, v.ul);
	case Value::Type::ull: return newRef(idx, v.ull);
	case Value::Type::sc: return newRef(idx, v.sc);
	case Value::Type::ss: return newRef(idx, v.ss);
	case Value::Type::si: return newRef(idx, v.si);
	case Value::Type::sl: return newRef(idx, v.sl);
	case Value::Type::sll: return newRef(idx, v.sll);
//	case Value::Type::fl: return newRef(idx, v.f);
//	case Value::Type::db: return newRef(idx, v.d);
	}
}

template <typename T>
void changeType(T& nv, Value::Type nt, Value& v)
{
	switch (v.type) {
	case Value::Type::uc: nv = (T) v.uc; break;
	case Value::Type::us: nv = (T) v.us; break;
	case Value::Type::ui: nv = (T) v.ui; break;
	case Value::Type::ul: nv = (T) v.ul; break;
	case Value::Type::ull: nv = (T) v.ull; break;
	case Value::Type::sc: nv = (T) v.sc; break;
	case Value::Type::ss: nv = (T) v.ss; break;
	case Value::Type::si: nv = (T) v.si; break;
	case Value::Type::sl: nv = (T) v.sl; break;
	case Value::Type::sll: nv = (T) v.sll; 	break;
	}
	v.type = nt;
}

void promote(Value& v)
{
	if (v.type  < Value::Type::si) {
		changeType(v.si, Value::Type::si, v);
	}
}

inline
bool typeOr(const Value& lhs, const Value& rhs, Value::Type match)
{
	return lhs.type == match || rhs.type == match;
}

void promote(Value& lhs, Value& rhs)
{
	if (typeOr(lhs, rhs, Value::Type::ull)) {
		changeType(lhs.ull, Value::Type::ull, lhs);
		changeType(rhs.ull, Value::Type::ull, rhs);
	}else if (typeOr(lhs, rhs, Value::Type::sll)) {
		changeType(lhs.sll, Value::Type::sll, lhs);
		changeType(rhs.sll, Value::Type::sll, rhs);
	}else if (typeOr(lhs, rhs, Value::Type::ul)) {
		changeType(lhs.ul, Value::Type::ul, lhs);
		changeType(rhs.ul, Value::Type::ul, rhs);
	}else if (typeOr(lhs, rhs, Value::Type::sl)) {
		changeType(lhs.sl, Value::Type::sl, lhs);
		changeType(rhs.sl, Value::Type::sl, rhs);
	}else if (typeOr(lhs, rhs, Value::Type::ui)) {
		changeType(lhs.ui, Value::Type::ui, lhs);
		changeType(rhs.ui, Value::Type::ui, rhs);
	}else /* if (typeOr(lhs, rhs, Value::Type::si)) {
		changeType(lhs.si, Value::Type::si, lhs);
		changeType(rhs.si, Value::Type::si, rhs);
	}else */ {
		changeType(lhs.si, Value::Type::si, lhs);
		changeType(rhs.si, Value::Type::si, rhs);
	}
}

} // anonymous namespace

Value& Value::operator = (uint64_t rhs) { ull = rhs; type = Type::ull; return *this; }
Value& Value::operator = (uint32_t rhs) { ui = rhs; type = Type::ui; return *this; }
Value& Value::operator = (uint16_t rhs) { us = rhs; type = Type::us; return *this; }
Value& Value::operator = (uint8_t rhs) { uc = rhs; type = Type::uc; return *this; }
Value& Value::operator = (int64_t rhs) { sll = rhs; type = Type::sll; return *this; }
Value& Value::operator = (int32_t rhs) { si = rhs; type = Type::si; return *this; }
Value& Value::operator = (int16_t rhs) { ss = rhs; type = Type::ss; return *this; }
Value& Value::operator = (int8_t rhs) { sc = rhs; type = Type::sc; return *this; }

Value Value::Cast(Type nt)
{
	Value nv = *this;
	switch (nt) {
	case Value::Type::uc: changeType(nv.uc, nt, nv); break;
	case Value::Type::us: changeType(nv.us, nt, nv); break;
	case Value::Type::ui: changeType(nv.ui, nt, nv); break;
	case Value::Type::ul: changeType(nv.ul, nt, nv); break;
	case Value::Type::ull: changeType(nv.ull, nt, nv); break;
	case Value::Type::sc: changeType(nv.sc, nt, nv); break;
	case Value::Type::ss: changeType(nv.ss, nt, nv); break;
	case Value::Type::si: changeType(nv.si, nt, nv); break;
	case Value::Type::sl: changeType(nv.sl, nt, nv); break;
	case Value::Type::sll: changeType(nv.sll, nt, nv); break;
	}
	return nv;
}

Value& Value::operator = (Value rhs) { newBase(0,*this) = newBase(1,rhs); return *this; }
Value Value::operator - (void) { Value ret = *this; promote(ret); return -newBase(0,ret); }

Value& Value::operator += (Value rhs) { newBase(0,*this) += newBase(1,rhs); return *this; }
Value& Value::operator -= (Value rhs) { newBase(0,*this) -= newBase(1,rhs); return *this; }
Value& Value::operator *= (Value rhs) { newBase(0,*this) *= newBase(1,rhs); return *this; }
Value& Value::operator /= (Value rhs) { newBase(0,*this) /= newBase(1,rhs); return *this; }
Value& Value::operator %= (Value rhs) { newBase(0,*this) %= newBase(1,rhs); return *this; }
Value& Value::operator |= (Value rhs) { newBase(0,*this) |= newBase(1,rhs); return *this; }
Value& Value::operator &= (Value rhs) { newBase(0,*this) &= newBase(1,rhs); return *this; }
Value& Value::operator ^= (Value rhs) { newBase(0,*this) ^= newBase(1,rhs); return *this; }
Value& Value::operator <<= (Value rhs) { newBase(0,*this) <<= newBase(1,rhs); return *this; }
Value& Value::operator >>= (Value rhs) { newBase(0,*this) >>= newBase(1,rhs); return *this; }
Value& Value::operator ++ (void) { ++newBase(0,*this); return *this; }
Value Value::operator ++ (int) { Value ret = *this; newBase(0,*this)++; return ret; }
Value& Value::operator -- (void) { --newBase(0,*this); return *this; }
Value Value::operator -- (int) { Value ret = *this; newBase(0,*this)--; return ret; }

Value Value::operator + (Value rhs) { Value lhs = *this; promote(lhs, rhs); return newBase(0,lhs) + newBase(1,rhs); }
Value Value::operator - (Value rhs) { Value lhs = *this; promote(lhs, rhs); return newBase(0,lhs) - newBase(1,rhs); }
Value Value::operator * (Value rhs) { Value lhs = *this; promote(lhs, rhs); return newBase(0,lhs) * newBase(1,rhs); }
Value Value::operator / (Value rhs) { Value lhs = *this; promote(lhs, rhs); return newBase(0,lhs) / newBase(1,rhs); }
Value Value::operator % (Value rhs) { Value lhs = *this; promote(lhs, rhs); return newBase(0,lhs) % newBase(1,rhs); }
Value Value::operator | (Value rhs) { Value lhs = *this; promote(lhs, rhs); return newBase(0,lhs) | newBase(1,rhs); }
Value Value::operator & (Value rhs) { Value lhs = *this; promote(lhs, rhs); return newBase(0,lhs) & newBase(1,rhs); }
Value Value::operator ^ (Value rhs) { Value lhs = *this; promote(lhs, rhs); return newBase(0,lhs) ^ newBase(1,rhs); }
Value Value::operator << (Value rhs) { Value lhs = *this; promote(lhs, rhs); return newBase(0,lhs) << newBase(1,rhs); }
Value Value::operator >> (Value rhs) { Value lhs = *this; promote(lhs, rhs); return newBase(0,lhs) >> newBase(1,rhs); }

bool Value::operator == (Value rhs) { return newBase(0,*this) == newBase(1,rhs); }
bool Value::operator < (Value rhs) { return newBase(0,*this) < newBase(1,rhs); }
bool Value::operator > (Value rhs) { return newBase(0,*this) > newBase(1,rhs); }
bool Value::operator <= (Value rhs) { return newBase(0,*this) <= newBase(1,rhs); }
bool Value::operator >= (Value rhs) { return newBase(0,*this) >= newBase(1,rhs); }

