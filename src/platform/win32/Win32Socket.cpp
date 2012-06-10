#include <XliPlatform/Socket.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

namespace Xli
{
	class Win32Address: public NetworkAddress
	{
	public:		
		NetworkAddressFamily af;
		NetworkProtocol protocol;

		int win32protocol;
		int win32type;

		sockaddr_storage addr;
		sockaddr_in* inAddr;
		sockaddr_in6* in6Addr;

		int addrSize;

		Win32Address(NetworkAddressFamily af, NetworkProtocol p, int port, CharString a)
		{
			this->af = af;
			this->protocol = p;

			memset(&addr, 0, sizeof(addr));
			this->inAddr = (sockaddr_in*)&addr;
			this->in6Addr = (sockaddr_in6*)&addr;

			switch (af)
			{
			case NetworkAddressFamilyIPv4: 
				inAddr->sin_family = AF_INET;
				addrSize = sizeof(inAddr);
				break;
			case NetworkAddressFamilyIPv6: 
				inAddr->sin_family = AF_INET6; 
				addrSize = sizeof(in6Addr);
				break;
			default: XLI_THROW("Unsupported address family");
			}
			switch (p)
			{
			case NetworkProtocolTCP:
				win32protocol = IPPROTO_TCP;
				win32type = SOCK_STREAM;
				break;
			case NetworkProtocolUDP:
				win32protocol = IPPROTO_UDP;
				win32type = SOCK_DGRAM;
				break;
			default: XLI_THROW("Unsupported protocol");
			}

			inAddr->sin_port = htons(port);

			if (a.Length() > 0)
			{
				hostent* host = gethostbyname(a.Data());
				char* ip = inet_ntoa(*(in_addr*)*host->h_addr_list);
				inAddr->sin_addr.s_addr = inet_addr(ip);
			}
			else
			{
				inAddr->sin_addr.s_addr = INADDR_ANY;
			}
		}

		Win32Address(NetworkProtocol p, sockaddr_storage a)
		{
			this->addr = a;

			this->inAddr = (sockaddr_in*)&addr;
			this->in6Addr = (sockaddr_in6*)&addr;
			switch (inAddr->sin_family)
			{
			case AF_INET: 
				af = NetworkAddressFamilyIPv4; 
				addrSize = sizeof(inAddr);
				break;
			case AF_INET6: 
				af = NetworkAddressFamilyIPv6; 
				addrSize = sizeof(in6Addr);
				break;
			default: XLI_THROW("Unsupported address family (incomming connection)");
			}
			protocol = p;
			switch (p)
			{
			case NetworkProtocolTCP:
				win32protocol = IPPROTO_TCP;
				win32type = SOCK_STREAM;
				break;
			case NetworkProtocolUDP:
				win32protocol = IPPROTO_UDP;
				win32type = SOCK_DGRAM;
				break;
			default: XLI_THROW("Unsupported protocol (incomming connection)");
			}
		}
	
		NetworkAddressFamily GetAddressFamily()
		{
			return af;
		}

		NetworkProtocol GetProtocol()
		{
			return protocol;
		}

		int GetPort()
		{
			return inAddr->sin_port;
		}

		CharString GetString()
		{
			return inet_ntoa(inAddr->sin_addr);
		}
	};

	int handleWSAError()
	{
		int e = WSAGetLastError();
		switch (e)
		{
		case WSANOTINITIALISED:	XLI_THROW("Unable to read data from socket: WSANOTINITIALISED");
		case WSAENETDOWN: XLI_THROW("Unable to read data from socket: WSAENETDOWN");
		case WSAEFAULT:	XLI_THROW("Unable to read data from socket: WSAEFAULT");
		case WSAENOTCONN: XLI_THROW("Unable to read data from socket: WSAENOTCONN");
		case WSAEINTR: XLI_THROW("Unable to read data from socket: WSAEINTR");
		case WSAEINPROGRESS: XLI_THROW("Unable to read data from socket: WSAEINPROGRESS");
		case WSAENETRESET: XLI_THROW("Unable to read data from socket: WSAENETRESET");
		case WSAENOTSOCK: XLI_THROW("Unable to read data from socket: WSAENOTSOCK");
		case WSAEOPNOTSUPP:	XLI_THROW("Unable to read data from socket: WSAEOPNOTSUPP");
		case WSAESHUTDOWN: XLI_THROW("Unable to read data from socket: WSAESHUTDOWN");
		case WSAEWOULDBLOCK: return 0;
		case WSAEMSGSIZE: XLI_THROW("Unable to read data from socket: WSAEMSGSIZE");
		case WSAEINVAL:	XLI_THROW("Unable to read data from socket: WSAEINVAL");
		case WSAECONNABORTED: throw ConnectionLostException();
		case WSAETIMEDOUT: XLI_THROW("Unable to read data from socket: WSAETIMEDOUT");
		case WSAECONNRESET:	throw ConnectionLostException();
		default: XLI_THROW("Unable to read data from socket: unknown: " + e);
		}
	}

	class Win32SocketStream: public Xli::Stream
	{
		SOCKET socket;
	public:
		Win32SocketStream(SOCKET socket)
		{
			this->socket = socket;
		}

		virtual bool CanRead() const { return true; }
		virtual bool CanWrite() const { return true; }

		virtual int BytesWaiting()
		{
			unsigned long count;
			ioctlsocket(socket, FIONREAD, &count);
			return count;
		}

