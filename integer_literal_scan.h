#pragma once

enum BaseType {
	BaseType_Decimal,
	BaseType_Hexadecimal,
	BaseType_Octal,
};

enum Suffix {
	Suffix_None = 0,
	Suffix_Unsigned = 1,
	Suffix_Long = 1 << 1,
	Suffix_LongLong = 1 << 2,
};

BaseType ScanBase(const char*& p);
int ScanSuffix(const char* p);

