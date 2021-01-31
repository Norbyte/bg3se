#pragma once

#if !defined(OSI_NO_DEBUGGER)

#include <cstdint>
#include <WinSock2.h>
#include "osidebug.pb.h"
#include "LuaDebug.pb.h"
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

namespace dse
{
	class SocketInterface
	{
	public:
		SocketInterface(uint16_t port);
		~SocketInterface();

		void SetConnectHandler(
			std::function<void()> connectHandler,
			std::function<void()> disconnectHandler
		);
		bool IsConnected() const;
		void Run();
		void Disconnect();

	protected:
		void SendProtobufMessage(uint8_t* buf, uint32_t length);
		virtual bool ProcessMessage(uint8_t* buf, uint32_t length) = 0;

	private:
		void Send(uint8_t* buf, uint32_t length);
		void MessageLoop(SOCKET sock);

		uint16_t port_;
		SOCKET socket_;
		SOCKET clientSocket_{ 0 };
		uint8_t receiveBuf_[0x10000];
		uint32_t receivePos_;
		std::function<void()> connectHandler_;
		std::function<void()> disconnectHandler_;
	};


	template <class TRecvMsg, class TSendMsg>
	class ProtobufSocketInterface : public SocketInterface
	{
	public:
		ProtobufSocketInterface(uint16_t port)
			: SocketInterface(port)
		{}

		void SetMessageHandler(std::function<bool(TRecvMsg const*)> messageHandler)
		{
			messageHandler_ = messageHandler;
		}

		void Send(TSendMsg const & msg)
		{
			uint32_t size = (uint32_t)msg.ByteSizeLong();

			uint8_t* buf = GameAllocArray<uint8_t>(size);
			if (!msg.SerializeToArray(buf, size)) {
				Fail("Unable to serialize message");
			}

			SendProtobufMessage(buf, size);
			GameFree(buf);
		}

	protected:
		bool ProcessMessage(uint8_t* buf, uint32_t length) override
		{
			google::protobuf::io::ArrayInputStream ais(buf, length);
			google::protobuf::io::CodedInputStream is(&ais);

			// Hack to avoid issues from debug/release CRT difference between protobuf and extender
#if defined(NDEBUG)
			TRecvMsg msg;
#else
			TRecvMsg* _msg = new TRecvMsg();
			TRecvMsg& msg = *_msg;
#endif
			if (!msg.ParseFromCodedStream(&is)) {
				ERR("Unable to decode protobuf message from coded stream.");
				return false;
			}

			return messageHandler_(&msg);
		}

	private:
		std::function<bool(TRecvMsg const*)> messageHandler_;
	};

	using OsirisDebugInterface = ProtobufSocketInterface<osidbg::DebuggerToBackend, osidbg::BackendToDebugger>;
	using LuaDebugInterface = ProtobufSocketInterface<lua::dbg::DebuggerToBackend, lua::dbg::BackendToDebugger>;
}

#endif
