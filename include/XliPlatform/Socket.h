#ifndef __XLI_SOCKET_H__
#define __XLI_SOCKET_H__

#include <Xli/Object.h>
#include <Xli/Exception.h>
#include <Xli/Stream.h>

namespace Xli
{
	/**
		Thrown when read/write is attempted on a TCP socket which has lost its connection.
	*/
	class ConnectionLostException: public Exception
	{
	public:
		ConnectionLostException(): Exception("Connection lost") {}
	};

	enum NetworkAddressFamily
	{
		NetworkAddressFamilyIPv4,
		NetworkAddressFamilyIPv6
	};

	enum NetworkProtocol
	{
		NetworkProtocolTCP,
		NetworkProtocolUDP
	};

	class NetworkAddress: public Object
	{
	public:
		virtual NetworkAddressFamily GetAddressFamily() = 0;
		virtual NetworkProtocol GetProtocol() = 0;
		virtual int GetPort() = 0;
		virtual CharString GetString() = 0;
	};

	class Socket: public Object
	{
	public:
		/**
			Enables incomming connections, turning this socket into a server.
		*/
		virtual void Listen() = 0;
			
		/**
			Accepts an incomming connection from the connection queue.
			@return An incomming connection, or NULL if no connection was waiting
		*/
		virtual Socket* Accept() = 0;

		/**
			Connects to a remote server.
			If unblocking connection is used, use IsConnected() to determine that the socket is connected.
			@param address Address to connect to
		*/
		virtual void Connect(NetworkAddress* address) = 0;

		/**
			Determines wether or not the socket is connected.
			Only valid for client sockets.
			@return True if the socket is connected.
		*/
		virtual bool IsConnected() = 0;

		/**
			Returns the address of the local socket.
			@return the address of the local socket.
		*/
		virtual NetworkAddress* GetLocalAddress() = 0;

		/**
			Returns the address of the remote host.
			@return the address of the remote host.
		*/
		virtual NetworkAddress* GetRemoteAddress() = 0;

		/**
			Sets the blocking mode of the socket.
			Defaults to true.
			@param mode True for blocking, false for non-blocking.
		*/
		virtual void SetBlockingMode(bool mode) = 0;

		/**
			Returns the blocking mode of the socket.
			Defaults to true.
			@return the blocking mode of the socket.
		*/
		virtual bool GetBlockingMode() = 0;

		/**
			Sends a datagram to a destination.
			Only valid for UDP sockets. For TCP sockets, use the stream interface through GetStream().
			If the socket is set to blocking mode, this funciton will block the program execution until data arrives.
			@param destination The destination address of the datagram
			@param data Pointer to the buffer containing the datagram payload
			@param byteCount The size of the datagram payload, in bytes
		*/
		virtual int SendTo(NetworkAddress* destination, void* data, int byteCount) = 0;

		/**
			Receives a datagram.
			Only valid for UDP sockets. For TCP sockets, use the stream interface through GetStream().
			The Address object must be released by the user.
			Note that UDP datagrams are not guarranteed to be correct, in order or non-duplicated.
			If the socket is set to blocking mode, this funciton will block the program execution until data arrives.
			@param data Pointer to the buffer to receive the datagram
			@param byteCount Pointer to an int which contains the capacity of the data buffer, and receives the size of the datagram received.
			@return The address of the sender of the datagram
		*/
		virtual NetworkAddress* ReceiveFrom(void* data, int* byteCount) = 0;

		/**
			Returns the stream associated with the socket.
			Only valid for TCP sockets.
			@return the stream associated with the socket.
		*/
		virtual Stream* GetStream() = 0;
	};

	class NetworkContext: public Object
	{
	public:
		virtual NetworkAddress* CreateAddress(NetworkAddressFamily addressFamily, NetworkProtocol protocol, int port, CharString address) = 0;
		virtual Socket* CreateSocket(NetworkAddressFamily addressFamily, NetworkProtocol protocol, int port) = 0;

		/**
			Creates a platform-specific network context.
		*/
		static NetworkContext* Create();
	};


}


#endif
