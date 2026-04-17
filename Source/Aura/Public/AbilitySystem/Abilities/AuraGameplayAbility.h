// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	// 初始 Ability 的 Tag 设置
	UPROPERTY(EditDefaultsOnly,Category= "Input")
	FGameplayTag StartupInputTag;

	
	
};
