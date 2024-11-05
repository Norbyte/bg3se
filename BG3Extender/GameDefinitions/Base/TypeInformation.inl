#include <Lua/Shared/LuaModule.h>

BEGIN_SE()

TypeInformation const& TypeInformationRef::Get() const
{
	if (ref_ && ref_->Type) {
		return *ref_->Type;
	} else {
		return TypeInformationRepository::GetInstance().GetUndefinedType();
	}
}

StaticTypeInformation::StaticTypeInformation(TypeInformation* type, InitializerProc* initializer)
	: Type(type), Initializer(initializer)
{
	if (initializer != nullptr) {
		TypeInformationRepository::GetInstance().RegisterInitializer(this);
	}
}

void StaticTypeInformation::DeferredInitialize()
{
	if (Type == nullptr && Initializer != nullptr) {
		Type = Initializer();
		assert(Type != nullptr);
	}

	if (Type) {
		Type->DeferredInitialize();
	}
}

void TypeInformation::DeferredInitialize()
{
	if (DeferredInitializationDone) return;
	DeferredInitializationDone = true;

	switch (Kind) {
	case LuaTypeId::Object:
		for (auto const& member : Members) {
			member.Value.GetStatic()->DeferredInitialize();
		}

		for (auto const& method : Methods) {
			for (auto const& param : method.Value.Params) {
				param.GetStatic()->DeferredInitialize();
			}

			for (auto const& retval : method.Value.ReturnValues) {
				retval.GetStatic()->DeferredInitialize();
			}
		}
		break;

	case LuaTypeId::Array:
		ElementType.GetStatic()->DeferredInitialize();
		if (!TypeName && ElementType) {
			TypeName = FixedString(STDString("Array<") + ElementType.Get().TypeName.GetString() + ">");
			TypeInformationRepository::GetInstance().RegisterType(this);
		}
		break;

	case LuaTypeId::Set:
		ElementType.GetStatic()->DeferredInitialize();
		if (!TypeName && ElementType) {
			TypeName = FixedString(STDString("Set<") + ElementType.Get().TypeName.GetString() + ">");
			TypeInformationRepository::GetInstance().RegisterType(this);
		}
		break;

	case LuaTypeId::Map:
		KeyType.GetStatic()->DeferredInitialize();
		ElementType.GetStatic()->DeferredInitialize();
		if (!TypeName && KeyType && ElementType) {
			TypeName = FixedString(STDString("LegacyMap<") + KeyType.Get().TypeName.GetString() + ", " + ElementType.Get().TypeName.GetString() + ">");
			TypeInformationRepository::GetInstance().RegisterType(this);
		}
		break;

	case LuaTypeId::Variant:
	{
		bool hasAllVariants{ true };
		for (auto const& var : Params) {
			var.GetStatic()->DeferredInitialize();
			hasAllVariants = hasAllVariants && (bool)var;
		}

		if (!TypeName && hasAllVariants) {
			STDString name("Variant<");
			bool first = true;
			for (auto const& var : Params) {
				if (!first) name += ",";
				first = false;
				name += var.Get().TypeName.GetString();
			}
			name += ">";
			TypeName = FixedString(name);
			TypeInformationRepository::GetInstance().RegisterType(this);
		}
		break;
	}
	}
}

void TypeInformation::Validate()
{
	if (Validated) return;
	Validated = true;

	if (!TypeName) {
		OsiError("Unnamed type found!");
	}

	switch (Kind) {
	case LuaTypeId::Object:
		for (auto& member : Members) {
			if (!member.Value) {
				OsiError("Type data missing for member: " << TypeName << "." << member.Key);
			} else {
				member.Value.GetStatic()->Type->Validate();
			}
		}

		for (auto const& method : Methods) {
			for (auto const& param : method.Value.Params) {
				if (!param) {
					OsiError("Type data missing for method parameter: " << TypeName << "." << method.Key);
				} else {
					param.GetStatic()->Type->Validate();
				}
			}

			for (auto const& retval : method.Value.ReturnValues) {
				if (!retval) {
					OsiError("Type data missing for method return value: " << TypeName << "." << method.Key);
				} else {
					retval.GetStatic()->Type->Validate();
				}
			}
		}
		break;

	case LuaTypeId::Array:
	case LuaTypeId::Set:
		if (!ElementType) {
			OsiError("Element type data missing for array: " << TypeName);
		} else {
			ElementType.GetStatic()->Type->Validate();
		}
		break;

	case LuaTypeId::Map:
		if (!KeyType) {
			OsiError("Key type data missing for map: " << TypeName);
		} else {
			KeyType.GetStatic()->Type->Validate();
		}
		if (!ElementType) {
			OsiError("Element type data missing for map: " << TypeName);
		} else {
			ElementType.GetStatic()->Type->Validate();
		}
		break;
	}
}

