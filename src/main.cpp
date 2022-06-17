#include "Client.h"

int main(int argc, char** argv)
{
	Client client;
	if(client.Init(argv[0]))
		client.DeltaLoop();
    return 0;
}
