// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = false;
}

void AAuraPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	CursorTrace();
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter)
{
	//目标角色必须有效 且必须配置好要创建的伤害文字组件类 才能继续执行
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		//创建一个伤害文字组件 TargetCharacter 新对象应该归入哪个对象层级 
		//简单来说就是根据 DamageTextComponentClass 指定的类 创建一个新的伤害文字组件 并把它放在 TargetCharacter 对应的对象层级下面 创建完成后 把这个新对象的指针交给 DamageText 变量保存
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		//把刚刚创建的组件注册到 Unreal Engine 的组件系统中 让它能够正常参与游戏运行
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform)
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);//从鼠标光标的位置向游戏世界检测，看鼠标指着什么
	if (!CursorHit.bBlockingHit) return;
	
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());//判断集中对象是否实现IEnemyInterface接口
	
	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HightlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagPressed(InputTag);
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
