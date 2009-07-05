#include <cstdio>
#include "simpleNet.h"
/*
QuitManager* QuitManager::mInstance = 0;
QuitManager* QuitManager::GetInstance()
{
	if(mInstance == 0) {
		mInstance = new QuitManager();
		mInstance->QuitStatus = false;
	}

	return mInstance;
}*/

bool QuitManager::mQuitStatus = false;

void QuitManager::Shutdown()
{
	mQuitStatus = true;
}

bool QuitManager::WaitingForShutdown()
{
	return mQuitStatus;
}

std::string StringContainer::stringCon = "";

void StringContainer::setString(const char *tmpString)
{
	StringContainer::stringCon = tmpString;
}

const char *StringContainer::getString()
{
	return StringContainer::stringCon.c_str();
}


TNL_IMPLEMENT_NETCONNECTION(SimpleEventConnection, NetClassGroupGame, true);


TNL_IMPLEMENT_RPC(SimpleEventConnection, rpcMessageClientToServer, (StringPtr messageString), (messageString),
NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirClientToServer, 0)
{
	const char *sTmp = messageString.getString();

	// display the message the client sent
	printf("Got message from client: %s\n", sTmp);

	// send a hello world back to the client.
	rpcMessageServerToClient(StringContainer::getString());

	if(!strcmp(sTmp, "-quit"))
		QuitManager::Shutdown();
}


TNL_IMPLEMENT_RPC(SimpleEventConnection, rpcMessageServerToClient, (StringPtr messageString), (messageString),
NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirServerToClient, 0)
{
	// display the message the server sent
	printf("Got a message from server: %s\n", messageString.getString());

	// once the client has heard back from the server, it should quit.
	QuitManager::Shutdown();
}

