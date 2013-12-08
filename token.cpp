#include "token.h"

#include <string.h>

char* ExtractString(const Token* token, char* buff)
{
	memcpy(buff, token->str, token->len);
	buff[token->len] = 0;
	return buff;
}

