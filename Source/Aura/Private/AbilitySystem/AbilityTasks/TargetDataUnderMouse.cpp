// Copyright CRABOI


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "Player/AuraPlayerController.h"


UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{

	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled) // 是本地控制，把数据传给服务端，并且本地也广播一下
	{
		SendMouseCursorData();
	}
	else // 不是本地控制，则监听 TargetData
	{
		// SpecHandle 和 ActivationPredictionKey 是委托 AbilityTargetDataSetDelegate 的传递参数，它们旨在唯一标识此次的 TargetData 是哪个 GA ，哪次请求
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		
		// 服务端绑定 [CallBackFunc]OnTargetDataReplicatedCallback 到 Delegate：AbilityTargetDataSetDelegate
		// [Delegate]AbilityTargetDataSetDelegate::Broadcast 时机：TargetData 送达服务端
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		// 正常流程：服务端需要绑定委托 -> 客户端 TargetData 到达 -> 回调函数。
		// 但是绑定和客户端信息到达，这两个事件是异步的，无法确认顺序，所以需要下文控制
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey); // AbilityTargetDataSetDelegate 是否收到客户端的打包数据
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData(); // 没收到过则等待
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	// PredictionKey 用于标识 Client 的每次操作请求和服务器的处理结果
	// 这个作用域的一次性预测的 PredictionKey 压入当前 ASC
	FScopedPredictionWindow ScopePrediction(AbilitySystemComponent.Get());

	// 获取并记录 FHitResult 数据
	AAuraPlayerController* PC = Cast<AAuraPlayerController>(Ability->GetCurrentActorInfo()->PlayerController.Get());
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	// 目的：层层创建，包装一个 FGameplayAbilityTargetDataHandle 类型的 DataHandle
	// DataHandle 其实就是 TargetData
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);

	// 客户端发送数据给服务端的实际操作
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(), // 这次 Ability 激活的唯一 Key
		DataHandle, // GameplayAbilityTargetData 的句柄
		FGameplayTag(), // ConfirmTag
		AbilitySystemComponent->ScopedPredictionKey // ScopedPredictionWindow 压入 ASC 的 PredictionKey
		);

	
	if (ShouldBroadcastAbilityTaskDelegates()) // 并不是每次连续点击的请求都会被承认，只有被服务端确认过的才能广播
	{
		// 触发自定义委托，给订阅者使用。传递一个 TargetData
		ValidData.Broadcast(DataHandle);
	}
	
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	// 清楚特定 SpecHandle 和 PredictionKey 对应的缓存（初衷用于 !bCalledDelegate 的情况）
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	
	if (ShouldBroadcastAbilityTaskDelegates()) 
	{
		// 触发自定义委托，给订阅者使用。传递一个 TargetData
		ValidData.Broadcast(DataHandle);
	}
}
