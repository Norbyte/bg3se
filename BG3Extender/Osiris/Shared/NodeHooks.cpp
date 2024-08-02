#include "stdafx.h"
#include <Osiris/Shared/NodeHooks.h>
#include <Osiris/Debugger/Debugger.h>
#include <Lua/Osiris/CallbackManager.h>

namespace bg3se
{
	template <typename T>
	class ROWriteAnchor
	{
	public:
		ROWriteAnchor(T * region)
			: region_(region)
		{
			BOOL succeeded = VirtualProtect((LPVOID)region_, sizeof(T), PAGE_READWRITE, &oldProtect_);
			if (!succeeded) Fail("VirtualProtect() on region failed");
		}
		
		~ROWriteAnchor()
		{
			BOOL succeeded = VirtualProtect((LPVOID)region_, sizeof(T), oldProtect_, &oldProtect_);
			if (!succeeded) Fail("VirtualProtect() on region failed");
		}

	private:
		T * region_;
		DWORD oldProtect_;
	};

	NodeVMTWrappers * gNodeVMTWrappers{ nullptr };

	NodeVMTWrapper::NodeVMTWrapper(NodeVMT * vmt, NodeWrapOptions & options)
		: vmt_(vmt), options_(options)
	{
		originalVmt_ = *vmt_;

		ROWriteAnchor<NodeVMT> _(vmt_);
		if (options_.WrapIsValid) {
			vmt_->IsValid = &s_WrappedIsValid;
		}

		if (options_.WrapPushDownTuple) {
			vmt_->PushDownTuple = &s_WrappedPushDownTuple;
		}

		if (options_.WrapPushDownTupleDelete) {
			vmt_->PushDownTupleDelete = &s_WrappedPushDownTupleDelete;
		}

		if (options_.WrapInsertTuple) {
			vmt_->InsertTuple = &s_WrappedInsertTuple;
		}

		if (options_.WrapDeleteTuple) {
			vmt_->DeleteTuple = &s_WrappedDeleteTuple;
		}

		if (options_.WrapCallQuery) {
			vmt_->CallQuery = &s_WrappedCallQuery;
		}
	}

	NodeVMTWrapper::~NodeVMTWrapper()
	{
		ROWriteAnchor<NodeVMT> _(vmt_);
		*vmt_ = originalVmt_;
	}

	bool NodeVMTWrapper::WrappedIsValid(Node * node, VirtTupleLL * tuple, uint32_t adapter)
	{
		return originalVmt_.IsValid(node, tuple, adapter);
	}

	void NodeVMTWrapper::WrappedPushDownTuple(Node * node, VirtTupleLL * tuple, uint32_t adapter, EntryPoint which)
	{
		originalVmt_.PushDownTuple(node, tuple, adapter, which);
	}

	void NodeVMTWrapper::WrappedPushDownTupleDelete(Node * node, VirtTupleLL * tuple, uint32_t adapter, EntryPoint which)
	{
		originalVmt_.PushDownTupleDelete(node, tuple, adapter, which);
	}

	void NodeVMTWrapper::WrappedInsertTuple(Node * node, TuplePtrLL * tuple)
	{
		originalVmt_.InsertTuple(node, tuple);
	}

	void NodeVMTWrapper::WrappedDeleteTuple(Node * node, TuplePtrLL * tuple)
	{
		originalVmt_.DeleteTuple(node, tuple);
	}

	bool NodeVMTWrapper::WrappedCallQuery(Node * node, OsiArgumentDesc * args)
	{
		return originalVmt_.CallQuery(node, args);
	}

	bool NodeVMTWrapper::s_WrappedIsValid(Node * node, VirtTupleLL * tuple, uint32_t adapter)
	{
		return gNodeVMTWrappers->WrappedIsValid(node, tuple, adapter);
	}

	void NodeVMTWrapper::s_WrappedPushDownTuple(Node * node, VirtTupleLL * tuple, uint32_t adapter, EntryPoint which)
	{
		gNodeVMTWrappers->WrappedPushDownTuple(node, tuple, adapter, which);
	}

	void NodeVMTWrapper::s_WrappedPushDownTupleDelete(Node * node, VirtTupleLL * tuple, uint32_t adapter, EntryPoint which)
	{
		gNodeVMTWrappers->WrappedPushDownTupleDelete(node, tuple, adapter, which);
	}

	void NodeVMTWrapper::s_WrappedInsertTuple(Node * node, TuplePtrLL * tuple)
	{
		gNodeVMTWrappers->WrappedInsertTuple(node, tuple);
	}

	void NodeVMTWrapper::s_WrappedDeleteTuple(Node * node, TuplePtrLL * tuple)
	{
		gNodeVMTWrappers->WrappedDeleteTuple(node, tuple);
	}

	bool NodeVMTWrapper::s_WrappedCallQuery(Node * node, OsiArgumentDesc * args)
	{
		return gNodeVMTWrappers->WrappedCallQuery(node, args);
	}

	NodeWrapOptions VMTWrapOptions[(unsigned)NodeType::Max + 1] = {
		{ false, false, false, false, false, false }, // None
		{ true, false, false, true, true, false }, // Database
		{ true, false, false, true, true, false }, // Proc
		{ true, false, false, false, false, true }, // DivQuery
		{ true, true, true, false, false, false }, // And
		{ true, true, true, false, false, false }, // NotAnd
		{ true, true, true, false, false, false }, // RelOp
		{ true, true, true, false, false, false }, // Rule
		{ true, false, false, false, false, false }, // InternalQuery
		{ true, false, false, false, false, false } // UserQuery
	};

