// Copyright CRABOI


#include "AbilitySystem/AuraAbilitySystemGlobals.h"

#include "AuraAbilityTypes.h"

FGameplayEffectContext* UAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	// new = “请在内存里帮我实例化一个我的自定义 Context，在堆栈中还划分了一些空间” 
	return new FAuraGameplayEffectContext();
}
