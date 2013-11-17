#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scan.h"

TokenType Scan(const char*& p, const char* eos)
{
	const char* yymarker;
initial:
	if (p == eos) {
		return TokenType::EndOfString;
	}
/*!re2c
	re2c:define:YYCTYPE  = "unsigned char";
	re2c:define:YYCURSOR = p;
	re2c:define:YYMARKER = yymarker;
	re2c:yyfill:enable   = 0;
	re2c:yych:conversion = 1;
	re2c:indent:top      = 1;
	
	Nondigit = [_a-zA-Z];
	Digit = [0-9];
	NonZerodigit = [1-9];
	OctalDigit = [0-7];
	HexadecimalDigit = [0-9a-fA-F];
	
	HexQuad = HexadecimalDigit{4};
	UniversalCharacterName = 
		("\\u" HexQuad) |
		("\\U" HexQuad{2});
	
	Identifier = Nondigit (Nondigit|Digit)*;
	
	DecimalConstant = NonZerodigit Digit*;
	
	OctalConstant = "0" OctalDigit*;
	HexadecimalPrefix = "0" [xX];
	HexadecimalConstant = HexadecimalPrefix HexadecimalDigit+;
	
	UnsignedSuffix = [uU];
	LongSuffix = [lL];
	LongLongSuffix = "ll" | "LL";
	IntegerSuffix = 
		(UnsignedSuffix LongSuffix?) |
		(UnsignedSuffix LongLongSuffix) |
		(LongSuffix UnsignedSuffix?) |
		(LongLongSuffix UnsignedSuffix?);
		
	IntegerConstant = (DecimalConstant | OctalConstant | HexadecimalConstant) IntegerSuffix?;
	WhiteSpace = [ \t\v\f\r\n]+;
	
	other = .;
	
	"\000"  { return TokenType::EndOfString; }
	"+"     { return TokenType::Add; }
	"-"     { return TokenType::Sub; }
	"*"     { return TokenType::Mul; }
	"/"     { return TokenType::Div; }
	"%"     { return TokenType::Mod; }
	"++"    { return TokenType::Increment; }
	"--"    { return TokenType::Decrement; }
	"|"     { return TokenType::BitwiseOr; }
	"&"     { return TokenType::BitwiseAnd; }
	"^"     { return TokenType::BitwiseXor; }
	"||"    { return TokenType::LogicalOr; }
	"&&"    { return TokenType::LogicalAnd; }
	"<<"    { return TokenType::LeftShift; }
	">>"    { return TokenType::RightShift; }
	"="     { return TokenType::Assign; }
	"!"     { return TokenType::Negate; }
	"+="    { return TokenType::AddAssign; }
	"-="    { return TokenType::SubAssign; }
	"*="    { return TokenType::MulAssign; }
	"/="    { return TokenType::DivAssign; }
	"%="    { return TokenType::ModAssign; }
	"<<="   { return TokenType::LeftShiftAssign; }
	">>="   { return TokenType::RightShiftAssign; }
	"&="    { return TokenType::AndAssign; }
	"^="    { return TokenType::XorAssign; }
	"|="    { return TokenType::OrAssign; }
	"=="    { return TokenType::Equal; }
	"!="    { return TokenType::NotEqual; }
	"<"     { return TokenType::LessThan; }
	"<="    { return TokenType::LessThanOrEqual; }
	">"     { return TokenType::GreaterThan; }
	">="    { return TokenType::GreaterThanOrEqual; }
	"?"     { return TokenType::Question; }
	","     { return TokenType::Comma; }
	":"     { return TokenType::Colon; }
	";"     { return TokenType::Semicolon; }
	"("     { return TokenType::LeftParentthesis; }
	")"     { return TokenType::RightParenthesis; }
	"["     { return TokenType::LeftSquareBracket; }
	"]"     { return TokenType::RightSquareBracket; }
	"{"     { return TokenType::LeftCurlyBracket; }
	"}"     { return TokenType::RightCurlyBracket; }
	"S1"    { return TokenType::S1; }
	"U1"    { return TokenType::U1; }
	"S2"    { return TokenType::S2; }
	"U2"    { return TokenType::U2; }
	"S4"    { return TokenType::S4; }
	"U4"    { return TokenType::U4; }
	"S8"    { return TokenType::S8; }
	"U8"    { return TokenType::U8; }
	"goto"  { return TokenType::Statement_goto; }
	"if"    { return TokenType::Statement_if; }
	"else"  { return TokenType::Statement_else; }
	"do"    { return TokenType::Statement_do; }
	"while" { return TokenType::Statement_while; }
	"for"   { return TokenType::Statement_for; }
	"continue"      { return TokenType::Statement_continue; }
	"break" { return TokenType::Statement_break; }
	"return"        { return TokenType::Statement_return; }
	"switch"        { return TokenType::Statement_switch; }
	"case"  { return TokenType::Statement_case; }
	"default"       { return TokenType::Statement_default; }
	"struct"        { return TokenType::Statement_struct; }
	"void"  { return TokenType::Statement_void; }
	"typedef"       { return TokenType::Statement_typedef; }
	
	WhiteSpace    { return TokenType::WhiteSpace; }
	IntegerConstant       { return TokenType::IntegerConstant; }
	Identifier    { return TokenType::Identifier; }

	other {
		goto initial;
	}
*/
}


