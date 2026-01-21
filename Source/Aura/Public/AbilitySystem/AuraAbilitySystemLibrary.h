// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

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
};
