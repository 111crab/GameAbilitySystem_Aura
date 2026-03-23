// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetContronller/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase , public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();
	
	/**	Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/**	End Enemy Interface */

	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
	/** end Combat Interface */

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	
	/**
	 * 受击 CallBack 函数
	 * @param CallbackTag 根据的特定 GameplayTag 
	 * @param NewCount 此 Tag 数量
	 */
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	// bool:是否受击
	UPROPERTY(BlueprintReadOnly , Category = "Cambat")
	bool bHitReacting = false;

	// 基础步行速度
	UPROPERTY(BlueprintReadOnly , Category = "Cambat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere ,BlueprintReadOnly , Category = "Cambat")
	float LifeSpan = 5.f;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void InitAbilityActorInfo() override;

	virtual void InitializedDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior; // 只是先给个默认，在编辑器中改

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;


	
};
