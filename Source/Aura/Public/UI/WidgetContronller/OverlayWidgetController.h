// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetContronller/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"
class UAuraUserWidget;
// 用于 UI 显示的数据表结构
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase  // 改行表继承自 FTableRowBase: 行数据表的基类
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

class UAuraUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewHealth); // 动态多播委托：只有动态的才能蓝图or编辑器可见，给美术用。

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature,FUIWidgetRow, Row);

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
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly , Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* Datatable,const FGameplayTag& Tag );
	
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* Datatable, const FGameplayTag& Tag)
{
	return Datatable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
