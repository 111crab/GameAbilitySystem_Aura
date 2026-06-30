// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* Asset Tags */)


/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	void AbilityActorInfoSet();
	
	FEffectAssetTags EffectAssetTags;

	/**
	 * 真正为角色添加初始 GA，通俗的说：就是设置角色可以使用哪些技能，放入一个待激活的技能序列。
	 * @param StartupAbilities 来自于 Character 在编辑器中配置的 StartupAbilities-Array
	 */
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	/*
	 * 按键触发的真正函数
	 */
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	
protected:

	
	/**
	 * 应用 GE 到自己的回调函数，由于委托来自宏，故而设置需要的输入。
	 * 注：加入 Client 前缀，为 ClientRPC 用法，委托仍在 Server Call 但是也会在客户端执行。此函数实现的时候需加后缀 " _Implementation "(目的： 为了在 Client 端操作的玩家也能执行拾取物品的 UI 反馈)
	 * @param AbilitySystemComponent 
	 * @param EffectSpec 
	 * @param ActiveEffectHandle 
	 */
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
