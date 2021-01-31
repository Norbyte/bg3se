#include "stdafx.h"
#include "OsirisProxy.h"

namespace dse
{
	IdentityAdapterMap::IdentityAdapterMap(OsirisStaticGlobals const & globals)
		: globals_(globals)
	{}

	void IdentityAdapterMap::UpdateAdapters()
	{
		if ((*globals_.Adapters) == nullptr) {
			DEBUG("Not syncing adapters - DB not available");
			return;
		}

		auto const & adapterDb = (*globals_.Adapters)->Db;
		for (unsigned i = 0; i < adapterDb.Size; i++) {
			auto adapter = adapterDb.Start[i];
			TryAddAdapter(adapter);
		}
	}

	bool IdentityAdapterMap::HasAllAdapters()
	{
		for (unsigned i = 0; i <= MaxColumns; i++) {
			auto it = adapters_.find(i);
			if (it == adapters_.end()) return false;
		}

		return true;
	}

	Adapter * IdentityAdapterMap::FindAdapter(uint8_t columns)
	{
		auto it = adapters_.find(columns);
		if (it == adapters_.end()) return nullptr;
		return it->second;
	}

	void IdentityAdapterMap::TryAddAdapter(Adapter * adapter)
	{
		if (adapter->Constants.Data.Items.Size > 0) return;

		for (size_t i = 0; i < adapter->VarToColumnMapCount; i++) {
			if (i != adapter->ColumnToVarMaps[i]) return;
		}

		adapters_[(uint8_t)adapter->VarToColumnMapCount] = adapter;
	}


	void __declspec(noinline) OsiArgumentValue::Set(int32_t value)
	{
		if (TypeId == ValueType::None) return;
		if (TypeId == ValueType::Integer) {
			Int32 = value;
		} else {
			OsiError("Tried to return int32 as a " << (unsigned)TypeId << " variable!");
		}
	}

	void __declspec(noinline) OsiArgumentValue::Set(int64_t value)
	{
		if (TypeId == ValueType::None) return;
		if (TypeId == ValueType::Integer64) {
			Int64 = value;
		} else {
			OsiError("Tried to return int64 as a " << (unsigned)TypeId << " variable!");
		}
	}

	void __declspec(noinline) OsiArgumentValue::Set(float value)
	{
		if (TypeId == ValueType::None) return;
		if (TypeId == ValueType::Real) {
			Float = value;
		} else {
			OsiError("Tried to return float as a " << (unsigned)TypeId << " variable!");
		}
	}

	void __declspec(noinline) OsiArgumentValue::Set(char const * value)
	{
		if (TypeId == ValueType::None) return;
		if ((unsigned)TypeId >= (unsigned)ValueType::String) {
			String = value;
		} else {
			OsiError("Tried to return string as a " << (unsigned)TypeId << " variable!");
		}
	}
}
