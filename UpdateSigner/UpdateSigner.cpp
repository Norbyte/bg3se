#include <iostream>
#include "../BG3Updater/Crypto.h"
#include <wincrypt.h>

std::string ToUTF8(std::wstring const& s);
std::wstring FromUTF8(std::string const& s);

HCRYPTPROV hCryptProv;

extern "C" {

int default_CSPRNG(uint8_t* dest, unsigned int size)
{
	return CryptGenRandom(hCryptProv, size, dest);
}

}

int main(int argc, char** argv)
{
    if (argc < 2) return 0;

	if (!CryptAcquireContext(
		&hCryptProv,
		NULL,
		(LPCWSTR)L"Microsoft Base Cryptographic Provider v1.0",
		PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT)) {
        std::cout << "CryptAcquireContext failed" << std::endl;
		return 1;
	}

    if (strcmp(argv[1], "genkey") == 0) {
        auto path = FromUTF8(argv[2]);
        if (CryptoUtils::GenerateKeys(path)) {
            std::cout << "Key written to " << argv[2] << std::endl;
            return 0;
        } else {
            std::cout << "Key generation failed!" << std::endl;
            return 1;
        }
    }

    if (strcmp(argv[1], "sign") == 0) {
        auto keyPath = FromUTF8(argv[2]);
        auto filePath = FromUTF8(argv[3]);
        if (CryptoUtils::SignFile(filePath, keyPath)) {
            std::cout << "Signed file " << argv[3] << " using key " << argv[2] << std::endl;
            return 0;
        } else {
            std::cout << "Signing failed!" << std::endl;
            return 1;
        }
    }

    if (strcmp(argv[1], "verify") == 0) {
        auto filePath = FromUTF8(argv[2]);
        std::string reason;
        if (CryptoUtils::VerifySignedFile(filePath, reason)) {
            std::cout << "Successfully verified file " << argv[2] << std::endl;
            return 0;
        } else {
            std::cout << "Verification failed: " << reason << std::endl;
            return 1;
        }
    }

    std::cout << "Unknown command" << std::endl;
    return 2;
}