	NodeVMTWrappers::NodeVMTWrappers(NodeVMT ** vmts)
		: vmts_(vmts)
	{
		assert(gNodeVMTWrappers == nullptr);
		gNodeVMTWrappers = this;
		for (unsigned i = 1; i < (unsigned)NodeType::Max + 1; i++) {
			wrappers_[i] = std::make_unique<NodeVMTWrapper>(vmts_[i], VMTWrapOptions[i]);
			vmtToTypeMap_[vmts[i]] = (NodeType)i;
		}
	}

	NodeVMTWrappers::~NodeVMTWrappers()
	{
		assert(gNodeVMTWrappers == this);
		gNodeVMTWrappers = nullptr;
	}

	NodeType NodeVMTWrappers::GetType(Node * node)
	{
		NodeVMT * vfptr = *reinterpret_cast<NodeVMT **>(node);
		auto typeIt = vmtToTypeMap_.find(vfptr);
		if (typeIt == vmtToTypeMap_.end()) Fail("Called virtual method on a node that could not be identified");
		return typeIt->second;
	}

	NodeVMTWrapper & NodeVMTWrappers::GetWrapper(Node * node)
	{
		NodeType type = GetType(node);
		assert(type >= NodeType::Database && type <= NodeType::Max);
		return *wrappers_[(unsigned)type].get();
	}

	bool NodeVMTWrappers::WrappedIsValid(Node * node, VirtTupleLL * tuple, uint32_t adapter)
	{
		auto & wrapper = GetWrapper(node);

		if (DebuggerAttachment) {
			DebuggerAttachment->IsValidPreHook(node, tuple, adapter);
		}

		bool succeeded = wrapper.WrappedIsValid(node, tuple, adapter);

		if (DebuggerAttachment) {
			DebuggerAttachment->IsValidPostHook(node, tuple, adapter, succeeded);
		}

		return succeeded;
	}

	void NodeVMTWrappers::WrappedPushDownTuple(Node * node, VirtTupleLL * tuple, uint32_t adapter, EntryPoint which)
	{
		auto & wrapper = GetWrapper(node);

		if (DebuggerAttachment) {
			DebuggerAttachment->PushDownPreHook(node, tuple, adapter, which, false);
		}

		wrapper.WrappedPushDownTuple(node, tuple, adapter, which);

		if (DebuggerAttachment) {
			DebuggerAttachment->PushDownPostHook(node, tuple, adapter, which, false);
		}
	}

	void NodeVMTWrappers::WrappedPushDownTupleDelete(Node * node, VirtTupleLL * tuple, uint32_t adapter, EntryPoint which)
	{
		auto & wrapper = GetWrapper(node);

		if (DebuggerAttachment) {
			DebuggerAttachment->PushDownPreHook(node, tuple, adapter, which, true);
		}

		wrapper.WrappedPushDownTupleDelete(node, tuple, adapter, which);

		if (DebuggerAttachment) {
			DebuggerAttachment->PushDownPostHook(node, tuple, adapter, which, true);
		}
	}

	void NodeVMTWrappers::WrappedInsertTuple(Node * node, TuplePtrLL * tuple)
	{
		auto & wrapper = GetWrapper(node);

		if (DebuggerAttachment) {
			DebuggerAttachment->InsertPreHook(node, tuple, false);
		}

		if (OsirisCallbacksAttachment) {
			OsirisCallbacksAttachment->InsertPreHook(node, tuple, false);
		}

		wrapper.WrappedInsertTuple(node, tuple);

		if (DebuggerAttachment) {
			DebuggerAttachment->InsertPostHook(node, tuple, false);
		}

		if (OsirisCallbacksAttachment) {
			OsirisCallbacksAttachment->InsertPostHook(node, tuple, false);
		}
	}

	void NodeVMTWrappers::WrappedDeleteTuple(Node * node, TuplePtrLL * tuple)
	{
		auto & wrapper = GetWrapper(node);

		if (DebuggerAttachment) {
			DebuggerAttachment->InsertPreHook(node, tuple, true);
		}

		if (OsirisCallbacksAttachment) {
			OsirisCallbacksAttachment->InsertPreHook(node, tuple, true);
		}

		wrapper.WrappedDeleteTuple(node, tuple);

		if (DebuggerAttachment) {
			DebuggerAttachment->InsertPostHook(node, tuple, true);
		}

		if (OsirisCallbacksAttachment) {
			OsirisCallbacksAttachment->InsertPostHook(node, tuple, true);
		}
	}

	bool NodeVMTWrappers::WrappedCallQuery(Node * node, OsiArgumentDesc * args)
	{
		auto & wrapper = GetWrapper(node);

		if (DebuggerAttachment) {
			DebuggerAttachment->CallQueryPreHook(node, args);
		}

		if (OsirisCallbacksAttachment) {
			OsirisCallbacksAttachment->CallQueryPreHook(node, args);
		}

		bool succeeded = wrapper.WrappedCallQuery(node, args);

		if (DebuggerAttachment) {
			DebuggerAttachment->CallQueryPostHook(node, args, succeeded);
		}

		if (OsirisCallbacksAttachment) {
			OsirisCallbacksAttachment->CallQueryPostHook(node, args, succeeded);
		}

		return succeeded;
	}
}
