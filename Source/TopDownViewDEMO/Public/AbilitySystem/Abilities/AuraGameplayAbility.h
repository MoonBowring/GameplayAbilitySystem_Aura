// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 我的 Ability 都可以继承这个类
 */
UCLASS()
class TOPDOWNVIEWDEMO_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	//增加一个“这个技能对应哪个输入 Tag”的属性
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
};
