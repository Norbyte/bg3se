#include <iostream>
#include "../BG3Updater/Crypto.h"
#include "../BG3Updater/Manifest.h"
#include <wincrypt.h>

std::string ToUTF8(std::wstring const& s);
std::wstring FromUTF8(std::string const& s);
bool SaveFile(std::wstring const& path, std::string const& body);
bool LoadFile(std::wstring const& path, std::vector<uint8_t>& body);
bool LoadFile(std::wstring const& path, std::string& body);

HCRYPTPROV hCryptProv;

extern "C" {

int default_CSPRNG(uint8_t* dest, unsigned int size)
{
	return CryptGenRandom(hCryptProv, size, dest);
}

}

int UpdateManifest(int argc, char** argv)
{
    if (argc != 9) {
        std::cout << "Usage: UpdateSigner update-manifest <ManifestPath> <ResourceName> <PackagePath> <DllPath> <MinVersion> <MaxVersion> <RootURL>" << std::endl;
        return 1;
    }

    auto manifestPath = FromUTF8(argv[2]);
    auto resource = std::string(argv[3]);
    auto packagePath = FromUTF8(argv[4]);
    auto dllPath = FromUTF8(argv[5]);
    auto minVersion = std::string(argv[6]);
    auto maxVersion = std::string(argv[7]);
    auto rootUrl = std::string(argv[8]);

    std::string manifestStr;
    if (!LoadFile(manifestPath, manifestStr)) {
        std::cout << "Failed to open manifest: " << ToUTF8(manifestPath) << std::endl;
        return 2;
    }

    ManifestSerializer parser;
    Manifest manifest;
    std::string parseError;
    if (parser.Parse(manifestStr, manifest, parseError) != ManifestParseResult::Successful) {
        std::cout << "Unable to parse manifest: " << parseError << std::endl;
        return 3;
    }

    auto resIt = manifest.Resources.find(resource);
    if (resIt == manifest.Resources.end()) {
        std::cout << "Resource not found in manifest file: " << resource << std::endl;
        return 4;
    }

    Manifest::ResourceVersion version;
    if (!version.UpdatePackageMetadata(packagePath)) {
        std::cout << "Failed to load package file for metadata generation: " << ToUTF8(packagePath) << std::endl;
        return 5;
    }

    if (!version.UpdateDLLMetadata(dllPath)) {
        std::cout << "Failed to load DLL file to check version number: " << ToUTF8(dllPath) << std::endl;
        return 6;
    }

    if (!minVersion.empty() && minVersion != "-") {
        auto minVer = VersionNumber::FromString(minVersion.c_str());
        if (!minVer) {
            std::cout << "Malformed version number: " << minVersion << std::endl;
            return 1;
        }

        version.MinGameVersion = minVer;
    } else {
        version.MinGameVersion = {};
    }

    if (!maxVersion.empty() && maxVersion != "-") {
        auto maxVer = VersionNumber::FromString(maxVersion.c_str());
        if (!maxVer) {
            std::cout << "Malformed version number: " << maxVersion << std::endl;
            return 1;
        }

        version.MaxGameVersion = maxVer;
    } else {
        version.MaxGameVersion = {};
    }

    version.URL = rootUrl + "v" + version.Version.ToString() + "-" + version.Digest + ".package";

    auto curIt = resIt->second.ResourceVersions.find(version.Digest);
    if (curIt != resIt->second.ResourceVersions.end()) {
        if (curIt->second.Revoked) {
            std::cout << "Attempted to publish a revoked digest: " << curIt->second.Digest << std::endl;
            return 8;
        }

        resIt->second.ResourceVersions.erase(curIt);
    }

    resIt->second.ResourceVersions.insert(std::make_pair(version.Digest, version));

    manifestStr = parser.Stringify(manifest);

    if (!SaveFile(manifestPath, manifestStr)) {
        std::cout << "Failed to open manifest for writing: " << ToUTF8(manifestPath) << std::endl;
        return 7;
    }

    std::cout << "Added resource '" << resource << "', version " << version.Version.ToString() << ", digest " << version.Digest << " to manifest" << std::endl;
    return 0;
}

int ComputePathDigest(int argc, char** argv)
{
    if (argc != 4) {
        std::cout << "Usage: UpdateSigner compute-path <PackagePath> <DLLPath>" << std::endl;
        return 1;
    }

    auto packagePath = FromUTF8(argv[2]);
    auto dllPath = FromUTF8(argv[3]);
    auto digest = GetFileDigest(packagePath);
    if (!digest) {
        std::cout << "Failed to load package file: " << ToUTF8(packagePath) << std::endl;
        return 5;
    }

    auto version = GetFileVersion(dllPath);
    if (!version) {
        std::cout << "Failed to load DLL file to check version number: " << ToUTF8(dllPath) << std::endl;
        return 6;
    }
    
    std::cout << "v" + version->ToString() + "-" + *digest + ".package" << std::endl;
    return 0;
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

        PackageSignature sig;
        if (CryptoUtils::GetFileSignature(filePath, sig)) {
            std::cout << "File " << argv[3] << " is already signed!" << std::endl;
            return 2;
        }

        if (CryptoUtils::SignFile(filePath, keyPath)) {
            std::cout << "Signed file " << argv[3] << " using key " << argv[2] << std::endl;
            return 0;
        } else {
            std::cout << "Signing failed!" << std::endl;
            return 3;
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
        return UpdateManifest(argc, argv);
    }

    if (strcmp(argv[1], "compute-path") == 0) {
        return ComputePathDigest(argc, argv);
    }

    std::cout << "Unknown command" << std::endl;
    return 2;
}
