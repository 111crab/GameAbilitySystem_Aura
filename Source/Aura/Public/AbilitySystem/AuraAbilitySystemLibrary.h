// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 蓝图可用的静态方法图书馆
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary | WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary | WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	/**
	 * 为角色赋予初始的 Attributes
	 * @param WorldContextObject 
	 * @param CharacterClass 
	 * @param Level 
	 * @param ASC 
	 */
	UFUNCTION(BlueprintCallable ,Category= "AuraAbilitySystemLibrary | CharacterClassDefault")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	/**
	 * 为角色赋予初始的 GA
	 * @param WorldContextObject 
	 * @param ASC 
	 */
	UFUNCTION(BlueprintCallable ,Category= "AuraAbilitySystemLibrary | CharacterClassDefault")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable ,Category= "AuraAbilitySystemLibrary | CharacterClassDefault")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	/* The Getter or Setter of bBlockedHit */
	UFUNCTION(BlueprintPure, Category= "AuraAbilitySystemLibrary | GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	/* The Getter or Setter of bCriticalHit */
	UFUNCTION(BlueprintPure, Category= "AuraAbilitySystemLibrary | GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	/* The Getter or Setter of bBlockedHit */
	UFUNCTION(BlueprintCallable, Category= "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	/* The Getter or Setter of bCriticalHit */
	UFUNCTION(BlueprintCallable, Category= "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
};
