// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"
class AAuraProjectile;
/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:

	/**
	 * 重写函数：激活 GA 的内部函数
	 * @param Handle 
	 * @param ActorInfo 
	 * @param ActivationInfo 
	 * @param TriggerEventData 
	 */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// 投射物类
	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	TSubclassOf<AAuraProjectile> ProjectileClass;
};
