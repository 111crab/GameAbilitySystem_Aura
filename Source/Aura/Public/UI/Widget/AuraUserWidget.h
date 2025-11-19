// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	/**
	 * Set WidgetController and call WidgetControllerSet()
	 * @param InWidgetController 
	 */
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:

	
	/**
	 *  在正确 Set WidgetController 之后，立刻调用，完成 what to do
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
};
