#pragma once

#include <GameDefinitions/Osiris.h>
#include <unordered_map>
#include <functional>

namespace bg3se
{
	struct NodeWrapOptions
	{
		bool WrapIsValid;
		bool WrapPushDownTuple;
		bool WrapPushDownTupleDelete;
		bool WrapInsertTuple;
		bool WrapDeleteTuple;
		bool WrapCallQuery;
	};

	class NodeVMTWrapper
	{
	public:
		NodeVMTWrapper(NodeVMT * vmt, NodeWrapOptions & options);
		~NodeVMTWrapper();

		bool WrappedIsValid(Node * node, VirtTupleLL * tuple, AdapterRef * adapter);
		void WrappedPushDownTuple(Node * node, VirtTupleLL * tuple, AdapterRef * adapter, EntryPoint which);
		void WrappedPushDownTupleDelete(Node * node, VirtTupleLL * tuple, AdapterRef * adapter, EntryPoint which);
		void WrappedInsertTuple(Node * node, TuplePtrLL * tuple);
		void WrappedDeleteTuple(Node * node, TuplePtrLL * tuple);
		bool WrappedCallQuery(Node * node, OsiArgumentDesc * args);

		static bool s_WrappedIsValid(Node * node, VirtTupleLL * tuple, AdapterRef * adapter);
		static void s_WrappedPushDownTuple(Node * node, VirtTupleLL * tuple, AdapterRef * adapter, EntryPoint which);
		static void s_WrappedPushDownTupleDelete(Node * node, VirtTupleLL * tuple, AdapterRef * adapter, EntryPoint which);
		static void s_WrappedInsertTuple(Node * node, TuplePtrLL * tuple);
		static void s_WrappedDeleteTuple(Node * node, TuplePtrLL * tuple);
		static bool s_WrappedCallQuery(Node * node, OsiArgumentDesc * args);

	private:
		NodeVMT * vmt_;
		NodeWrapOptions & options_;
		NodeVMT originalVmt_;
	};

	class NodeVMTWrappers
	{
	public:
		NodeVMTWrappers(NodeVMT ** vmts);

		bool WrappedIsValid(Node * node, VirtTupleLL * tuple, AdapterRef * adapter);
		void WrappedPushDownTuple(Node * node, VirtTupleLL * tuple, AdapterRef * adapter, EntryPoint which);
		void WrappedPushDownTupleDelete(Node * node, VirtTupleLL * tuple, AdapterRef * adapter, EntryPoint which);
		void WrappedInsertTuple(Node * node, TuplePtrLL * tuple);
		void WrappedDeleteTuple(Node * node, TuplePtrLL * tuple);
		bool WrappedCallQuery(Node * node, OsiArgumentDesc * args);

		std::function<void (Node *, VirtTupleLL *, AdapterRef *)> IsValidPreHook;
		std::function<void (Node *, VirtTupleLL *, AdapterRef *, bool)> IsValidPostHook;
		std::function<void (Node *, VirtTupleLL *, AdapterRef *, EntryPoint, bool)> PushDownPreHook;
		std::function<void (Node *, VirtTupleLL *, AdapterRef *, EntryPoint, bool)> PushDownPostHook;
		std::function<void (Node *, TuplePtrLL *, bool)> InsertPreHook;
		std::function<void (Node *, TuplePtrLL *, bool)> InsertPreHookLua;
		std::function<void (Node *, TuplePtrLL *, bool)> InsertPostHook;
		std::function<void (Node *, TuplePtrLL *, bool)> InsertPostHookLua;
		std::function<void(Node *, OsiArgumentDesc *)> CallQueryPreHook;
		std::function<void(Node *, OsiArgumentDesc *)> CallQueryPreHookLua;
		std::function<void(Node *, OsiArgumentDesc *, bool)> CallQueryPostHook;
		std::function<void(Node *, OsiArgumentDesc *, bool)> CallQueryPostHookLua;

		NodeType GetType(Node * node);
		NodeVMTWrapper & GetWrapper(Node * node);

	private:
		NodeVMT ** vmts_;
		std::unique_ptr<NodeVMTWrapper> wrappers_[(unsigned)NodeType::Max + 1];
		std::unordered_map<NodeVMT *, NodeType> vmtToTypeMap_;
	};

	extern std::unique_ptr<NodeVMTWrappers> gNodeVMTWrappers;
}
