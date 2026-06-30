// Copyright CRABOI


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "Engine/HitResult.h"              
#include "GameFramework/Pawn.h"             
#include "Engine/LocalPlayer.h"             
#include "EnhancedInputSubsystems.h"        
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	
}

void AAuraPlayerController::PlayerTick(float DeltaSeconds)
{
	Super::PlayerTick(DeltaSeconds);
	CursorTrace();
	AutoRun();
	
	
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn  = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::BroadcastInitialValues()
{
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass); // 组件的 Outer 也就是所有者必须是其挂载点，不然也不会渲染。
		DamageText->RegisterComponent();  // 游戏运行时创建的组件，不像 CreateDefaultsComponent，需要手动注册，不然有些东西不会渲染
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void AAuraPlayerController::CursorTrace()
{
	// Func:GetHitResultUnderCursor 来自 PlayerController 或者其子类的方法，用于获取光标下射线能检测到目标的 FHitResult。
	// 参数（通道，是否为复杂追踪，存到哪个 FHitResult），其中 ECC_Visibility 通道常用于捕捉没有物理模拟但是可见（类似 UI）的物体。注：我们只能捕捉在 Visibility 上为 Blocked or Overlapped 的物体
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}

}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	if (!bTargeting && !bShiftDown)
	{
		APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			bAutoRunning = true;
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointsLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointsLoc, ESplineCoordinateSpace::World);
					// DrawDebugSphere(GetWorld(), PointsLoc,8.f,8,FColor::Green,false,5.f);
				}
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
			}
		}
		FollowTime = 0.0f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// 判断如果不是左键，则可以激活 GA
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}
	// 判断目标是否是一个有效 Target
	if (bTargeting || bShiftDown)
	{
		// 是：激活 GA
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		// 否：实现持续跟踪鼠标移动
		
		FollowTime += GetWorld()->GetDeltaSeconds(); // 存储鼠标 Held 多久
		
		if (CursorHit.bBlockingHit) 
		{
			CachedDestination = CursorHit.ImpactPoint;  // 获取坐标，也可以用 Location，这两个在射线检测时的效果是相同的
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}




void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	//------为本地玩家子系统挂上IMC-------
	check(AuraContext);
	//获取本地玩家子系统
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		//添加IMC并且设置优先级，0最高。
		Subsystem->AddMappingContext(AuraContext,0);
	}
	
	
	
	//------光标样式和响应模式设置------
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
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	//绑定动作，只要 MoveAction 被持续触发，this Controller的 Move函数持续被回调。
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started,this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed,this, &AAuraPlayerController::ShiftReleased);
	// 绑定按键事件函数并传回 InputTag
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased,&ThisClass::AbilityInputTagHeld);
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


