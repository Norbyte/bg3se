#include <iostream>
#include <fstream>
#include <filesystem>

class LuaBundler
{
public:
	void AddResources(std::string const& path)
	{
		auto root = std::filesystem::canonical(path);

		for (auto const& path : std::filesystem::recursive_directory_iterator(root)) {
			if (path.is_regular_file()) {
				ResourceInfo res;
				res.FilesystemPath = path.path();
				res.BundlePath = path.path().string().substr(root.wstring().length() + 1);
				std::replace_if(res.BundlePath.begin(), res.BundlePath.end(), [](char c) { return c == '\\'; }, '/');
				paths_.push_back(res);
			}
		}
	}

	std::vector<uint8_t> Pack()
	{
		std::vector<uint8_t> bundle;

		for (auto const& res : paths_) {
			std::ifstream f(res.FilesystemPath.c_str(), std::ios::in | std::ios::binary);
			if (!f.good()) {
				std::cout << "Couldn't read file: " << res.BundlePath << std::endl;
				exit(1);
			}

			std::size_t len;
			f.seekg(0, std::ifstream::end);
			len = f.tellg();
			f.seekg(0, std::ifstream::beg);
			std::vector<uint8_t> fbuf;
			fbuf.resize(len);
			f.read((char*)fbuf.data(), len);

			auto offset = bundle.size();
			bundle.resize(offset + sizeof(HeaderInfo) + fbuf.size() + res.BundlePath.size());
			auto phdr = bundle.data() + offset;

			HeaderInfo hdr;
			hdr.FileNameSize = (uint32_t)res.BundlePath.size();
			hdr.FileSize = (uint32_t)fbuf.size();
			memcpy(phdr, &hdr, sizeof(hdr));
			memcpy(phdr + sizeof(hdr), res.BundlePath.c_str(), res.BundlePath.size());
			memcpy(phdr + sizeof(hdr) + res.BundlePath.size(), fbuf.data(), fbuf.size());
		}

		return bundle;
	}

private:
	struct ResourceInfo
	{
		std::wstring FilesystemPath;
		std::string BundlePath;
	};

	struct HeaderInfo
	{
		uint32_t FileNameSize;
		uint32_t FileSize;
	};

	std::vector<ResourceInfo> paths_;
};

int main(int argc, char const ** argv)
{
	LuaBundler bundler;
	bundler.AddResources(argv[1]);
	auto pack = bundler.Pack();

	std::ofstream f(argv[2], std::ios::out | std::ios::binary);
	if (!f.good()) {
		std::cout << "Couldn't open bundle file: " << argv[2] << std::endl;
		return 1;
	}

	f.write((char *)pack.data(), pack.size());
	f.close();
	return 0;
}
