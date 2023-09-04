#pragma once

#include <tinycrypt/constants.h>
#include <tinycrypt/ecc.h>
#include <tinycrypt/ecc_dh.h>
#include <tinycrypt/ecc_dsa.h>
#include <tinycrypt/sha256.h>

#include <iostream>
#include <fstream>

BEGIN_SE()

#pragma pack(push, 1)
struct PackageSignatureBase
{
	static constexpr uint32_t MAGIC_V1 = 'BGS1';
	static constexpr uint8_t VER_INITIAL = 0;
	static constexpr uint8_t VER_SIGNATURE_IN_MANIFEST = 1;

	uint32_t Magic;
	uint8_t EccSignature[NUM_ECC_BYTES*2];
	uint8_t Version;
	uint32_t SignedBytes;
};

struct PackageSignature : public PackageSignatureBase
{
	uint8_t Unused[187];
};
#pragma pack(pop)

static_assert(sizeof(PackageSignature) == 260, "Signature footer must have fixed size");


class CryptoUtils
{
public:
	static bool SHA256(uint8_t* data, size_t len, uint8_t* digest);
	static bool EccVerify(uint8_t* data, size_t len, uint8_t* publicKey, uint8_t* signature);
	static bool EccSign(uint8_t* data, size_t len, uint8_t* privateKey, uint8_t* signature);
	static bool GetFileSignature(std::wstring const& path, PackageSignature& signature);
	static bool SignFile(std::wstring const& zipPath, std::wstring const& privateKeyPath);
	static bool GenerateKeys(std::wstring const& privateKeyPath);
	static bool VerifySignedFile(std::wstring const& zipPath, std::string& reason);
};

END_SE()
