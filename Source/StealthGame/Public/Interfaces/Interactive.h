#pragma once

#include "Interactive.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractiveInterface : public UInterface
{
	GENERATED_BODY()
};


class IInteractiveInterface
{    
	GENERATED_BODY()

public:
	virtual bool IsAvailableForInteraction() const = 0;
	virtual void OnSuccessfulInteraction() = 0;
};
