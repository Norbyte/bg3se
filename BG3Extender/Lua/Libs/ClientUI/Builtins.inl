#include <GameDefinitions/UI.h>
#include <NsGui/UIElementEvents.h>
#include <NsGui/Uri.h>
#include <NsDrawing/Color.h>
#include <NsMath/Vector.h>
#include <Lua/Libs/ClientUI/ClassCache.h>
#include <Lua/Libs/ClientUI/Symbols.inl>
#include <Lua/Libs/ClientUI/ClassCache.inl>

BEGIN_BARE_NS(Noesis)

namespace Boxing
{
    void* BoxingAllocate(size_t size)
    {
        return bg3se::GameAllocRaw(size);
    }

    void BoxingDeallocate(void* ptr, size_t size)
    {
        bg3se::GameFree(ptr);
    }

    Boxed<bool>* gTrueBoxed{ nullptr };
    Boxed<bool>* gFalseBoxed{ nullptr };

    BoxedValue* TrueBoxed()
    {
        if (gTrueBoxed == nullptr) {
            gTrueBoxed = new Boxed<bool>(true);
        }

        return gTrueBoxed;
    }

    BoxedValue* FalseBoxed()
    {
        if (gFalseBoxed == nullptr) {
            gFalseBoxed = new Boxed<bool>(false);
        }

        return gFalseBoxed;
    }
}

namespace Impl
{
    String ToString(bool value)
    {
        return value ? "True" : "False";
    }
    
    String ToString(int8_t value)
    {
        return std::to_string(value).c_str();
    }
    
    String ToString(uint8_t value)
    {
        return std::to_string(value).c_str();
    }
    
    String ToString(int16_t value)
    {
        return std::to_string(value).c_str();
    }
    
    
    String ToString(uint16_t value)
    {
        return std::to_string(value).c_str();
    }
    
    String ToString(int32_t value)
    {
        return std::to_string(value).c_str();
    }
    
    String ToString(uint32_t value)
    {
        return std::to_string(value).c_str();
    }

    String ToString(int64_t value)
    {
        return std::to_string(value).c_str();
    }

    String ToString(uint64_t value)
    {
        return std::to_string(value).c_str();
    }

    String ToString(float value)
    {
        return std::to_string(value).c_str();
    }

    String ToString(double value)
    {
        return std::to_string(value).c_str();
    }

    String ToString(String const& value)
    {
        return value;
    }
}

String Vector2::ToString() const
{
    char s[100];
    sprintf_s(s, "(%f,%f)", x, y);
    return s;
}

String Vector3::ToString() const
{
    char s[100];
    sprintf_s(s, "(%f,%f,%f)", x, y, z);
    return s;
}

String Point::ToString() const
{
    char s[100];
    sprintf_s(s, "%g,%g", x, y);
    return s;
}

String Color::ToString() const
{
    auto col = ((uint32_t)(r * 255.0f + 0.5f) << 24)
        | ((uint32_t)(g * 255.0f + 0.5f) << 16)
        | ((uint32_t)(b * 255.0f + 0.5f) << 8)
        | ((uint32_t)(a * 255.0f + 0.5f) << 0);

    char s[100];
    sprintf_s(s, "#%08x", col);
    return s;
}

String Rect::ToString() const
{
    char s[100];
    sprintf_s(s, "%g,%g,%g,%g", x, y, width, height);
    return s;
}

void* Cast(const TypeClass* type, BaseObject* obj)
{
    if (obj == nullptr) {
        return nullptr;
    }

    auto cls = obj->GetClassType();
    if (cls == type) {
        return obj;
    }

    const TypeClass::AncestorInfo* ancestor{ nullptr };
    if (cls->IsDescendantOf(type, &ancestor)) {
        return (void*)((uintptr_t)obj + ancestor->offset);
    }

    return nullptr;
}

void* BaseObject::operator new(unsigned __int64 sz)
{
    return GameAllocRaw(sz);
}

void BaseObject::operator delete(void* ptr)
{
    GameFree(ptr);
}

BaseObject::BaseObject() {}
BaseObject::~BaseObject() {}

const TypeClass* BaseObject::GetClassType() const
{
    return gStaticSymbols.TypeClasses.BaseObject.Type;
}

String BaseObject::ToString() const
{
    return GetClassType()->GetName();
}

bool BaseObject::Equals(const BaseObject* left, const BaseObject* right)
{
    return left == right
        || left->Equals(right);
}

bool BaseObject::Equals(const BaseObject* o) const
{
    return this == o;
}

BaseComponent::BaseComponent() {}
BaseComponent::~BaseComponent() {}

