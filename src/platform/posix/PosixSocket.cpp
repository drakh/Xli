#include <XliPlatform/Socket.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>

#define SOCKET_ERROR -1

namespace Xli
{
	class PosixNetworkAddress: public NetworkAddress
	{
	public:
		NetworkAddressFamily af;
		NetworkProtocol protocol;

		int linuxprotocol;
		int linuxtype;

		sockaddr_storage addr;
		sockaddr_in* inAddr;
		sockaddr_in6* in6Addr;

		int addrSize;

		PosixNetworkAddress(NetworkAddressFamily af, NetworkProtocol p, int port, CharString a)
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
				linuxprotocol = IPPROTO_TCP;
				linuxtype = SOCK_STREAM;
				break;
			case NetworkProtocolUDP:
				linuxprotocol = IPPROTO_UDP;
				linuxtype = SOCK_DGRAM;
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

		PosixNetworkAddress(NetworkProtocol p, sockaddr_storage a)
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
				linuxprotocol = IPPROTO_TCP;
				linuxtype = SOCK_STREAM;
				break;
			case NetworkProtocolUDP:
				linuxprotocol = IPPROTO_UDP;
				linuxtype = SOCK_DGRAM;
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

	int handleErrno()
	{
		switch (errno)
		{
		/*case WSANOTINITIALISED:	XLI_THROW("Unable to read data from socket: WSANOTINITIALISED");
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
		case WSAECONNRESET:	throw ConnectionLostException();*/
        default:
            XLI_THROW("Unable to read data from socket: ");
		}
	}

	class PosixSocketStream: public Stream
	{
		int socket;

	public:
		PosixSocketStream(int socket)
		{
			this->socket = socket;
		}
		virtual bool CanRead() const { return true; }
		virtual bool CanWrite() const { return true; }
		
		virtual int Read(void* data, int elementSize, int elementCount)
		{
			int count = recv(socket, (char*)data, elementSize*elementCount, 0);
			if (count == SOCKET_ERROR) return handleErrno();
			return count / elementSize;
		}
		virtual int Write(const void* data, int elementSize, int elementCount)
		{
			int count = send(socket, (char*)data, elementSize*elementCount, 0);
			if (count == SOCKET_ERROR) return handleErrno();
			return count / elementSize;
		}
	};

	class PosixSocket: public Socket
	{
		int sock;
		PosixNetworkAddress* remoteAddress;
		PosixNetworkAddress* localAddress;
		bool blockMode;
		Stream* stream;

	public:
		PosixSocket(PosixNetworkAddress* localAddress)
		{
			this->stream = 0;
			this->localAddress = localAddress;
			sock = socket(localAddress->inAddr->sin_family, localAddress->linuxtype, localAddress->linuxprotocol);
			if (sock == -1) XLI_THROW("Unable to create socket");
			if (bind(sock, (sockaddr*)&localAddress->addr, sizeof(localAddress->addr)) == SOCKET_ERROR) XLI_THROW("Unable to bind socket");

			SetBlockingMode(true);
		}

		PosixSocket(PosixNetworkAddress* localAddress, PosixNetworkAddress* remoteAddress, int sock)
		{
			this->stream = 0;
			this->localAddress = localAddress;
			this->remoteAddress = remoteAddress;
			this->sock = sock;
			SetBlockingMode(true);
		}

		~PosixSocket()
		{
			close(sock);
			if (stream) stream->Release();
		}

		NetworkAddress* ReceiveFrom(void* data, int* byteCount)
		{
			if (localAddress->protocol != NetworkProtocolUDP) XLI_THROW("ReceiveFrom(): Only valid for UDP sockets");
			sockaddr_storage srcAddr;
			socklen_t srcAddrSize = sizeof(srcAddr);
			int count = recvfrom(sock, (char*)data, *byteCount, 0, (sockaddr*)&srcAddr, &srcAddrSize);
			if (count == SOCKET_ERROR) if (handleErrno() == 0) count = 0;
			*byteCount = count;
			return new PosixNetworkAddress(NetworkProtocolUDP, srcAddr);
		}

		int SendTo(NetworkAddress* address, void* data, int byteCount)
		{
			int count = sendto(sock, (char*)data, byteCount, 0, (sockaddr*)&((PosixNetworkAddress*)address)->addr, sizeof(sockaddr_storage));
			if (count == SOCKET_ERROR) return handleErrno();
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
			socklen_t addrSize = sizeof(sockaddr_storage);
			int client = accept(sock, (sockaddr*)&addr, &addrSize);
			if (client == -1) return 0;

			PosixNetworkAddress* remoteAddress = new PosixNetworkAddress(NetworkProtocolTCP, addr);
			return new PosixSocket(localAddress, remoteAddress, client);
		}

		void Connect(NetworkAddress* address)
		{
			remoteAddress = (PosixNetworkAddress*)address;
			if (connect(sock, (sockaddr*)&remoteAddress->addr, sizeof(remoteAddress->addr)) == SOCKET_ERROR)
			{
				XLI_THROW("Unable to connect");
			}
		}

		bool IsConnected()
		{
			fd_set set;
			/*set.fd_array[0] = sock;
			set.fd_count = 1;*/
            FD_SET(sock, &set);
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
			u_long block = mode? 0 : 1; // | O_NONBLOCK
            if (fcntl(sock, F_SETFL, block) == SOCKET_ERROR) XLI_THROW("Unable to set blocking mode on socket");
			this->blockMode = mode;
		}

		bool GetBlockingMode()
		{
			return blockMode;
		}

		Stream* GetStream()
		{
			if (localAddress->protocol == NetworkProtocolUDP) XLI_THROW("GetStream(): Only valid for TCP sockets");
			if (stream == 0) stream = new PosixSocketStream(sock);
			return stream;
		}

	};

	class PosixContext: public NetworkContext
	{
	public:
		Socket* CreateSocket(NetworkAddressFamily addressFamily, NetworkProtocol protocol, int port)
		{
			return new PosixSocket(new PosixNetworkAddress(addressFamily, protocol, port, ""));
		}
		NetworkAddress* CreateAddress(NetworkAddressFamily addressFamily, NetworkProtocol protocol, int port, CharString address)
		{
			return new PosixNetworkAddress(addressFamily, protocol, port, address);
		}
	};

	NetworkContext* NetworkContext::Create()
	{
		return new PosixContext();
	}
}
