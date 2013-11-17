#include "token.h"

#include <string.h>

const char* ExtractString(const Token* token, char* buff)
{
	size_t len = slen(token);
	memcpy(buff, token->str, len);
	buff[len] = 0;
	return buff;
}

