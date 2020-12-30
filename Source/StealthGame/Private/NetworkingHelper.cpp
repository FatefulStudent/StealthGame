#include "NetworkingHelper.h"

bool FNetworkingHelper::HasCosmetics(AActor* Actor)
{
	return Actor->GetNetMode() != NM_DedicatedServer;
}

bool FNetworkingHelper::IsAutonomousClient(AActor* Actor)
{
	return Actor->GetLocalRole() == ROLE_AutonomousProxy; 
}
