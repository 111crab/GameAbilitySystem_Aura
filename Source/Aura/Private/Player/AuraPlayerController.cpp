// Copyright CRABOI


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"



AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	//------为本地玩家子系统挂上IMC-------
	check(AuraContext);
	//获取本地玩家子系统
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	//添加IMC并且设置优先级，0最高。
	Subsystem->AddMappingContext(AuraContext,0);
	
	
	//------光标设置------
	//显示光标
	bShowMouseCursor = true;
	//光标样式
	DefaultMouseCursor = EMouseCursor::Default;
	//选择 “游戏和 UI 同时响应” 的输入模式
	FInputModeGameAndUI InputModeData;
	//不把鼠标锁在窗口内
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//鼠标按下时也不隐藏光标
	InputModeData.SetHideCursorDuringCapture(false);
	//应用此模式
	SetInputMode(InputModeData);
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//CastChecked:检查并且强转，把普通的输入转成增强输入
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	//绑定动作，只要MoveAction被持续触发，this Controller的 Move函数持续被回调。
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

	// FRotationMatrix(YawRotation)：这个矩阵能在明确Yaw偏航（绕Z旋转）的情况下，查询X，Y正方向（EAxis::X指代X正方向）指向哪里。此处GetUnitAxis顺便归一化了。
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}