TypeInformationRepository& TypeInformationRepository::GetInstance()
{
	static std::unique_ptr<TypeInformationRepository> instance = std::make_unique<TypeInformationRepository>();
	return *instance;
}

TypeInformationRepository::TypeInformationRepository()
{
	types_.ResizeHashtable(GetNearestLowerPrime(3000));
	undefinedType_.TypeName = FixedString("(undefined)");
	undefinedType_.Kind = LuaTypeId::Unknown;
}

void RegisterObjectProxyTypeInformation();

void TypeInformationRepository::Initialize()
{
	RegisterStaticType<bool>("bool", LuaTypeId::Boolean);
	RegisterStaticType<int8_t>("int8", LuaTypeId::Integer);
	RegisterStaticType<uint8_t>("uint8", LuaTypeId::Integer);
	RegisterStaticType<int16_t>("int16", LuaTypeId::Integer);
	RegisterStaticType<uint16_t>("uint16", LuaTypeId::Integer);
	RegisterStaticType<int32_t>("int32", LuaTypeId::Integer);
	RegisterStaticType<uint32_t>("uint32", LuaTypeId::Integer);
	RegisterStaticType<int64_t>("int64", LuaTypeId::Integer);
	RegisterStaticType<uint64_t>("uint64", LuaTypeId::Integer);
	RegisterStaticType<float>("float", LuaTypeId::Float);
	RegisterStaticType<double>("double", LuaTypeId::Float);
	RegisterStaticType<FixedString>("FixedString", LuaTypeId::String);
	RegisterStaticType<STDString>("STDString", LuaTypeId::String);
	RegisterStaticType<STDWString>("STDWString", LuaTypeId::String);
	RegisterStaticType<StringView>("StringView", LuaTypeId::String);
	RegisterStaticType<LSStringView>("LSStringView", LuaTypeId::String);
#if defined(ENABLE_UI)
	RegisterStaticType<Noesis::Symbol>("Noesis::Symbol", LuaTypeId::String);
	RegisterStaticType<Noesis::String>("Noesis::String", LuaTypeId::String);
#endif
	RegisterStaticType<Path>("Path", LuaTypeId::String);
	RegisterStaticType<NetId>("NetId", LuaTypeId::Integer);
	RegisterStaticType<UserId>("UserId", LuaTypeId::Integer);
	// FIXME - revisit typing for handles later!
	RegisterStaticType<ComponentHandle>("ComponentHandle", LuaTypeId::Integer);
	RegisterStaticType<EntityHandle>("EntityHandle", LuaTypeId::Integer);
	RegisterStaticType<ecs::EntityRef>("EntityRef", LuaTypeId::Integer);
	RegisterStaticType<char const*>("CString", LuaTypeId::String);
	RegisterStaticType<Guid>("Guid", LuaTypeId::String);
	//RegisterStaticType<TemplateHandle>("TemplateHandle", LuaTypeId::Integer);
	RegisterStaticType<lua::Ref>("Ref", LuaTypeId::Any);
	RegisterStaticType<lua::AnyRef>("AnyRef", LuaTypeId::Any);
	RegisterStaticType<lua::AnyUserdataRef>("AnyUserdataRef", LuaTypeId::Object);
	RegisterStaticType<lua::FunctionRef>("FunctionRef", LuaTypeId::Function);
	RegisterStaticType<lua::RegistryEntry>("RegistryEntry", LuaTypeId::Any);
	RegisterStaticType<lua::PersistentRef>("PersistentRef", LuaTypeId::Any);
	RegisterStaticType<lua::PersistentRegistryEntry>("PersistentRegistryEntry", LuaTypeId::Any);
	RegisterStaticType<UserReturn>("UserReturn", LuaTypeId::Any);
	RegisterStaticType<lua::ImguiHandle>("ImguiHandle", LuaTypeId::Object);

	auto& ivec2 = RegisterStaticType<glm::ivec2>("ivec2", LuaTypeId::Array);
	ivec2.ElementType = GetStaticTypeInfo(Overload<int32_t>{});
	auto& ivec4 = RegisterStaticType<glm::ivec4>("ivec4", LuaTypeId::Array);
	ivec4.ElementType = GetStaticTypeInfo(Overload<int32_t>{});
	auto& i16vec2 = RegisterStaticType<glm::i16vec2>("i16vec2", LuaTypeId::Array);
	i16vec2.ElementType = GetStaticTypeInfo(Overload<int16_t>{});
	auto& vec2 = RegisterStaticType<glm::vec2>("vec2", LuaTypeId::Array);
	vec2.ElementType = GetStaticTypeInfo(Overload<float>{});
	auto& vec3 = RegisterStaticType<glm::vec3>("vec3", LuaTypeId::Array);
	vec3.ElementType = GetStaticTypeInfo(Overload<float>{});
	auto& vec4 = RegisterStaticType<glm::vec4>("vec4", LuaTypeId::Array);
	vec4.ElementType = GetStaticTypeInfo(Overload<float>{});
	auto& avec4 = RegisterStaticType<glm::aligned_highp_vec4>("avec4", LuaTypeId::Array);
	avec4.ElementType = GetStaticTypeInfo(Overload<float>{});
	auto& quat = RegisterStaticType<glm::quat>("quat", LuaTypeId::Array);
	quat.ElementType = GetStaticTypeInfo(Overload<float>{});
	auto& mat3 = RegisterStaticType<glm::mat3>("mat3", LuaTypeId::Array);
	mat3.ElementType = GetStaticTypeInfo(Overload<float>{});
	auto& mat4 = RegisterStaticType<glm::mat4>("mat4", LuaTypeId::Array);
	mat4.ElementType = GetStaticTypeInfo(Overload<float>{});
	auto& mat3x4 = RegisterStaticType<glm::mat3x4>("mat3x4", LuaTypeId::Array);
	mat3x4.ElementType = GetStaticTypeInfo(Overload<float>{});
	auto& mat4x3 = RegisterStaticType<glm::mat4x3>("mat4x3", LuaTypeId::Array);
	mat4x3.ElementType = GetStaticTypeInfo(Overload<float>{});

	auto& version = RegisterStaticType<Version>("Version", LuaTypeId::Array);
	version.ElementType = GetStaticTypeInfo(Overload<int32_t>{});

	auto& typeRef = RegisterStaticType<TypeInformationRef>("TypeInformationRef", LuaTypeId::Object);
	typeRef.ParentType = GetStaticTypeInfo(Overload<TypeInformation>{});

	auto& nsCollection = RegisterStaticType<Noesis::BaseCollection>("Noesis::BaseCollection", LuaTypeId::Array);
	nsCollection.ElementType = GetStaticTypeInfo(Overload<Noesis::BaseObject>{});

	RegisterStaticType<stats::ConditionId>("stats::ConditionId", LuaTypeId::String);

	RegisterObjectProxyTypeInformation();

	lua::gModuleRegistry.RegisterTypeInformation();

	for (auto type : initializers_) {
		type->DeferredInitialize();
	}

	for (auto& type : types_) {
		type.Value->DeferredInitialize();
	}

	for (auto& type : types_) {
		type.Value->Validate();
	}
}

