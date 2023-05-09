#include "HGUNet.h"

void SocketUtil::ReportError(const char* inOperationDesc)
{
	LOG("Error: %hs", inOperationDesc);
}

int SocketUtil::GetLastError()
{
	return errno;
}

TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily inFamily)
{
	SOCKET s = socket(inFamily, SOCK_STREAM, IPPROTO_TCP);
	
	if (s != INVALID_SOCKET)
	{
		return TCPSocketPtr(new TCPSocket(s));
	}
	else
	{
		ReportError("SocketUtil::CreateTCPSocket");
		return nullptr;
	}
}

int SocketUtil::Select(const vector< TCPSocketPtr >* inReadSet,
					   vector< TCPSocketPtr >* outReadSet,
					   const vector< TCPSocketPtr >* inWriteSet,
					   vector< TCPSocketPtr >* outWriteSet,
					   const vector< TCPSocketPtr >* inExceptSet,
					   vector< TCPSocketPtr >* outExceptSet)
{
	//build up some sets from our vectors
	fd_set read, write, except;
	
	int nfds = 0;
	
	fd_set *readPtr = FillSetFromVector(read, inReadSet, nfds);
	fd_set *writePtr = FillSetFromVector(write, inWriteSet, nfds);
	fd_set *exceptPtr = FillSetFromVector(except, inExceptSet, nfds);
	
	int toRet = select(nfds + 1, readPtr, writePtr, exceptPtr, nullptr);
	
	if (toRet > 0)
	{
		FillVectorFromSet(outReadSet, inReadSet, read);
		FillVectorFromSet(outWriteSet, inWriteSet, write);
		FillVectorFromSet(outExceptSet, inExceptSet, except);
	}
	return toRet;
}

fd_set* SocketUtil::FillSetFromVector(fd_set& outSet, const vector< TCPSocketPtr >* inSockets, int& ioNaxNfds)
{
	if (inSockets)
	{
		FD_ZERO(&outSet);
		
		for(const TCPSocketPtr& socket : *inSockets)
		{
			FD_SET(socket->mSocket, &outSet);
			ioNaxNfds = std::max(ioNaxNfds, socket->mSocket);
		}
		return &outSet;
	}
	else
	{
		return nullptr;
	}
}

void SocketUtil::FillVectorFromSet(vector<TCPSocketPtr>* outSockets, const vector<TCPSocketPtr>* inSockets, const fd_set& inSet)
{
	if (inSockets && outSockets)
	{
		outSockets->clear();
		for (const TCPSocketPtr& socket : *inSockets)
		{
			if (FD_ISSET(socket->mSocket, &inSet))
			{
				outSockets->push_back(socket);
			}
		}
	}
}


