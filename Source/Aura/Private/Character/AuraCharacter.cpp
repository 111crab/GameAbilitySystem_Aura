// Copyright CRABOI


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

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

	// Init ability actor info for the Server 
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client 
	InitAbilityActorInfo();
	
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState,this);
}
