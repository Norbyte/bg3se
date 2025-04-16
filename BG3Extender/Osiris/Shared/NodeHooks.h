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

        bool WrappedIsValid(Node * node, SmallTuple* tuple, uint32_t adapter);
        void WrappedPushDownTuple(Node * node, SmallTuple* tuple, uint32_t adapter, EntryPoint which);
        void WrappedPushDownTupleDelete(Node * node, SmallTuple* tuple, uint32_t adapter, EntryPoint which);
        void WrappedInsertTuple(Node * node, TuplePtrLL * tuple);
        void WrappedDeleteTuple(Node * node, TuplePtrLL * tuple);
        bool WrappedCallQuery(Node * node, OsiArgumentDesc * args);

        static bool s_WrappedIsValid(Node * node, SmallTuple* tuple, uint32_t adapter);
        static void s_WrappedPushDownTuple(Node * node, SmallTuple* tuple, uint32_t adapter, EntryPoint which);
        static void s_WrappedPushDownTupleDelete(Node * node, SmallTuple* tuple, uint32_t adapter, EntryPoint which);
        static void s_WrappedInsertTuple(Node * node, TuplePtrLL * tuple);
        static void s_WrappedDeleteTuple(Node * node, TuplePtrLL * tuple);
        static bool s_WrappedCallQuery(Node * node, OsiArgumentDesc * args);

    private:
        NodeVMT * vmt_;
        NodeWrapOptions & options_;
        NodeVMT originalVmt_;
    };

    class NodeVMTWrappers : Noncopyable<NodeVMTWrappers>
    {
    public:
        NodeVMTWrappers(NodeVMT ** vmts);
        ~NodeVMTWrappers();

        bool WrappedIsValid(Node * node, SmallTuple* tuple, uint32_t adapter);
        void WrappedPushDownTuple(Node * node, SmallTuple* tuple, uint32_t adapter, EntryPoint which);
        void WrappedPushDownTupleDelete(Node * node, SmallTuple* tuple, uint32_t adapter, EntryPoint which);
        void WrappedInsertTuple(Node * node, TuplePtrLL * tuple);
        void WrappedDeleteTuple(Node * node, TuplePtrLL * tuple);
        bool WrappedCallQuery(Node * node, OsiArgumentDesc * args);

        osidbg::Debugger* DebuggerAttachment{ nullptr };
        esv::lua::OsirisCallbackManager* OsirisCallbacksAttachment{ nullptr };

        NodeType GetType(Node * node);
        NodeVMTWrapper & GetWrapper(Node * node);

    private:
        NodeVMT ** vmts_;
        std::unique_ptr<NodeVMTWrapper> wrappers_[(unsigned)NodeType::Max + 1];
        std::unordered_map<NodeVMT *, NodeType> vmtToTypeMap_;
    };
}
