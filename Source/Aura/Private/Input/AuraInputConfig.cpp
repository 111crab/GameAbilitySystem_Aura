// Copyright CRABOI


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FAuraInputAction Action : AbilityInputActions)
	{
		if (Action.InputAction && InputTag == Action.InputTag) return Action.InputAction;
	}

	if (bLogNotFound)
	{
		// ToString(), GetNameSafe 均返回 FString 是不能被 %s 认同的，所以要在前面加上 * 取出底层的 TCHAR* C字符串指针
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"),*InputTag.ToString(), *GetNameSafe(this))
	}
	return nullptr;
}
