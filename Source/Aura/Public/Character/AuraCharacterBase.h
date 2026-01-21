// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"

#include "AuraCharacterBase.generated.h"
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage;
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter , public IAbilitySystemInterface , public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	
	/**
	 * 获取 Character（this） 的 ASC，因为 ASC 存在当前类的 Protected
	 * @return ASC
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/**
	 * 获取 Character（this） 的 AS，因为 AS 存在当前类的 Protected
	 * @return AS
	 */
	UAttributeSet* GetAttributeSet() const{ return AttributeSet;}
	
	/**
	 * CombatInterface接口实现
	 * @return 受击蒙太奇
	 */
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category= "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	// 武器尖端名称，用于返回其 FVector 位置
	UPROPERTY(EditAnywhere,Category= "Combat")
	FName WeaponTipSocketName;

	/**
	 * 获得武器尖端的 FVector 位置
	 * @return 
	 */
	virtual FVector GetCombatSocketLocation() override;
	

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
	virtual void InitializedDefaultAttributes() const;


	/**
	 * 注册输入-技能的绑定。真正注册操作应在 ASC 中的 AddCharacterAbilities，故而 call
	 * 调用时机：Character 被 PossessedBy 的时候
	 */
	void AddCharacterAbilities() ;
private:

	// 角色初始 GA-Array，编辑器中自配置
	UPROPERTY(EditAnywhere,Category= "Attributes")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	// 角色受击蒙太奇
	UPROPERTY(EditDefaultsOnly, Category= "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

};
