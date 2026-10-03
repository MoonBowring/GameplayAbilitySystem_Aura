// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = false;
}

void AAuraPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);//从鼠标光标的位置向游戏世界检测，看鼠标指着什么
	if (!CursorHit.bBlockingHit) return;
	
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());//判断集中对象是否实现IEnemyInterface接口
	
	/*
	 * 从光标处绘制线条 有这几种情况
	 * 1.如果上一个角色为空 且当前角色也为空 那么不执行任何操作
	 * 2.如果上一个角色为空 但当前角色有效 那么突出显示当前角色
	 * 3.如果上一个角色有效 但是当前角色为空 那么取消上一个角色的高亮显示
	 * 4.如果当前角色和上一个角色都有效 并且当前角色不等于上一个角色 那么这取消显示上个一角色高亮
	 * 5.当前角色和上一个角色都是有效的 则什么都不做
	 */
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			//2.如果上一个角色为空 但当前角色有效 那么突出显示当前角色
			ThisActor->HightlightActor();
		}
		else
		{
			//1.如果上一个角色为空 且当前角色也为空 那么不执行任何操作
		}
	}
	else //上一个角色是有效的
	{
		if (ThisActor == nullptr)
		{
			//3.如果上一个角色有效 但是当前角色为空 那么取消上一个角色的高亮显示
			LastActor->UnHighlightActor();
		}
		else//两者都有效
		{
			if (LastActor != ThisActor)
			{
				//4.如果当前角色和上一个角色都有效 并且当前角色不等于上一个角色 那么这取消显示上个一角色高亮
				LastActor->UnHighlightActor();
				ThisActor->HightlightActor();
			}
			else
			{
				//5.当前角色和上一个角色都是有效的 则什么都不做
			}
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);
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
	
	check(AuraContext)
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());	
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	
	bShowMouseCursor = true;//设置默认显示鼠标
	DefaultMouseCursor = EMouseCursor::Default;//鼠标显示类型为指针
	
	FInputModeGameAndUI InputModeData;//输入配置对象
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);//设置鼠标不要被限制在游戏窗口里面
	InputModeData.SetHideCursorDuringCapture(false);//鼠标被游戏捕获的时候 要不要隐藏
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
