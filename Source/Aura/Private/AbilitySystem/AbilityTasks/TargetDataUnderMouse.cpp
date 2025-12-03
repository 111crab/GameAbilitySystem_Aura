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
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();

		// 因为服务端收到数据，和执行 Activate()中触发 AbilityTargetDataSetDelegate 绑定 CallBack 有先后，这个先后是无法确定的。而想触发 Delegate 必须要先绑定后收到数据
		// 故而有以下的情况操作
		// 若收到 Client 的消息，则调用 CallBack 函数
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey); // 判断是否已经调用过 AbilityTargetDataSetDelegate
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData(); // 没调用过则等待
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	// PredictionKey 用于标识 Client 的每次操作请求和服务器的处理结果
	// 这个作用域的一次性预测的 PredictionKey 压入当前 ASC
	FScopedPredictionWindow ScopePrediction(AbilitySystemComponent.Get());
	
	AAuraPlayerController* PC = Cast<AAuraPlayerController>(Ability->GetCurrentActorInfo()->PlayerController.Get());
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);
	
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(), // 这次 Ability 激活的唯一 Key
		DataHandle, // GameplayAbilityTargetData 的句柄
		FGameplayTag(), // ConfirmTag
		AbilitySystemComponent->ScopedPredictionKey // ScopedPredictionWindow 压入 ASC 的 PredictionKey
		);

	
	if (ShouldBroadcastAbilityTaskDelegates()) // 并不是每次连续点击的请求都会被承认，只有被服务端确认过的才能广播
	{
		ValidData.Broadcast(DataHandle);
	}
	
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	// 清楚特定 SpecHandle 和 PredictionKey 对应的缓存（初衷用于 ！bCalledDelegate 的情况）
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates()) 
	{
		// 收到了客户端的 TargetData，你也得广播给服务器端的 FMouseTargetDataSignature 执行，不然还是不同步
		ValidData.Broadcast(DataHandle);
	}
}
