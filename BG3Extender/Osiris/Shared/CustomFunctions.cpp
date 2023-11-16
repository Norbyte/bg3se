#pragma once

#include "stdafx.h"
#include "CustomFunctions.h"
#include <Extender/ScriptExtender.h>
#include <fstream>
#include <sstream>

namespace bg3se {


CustomFunction::~CustomFunction()
{}

bool CustomFunction::ValidateArgs(OsiArgumentDesc const & params) const
{
	if (params.Count() != params_.size()) {
		OsiError("Function " << name_  << "/" << params_.size() << ": Argument count mismatch");
		return false;
	}

	for (auto i = 0; i < params_.size(); i++) {
		auto const & value = params.Get(i);
		auto const & param = params_[i];
		if (!ValidateParam(param, value)) {
			return false;
		}
	}

	return true;
}

bool CustomFunction::ValidateParam(CustomFunctionParam const & param, OsiArgumentValue const & value) const
{
	if (param.Type == ValueType::None
		|| param.Dir == FunctionArgumentDirection::Out) {
		return true;
	}

	auto typeMap = *gExtender->GetServer().Osiris().GetWrappers().Globals.Types;
	auto typeId = typeMap->ResolveAlias((uint32_t)value.TypeId);
	auto paramTypeId = typeMap->ResolveAlias((uint32_t)param.Type);

	if (typeId != paramTypeId) {
		OsiError("Function " << name_ << "/" << params_.size() << ": Argument '" << param.Name
			<< "' type mismatch; expected " << (unsigned)paramTypeId << ", got " << (unsigned)typeId);
		return false;
	}

	return true;
}

// FIXME - REMOVE - FunctionNameHash is the same
uint32_t OsiTypeHash(char const* str)
{
	uint32_t hash{ 0 };
	while (*str) {
		hash = (*str++ | 0x20) + 129 * (hash % 4294967);
	}

	return hash;
}

std::unordered_map<ValueType, STDString> ConstructOsiTypeMap()
{
	std::unordered_map<ValueType, STDString> types;
	types.insert(std::make_pair(ValueType::None, "STRING")); // "ANY" type passed from Osiris as string
	types.insert(std::make_pair(ValueType::Integer, "INTEGER"));
	types.insert(std::make_pair(ValueType::Integer64, "INTEGER64"));
	types.insert(std::make_pair(ValueType::Real, "REAL"));
	types.insert(std::make_pair(ValueType::String, "STRING"));
	types.insert(std::make_pair(ValueType::GuidString, "GUIDSTRING"));

	auto const& globals = gExtender->GetServer().Osiris().GetGlobals();
	(*globals.Types)->Iterate([&types](OsiString const& key, OsirisTypeInfo const& type) {
		types.insert(std::make_pair((ValueType)type.TypeId, key));
	});

	return types;
}

char const* OsiFunctionTypeToString(EoCFunctionType type)
{
	switch (type) {
	case EoCFunctionType::Call: return "call";
	case EoCFunctionType::Query: return "query";
	case EoCFunctionType::Event: return "event";
	default: throw new std::runtime_error("EoC function type not supported");
	}
}

void CustomFunction::GenerateHeader(std::stringstream & ss, std::unordered_map<ValueType, STDString>& typeMap) const
{
	ss << OsiFunctionTypeToString(handle_.type()) << " ";
	ss << name_ << "(";

	for (unsigned i = 0; i < params_.size(); i++) {
		if (i > 0) {
			ss << ", ";
		}

		auto const & param = params_[i];
		if (handle_.type() == EoCFunctionType::Query) {
			if (param.Dir == FunctionArgumentDirection::In) {
				ss << "[in]";
			} else {
				ss << "[out]";
			}
		}

		ss << '(' << typeMap[param.Type] << ")_" << param.Name;
	}

	ss << ") (" << (unsigned)handle_.type() << "," << handle_.classIndex() << "," << handle_.functionIndex() << ",1)\r\n";
}


bool CustomCall::Call(OsiArgumentDesc const & params)
{
	if (!ValidateArgs(params)) {
		return false;
	}

	handler_(std::ref(params));
	return true;
}


bool CustomQuery::Query(OsiArgumentDesc & params)
{
	if (!ValidateArgs(params)) {
		return false;
	}

	return handler_(std::ref(params));
}


void CustomFunctionManager::BeginStaticRegistrationPhase()
{
	assert(!staticRegistrationDone_);
}

void CustomFunctionManager::EndStaticRegistrationPhase()
{
	staticRegistrationDone_ = true;
	numStaticCalls_ = calls_.size();
	numStaticQueries_ = queries_.size();
	numStaticEvents_ = events_.size();
}

void CustomFunctionManager::ClearDynamicEntries()
{
	assert(staticRegistrationDone_);
	DEBUG("CustomFunctionManager::ClearDynamicEntries(): Clearing dynamic functions");

	for (auto it = dynamicSignatures_.begin(); it != dynamicSignatures_.end(); ++it) {
		it->second.Function = nullptr;
	}

	for (auto it = calls_.begin() + numStaticCalls_; it != calls_.end(); ++it) {
		it->reset();
	}

	for (auto it = queries_.begin() + numStaticQueries_; it != queries_.end(); ++it) {
		it->reset();
	}

	for (auto it = events_.begin() + numStaticEvents_; it != events_.end(); ++it) {
		it->reset();
	}
}

FunctionHandle CustomFunctionManager::Register(std::unique_ptr<CustomCallBase> call)
{
	assert(!staticRegistrationDone_);
	RegisterSignature(call.get());
	auto index = (uint32_t)calls_.size();
	FunctionHandle handle{ EoCFunctionType::Call, CallClassId, index | CustomFunctionId };
	call->AssignHandle(handle);
	calls_.push_back(std::move(call));
	return handle;
}

FunctionHandle CustomFunctionManager::Register(std::unique_ptr<CustomQueryBase> qry)
{
	assert(!staticRegistrationDone_);
	RegisterSignature(qry.get());
	auto index = (uint32_t)queries_.size();
	FunctionHandle handle{ EoCFunctionType::Query, QueryClassId, index | CustomFunctionId };
	qry->AssignHandle(handle);
	queries_.push_back(std::move(qry));
	return handle;
}

FunctionHandle CustomFunctionManager::Register(std::unique_ptr<CustomEvent> event)
{
	assert(!staticRegistrationDone_);
	RegisterSignature(event.get());
	auto index = (uint32_t)events_.size();
	FunctionHandle handle{ EoCFunctionType::Event, EventClassId, index | CustomFunctionId };
	event->AssignHandle(handle);
	events_.push_back(std::move(event));
	return handle;
}

std::optional<FunctionHandle> CustomFunctionManager::RegisterDynamic(std::unique_ptr<CustomCallBase> call)
{
	assert(staticRegistrationDone_);
	uint32_t index;
	if (RegisterDynamicSignature(call.get(), index)) {
		FunctionHandle handle{ EoCFunctionType::Call, CallClassId, index | CustomFunctionId };
		call->AssignHandle(handle);
		calls_[index] = std::move(call);
		return handle;
	} else {
		return {};
	}
}

std::optional<FunctionHandle> CustomFunctionManager::RegisterDynamic(std::unique_ptr<CustomQueryBase> query)
{
	assert(staticRegistrationDone_);
	uint32_t index;
	if (RegisterDynamicSignature(query.get(), index)) {
		FunctionHandle handle{ EoCFunctionType::Query, QueryClassId, index | CustomFunctionId };
		query->AssignHandle(handle);
		queries_[index] = std::move(query);
		return handle;
	} else {
		return {};
	}
}

std::optional<FunctionHandle> CustomFunctionManager::RegisterDynamic(std::unique_ptr<CustomEvent> event)
{
	assert(staticRegistrationDone_);
	uint32_t index;
	if (RegisterDynamicSignature(event.get(), index)) {
		FunctionHandle handle{ EoCFunctionType::Event, EventClassId, index | CustomFunctionId };
		event->AssignHandle(handle);
		events_[index] = std::move(event);
		return handle;
	} else {
		return {};
	}
}


bool CustomFunctionManager::RegisterDynamicSignature(CustomFunction * func, uint32_t & index)
{
	auto sig = func->NameAndArity();
	if (signatures_.find(sig) != signatures_.end()) {
		OsiError("A static function with the same name already exists: " << sig.Name);
		return false;
	}

	auto dynamicSig = dynamicSignatures_.find(sig);
	auto type = func->GetType();
	if (dynamicSig == dynamicSignatures_.end()) {
		switch (type) {
		case FunctionType::Call:
			index = (uint32_t)calls_.size();
			calls_.push_back(std::unique_ptr<CustomCallBase>());
			break;

		case FunctionType::Query:
			index = (uint32_t)queries_.size();
			queries_.push_back(std::unique_ptr<CustomQueryBase>());
			break;

		case FunctionType::Event:
			index = (uint32_t)events_.size();
			events_.push_back(std::unique_ptr<CustomEvent>());
			break;

		default:
			Fail("Unsupported function type!");
		}

		DynamicFunctionBindingInfo binding{ type, index, func };
		dynamicSignatures_.insert(std::make_pair(sig, binding));
		return true;
	} else {
		if (dynamicSig->second.Type != type) {
			OsiError("Attempted to register function '" << func->Name() << "' with different type");
			return false;
		}

		if (dynamicSig->second.Function != nullptr) {
			OsiError("Dynamic function '" << func->Name() << "' is already bound!");
			return false;
		}

		dynamicSig->second.Function = func;
		index = dynamicSig->second.Index;
		return true;
	}
}

CustomFunction * CustomFunctionManager::Find(FunctionNameAndArity const & signature)
{
	auto it = signatures_.find(signature);
	if (it != signatures_.end()) {
		return it->second;
	}

	auto itDyn = dynamicSignatures_.find(signature);
	if (itDyn != dynamicSignatures_.end()) {
		return itDyn->second.Function;
	} else {
		return nullptr;
	}
}

bool CustomFunctionManager::Call(FunctionHandle handle, OsiArgumentDesc const & params)
{
	if (handle.classIndex() != CallClassId || !(handle.functionIndex() & CustomFunctionId)) {
		OsiError("Cannot call " << (uint64_t)handle << " - not a custom function!");
		return false;
	}

	auto index = handle.functionIndex() & ~CustomFunctionId;
	if (index >= calls_.size()) {
		OsiError("Call index " << index << " out of bounds!");
		return false;
	}

	if (!calls_[index]) {
		OsiError("Call index " << index << " not mapped to a custom function!");
		return false;
	}

	return calls_[index]->Call(params);
}

bool CustomFunctionManager::Query(FunctionHandle handle, OsiArgumentDesc & params)
{
	if (handle.classIndex() != QueryClassId || !(handle.functionIndex() & CustomFunctionId)) {
		OsiError("Cannot query " << (uint64_t)handle << " - not a custom function!");
		return false;
	}

	auto index = handle.functionIndex() & ~CustomFunctionId;
	if (index >= queries_.size()) {
		OsiError("Query index " << index << " out of bounds!");
		return false;
	}

	if (!queries_[index]) {
		OsiError("Query index " << index << " not mapped to a custom function!");
		return false;
	}

	return queries_[index]->Query(params);
}

void CustomFunctionManager::RegisterSignature(CustomFunction * func)
{
	auto signature = func->NameAndArity();
	if (signatures_.find(signature) != signatures_.end()) {
		throw std::runtime_error(std::string("Could not register function ") + signature.Name.c_str() + "/"
			+ std::to_string(signature.Arity) + " - already registered!");
	}

	signatures_.insert(std::make_pair(signature, func));
}

STDString CustomFunctionManager::GenerateHeaders() const
{
	auto typeMap = ConstructOsiTypeMap();
	std::stringstream ss;

	for (auto const & call : calls_) {
		if (call) {
			call->GenerateHeader(ss, typeMap);
		}
	}

	for (auto const & query : queries_) {
		if (query) {
			query->GenerateHeader(ss, typeMap);
		}
	}

	for (auto const & event : events_) {
		if (event) {
			event->GenerateHeader(ss, typeMap);
		}
	}

	return STDString(ss.str());
}

void CustomFunctionManager::PreProcessStory(STDString const & original, STDString & postProcessed)
{
	STDString ph1;
	ph1.reserve(original.size());
	postProcessed.reserve(original.size());

	STDString::size_type pos = 0;
	while (pos < original.size()) {
		auto next = original.find("/* [EXTENDER_ONLY]", pos);
		if (next == STDString::npos) {
			ph1 += original.substr(pos);
			break;
		}

		auto end = original.find("*/", next);
		if (end == STDString::npos) {
			ph1 += original.substr(pos);
			break;
		}

		ph1 += original.substr(pos, next - pos);
		ph1 += original.substr(next + 19, end - next - 19);
		pos = end + 2;
	}

	pos = 0;
	while (pos < ph1.size()) {
		auto next = ph1.find("// [BEGIN_NO_EXTENDER]", pos);
		if (next == STDString::npos) {
			postProcessed += ph1.substr(pos);
			break;
		}

		auto end = ph1.find("// [END_NO_EXTENDER]", next);
		if (end == STDString::npos) {
			postProcessed += ph1.substr(pos);
			break;
		}

		postProcessed += ph1.substr(pos, next - pos);
		pos = end + 21;
	}
}

void CustomFunctionManager::PreProcessStory(wchar_t const * path)
{
	STDString original;
	STDString postProcessed;

	{
		std::ifstream f(path, std::ios::in | std::ios::binary);
		if (!f.good()) return;

		f.seekg(0, std::ios::end);
		original.resize((uint32_t)f.tellg());
		f.seekg(0, std::ios::beg);
		f.read(original.data(), original.size());
	}

	// Clear compile trace flags to avoid large compile traces
	auto debugPos = original.find("option compile_trace\r\n");
	if (debugPos != STDString::npos) {
		for (STDString::size_type i = debugPos; i < debugPos + 20; i++) {
			original[i] = ' ';
		}
	}

	if (esv::ExtensionState::Get().HasFeatureFlag("Preprocessor")) {
		PreProcessStory(original, postProcessed);
	} else {
		postProcessed = original;
	}

	{
		std::ofstream f(path, std::ios::out | std::ios::binary);
		if (!f.good()) return;

		f.write(postProcessed.data(), postProcessed.size());
	}
}


CustomFunctionInjector::CustomFunctionInjector(OsirisWrappers & wrappers, CustomFunctionManager & functions)
	: wrappers_(wrappers), functions_(functions)
{}

CustomFunctionInjector::~CustomFunctionInjector()
{
	wrappers_.GetFunctionMappings.ClearHook();
	wrappers_.Call.ClearHook();
	wrappers_.Query.ClearHook();
	wrappers_.CreateFileW.ClearHook();
	wrappers_.CloseHandle.ClearHook();
}

void CustomFunctionInjector::Initialize()
{
	wrappers_.GetFunctionMappings.SetPostHook(&CustomFunctionInjector::OnAfterGetFunctionMappings, this);
	wrappers_.Call.SetWrapper(&CustomFunctionInjector::StaticCallWrapper);
	wrappers_.Query.SetWrapper(&CustomFunctionInjector::StaticQueryWrapper);
	wrappers_.CreateFileW.SetPostHook(&CustomFunctionInjector::OnCreateFile, this);
	wrappers_.CloseHandle.SetPostHook(&CustomFunctionInjector::OnCloseHandle, this);
}

unsigned gCustomEventDepth{ 0 };

struct CustomEventGuard
{
	static constexpr unsigned MaxDepth = 10;

