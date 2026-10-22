// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UDamageTextComponent;
class UAuraAbilitySystemComponent;
struct FGameplayTag;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraInputConfig;

/*
 * 凡是跟“玩家怎么操作游戏”有关的，通常就适合放在 AAuraPlayerController 里
 */
UCLASS()
class TOPDOWNVIEWDEMO_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();
	virtual void Tick(float DeltaSeconds) override;
	
	//显示伤害数字 DamageAmount 显示这次打了多少伤害 例如 50 TargetCharacter 伤害打到了哪个角色 也就是数字要显示在哪里
	//由服务器调用时 让拥有这个 PlayerController 的客户端执行
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter);
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	void Move(const FInputActionValue& InputActionValue);
	
	void CursorTrace();//光标的轨迹 鼠标悬停选敌系统
	IEnemyInterface*  LastActor;//光标上一帧指向的Actor
	IEnemyInterface* ThisActor;//光标这一阵指向的Actor
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;
	
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	UAuraAbilitySystemComponent* GetASC();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
