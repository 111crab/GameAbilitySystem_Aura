// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetContronller/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:

	/*UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);*/
	
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

protected:
	

};
