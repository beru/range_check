#pragma once

#include <stdio.h>
#include <vector>

template <typename T>
bool in(T v, T c0) {
	return v == c0;
}
template <typename T, typename... Types>
bool in(T v, T c0, Types... types) {
	return v == c0 || in(v, types...);
}

static inline
size_t getFileSize(FILE* file)
{
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	return length;
}

static inline
bool readFile(const char* path, std::vector<char>& buff)
{
	FILE* f = fopen(path, "rb");
	if (!f) {
		return false;
	}
	size_t sz = getFileSize(f);
	buff.resize(sz);
	fread(&buff[0], 1, sz, f);
	fclose(f);
	// TODO: to check read failure
	return true;
}

