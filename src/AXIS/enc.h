#pragma once

inline std::vector<uint8_t> to_bytes(const std::string& s) {
	return std::vector<uint8_t>(s.begin(), s.end());
}

inline std::string bytes_to_hex(const std::vector<uint8_t>& b) {
	std::ostringstream oss;
	for (auto x : b) oss << std::hex << std::setw(2) << std::setfill('0') << (int)x;
	return oss.str();
}

inline std::vector<uint8_t> hex_to_bytes(const std::string& hex) {
	std::vector<uint8_t> out;
	out.reserve(hex.size() / 2);
	for (size_t i = 0; i < hex.size(); i += 2) {
		std::string byteStr = hex.substr(i, 2);
		uint8_t byte = (uint8_t)std::stoi(byteStr, nullptr, 16);
		out.push_back(byte);
	}
	return out;
}

// PKCS#7 padding to multiple of 16
inline void pkcs7_pad(std::vector<uint8_t>& data) {
	const size_t block = 16;
	size_t pad = block - (data.size() % block);
	if (pad == 0) pad = block;
	for (size_t i = 0; i < pad; ++i) data.push_back((uint8_t)pad);
}

inline bool pkcs7_unpad(std::vector<uint8_t>& data) {
	if (data.empty()) return false;
	uint8_t last = data.back();
	if (last == 0 || last > 16) return false;
	size_t pad = (size_t)last;
	if (pad > data.size()) return false;
	// check padding bytes
	for (size_t i = 0; i < pad; ++i) {
		if (data[data.size() - 1 - i] != last) return false;
	}
	data.resize(data.size() - pad);
	return true;
}

// derive 16-byte key from keyword (repeat & simple fold)
inline std::vector<uint8_t> derive_key_from_keyword(const std::string& kw) {
	std::vector<uint8_t> key(16, 0);
	if (kw.empty()) return key;
	for (size_t i = 0; i < 16; ++i) {
		key[i] = (uint8_t)kw[i % kw.size()] + (uint8_t)i; // small mixing
	}
	return key;
}

inline std::string ibks_encrypt(const std::string& plain, const std::string& keyword) {
	auto data = to_bytes(plain);
	pkcs7_pad(data);
	auto key = derive_key_from_keyword(keyword);

	// XOR each byte with key (key repeated), block-wise same effect
	for (size_t i = 0; i < data.size(); ++i) {
		data[i] ^= key[i % key.size()];
	}

	return bytes_to_hex(data);
}

inline std::string ibks_decrypt(const std::string& hex_cipher, const std::string& keyword) {
	auto data = hex_to_bytes(hex_cipher);
	auto key = derive_key_from_keyword(keyword);

	// XOR again to recover
	for (size_t i = 0; i < data.size(); ++i) {
		data[i] ^= key[i % key.size()];
	}

	// remove padding
	if (!pkcs7_unpad(data)) {
		return ""; // padding error -> 복호화 실패
	}
	return std::string(data.begin(), data.end());
}
