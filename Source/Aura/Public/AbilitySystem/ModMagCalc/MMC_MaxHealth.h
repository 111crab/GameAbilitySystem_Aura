// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	
	UMMC_MaxHealth();

	/**
	 * 实现自定义的复杂计算
	 * @param Spec 可以获取到 Spec 所以功能强大
	 * @return 
	 */
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	/**
	 * 定义了 GE 在运行过程中需要捕获的属性，具体定义了：（以下选择在构造函数中绑定）
	 * 要捕获哪个属性（Attribute）
	 * 从哪个对象捕获（Source 还是 Target）
	 * 何时捕获（SnapShot）
	 */
	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
