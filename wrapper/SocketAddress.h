class SocketAddress
{
public:
	SocketAddress(uint32_t inAddress, uint16_t inPort)
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetIP4Ref() = inAddress;
		GetAsSockAddrIn()->sin_port = htons(inPort);
	}

	SocketAddress(const struct sockaddr_in& inSockAddr)
	{
		memcpy(&mSockAddr, &inSockAddr, sizeof(struct sockaddr_in));
	}

	SocketAddress()
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetIP4Ref() = INADDR_ANY;
		GetAsSockAddrIn()->sin_port = 0;
	}

	bool operator==(const SocketAddress& inOther) const
	{
		return (mSockAddr.sin_family == AF_INET &&
				 GetAsSockAddrIn()->sin_port == inOther.GetAsSockAddrIn()->sin_port) &&
				 (GetIP4Ref() == inOther.GetIP4Ref());
	}

	size_t GetHash() const
	{
		return (GetIP4Ref()) |
				((static_cast<uint32_t>(GetAsSockAddrIn()->sin_port)) << 13) |
				mSockAddr.sin_family;
	}

	uint32_t					GetSize()			const	{ return sizeof(struct sockaddr_in); }
	string						ToString()			const;

private:
	friend class UDPSocket;
	friend class TCPSocket;

	struct sockaddr_in mSockAddr;

	uint32_t&					GetIP4Ref()					{ return GetAsSockAddrIn()->sin_addr.s_addr; }
	const uint32_t&				GetIP4Ref()			const	{ return GetAsSockAddrIn()->sin_addr.s_addr; }

	struct sockaddr_in*			GetAsSockAddrIn()			{ return reinterpret_cast<sockaddr_in*>( &mSockAddr ); }
	const struct sockaddr_in*	GetAsSockAddrIn()	const	{ return reinterpret_cast<const sockaddr_in*>( &mSockAddr ); }

};

typedef shared_ptr<SocketAddress> SocketAddressPtr;
