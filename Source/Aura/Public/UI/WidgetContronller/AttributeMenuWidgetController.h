// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "UI/WidgetContronller/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"


/*
 * 小知识
 * 这里的 const FAuraAttributeInfo& 加了引用符号，因为这个是其他类的结构体，只加前向声明的话会定义不完整。
 * 如果你是 #include "AbilitySystem/Data/AttributeInfo.h"  与前向声明不同，你可以不加 "&" 引用符号
 * 你的命名 Info 只是宏定义时的参数名，和你C++代码中的变量名没有关系。
 */
struct FAuraAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category= "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:

	/**
	 * 用于广播 AuraAttributeInfo 的减冗余函数
	 * @param AttributeTag 
	 * @param Attribute 
	 */
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
