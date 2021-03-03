#include <iostream>
#include "../BG3Updater/Crypto.h"
#include "../BG3Updater/Manifest.h"
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

    if (strcmp(argv[1], "update-manifest") == 0) {
        auto manifestPath = FromUTF8(argv[2]);
        auto version = std::string(argv[3]);
        auto filePath = FromUTF8(argv[4]);

        std::vector<char> contents;
        {
            std::ifstream f(filePath, std::ifstream::in | std::ifstream::binary);
            if (!f.good()) {
                std::cout << "Failed to open package: " << argv[4] << std::endl;
                return 1;
            }
            f.seekg(0, std::ios::end);
            contents.resize(f.tellg());
            f.seekg(0, std::ios::beg);
            f.read(contents.data(), contents.size());
            f.close();
        }

        uint8_t digest[TC_SHA256_DIGEST_SIZE];
        CryptoUtils::SHA256((uint8_t*)contents.data(), contents.size(), digest);

        std::string manifestStr;
        {
            std::ifstream f(manifestPath, std::ifstream::in | std::ifstream::binary);
            if (!f.good()) {
                std::cout << "Failed to open manifest: " << argv[2] << std::endl;
                return 1;
            }
            f.seekg(0, std::ios::end);
            manifestStr.resize(f.tellg());
            f.seekg(0, std::ios::beg);
            f.read(manifestStr.data(), manifestStr.size());
            f.close();
        }

        ManifestParser parser;
        Manifest manifest;
        std::string parseError;
        if (!parser.Parse(manifestStr, manifest, parseError)) {
            std::cout << "Unable to parse manifest: " << parseError << std::endl;
            return 1;
        }

        auto it = manifest.Versions.find(version);
        if (it == manifest.Versions.end()) {
            std::cout << "Version not found in manifest file: " << version << std::endl;
            return 1;
        }

        static char const* hex = "0123456789abcdef";

        std::string digestStr;
        for (auto i = 0; i < std::size(digest); i++) {
            digestStr += hex[digest[i] >> 4];
            digestStr += hex[digest[i] & 0x0f];
        }

        it->second.Digest = digestStr;

        manifestStr = parser.Write(manifest);

        {
            std::ofstream f(manifestPath, std::ifstream::out | std::ifstream::binary);
            if (!f.good()) {
                std::cout << "Failed to open manifest for writing: " << argv[2] << std::endl;
                return 1;
            }
            f.write(manifestStr.data(), manifestStr.size());
            f.close();
        }

        return 0;
    }

    std::cout << "Unknown command" << std::endl;
    return 2;
}