const TypeClass* BaseComponent::StaticGetClassType(TypeTag<BaseComponent>*)
{
    return gStaticSymbols.TypeClasses.BaseComponent.Type;
}

const TypeClass* BaseComponent::GetClassType() const
{
    return gStaticSymbols.TypeClasses.BaseComponent.Type;
}

const TypeClass* BaseRefCounted::GetClassType() const
{
    return gStaticSymbols.TypeClasses.BaseRefCounted.Type;
}


Type::Type(Symbol name)
    : mName(name)
{}

const TypeClass* Type::GetClassType() const
{
    return gStaticSymbols.TypeClasses.Type.Type;
}

bool Type::IsAssignableFrom(const Type* type) const
{
    return type == this;
}


TypeMeta::TypeMeta(Symbol name)
    : Type(name)
{}

TypeMeta::~TypeMeta()
{}


TypePtr::TypePtr(Symbol name)
    : TypeMeta(name)
{}

const TypeClass* TypePtr::GetClassType() const
{
    return gStaticSymbols.TypeClasses.TypePtr.Type;
}

void TypePtr::SetStaticContentType(Type const* t)
{
    mContentType = t;
}


struct TypeClassAncestors
{
    TypeClassAncestors()
    {
        InitializeSRWLock(&Lock);
    }

    bool Collapsed{ false };
    Vector<TypeClass::AncestorInfo> Ancestors;
    SRWLOCK Lock;
};

const TypeClass* TypeClass::StaticGetClassType(TypeTag<TypeClass>*)
{
    return gStaticSymbols.TypeClasses.TypeClass.Type;
}

const TypeClass* TypeMeta::GetClassType() const
{
    return gStaticSymbols.TypeClasses.TypeMeta.Type;
}

TypeMetaData* TypeMeta::FindMeta(const TypeClass* metaDataType) const
{
    for (auto const& meta : mMetaData) {
        auto m = meta->GetClassType();
        if (m == metaDataType) {
            return meta;
        }
    }

    return nullptr;
}


TypeProperty::TypeProperty(Symbol name, const Type* type)
    : mName(name),
    mType(type)
{}

TypeProperty::~TypeProperty()
{}

bool TypeProperty::IsReadOnly() const
{
    return true;
}

Ptr<BaseComponent> TypeProperty::GetComponent(const void* ptr) const
{
    throw std::runtime_error("Not implemented");
}

void TypeProperty::SetComponent(void* ptr, BaseComponent* value) const
{
    throw std::runtime_error("Not implemented");
}

void TypeProperty::GetCopy(const void* ptr, void* dest) const
{
    throw std::runtime_error("Not implemented");
}

const void* TypeProperty::Get(const void* ptr) const
{
    throw std::runtime_error("Not implemented");
}

void TypeProperty::Set(void* ptr, const void* value) const
{
    throw std::runtime_error("Not implemented");
}


TypeClass::TypeClass(Symbol name, bool isInterface)
    : TypeMeta(name),
    mIsInterface(isInterface),
    mBase(nullptr),
    mAncestors(nullptr)
{
    mAncestors = (TypeClass::Ancestors*)GameAlloc<TypeClassAncestors>();
}

TypeClass::~TypeClass()
{}

const TypeClass* TypeClass::GetClassType() const
{
    return gStaticSymbols.TypeClasses.TypeClass.Type;
}

bool TypeClass::IsAssignableFrom(const Type* type) const
{
    if (type == this) return true;

    auto cls = DynamicCast<TypeClass const*>(type);
    return cls != nullptr
        && cls->IsDescendantOf(this, nullptr);
}

bool TypeClass::IsDescendantOf(const TypeClass* ancestorType,
    const AncestorInfo** ancestorInfo) const
{
    auto ancestors = (TypeClassAncestors*)mAncestors;
    if (ancestors->Collapsed) {
        for (auto& ancestor : ancestors->Ancestors) {
            if (ancestor.type == ancestorType) {
                if (ancestorInfo) {
                    *ancestorInfo = &ancestor;
                }

                return true;
            }
        }
    }

    return false;
}

void TypeClassBuilder::AddBase(const TypeClass* baseType)
{
    mBase = baseType;
}

void TypeClassBuilder::AddInterface(const TypeClass* interfaceType, uint32_t offset)
{
    mInterfaces.PushBack(AncestorInfo(interfaceType, offset));
}

void TypeClassBuilder::AddProperty(TypeProperty* prop)
{
    auto insertAfter = mProperties.Begin();
    while (insertAfter != mProperties.End() && (*insertAfter)->mName < prop->mName) insertAfter++;

    mProperties.Insert(insertAfter, prop);
}


