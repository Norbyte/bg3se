#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <string_view>
#include <dia2.h>
#include <dbghelp.h>
#include <atlbase.h>
#include <filesystem>

struct Symbol
{
    uint64_t rva;
    uint64_t size{ 0 };
    std::string name;
};

void Fatal(char const* s)
{
    std::cout << s << std::endl;
    exit(2);
}

class PDBSymbolLoader
{
public:
    bool Initialize(std::string const& path)
    {
        auto hr = CoCreateInstance(CLSID_DiaSource,
            NULL,
            CLSCTX_INPROC_SERVER,
            __uuidof(IDiaDataSource),
            (void**)&source_);

        if (FAILED(hr)) {
            std::cout << "Could not CoCreate CLSID_DiaSource. Register the COM DLL msdia140.dll." << std::endl;
            std::cout << "(Try 'regsvr32 \"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\DIA SDK\\bin\\amd64\\msdia140.dll\"')" << std::endl;
            return false;
        }

        wchar_t wszFilename[_MAX_PATH];
        size_t len;
        mbstowcs_s(&len, wszFilename, path.c_str(), sizeof(wszFilename) / sizeof(wszFilename[0]));
        if (FAILED(source_->loadDataFromPdb(wszFilename))) {
            std::cout << "loadDataFromPdb failed" << std::endl;
            return false;
        }

        if (FAILED(source_->openSession(&session_))) {
            std::cout << "openSession failed" << std::endl;
            return false;
        }

        CComPtr<IDiaEnumTables> pTables;
        if (session_->getEnumTables(&pTables) != S_OK) {
            return false;
        }

        VARIANT var;
        var.vt = VT_BSTR;
        var.bstrVal = SysAllocString(DiaTable_Sections);
        IDiaTable* pTable;
        if (pTables->Item(var, &pTable) != S_OK) {
            std::cout << "Couldn't get section list" << std::endl;
            return false;
        }

        CComPtr<IDiaEnumSectionContribs> pEnumSections;
        if (pTable->QueryInterface(__uuidof(IDiaEnumSectionContribs), (void**)&pEnumSections) != S_OK)
        {
            std::cout << "Couldn't get IDiaEnumSegments" << std::endl;
            return false;
        }

        ULONG celt;
        CComPtr<IDiaSectionContrib> pSection;
        while (SUCCEEDED(pEnumSections->Next(1, &pSection, &celt)) && celt == 1) {
            DWORD segmentNo;
            pSection->get_addressSection(&segmentNo);
            ULONGLONG va;
            pSection->get_virtualAddress(&va);
            DWORD rva;
            pSection->get_relativeVirtualAddress(&rva);
            segmentRva_.insert(std::make_pair(segmentNo, rva));
            pSection = NULL;
        }

        return true;
    }

    void CollectSymbols()
    {
        CComPtr<IDiaSymbol> pglobal;
        if (FAILED(session_->get_globalScope(&pglobal)))
        {
            std::cout << "get_globalScope failed" << std::endl;
            return;
        }

        CollectSymbols(pglobal);

        std::sort(symbols_.begin(), symbols_.end(), [](Symbol const& a, Symbol const& b) {
            return a.rva < b.rva;
        });

        uint64_t lastVa{ 0 };
        for (auto const& sym : symbols_) {
            if (sym.rva > lastVa) {
                lastVa = sym.rva;
                filteredSymbols_.push_back(sym);
            }
        }
    }

    static void TrimOccurrences(std::string& s, std::string_view f)
    {
        for (;;) {
            auto pos = s.find(f);
            if (pos == std::string::npos) break;

            memmove(s.data() + pos, s.data() + pos + f.size(), s.size() - pos - f.size());
            s.resize(s.size() - f.size());
        }
    }

    static void TrimName(std::string& s)
    {
        // If symbol name contains parameters, remove them
        if (*s.rbegin() == ')') {
            auto brStart = s.rfind('(');
            if (brStart != std::string::npos) {
                s.resize(brStart);
            }
        }

        TrimOccurrences(s, "static ");
        TrimOccurrences(s, "const ");
        TrimOccurrences(s, "void ");
        TrimOccurrences(s, "int ");
        TrimOccurrences(s, "enum ");
        TrimOccurrences(s, "dse::");
        TrimOccurrences(s, "__cdecl");
        TrimOccurrences(s, ",GameMemoryAllocator,1");
        TrimOccurrences(s, ",GameMemoryAllocator,0");

        if (s.size() > 120) {
            s = s.substr(0, 120) + "...";
        }
    }

