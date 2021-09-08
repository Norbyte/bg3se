#include "stdafx.h"

#include "Crypto.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

static uint8_t UpdaterPublicKey[2 * NUM_ECC_BYTES] = { 
	0x50, 0x54, 0x01, 0xc8, 0xba, 0xc2, 0xbd, 0xd9, 0xcb, 0xbd, 0x34, 0xd7, 0x7d, 0xe1, 0xdb, 0x53, 
	0x09, 0x9d, 0x32, 0xd5, 0x7f, 0x68, 0xe7, 0xef, 0x5c, 0xc0, 0x80, 0xa7, 0x03, 0xd3, 0x81, 0x75, 
	0x87, 0xa5, 0xd2, 0xe7, 0xf2, 0x05, 0x7a, 0x4d, 0xae, 0x74, 0x56, 0xf8, 0x7a, 0x71, 0x7c, 0x6b,
	0x04, 0x21, 0x6b, 0xc4, 0x43, 0x48, 0xc8, 0xac, 0x25, 0x1d, 0x0a, 0xaf, 0x59, 0xca, 0x0b, 0x07
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

bool CryptoUtils::SignFile(std::wstring const& zipPath, std::wstring const& privateKeyPath)
{
	std::vector<uint8_t> contents;
	std::vector<uint8_t> key;
	if (!LoadFile(zipPath, contents)) return false;
	if (!LoadFile(privateKeyPath, key)) return false;

	if (key.size() != NUM_ECC_BYTES) return false;

	PackageSignature sig;
	memset(&sig, 0, sizeof(sig));
	sig.Magic = PackageSignature::MAGIC_V1;

	if (!EccSign(contents.data(), contents.size(), key.data(), sig.EccSignature)) {
		return false;
	}

	auto contentLength = contents.size();
	contents.resize(contentLength + sizeof(sig));
	memcpy(contents.data() + contentLength, &sig, sizeof(sig));

	return SaveFile(zipPath, contents);
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

bool CryptoUtils::GetFileSignature(std::wstring const& path, PackageSignature& signature)
{
	std::vector<uint8_t> contents;
	if (!LoadFile(path, contents)) return false;

	if (contents.size() < sizeof(PackageSignature)) return false;

	auto sig = reinterpret_cast<PackageSignature*>(contents.data() + contents.size() - sizeof(PackageSignature));
	if (sig->Magic != PackageSignature::MAGIC_V1) return false;

	signature = *sig;
	return true;
}

bool CryptoUtils::VerifySignedFile(std::wstring const& zipPath, std::string& reason)
{
	std::vector<uint8_t> contents;
	if (!LoadFile(zipPath, contents)) {
		reason = "Script Extender update failed:\r\nUnable to open update package";
		return false;
	}

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
