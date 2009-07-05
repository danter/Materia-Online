/*
	if(argc != 4)
	{
		printf("usage: %s <-server|-client> <connectAddress> <replyMsg>", argv[0]);
		return 1;
	}
	bool isClient = !strcmp(argv[1], "-client");

	RefPtr<NetInterface> theNetInterface;

	StringContainer::setString(argv[3]);

	// convert the command-line address into TNL address form
	Address cmdAddress(argv[2]);

	if(isClient)
	{
		Address bindAddress(IPProtocol, Address::Any, 0);

		// create a new NetInterface bound to any interface, any port (0)
		theNetInterface = new NetInterface(bindAddress);

		// create a new SimpleEventConnection and tell it to connect to the
		// server at cmdAddress.
		SimpleEventConnection *newConnection = new SimpleEventConnection;
		newConnection->connect(theNetInterface, cmdAddress);

		// post an RPC, to be executed when the connection is established
		newConnection->rpcMessageClientToServer(StringContainer::getString());
	}
	else
	{
		// create a server net interface, bound to the cmdAddress
		theNetInterface = new NetInterface(cmdAddress);

		// notify the NetInterface that it can allow connections
		theNetInterface->setAllowsConnections(true);
	}

	// now just loop, processing incoming packets and sending outgoing packets
	// until the global quit flag is set.
	while(!QuitManager::WaitingForShutdown())
	{
		theNetInterface->checkIncomingPackets();
		theNetInterface->processConnections();
		Platform::sleep(1);
	}

*/
