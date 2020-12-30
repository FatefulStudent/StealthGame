#include "NetworkingHelper.h"

bool FNetworkingHelper::HasCosmetics()
{
	return !IsRunningDedicatedServer();
}

bool FNetworkingHelper::IsAutonomousClient(AActor* Actor)
{
	return Actor->GetLocalRole() == ROLE_AutonomousProxy; 
}
