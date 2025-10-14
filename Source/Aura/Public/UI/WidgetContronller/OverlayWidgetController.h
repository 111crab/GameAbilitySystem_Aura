// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetContronller/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature,float,NewHealth); // 动态多播委托：只有动态的才能蓝图or编辑器可见，给美术用。
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature,float,NewMaxHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature,float,NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature,float,NewMaxMana);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable) // BluprintType 意味着可以给蓝图中的变量做类型。 Blueprintable 意味着可以被蓝图继承。
class AURA_API UOverlayWidgetController : public UAuraWidgetController 
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnManaChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;

protected:
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;

	void ManaChanged(const FOnAttributeChangeData& Data) const;
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;


	
};
