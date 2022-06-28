#include "Client.h"

int main(int argc, char** argv)
{
	Client client(argv[0]);
	if(client.isInit())
		return client.deltaLoop();
    return 0;
}
