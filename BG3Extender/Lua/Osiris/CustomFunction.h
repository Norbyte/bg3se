#pragma once

#include <Lua/LuaBinding.h>
#include <GameDefinitions/Osiris.h>
#include <Osiris/Shared/CustomFunctions.h>

BEGIN_NS(esv::lua)

using namespace bg3se::lua;

class CustomLuaCall : public CustomCallBase
{
public:
    inline CustomLuaCall(STDString const & name, std::vector<CustomFunctionParam> params,
        RegistryEntry handler)
        : CustomCallBase(name, std::move(params)), handler_(std::move(handler))
    {}

    virtual bool Call(OsiArgumentDesc const & params) override;

private:
    RegistryEntry handler_;
};


class CustomLuaQuery : public CustomQueryBase
{
public:
    inline CustomLuaQuery(STDString const & name, std::vector<CustomFunctionParam> params,
        RegistryEntry handler)
        : CustomQueryBase(name, std::move(params)), handler_(std::move(handler))
    {}

    virtual bool Query(OsiArgumentDesc & params) override;

private:
    RegistryEntry handler_;
};

END_NS()
