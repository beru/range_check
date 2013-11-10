#pragma once

template <typename T>
bool in(T v, T c0) {
	return v == c0;
}
template <typename T>
bool in(T v, T c0, T c1) {
	return v == c0 || v == c1;
}
template <typename T>
bool in(T v, T c0, T c1, T c2) {
	return v == c0 || v == c1 || v == c2;
}
template <typename T>
bool in(T v, T c0, T c1, T c2, T c3) {
	return v == c0 || v == c1 || v == c2 || v == c3;
}
