#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "integer_literal_scan.h"

BaseType ScanBase(const char*& p)
{
	const char* yymarker;
/*!re2c
	re2c:define:YYCTYPE  = "unsigned char";
	re2c:define:YYCURSOR = p;
	re2c:define:YYMARKER = yymarker;
	re2c:yyfill:enable   = 0;
	re2c:yych:conversion = 1;
	re2c:indent:top      = 1;
	
	Digit = [0-9];
	NonZerodigit = [1-9];
	OctalDigit = [0-7];
	HexadecimalDigit = [0-9a-fA-F];
	
	DecimalConstant = NonZerodigit Digit*;
	OctalConstant = "0" OctalDigit*;
	HexadecimalPrefix = "0" [xX];
	HexadecimalConstant = HexadecimalPrefix HexadecimalDigit+;
	
	DecimalConstant		{ return BaseType_Decimal; }
	HexadecimalConstant	{ return BaseType_Hexadecimal; }
	OctalConstant		{ return BaseType_Octal; }
*/
}

int ScanSuffix(const char* p)
{
	const char* yymarker;
/*!re2c
	UnsignedSuffix = [uU];
	LongSuffix = [lL];
	LongLongSuffix = "ll" | "LL";
	other = .;
	
	(LongLongSuffix UnsignedSuffix) | (UnsignedSuffix LongLongSuffix)
					{ return Suffix_LongLong | Suffix_Unsigned; }
	
	(LongSuffix UnsignedSuffix) | (UnsignedSuffix LongSuffix)
					{ return Suffix_Long | Suffix_Unsigned; }
	
	LongLongSuffix	{ return Suffix_LongLong; }
	
	LongSuffix		{ return Suffix_Long; }
	
	UnsignedSuffix	{ return Suffix_Unsigned; }
	
	other {
		return Suffix_None;
	}
*/
}


