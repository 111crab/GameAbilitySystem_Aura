// Copyright CRABOI


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Math/UnitConversion.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	// 此委托类型仅在 Sever 端 Call
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}


void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities) // 遍历角色的初始 GA 列表（在编辑器中定义的）
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1); // 为 GA 创建 Spec
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))  // 如果这个 Spec 对应的 GA 属于 AuraGA 类
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag); // 为 GA 建立 GA-Tag 映射
			
			// TODO: 暂时理解为能被触发了，所以后续能在 GetActivatableAbilities 中找到
			GiveAbility(AbilitySpec); 
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return; // 安全检查：无效标签直接返回

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) // 获取所有已授予的 GASpec
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) // 如果当前 InputTag 所代表的按键确实可以触发我这个 Spec 的 GA
		{
			// 算是一种通知吧: 嘿！我这个 Spec 对应的按键已经被 Pressed
			// Spec 自己也有一个成员变量来存储自己是否被 Pressed，而且这个函数里面自带一个虚函数 InputPressed 被调用，我们可以重写这个函数搞点自定义事件（当我们想在 Pressed 之后触发些什么）。
			AbilitySpecInputPressed(AbilitySpec); 
			if (!AbilitySpec.IsActive()) 
			{
				TryActivateAbility(AbilitySpec.Handle); // 激活取消都需要依靠 Handle
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
	
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{

	FGameplayTagContainer TagContainer;

	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
	
}