TypeInformation& TypeInformationRepository::RegisterType(FixedString const& typeName)
{
	if (types_.find(typeName)) {
		throw std::runtime_error("Type already registered!");
	}

	auto ty = *types_.insert(typeName, GameAlloc<TypeInformation>());
	ty->TypeName = typeName;
	return *ty;
}

void TypeInformationRepository::RegisterType(TypeInformation* typeInfo)
{
	types_.insert(typeInfo->TypeName, typeInfo);
}

void TypeInformationRepository::RegisterInitializer(StaticTypeInformation* typeInfo)
{
	initializers_.push_back(typeInfo);
}

TypeInformation const* TypeInformationRepository::TryGetType(FixedString const& typeName)
{
	auto type = types_.find(typeName);
	if (type) {
		return type.Value();
	}

	return nullptr;
}

TypeInformation const& TypeInformationRepository::GetType(FixedString const& typeName)
{
	auto type = types_.find(typeName);
	if (type) {
		return *type.Value();
	}

	return undefinedType_;
}

TypeInformation const& TypeInformationRepository::GetUndefinedType()
{
	return undefinedType_;
}

LegacyMap<FixedString, TypeInformation*> const& TypeInformationRepository::GetAllTypes()
{
	return types_;
}

void TypeInformationRepository::RegisterArrayType(char const* arrayClass, TypeInformationRef const& elementType)
{
	if (!elementType) {
		throw std::runtime_error("Trying to register array type whose element type was not registered yet?");
	}

	STDString name = arrayClass;
	name += "<";
	name += elementType.Get().TypeName.GetString();
	name += ">";

	auto& ty = RegisterType(FixedString(name));
	ty.Kind = LuaTypeId::Array;
	ty.ElementType = elementType;
}

TypeInformation& TypeInformationRepository::RegisterType(char const* name, LuaTypeId typeId)
{
	auto& ty = RegisterType(FixedString(name));
	ty.Kind = typeId;
	ty.IsBuiltin = true;
	return ty;
}

END_SE()
