// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"

#include "AuraCharacterBase.generated.h"
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter , public IAbilitySystemInterface , public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const{ return AttributeSet;}
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category= "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	// Using to default setting various attributes 
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category= "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category= "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category= "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;



	/**
	 * ApplyGameplayEffectSpecToTarget()的通用写法，旨在实现任意 GE 对 Target，在这Target就是自己
	 * @param GameplayEffectClass 
	 * @param Level 
	 */
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass,float Level) const;

	

	/**
	 * 使用 GE 初始化属性，或者更新派生属性
	 */
	void InitializedDefaultAttributes() const;

};
