// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * Customized UAbilitySystemGlobals Class
 * Reason: 这样就通过自定义的 Global 类来访问或者创建 自定的其他重要类(比如 Custom GEContext，因为在创建 Context 而使用的 MakeEffectContext 会自动调用系统的 AbilitySystemGlobals)
 * Tips: 想让系统自己调用这个 Custom Global 函数，需要修改配置文件 DefaultGame.ini, 详见视频 14-6
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	/**
	 * new 一个新的 AuraGameplayEffectContext 在堆并且 return
	 * Tips: 一般会在 MakeEffectContext 中被调用
	 * @return FAuraGameplayEffectContext
	 */
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
