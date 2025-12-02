// Copyright CRABOI

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:

	
	/**
	 * 作为静态函数创建 AbilityTask
	 * 小知识：meta（DisplayName = “在蓝图中所需的名字”，HidePin = “需隐藏的参数”， DefaultToSelf = “默认初值为Self or This 的参数”，BlueprintInternalUseOnly 仅在函数内部使用）
	 * @param OwningAbility 需要传入自己属于哪个 GA
	 * @return 返回自己这个 Task 的指针
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",meta = (DisplayName = "TargetDataUnderMouse",HidePin = "OwningAbility", DefaultToSelf = "OwningAbility",BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	// 作为蓝图中的输入 Pin，用委托的形式配置的。
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:

	/**
	 * 当前 Task 的具体操作可写入，宏自调用
	 */
	virtual void Activate() override;

	/**
	 * Client 发送数据给 Server, 含有 预测-验证 处理	
	 */
	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
