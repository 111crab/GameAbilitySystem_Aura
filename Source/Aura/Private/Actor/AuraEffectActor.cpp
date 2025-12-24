// Copyright CRABOI


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Character/AuraCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"


AAuraEffectActor::AAuraEffectActor()
{

	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootComponent"));
}




void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();


	
}


/**
 * FUNC:  应用GE给TargetActor
 * 
 * @param TargetActor 目标Actor
 * @param GameplayEffectClass GE的类
 */
void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// 1. 快捷安全的拿到 TargetActor 的ASC
	UAbilitySystemComponent* TargetASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (nullptr == TargetASC) return; // 如果这个 Actor不具备 ASC 功能则啥都不发生
	

	// 2. TargetASC调用 ApplyGameplayEffectSpecToSelf 给自己施加GE
	
	check(GameplayEffectClass);  // 检查蓝图里填GE了没->给成员函数（eg. InstantEffect）填，再配到 GameplayEffectClass
	
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext(); // 创建上下文 Handle，作为 MakeOutgoingSpec 的参数。
	
	EffectContextHandle.AddSourceObject(this); // 记录GE的来源，this就是指这个EffectActor就是来源本身。
	
	const FGameplayEffectSpecHandle EffectSpecHandle =  TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle); // 创建 EffectSpecHandle，作为 ApplyGameplayEffectSpecToSelf 参数。
	
	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle =  TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get()); // 这里需要一个 const引用 作为参数。从 EffectSpecHandle 的 wrap 中取 Data（TSharePtr也是一种 wrap） 用 get（）取裸指针。再 ‘*’ 解引用符合入参类型。

	// 3. 存储 ActiveGameplayEffectHandle 句柄，为移除无限效果做准备。
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def->DurationPolicy == EGameplayEffectDurationType::Infinite; // 看当前GE是否为 Infinite 类型
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) // 如果是 Infinite 类型，而且也打算移除。（不想移除存个 P）
	{
		ActiveEffectHandles.Add(ActiveGameplayEffectHandle , TargetASC); // 存储映射（自定义映射）
		
	}
}
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) // 移除 Infinite 逻辑
	{
		
		UAbilitySystemComponent* TargetASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor); // 拿到ASC
		if (!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove; // 在 for 循环中不能直接移除 ActiveEffectHandles 的记录（因为在循环它本身，过程中还得删本身的东西，那肯定不行），会崩溃，先存起来
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePar: ActiveEffectHandles)
		{
			if (TargetASC == HandlePar.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePar.Key); // 移除指定 FActiveGameplayEffectHandle，后面的1代表删除一层堆栈。
				HandlesToRemove.Add(HandlePar.Key);
			}
			
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle); // 另外删本地记录
		}
		
	}
	
}
