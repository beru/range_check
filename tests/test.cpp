
#include <stdio.h>

#include "value.h"
#include <limits.h>

int main(int argc, char* argv[])
{
	Value v0(Value::Type::si);
	Value v1(Value::Type::si);
	Value v2(Value::Type::si);

	v0.type = Value::Type::si;
	v0.si = 120;
	v1.type = Value::Type::sll;
	v1.sll = -100;
	v2.type = Value::Type::ull;
	v2.ull = 100000000000000;

	signed long long sll = -100;
	unsigned long long ull = v2.ull;
	ull *= sll;
	if (ull < sll) {
		int hoge = 0;
	}

	v0 = v2;
	v0 += v1;
	v0 -= v1;
	v0 *= v1;
	v0 *= v1;
	v0 *= v1;
	v0 /= v1;
	v2 *= v1;

	if (v2 < v1) {
		int hoge = 0;
	}

	v2.type = Value::Type::sll;
	v2 = v0 + v1;
	Value v = v0 * v1 * v1;

	int a = INT_MIN;
	int b = -a;

	Value va(Value::Type::si);
	va.si = a;
	Value vb(Value::Type::si);
	vb = -va;

	unsigned int c = UINT_MAX;
	int d = -(int)c;

	Value vc(Value::Type::ui);
	vc.ui = c;
	Value vd(Value::Type::si);
	vd = -vc;


	return 0;
}

