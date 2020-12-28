#pragma once

#include "Interactive.h"

#include "Interactor.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractorInterface : public UInterface
{
	GENERATED_BODY()
};


class IInteractorInterface
{    
	GENERATED_BODY()

public:
	virtual bool WantToInteract(IInteractiveInterface* Interactive) const = 0;
	virtual void Interact(IInteractiveInterface* Interactive) = 0;
};
