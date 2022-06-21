#include "Engine.h"

int main(int argc, char** argv)
{
	Engine engine;
	if(engine.Init(argv[0]))
		engine.DeltaLoop();
    return 0;
}
