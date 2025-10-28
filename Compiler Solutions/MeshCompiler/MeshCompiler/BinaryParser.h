#pragma once
#include <string>
#include <bitset>
class BinaryReader {
public:
	template <typename T> std::string EncodeBinary(T value) {
		return std::string(
			reinterpret_cast<const char*>(&value),
			sizeof(T)
		);
	}
	template <typename T> T DecodeBinary(std::string bin) {
		T value{};
		char* bytes = reinterpret_cast<char*>(&value);

		for (size_t i = 0; i < sizeof(T); ++i) {
			std::bitset<8> bits(bin.substr(i * 8, 8));
			bytes[i] = static_cast<char>(bits.to_ulong());
		}

		return value;
	}


};