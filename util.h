#pragma once

template <typename T>
bool in(T v, T c0) {
	return v == c0;
}
template <typename T, typename... Types>
bool in(T v, T c0, Types... types) {
	return v == c0 || in(v, types...);
}