    void EmitSymbol(IDiaSymbol* symbol)
    {
        DWORD offset;
        symbol->get_addressOffset(&offset);
        if (offset == 0) return;

        ULONGLONG staticSize;
        symbol->get_length(&staticSize);
        if (staticSize <= 0x30) return;

        DWORD tag;
        symbol->get_symTag(&tag);
        DWORD section;
        symbol->get_addressSection(&section);

        if (tag == SymTagPublicSymbol) {
            if (section != 1) return;
        }

        BSTR oleName = nullptr;
        DWORD undFlags = UNDNAME_NO_LEADING_UNDERSCORES
            | UNDNAME_NO_MS_KEYWORDS
            | UNDNAME_NO_FUNCTION_RETURNS
            | UNDNAME_NO_ALLOCATION_MODEL
            | UNDNAME_NO_ALLOCATION_LANGUAGE
            | UNDNAME_NO_THISTYPE
            | UNDNAME_NO_ACCESS_SPECIFIERS
            | UNDNAME_NO_THROW_SIGNATURES
            | UNDNAME_NO_MEMBER_TYPE
            | UNDNAME_NO_RETURN_UDT_MODEL
            | UNDNAME_NAME_ONLY
            | UNDNAME_NO_ARGUMENTS
            | UNDNAME_NO_SPECIAL_SYMS;
        if (symbol->get_undecoratedNameEx(UNDNAME_NAME_ONLY, &oleName) != S_OK) return;

        char name[0x200];
        size_t nameLength;
        wcstombs_s(&nameLength, name, sizeof(name) - 1, oleName, sizeof(name) - 2);
        name[nameLength] = 0;

        if (strncmp(name, "sub_", 4) == 0) return;

        if (tag == SymTagPublicSymbol) {
            if (strncmp(name, "ecl:", 4) != 0
                && strncmp(name, "esv:", 4) != 0
                && strncmp(name, "eoc:", 4) != 0
                && strncmp(name, "net:", 4) != 0
                && strncmp(name, "ls::", 4) != 0) return;
        }

        Symbol sym;
        sym.rva = offset + segmentRva_[section];
        sym.size = staticSize;
        sym.name = name;

        // Skip _atexit destructors
        if (sym.name.find("atexit") != std::string::npos) return;

        // Skip closure wrappers
        if (sym.name.find("Closure_wrapper") != std::string::npos) return;
        if (sym.name.find("InitObjectProxyPropertyMaps") != std::string::npos) return;
        if (sym.name.find("RegisterObjectProxyTypeInformation") != std::string::npos) return;

        TrimName(sym.name);

        symbols_.push_back(std::move(sym));
        SysFreeString(oleName);
    }

    void CollectSymbols(IDiaSymbol* parent)
    {
        CComPtr<IDiaEnumSymbols> pSymbols;
        if (session_->findChildren(parent, SymTagNull, NULL, nsfCaseInsensitive | nsfUndecoratedName, &pSymbols) != S_OK) {
            return;
        }

        CComPtr<IDiaSymbol> pSymbol;
        ULONG celt;
        while (SUCCEEDED(pSymbols->Next(1, &pSymbol, &celt)) && celt == 1) {
            DWORD tag;
            pSymbol->get_symTag(&tag);

            switch (tag) {
            case SymTagFunction:
                hasFunctions_ = true;
                EmitSymbol(pSymbol);
                break;

            case SymTagPublicSymbol:
                if (!hasFunctions_) {
                    EmitSymbol(pSymbol);
                }
                break;
            }

            switch (tag) {
            case SymTagExe:
            case SymTagCompiland:
                CollectSymbols(pSymbol);
                break;
            }

            pSymbol = NULL;
        }
    }

    bool ExportSymbolTable(std::string const& path)
    {
        size_t bufSize{ 4 };
        size_t stringTableSize{ 4 };
        for (auto const& sym : filteredSymbols_) {
            bufSize += 12 + 1 + sym.name.size();
            stringTableSize += 12;
        }

        std::vector<uint8_t> buf;
        buf.resize(bufSize);

        uint8_t* p = buf.data();
        uint8_t* pStr = buf.data() + stringTableSize;
        size_t strOffset{ 0 };

        *(uint32_t*)p = (uint32_t)filteredSymbols_.size();
        p += 4;
        for (auto const& sym : filteredSymbols_) {
            *(uint32_t*)p = (uint32_t)sym.rva;
            p += 4;
            *(uint32_t*)p = (uint32_t)sym.size;
            p += 4;
            *(uint32_t*)p = (uint32_t)strOffset;
            p += 4;
            memcpy(pStr, sym.name.c_str(), sym.name.size() + 1);
            pStr += sym.name.size() + 1;
            strOffset += sym.name.size() + 1;
        }

        std::ofstream f(path.c_str(), std::ios::out | std::ios::binary);
        if (!f.good()) return false;
        f.write((char*)buf.data(), buf.size());
        return true;
    }

private:
    CComPtr<IDiaDataSource> source_;
    CComPtr<IDiaSession> session_;
    bool hasFunctions_{ false };
    std::vector<Symbol> symbols_;
    std::vector<Symbol> filteredSymbols_;
    std::map<uint32_t, uint32_t> segmentRva_;
};

int main(int argc, char** argv)
{
    if (argc <= 2) {
        std::cout << "Usage: SymbolTableGenerator <PDB path> <Symtab path>" << std::endl;
        return 1;
    }
    
    if (CoInitialize(NULL) != S_OK) {
        Fatal("CoInitialize() failed");
    }

    PDBSymbolLoader loader;
    if (std::filesystem::exists(argv[1])) {
        if (!loader.Initialize(argv[1])) {
            Fatal("Failed to load PDB file");
        }

        loader.CollectSymbols();
        if (!loader.ExportSymbolTable(argv[2])) {
            Fatal("Failed to write symtab file");
        }
    } else {
        std::cout << "PDB file '" << argv[1] << "' does not exist. Will write a dummy symtab file." << std::endl;
        std::ofstream f(argv[2], std::ios::out | std::ios::binary);
    }

    return 0;
}
