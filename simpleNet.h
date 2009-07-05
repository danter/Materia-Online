#ifndef __SIMPLE_NET
#define __SIMPLE_NET

#include <string>

#include "libtnl/tnl.h"
#include "libtnl/tnlEventConnection.h"
#include "libtnl/tnlNetInterface.h"
#include "libtnl/tnlRPC.h"

using namespace TNL; // make sure we can simply use the TNL classes.


class QuitManager
{
public:
	static void Shutdown();
	static bool WaitingForShutdown();

private:
	static bool mQuitStatus;
};


class StringContainer
{
public:
	static void setString(const char *tmpString);
	static const char* getString();

private:
	static std::string stringCon;
};


class SimpleEventConnection : public EventConnection
{
	typedef EventConnection Parent;

public:
//	bool gQuit; // a flag used when the client wants to quit.

//	SimpleEventConnection() { gQuit = false; }

	// Let the network system know this is a valid network connection.
	TNL_DECLARE_NETCONNECTION(SimpleEventConnection);

	// declare the client to server message
	TNL_DECLARE_RPC(rpcMessageClientToServer, (StringPtr theMessageString));

	// declare the server to client message
	TNL_DECLARE_RPC(rpcMessageServerToClient, (StringPtr theMessageString));

};


#endif
