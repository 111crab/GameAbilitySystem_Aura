// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()


	
protected:
	// WidgetController主要需要关注的四个地方的数据，故而创建引用。
	UPROPERTY(BlueprintReadOnly, Category= "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category= "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category= "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category= "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
