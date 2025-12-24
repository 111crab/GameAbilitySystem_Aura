// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"


UINTERFACE(MinimalAPI,BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 * 
 */

class AURA_API ICombatInterface
{
	GENERATED_BODY()

public:

	
	/**
	 * Convenient to get self level that do not need implement details
	 * @return Level
	 */
	virtual int32 GetPlayerLevel();

	
	/**
	 * 获得武器尖端的 FVector 位置，需 Implement
	 * @return 
	 */
	virtual FVector GetCombatSocketLocation();

	
	/**
	 * Pass the Facing Target for MotionWarping (Anim Montage)
	 * @param Target 
	 */
	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable) // "BlueprintImplementableEvent" not use virtual
	void UpdateFacingTarget(const FVector& Target);
	
};