const TypeClass* BoxedValue::GetClassType() const
{
    return gStaticSymbols.TypeClasses.BoxedValue.Type;
}

const TypeClass* BoxedValue::StaticGetClassType(TypeTag<BoxedValue>*)
{
    return gStaticSymbols.TypeClasses.BoxedValue.Type;
}


NS_CORE_KERNEL_API Type* Reflection::RegisterType(const char* name, CreatorFn creator, FillerFn filler)
{
    return RegisterType(Symbol(name), creator, filler);
}

// Hack to allow re-registering types in developer mode
unsigned gAllowTypeReregistration{ 0 };

NS_CORE_KERNEL_API Type* Reflection::RegisterType(Symbol name, CreatorFn creator, FillerFn filler)
{
    auto tid = GetCurrentThreadId();
    auto lockTid = GetStaticSymbols().Noesis__Reflection__LockedByThreadId;
    auto lock = GetStaticSymbols().Noesis__Reflection__Lock;
    auto numLocks = GetStaticSymbols().Noesis__Reflection__NumLocks;

    if (tid != *lockTid) {
        AcquireSRWLockExclusive(lock);
        *lockTid = tid;
    }

    auto locks = ++*numLocks;

    auto reflection = GetReflection();
    Type* cls{ nullptr };

    auto currentTy = reflection->NameToType.Find((uint32_t)name);
    if (currentTy != reflection->NameToType.End() && gAllowTypeReregistration == 0) {
        WARN("Re-registering type '%s'?", name.Str());
        cls = currentTy->value;
    } else {
        cls = creator(name);
        if (filler) {
            filler(cls);
        }

        reflection->NameToType.Insert((uint32_t)name, cls);
        reflection->Names.PushBack(name);
    }

    --*numLocks;
    if (locks == 1) {
        *lockTid = 0xffffffffu;
        ReleaseSRWLockExclusive(lock);
    }

    return cls;
}

uint32_t SymFindStringInternal(char const* k)
{
    auto const& keys = GetSymbolManager()->Keys;
    auto it = keys.Find(k);
    if (it != keys.End()) {
        return it->value;
    }

    auto it2 = gStaticSymbols.Symbols.find(k);
    if (it2 != gStaticSymbols.Symbols.end()) {
        return it2->second;
    }

    // If the symbol table changed since the last sync, re-sync and try again
    if (keys.Size() != gStaticSymbols.Symbols.size()) {
        auto& strings = Noesis::GetSymbolManager()->Strings;
        for (uint32_t i = 0; i < strings.Size(); i++) {
            gStaticSymbols.Symbols.insert(std::make_pair(strings[i], i));
        }

        auto it3 = gStaticSymbols.Symbols.find(k);
        if (it3 != gStaticSymbols.Symbols.end()) {
            return it3->second;
        }
    }

    return 0;
}

uint32_t SymbolManager::FindString(char const* k)
{
    auto lock = GetStaticSymbols().Noesis__SymbolManager__Lock;
    AcquireSRWLockShared(lock);

    auto idx = SymFindStringInternal(k);

    ReleaseSRWLockShared(lock);

    return idx;
}

uint32_t SymbolManager::AddStaticString(char const* k)
{
    return AddString(k);
}

uint32_t SymbolManager::AddString(char const* k)
{
    auto lock = GetStaticSymbols().Noesis__SymbolManager__Lock;
    AcquireSRWLockExclusive(lock);

    uint32_t idx;
    auto sym = GetSymbolManager();

    auto it = sym->Keys.Find(k);
    if (it != sym->Keys.End()) {
        idx = it->value;
    } else {
        idx = sym->Strings.Size();
        auto keyCopy = _strdup(k);
        sym->Strings.PushBack(keyCopy);
        sym->Keys.Insert(keyCopy, idx);

        gStaticSymbols.Symbols.insert(std::make_pair(keyCopy, idx));
    }

    ReleaseSRWLockExclusive(lock);
    return idx;
}

using AddVisualChildProc = void (Visual*, Visual*);

void Visual::AddVisualChild(Visual* child)
{
    auto addProc = (AddVisualChildProc*)GetStaticSymbols().Noesis__Visual__AddVisualChild;
    if (addProc) addProc(this, child);
}

void Visual::RemoveVisualChild(Visual* child)
{
    auto removeProc = (AddVisualChildProc*)GetStaticSymbols().Noesis__Visual__RemoveVisualChild;
    if (removeProc) removeProc(this, child);
}

END_BARE_NS()
