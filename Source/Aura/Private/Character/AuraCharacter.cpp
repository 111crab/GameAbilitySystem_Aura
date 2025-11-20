// Copyright CRABOI


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


AAuraCharacter::AAuraCharacter()
{
	//启用自动朝向角色移动方向旋转
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//旋转速度调整
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	//约束角色在某一个特定平面
	GetCharacterMovement()->bConstrainToPlane = true;
	//将初始位置对齐平面
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	//禁止角色跟随控制器三个方面的旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init AbilityActorInfo for the Server 
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init AbilityActorInfo for the Client 
	InitAbilityActorInfo();
	
}

int32 AAuraCharacter::GetPlayerLevel()
{
	// Aura 的等级在 PlayerState 中。
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo() 
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	// 这里的 InitAbilityActorInfo 是库函数，实际上这里算是将 PlayerState 和真正的 ASC 和 AS绑定起来，让 PS 真正拥有 ASC。
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AblityActorInfoSet();
	
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController,AuraPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}

	// 此方法只在 Server 端调用即可，因为属性本身有复制属性，会同步到 Client
	InitializedDefaultAttributes();
	
}