		int Read(void* data, int elementSize, int elementCount)
		{
			if (!(elementSize*elementCount)) return 0;
			int count = recv(socket, (char*)data, elementSize*elementCount, MSG_WAITALL);
			if (count == SOCKET_ERROR) return handleWSAError();
			return count / elementSize;				
		}

		int Write(const void* data, int elementSize, int elementCount)
		{
			if (!(elementSize*elementCount)) return 0;
			int count = 0;
			while (count != elementSize*elementCount) count += send(socket, (char*)data + count, elementSize*elementCount - count, 0);
			if (count == SOCKET_ERROR) return handleWSAError();
			return count / elementSize;
		}
	};

	class Win32Socket: public Socket
	{
		SOCKET sock;
		Win32Address* remoteAddress;
		Win32Address* localAddress;
		bool blockMode;
		Xli::Stream* stream;
	public:
		Win32Socket(Win32Address* localAddress)
		{
			this->stream = 0;
			this->localAddress = localAddress;
			sock = socket(localAddress->inAddr->sin_family, localAddress->win32type, localAddress->win32protocol);
			if (sock == INVALID_SOCKET) XLI_THROW("Unable to create socket");
			if (bind(sock, (sockaddr*)&localAddress->addr, sizeof(localAddress->addr)) == SOCKET_ERROR) XLI_THROW("Unable to bind socket");

			SetBlockingMode(true);
		}

		Win32Socket(Win32Address* localAddress, Win32Address* remoteAddress, SOCKET sock)
		{
			this->stream = 0;
			this->localAddress = localAddress;
			this->remoteAddress = remoteAddress;
			this->sock = sock;
			SetBlockingMode(true);
		}

		~Win32Socket()
		{
			closesocket(sock);
			if (stream) stream->Release();
		}

		NetworkAddress* ReceiveFrom(void* data, int* byteCount)
		{
			if (localAddress->protocol != NetworkProtocolUDP) XLI_THROW("ReceiveFrom(): Only valid for UDP sockets");
			sockaddr_storage srcAddr;
			int srcAddrSize = sizeof(srcAddr);
			int count = recvfrom(sock, (char*)data, *byteCount, 0, (sockaddr*)&srcAddr, &srcAddrSize);
			if (count == SOCKET_ERROR) if (handleWSAError() == 0) count = 0;
			*byteCount = count;
			return new Win32Address(NetworkProtocolUDP, srcAddr);			
		}

		int SendTo(NetworkAddress* address, void* data, int byteCount)
		{
			int count = sendto(sock, (char*)data, byteCount, 0, (sockaddr*)&((Win32Address*)address)->addr, sizeof(sockaddr_storage));
			if (count == SOCKET_ERROR) return handleWSAError();
			return count;
		}

		void Listen()
		{
			if (listen(sock, SOMAXCONN) == SOCKET_ERROR) XLI_THROW("Unable to start listening");
		}

		Socket* Accept()
		{
			sockaddr_storage addr;
			memset(&addr, 0, sizeof(sockaddr_storage));
			int addrSize = sizeof(sockaddr_storage);
			SOCKET client = accept(sock, (sockaddr*)&addr, &addrSize);
			if (client == INVALID_SOCKET) return 0;
				
			Win32Address* remoteAddress = new Win32Address(NetworkProtocolTCP, addr);			
			return new Win32Socket(localAddress, remoteAddress, client);
		}

		void Connect(NetworkAddress* address)
		{
			remoteAddress = (Win32Address*)address;
			if (connect(sock, (sockaddr*)&remoteAddress->addr, sizeof(remoteAddress->addr)) == SOCKET_ERROR)
			{
				XLI_THROW("Unable to connect");
			}
		}

		bool IsConnected()
		{
			fd_set set;
			set.fd_array[0] = sock;
			set.fd_count = 1;
			int count = select(0, 0, &set, 0, 0);
			if (count == SOCKET_ERROR) XLI_THROW("Unable to check socket status");
			return (count != 0);
		}

		NetworkAddress* GetLocalAddress()
		{
			return localAddress;
		}

		NetworkAddress* GetRemoteAddress()
		{
			return remoteAddress;
		}

		void SetBlockingMode(bool mode)
		{
			u_long block = mode? 0 : 1;
			if (ioctlsocket(sock, FIONBIO, &block) == SOCKET_ERROR) XLI_THROW("Unable to set blocking mode on socket");
			this->blockMode = mode;
		}

		bool GetBlockingMode() 
		{
			return blockMode;
		}

		Xli::Stream* GetStream()
		{
			if (localAddress->protocol == NetworkProtocolUDP) XLI_THROW("GetStream(): Only valid for TCP sockets");
			if (stream == 0) stream = new Win32SocketStream(sock);
			return stream;
		}

	};

	class Win32Context: public NetworkContext
	{
		WSADATA info;
	public:
		Win32Context()
		{
			if (WSAStartup(MAKEWORD(2, 0), &info) != 0) XLI_THROW("Unable to initialize Winsock 2.0");
		}
		~Win32Context()
		{
			WSACleanup();
		}
		Socket* CreateSocket(NetworkAddressFamily addressFamily, NetworkProtocol protocol, int port)
		{
			return new Win32Socket(new Win32Address(addressFamily, protocol, port, ""));
		}
		NetworkAddress* CreateAddress(NetworkAddressFamily addressFamily, NetworkProtocol protocol, int port, CharString address)
		{
			return new Win32Address(addressFamily, protocol, port, address);
		}
	};

	NetworkContext* NetworkContext::Create()
	{
		return new Win32Context();
	}
}