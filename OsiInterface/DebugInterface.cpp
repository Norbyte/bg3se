#include "stdafx.h"
#include "DebugInterface.h"
#include "NodeHooks.h"
#include "OsirisProxy.h"

#if !defined(OSI_NO_DEBUGGER)

#include <sstream>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <WS2tcpip.h>

namespace dse
{
	SocketInterface::SocketInterface(uint16_t port)
		: port_(port)
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		uint32_t ip;
		inet_pton(AF_INET, "127.0.0.1", &ip);
		socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = ip;
		addr.sin_port = htons(port_);
		if (bind(socket_, (sockaddr *)&addr, sizeof(addr)) != 0) {
			ERR("Could not bind debugger server socket: %d", WSAGetLastError());
			throw std::runtime_error("Debug server start failed");
		}

		if (listen(socket_, 30) != 0) {
			ERR("Could not listen on server socket: %d", WSAGetLastError());
			throw std::runtime_error("Debug server start failed");
		}
	}

	SocketInterface::~SocketInterface()
	{
		closesocket(socket_);
	}

	void SocketInterface::SetConnectHandler(
		std::function<void()> connectHandler,
		std::function<void()> disconnectHandler)
	{
		connectHandler_ = connectHandler;
		disconnectHandler_ = disconnectHandler;
	}

	bool SocketInterface::IsConnected() const
	{
		return clientSocket_ != 0;
	}

	void SocketInterface::SendProtobufMessage(uint8_t* buf, uint32_t length)
	{
		if (clientSocket_ == 0) {
			DEBUG("ProtobufSocketInterface::Send(): Not connected to debugger frontend");
			return;
		}

		uint32_t packetSize = length + 4;
		Send((uint8_t*)&packetSize, 4);
		Send(buf, length);
	}

	void SocketInterface::Send(uint8_t * buf, uint32_t length)
	{
		while (length > 0) {
			int sent = send(clientSocket_, (char *)buf, (int)length, 0);
			if (sent <= 0) {
				ERR("Socket send failed: %d, error %d", sent, WSAGetLastError());
				Disconnect();
				return;
			}

			buf += sent;
			length -= sent;
		}
	}

	void SocketInterface::Disconnect()
	{
		if (!IsConnected()) return;

		closesocket(clientSocket_);
		clientSocket_ = 0;

		if (disconnectHandler_) {
			disconnectHandler_();
		}
	}

	void SocketInterface::MessageLoop(SOCKET sock)
	{
		receivePos_ = 0;
		for (;;) {
			int len = recv(sock, (char *)&receiveBuf_[receivePos_], sizeof(receiveBuf_) - receivePos_, 0);
			if (len < 0) {
				ERR("Socket recv failed: %d, error %d", len, WSAGetLastError());
				return;
			}

			receivePos_ += len;
			while (receivePos_ >= 4) {
				uint32_t messageLength = *reinterpret_cast<uint32_t *>(&receiveBuf_[0]);

				if (messageLength < 4 || messageLength > sizeof(receiveBuf_)) {
					ERR("DebugInterface::MessageLoop(): Illegal message length: %d", messageLength);
					return;
				}

				if (receivePos_ >= messageLength) {
					if (!ProcessMessage(&receiveBuf_[4], messageLength - 4)) {
						WARN("DebugInterface::MessageLoop(): Message processing failed");
						return;
					}

					memmove(&receiveBuf_[0], &receiveBuf_[messageLength], receivePos_ - messageLength);
					receivePos_ -= messageLength;
				}
				else {
					break;
				}
			}
		}
	}

	void SocketInterface::Run()
	{
		for (;;) {
			sockaddr_in addr;
			int addrlen = sizeof(addr);
			clientSocket_ = accept(socket_, (sockaddr *)&addr, &addrlen);
			DEBUG("Accepted debug connection.");
			if (connectHandler_) {
				connectHandler_();
			}

			MessageLoop(clientSocket_);
			Disconnect();
		}
	}
}

#endif