#include "stdafx.h"

#include "Crypto.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

static uint8_t UpdaterPublicKey[2 * NUM_ECC_BYTES] = { 
	0x6e, 0x6c, 0x8f, 0x54, 0x32, 0x16, 0x53, 0x65, 0xf1, 0x07, 0x56, 0x3d, 0xd8, 0xbd, 0x41, 0xdc, 
	0xb5, 0xe6, 0x2b, 0xf3, 0x39, 0xe4, 0x30, 0xfd, 0x9b, 0xb7, 0x0d, 0x6c, 0xbf, 0xd4, 0x3a, 0xc1, 
	0x97, 0x44, 0x40, 0xea, 0x12, 0xc8, 0x6d, 0x8d, 0xe0, 0x9c, 0x21, 0x9a, 0x09, 0xdb, 0xfc, 0x2d, 
	0x31, 0xd0, 0x5d, 0x67, 0xcd, 0xad, 0x2d, 0x99, 0x5b, 0x7b, 0x2d, 0x27, 0xf5, 0xc7, 0x8b, 0x2c 
};

bool CryptoUtils::SHA256(uint8_t* data, size_t len, uint8_t* digest)
{
	tc_sha256_state_struct sha;
	if (tc_sha256_init(&sha) != TC_CRYPTO_SUCCESS) {
		return false;
	}

	if (tc_sha256_update(&sha, data, len) != TC_CRYPTO_SUCCESS) {
		return false;
	}

	return tc_sha256_final(digest, &sha) == TC_CRYPTO_SUCCESS;
}


bool CryptoUtils::EccVerify(uint8_t* data, size_t len, uint8_t* publicKey, uint8_t* signature)
{
	uint8_t digest[TC_SHA256_DIGEST_SIZE];
	if (!SHA256(data, len, digest)) {
		return false;
	}

	return uECC_verify(publicKey, digest, sizeof(digest), signature, uECC_secp256r1()) == TC_CRYPTO_SUCCESS;
}


bool CryptoUtils::EccSign(uint8_t* data, size_t len, uint8_t* privateKey, uint8_t* signature)
{
	uint8_t digest[TC_SHA256_DIGEST_SIZE];
	if (!SHA256(data, len, digest)) {
		return false;
	}

	return uECC_sign(privateKey, digest, sizeof(digest), signature, uECC_secp256r1()) == TC_CRYPTO_SUCCESS;
}

bool CryptoUtils::ReadFile(std::wstring const& path, std::vector<char>& contents)
{
	std::ifstream f(path.c_str(), std::ios::in | std::ios::binary);
	if (!f.good()) {
		return false;
	}

	f.seekg(0, std::ios::end);
	auto len = f.tellg();
	contents.resize(len);

	f.seekg(0, std::ios::beg);
	f.read(contents.data(), len);
	f.close();
	return true;
}

bool CryptoUtils::SignFile(std::wstring const& zipPath, std::wstring const& privateKeyPath)
{
	std::vector<char> contents;
	std::vector<char> key;
	if (!ReadFile(zipPath, contents)) return false;
	if (!ReadFile(privateKeyPath, key)) return false;

	if (key.size() != NUM_ECC_BYTES) return false;

	PackageSignature sig;
	memset(&sig, 0, sizeof(sig));
	sig.Magic = PackageSignature::MAGIC_V1;

	if (!EccSign(reinterpret_cast<uint8_t*>(contents.data()), contents.size(),
		reinterpret_cast<uint8_t*>(key.data()), sig.EccSignature)) {
		return false;
	}

	auto contentLength = contents.size();
	contents.resize(contentLength + sizeof(sig));
	memcpy(contents.data() + contentLength, &sig, sizeof(sig));

	std::ofstream of(zipPath.c_str(), std::ios::out | std::ios::binary);
	if (!of.good()) {
		return false;
	}

	of.write(contents.data(), contents.size());
	return true;
}

bool CryptoUtils::GenerateKeys(std::wstring const& privateKeyPath)
{
	uint8_t privateKey[NUM_ECC_BYTES];
	uint8_t publicKey[2*NUM_ECC_BYTES];
	if (!uECC_make_key(publicKey, privateKey, uECC_secp256r1())) {
		return false;
	}

	std::ofstream of(privateKeyPath.c_str(), std::ios::out | std::ios::binary);
	if (!of.good()) {
		return false;
	}

	of.write(reinterpret_cast<char*>(privateKey), sizeof(privateKey));

	std::cout << "Public key:" << std::endl;
	for (auto i = 0; i < sizeof(publicKey); i++) {
		std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2) << (unsigned)publicKey[i] << ", ";
	}

	return true;
}

bool CryptoUtils::VerifySignedFile(std::wstring const& zipPath, std::string& reason)
{
	std::vector<char> contents;
	std::ifstream f(zipPath.c_str(), std::ios::in | std::ios::binary);
	if (!f.good()) {
		reason = "Script Extender update failed:\r\nUnable to open update package";
		return false;
	}

	f.seekg(0, std::ios::end);
	auto len = f.tellg();
	contents.resize(len);

	f.seekg(0, std::ios::beg);
	f.read(contents.data(), len);
	f.close();

	if (contents.size() < sizeof(PackageSignature)) {
		reason = "Script Extender update failed:\r\nUpdate package not cryptographically signed.";
		return false;
	}

	auto sig = reinterpret_cast<PackageSignature*>(contents.data() + contents.size() - sizeof(PackageSignature));
	if (sig->Magic != PackageSignature::MAGIC_V1) {
		reason = "Script Extender update failed:\r\nUpdate package not cryptographically signed.";
		return false;
	}

	if (!EccVerify(reinterpret_cast<uint8_t*>(contents.data()), contents.size() - sizeof(PackageSignature), UpdaterPublicKey, sig->EccSignature)) {
		reason = "Script Extender update failed:\r\nCryptographic signature on update package is incorrect.";
		return false;
	}

	return true;
}
