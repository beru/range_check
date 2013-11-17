#pragma once

#define D(VAR) VAR,
enum class TokenType {
#include "tokens_dump.txt"
};
#undef D

#define D(VAR) #VAR,
static const char* TokenTypeNames[] = {
#include "tokens_dump.txt"
};
#undef D

inline const char* ToString(TokenType t) { return TokenTypeNames[(int)t]; }

struct Token
{
	TokenType type;
	const char* str;
};

inline
size_t slen(const Token* token)
{
	return (token+1)->str - token->str;
}

const char* ExtractString(const Token* token, char* buff);

