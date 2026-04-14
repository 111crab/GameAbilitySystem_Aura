
#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

/*
 * 自定义的 GEContext 结构
 * Reason: 原本的 FGameplayEffectContext 无法存储我们想要的一些属性内容（比如是否暴击）
 */
USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	
	/* The Getter or Setter of member variable */
	bool IsCriticalHit() const{ return bIsCriticalHit; }
	bool IsBlockedHit() const{ return bIsBlockedHit; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bIsInBlockedHit) { bIsBlockedHit = bIsInBlockedHit;}
	/* The Getter or Setter of member variable */

	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}


	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
protected:

	UPROPERTY();
	bool bIsBlockedHit = false;
	
	UPROPERTY();
	bool bIsCriticalHit = false;
}; 

/** type traits to cover the custom aspects of a script struct **/
/* 告诉你 FAuraGameplayEffectContext 有哪些特征（存在于 TStructOpsTypeTraitsBase2 中）*/
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
