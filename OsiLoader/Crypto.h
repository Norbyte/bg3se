#pragma once
#include "stdafx.h"

#include <tinycrypt/constants.h>
#include <tinycrypt/ecc.h>
#include <tinycrypt/ecc_dh.h>
#include <tinycrypt/ecc_dsa.h>
#include <tinycrypt/sha256.h>

#include <vector>
#include <iostream>
#include <fstream>

class CryptoUtils
{
public:
#pragma pack(push, 1)
	struct PackageSignature
	{
		static constexpr uint32_t MAGIC_V1 = 'NSE1';

		uint32_t Magic;
		uint8_t EccSignature[NUM_ECC_BYTES];
		uint8_t Unused[128];
	};
#pragma pack(pop)


	static bool SHA256(uint8_t* data, size_t len, uint8_t* digest);
	static bool EccVerify(uint8_t* data, size_t len, uint8_t* publicKey, uint8_t* signature);
	static bool EccSign(uint8_t* data, size_t len, uint8_t* privateKey, uint8_t* signature);
	static bool ReadFile(std::wstring const& path, std::vector<char>& contents);
	static bool SignFile(std::wstring const& zipPath, std::wstring const& privateKeyPath);
	static bool GenerateKeys(std::wstring const& privateKeyPath);
	static bool VerifySignedFile(std::wstring const& zipPath, std::string& reason);
};

