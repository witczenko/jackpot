#include "JackPot.h" 

int main(int argc, char* argv[])
{
	std::unique_ptr<JackPot> jackPotGame = std::make_unique<JackPot>(argc, argv);

	if (jackPotGame->IsInitialized()) jackPotGame->Run();
	jackPotGame->DumpLogsToStream();

	return 0;
}