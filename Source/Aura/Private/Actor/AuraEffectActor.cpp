// Copyright CRABOI


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"


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
	
	const FGameplayEffectSpecHandle EffectSpecHandle =  TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle); // 创建 EffectSpecHandle，作为 ApplyGameplayEffectSpecToSelf 参数。
	
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get()); // 这里需要一个 const引用 作为参数。从 EffectSpecHandle 的 wrap 中取 Data（TSharePtr也是一种 wrap） 用 get（）取裸指针。再 ‘*’ 解引用符合入参类型。

	
}