	CustomEventGuard() { gCustomEventDepth++; }
	~CustomEventGuard() { gCustomEventDepth--; }

	bool CanThrowEvent() const
	{
		return gCustomEventDepth < MaxDepth;
	}
};

void CustomFunctionInjector::ThrowEvent(FunctionHandle handle, OsiArgumentDesc * args) const
{
	auto it = divToOsiMappings_.find(handle);
	if (it != divToOsiMappings_.end()) {
		CustomEventGuard guard;
		if (guard.CanThrowEvent()) {
			auto osiris = gExtender->GetServer().Osiris().GetDynamicGlobals().OsirisObject;
			gExtender->GetServer().Osiris().GetWrappers().Event.CallOriginal(osiris, it->second, args);
		} else {
			OsiError("Maximum Osiris event depth (" << gCustomEventDepth << ") exceeded");
		}
	} else {
		OsiError("Event handle not mapped: " << std::hex << (unsigned)handle);
	}
}

void OsiFunctionToSymbolInfo(Function & func, OsiSymbolInfo & symbol)
{
	symbol.name = func.Signature->Name;
	symbol.type = func.Type;
	symbol.nodeId = func.Node.Id;
	symbol.EoCFunctionId = 0;

	symbol.params.resize(func.Signature->Params->Params.Size);
	uint32_t paramIdx = 0;
	auto paramHead = func.Signature->Params->Params.Head;
	auto param = paramHead->Next;
	while (param != paramHead) {
		symbol.params[paramIdx].output = func.Signature->OutParamList.isOutParam(paramIdx);
		symbol.params[paramIdx++].type = (ValueType)param->Item.Type;
		param = param->Next;
	}
}

void CustomFunctionInjector::CreateOsirisSymbolMap(MappingInfo ** Mappings, uint32_t * MappingCount)
{
	// Create a map of Osiris symbols
	osiSymbols_.clear();
	osiSymbols_.reserve(10000);

	std::unordered_map<FunctionNameAndArity, uint32_t> symbolMap;
	auto funcs = *gExtender->GetServer().Osiris().GetGlobals().Functions;
	auto visit = [&symbolMap, this](OsiString const & str, Function * func) {
		OsiSymbolInfo symbol;
		OsiFunctionToSymbolInfo(*func, symbol);

		FunctionNameAndArity sig{ symbol.name, (uint32_t)symbol.params.size() };
		symbolMap.insert(std::make_pair(sig, (uint32_t)osiSymbols_.size()));

		osiSymbols_.push_back(std::move(symbol));
	};

	for (auto i = 0; i < 0x3ff; i++) {
		auto map = funcs->Hash[i].NodeMap;
		map.Iterate(visit);
	}

	// Map EoC function indices to Osiris functions
	for (unsigned i = 0; i < *MappingCount; i++) {
		auto const & mapping = (*Mappings)[i];
		FunctionNameAndArity sig{ mapping.Name, mapping.NumParams };
		auto it = symbolMap.find(sig);
		if (it != symbolMap.end()) {
			osiSymbols_[it->second].EoCFunctionId = mapping.Id;
		} else {
			WARN("No Osiris definition found for engine function %s/%d", mapping.Name, mapping.NumParams);
		}
	}
}

void CustomFunctionInjector::OnAfterGetFunctionMappings(void * Osiris, MappingInfo ** Mappings, uint32_t * MappingCount)
{
	CreateOsirisSymbolMap(Mappings, MappingCount);

	// Osiris function mapping may fail after an unsuccessful merge
	if (*Mappings == nullptr) {
		return;
	}

	// FIXME - remove, move to server state load event?
	gExtender->GetServer().GetExtensionState().Reset();

	auto mappingsPtr = (*Mappings)[0].ParamTypes;
	uint32_t numParams = 0;
	for (unsigned i = 0; i < *MappingCount; i++) {
		numParams += (*Mappings)[i].NumParams;
	}

	// Remove local functions
	auto outputIndex = 0;
	osiToDivMappings_.clear();
	divToOsiMappings_.clear();
	for (unsigned i = 0; i < *MappingCount; i++) {
		auto const & mapping = (*Mappings)[i];
		FunctionNameAndArity sig{ mapping.Name, mapping.NumParams };
		auto mapped = functions_.Find(sig);
		if (mapped == nullptr) {
			(*Mappings)[outputIndex++] = mapping;
		} else {
			osiToDivMappings_.insert(std::make_pair(mapping.Id, mapped->Handle()));
			divToOsiMappings_.insert(std::make_pair(mapped->Handle(), mapping.Id));
#if 0
			DEBUG("Function mapping (%s): %08x --> %08x", mapping.Name, mapping.Id, (unsigned int)mapped->Handle());
#endif
		}
	}

	// Mappings[0].ParamTypes needs to point to the beginning of the allocated parameter buffer,
	// as Osiris will try to free that pointer after function mapping was done. Since we can remove
	// functions from the beginning of the mapped function list, we need to remap the parameter list buffer.
	auto removedMappings = (*Mappings)[0].ParamTypes - mappingsPtr;
	auto newMappings = GameAllocArray<uint8_t>(numParams);
	memcpy(newMappings, (*Mappings)[0].ParamTypes, numParams - removedMappings);

	for (unsigned i = 0; i < *MappingCount; i++) {
		auto& mapping = (*Mappings)[i];
		mapping.ParamTypes = newMappings + (mapping.ParamTypes - mappingsPtr) - removedMappings;
	}

	DEBUG("CustomFunctionInjector mapping phase: %d -> %d functions", *MappingCount, outputIndex);
	*MappingCount = outputIndex;

	esv::ExtensionState::Get().StoryFunctionMappingsUpdated();
}

bool CustomFunctionInjector::StaticCallWrapper(DivFunctions::CallProc next, uint32_t handle, OsiArgumentDesc* params)
{
	auto& self = gExtender->GetServer().Osiris().GetCustomFunctionInjector();
	return self.CallWrapper(next, handle, params);
}

bool CustomFunctionInjector::StaticQueryWrapper(DivFunctions::CallProc next, uint32_t handle, OsiArgumentDesc* params)
{
	auto& self = gExtender->GetServer().Osiris().GetCustomFunctionInjector();
	return self.QueryWrapper(next, handle, params);
}

bool CustomFunctionInjector::CallWrapper(DivFunctions::CallProc next, uint32_t handle, OsiArgumentDesc * params)
{
	auto it = osiToDivMappings_.find(handle);

	if (it != osiToDivMappings_.end()) {
		return functions_.Call(it->second, *params);
	} else {
		return next(handle, params);
	}
}

bool CustomFunctionInjector::QueryWrapper(DivFunctions::CallProc next, uint32_t handle, OsiArgumentDesc * params)
{
	auto it = osiToDivMappings_.find(handle);
	if (it != osiToDivMappings_.end()) {
		return functions_.Query(it->second, *params);
	} else {
		return next(handle, params);
	}
}

void CustomFunctionInjector::ExtendStoryHeader(std::wstring const & headerPath)
{
	extendingStory_ = true;

#if 0
	DEBUG("CustomFunctionInjector::ExtendStoryHeader(): Appending to header:\r\n");
	OutputDebugStringA(headers.c_str());
	std::cout << headers << std::endl;
#endif

	auto headers = functions_.GenerateHeaders();
	std::ofstream wf(headerPath.c_str(), std::ios::out | std::ios::binary | std::ios::app);
	wf.write(headers.data(), headers.size());
	wf.close();

#if !defined(OSI_EOCAPP)
	// Workaround to Windows Defender keeping the file open while 
	// the editor is trying to read the generated headers
	Sleep(100);
#endif

	extendingStory_ = false;
}

void CustomFunctionInjector::AddJunkToStory(std::wstring const& storyPath)
{
	extendingStory_ = true;

	std::ofstream wf(storyPath.c_str(), std::ios::out | std::ios::binary | std::ios::app);
	wf.write("\r\nJUNK JUNK JUNK JUNK JUNK\r\n", 28);
	wf.close();

	extendingStory_ = false;
}

void CustomFunctionInjector::OnCreateFile(LPCWSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile,
	HANDLE hFile)
{
	if (!extendingStory_ && (dwDesiredAccess & GENERIC_WRITE) && gExtender->GetConfig().ExtendStory) {
		auto length = wcslen(lpFileName);
		if (length > 16 && wcscmp(&lpFileName[length - 16], L"story_header.div") == 0) {
			DEBUG("CustomFunctionInjector::OnCreateFile: %s", ToStdUTF8(lpFileName).c_str());
			storyHeaderFile_ = hFile;
			storyHeaderPath_ = lpFileName;
		}

		if (length > 19 && wcscmp(&lpFileName[length - 19], L"story_generated.div") == 0) {
			DEBUG("CustomFunctionInjector::OnCreateFile: %s", ToStdUTF8(lpFileName).c_str());
			storyGeneratedFile_ = hFile;
			storyGeneratedPath_ = lpFileName;
		}
	}
}

void CustomFunctionInjector::OnCloseHandle(HANDLE hFile, BOOL bSucceeded)
{
	if (bSucceeded && !extendingStory_) {
		if (storyHeaderFile_ != NULL && hFile == storyHeaderFile_) {
			if (esv::ExtensionState::Get().HasFeatureFlag("Osiris")) {
				ExtendStoryHeader(storyHeaderPath_);
			}

			storyHeaderFile_ = NULL;
		}

		if (storyGeneratedFile_ != NULL && hFile == storyGeneratedFile_) {
			if (gExtender->GetConfig().DisableStoryCompilation) {
				AddJunkToStory(storyGeneratedPath_);
			}

			storyHeaderFile_ = NULL;
		}
	}
}

}