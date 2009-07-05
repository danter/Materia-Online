#include "libtnl\tnl.h"
#include "libtnl\tnlEventConnection.h"
#include "libtnl\tnlNetInterface.h"
#include "libtnl\tnlRPC.h"
#include <stdio.h>

bool gQuit = false; // a flag used when the client wants to quit.

using namespace TNL; // make sure we can simply use the TNL classes.

class SimpleEventConnection : public EventConnection
{
    typedef EventConnection Parent;

public:
    // Let the network system know this is a valid network connection.
    TNL_DECLARE_NETCONNECTION(SimpleEventConnection);

    // declare the client to server message
    TNL_DECLARE_RPC(rpcMessageClientToServer, (const char *theMessageString));

    // declare the server to client message
    TNL_DECLARE_RPC(rpcMessageServerToClient, (const char *theMessageString));
};


TNL_IMPLEMENT_NETCONNECTION(SimpleEventConnection, NetClassGroupGame, true);

/*
TNL_IMPLEMENT_RPC(
			className,
			name,
			args,
			argNames,
			groupMask,
			guaranteeType,
			eventDirection,
			rpcVersion )
*/
/*
TNL_IMPLEMENT_RPC(SimpleEventConnection, rpcMessageClientToServer,
				  (StringPtr messageString), (messageString),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirClientToServer, 0)
{
    // display the message the client sent
    printf("Got message from client: ");
	printf(messageString);

}
*/

TNL_IMPLEMENT_RPC(SimpleEventConnection, rpcMessageClientToServer,
				(StringPtr messageString), (messageString),
				NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirClientToServer, 0)
{
    // display the message the client sent
    printf("Got message from client: %s\n", messageString);
    // send a hello world back to the client.
    rpcMessageServerToClient("Hello, World!");
}


TNL_IMPLEMENT_RPC(SimpleEventConnection, rpcMessageServerToClient,
				(StringPtr messageString), (messageString),
				NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirServerToClient, 0)
{
    // display the message the server sent
    printf("Got a message from server: %s\n", messageString);

    // once the client has heard back from the server, it should quit.
    gQuit = true;
}
