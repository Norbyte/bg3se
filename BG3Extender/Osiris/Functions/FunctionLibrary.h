#pragma once

namespace bg3se
{
    class OsirisExtender;
}

namespace bg3se::esv
{
    class CustomFunctionLibrary
    {
    public:
        CustomFunctionLibrary(OsirisExtender& osiris);

        void Register();
        void RegisterLuaFunctions();

    private:
        OsirisExtender& osiris_;
    };

}