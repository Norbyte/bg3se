#pragma once

#include <GameDefinitions/GuidResources.h>

BEGIN_NS(lua::res)

using namespace bg3se::resource;

class GuidResourceBankHelperBase
{
public:
    virtual bool Push(lua_State* L, Guid resourceGuid) = 0;
    virtual bool Create(lua_State* L, std::optional<Guid> resourceGuid, bool allowUnsafeExpansion) = 0;
    virtual Array<Guid> GetAll() = 0;
    virtual HashMap<Guid, Array<Guid>>* GetSources() = 0;
    virtual Array<Guid>* GetByModId(Guid modGuid) = 0;
    virtual GuidResourceBankBase* GetBank() const = 0;
};

class NullGuidResourceBankHelper : public GuidResourceBankHelperBase
{
public:
    bool Push(lua_State* L, Guid resourceGuid) override;
    bool Create(lua_State* L, std::optional<Guid> resourceGuid, bool allowUnsafeExpansion) override;
    Array<Guid> GetAll() override;
    HashMap<Guid, Array<Guid>>* GetSources() override;
    Array<Guid>* GetByModId(Guid modGuid) override;
    GuidResourceBankBase* GetBank() const override;

private:
    HashMap<Guid, Array<Guid>> dummySources_;
    Array<Guid> dummyResources_;
};

class GuidResourceHelpers
{
public:
    inline GuidResourceHelpers(State& state)
        : state_(state)
    {}

    template <class T>
    GuidResourceBankHelperBase* MakeHelper(ExtResourceManagerType type);
    GuidResourceBankHelperBase* Get(ExtResourceManagerType type);

private:
    State& state_;
    std::array<std::unique_ptr<GuidResourceBankHelperBase>, (unsigned)ExtResourceManagerType::Max + 1> helpers_;
    NullGuidResourceBankHelper nullHelper_;
};

END_NS()
