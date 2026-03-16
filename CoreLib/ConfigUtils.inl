BEGIN_SE()

using namespace rapidjson;

void ConfigGet(Value& node, char const* key, bool& value)
{
    auto configVar = node.FindMember(key);
    if (configVar != node.MemberEnd() && configVar->value.IsBool()) {
        value = configVar->value.GetBool();
    }
}

void ConfigGet(Value& node, char const* key, uint32_t& value)
{
    auto configVar = node.FindMember(key);
    if (configVar != node.MemberEnd() && configVar->value.IsUint()) {
        value = configVar->value.GetUint();
    }
}

void ConfigGet(Value& node, char const* key, std::string& value)
{
    auto configVar = node.FindMember(key);
    if (configVar != node.MemberEnd() && configVar->value.IsString()) {
        value = configVar->value.GetString();
    }
}

void ConfigGet(Value& node, char const* key, std::wstring& value)
{
    auto configVar = node.FindMember(key);
    if (configVar != node.MemberEnd() && configVar->value.IsString()) {
        value = FromStdUTF8(configVar->value.GetString());
    }
}

bool ReadConfigFile(std::wstring const& configPath, std::string& configJson)
{
    std::ifstream f(configPath, std::ios::in | std::ios::binary);
    if (f.good()) {
        f.seekg(0, std::ios::end);
        configJson.resize((unsigned)f.tellg());
        f.seekg(0, std::ios::beg);
        f.read(configJson.data(), configJson.size());
        return true;
    }

    return false;
}

template <class TConfig>
void ApplyConfigFile(std::wstring const& configPath, TConfig& config, void (*applyFunc)(TConfig& config, Document& root))
{
    std::string configJson;
    if (!ReadConfigFile(configPath, configJson)) {
        return;
    }

    Document root;
    if (root.ParseInsitu(configJson.data()).HasParseError()) {
        std::wstringstream err;
        err << L"Failed to load configuration file '" << configPath << "'";
        Fail(ToStdUTF8(err.str()).c_str());
    }

    applyFunc(config, root);
}

END_SE()
