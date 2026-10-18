// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 我的 Ability 都可以继承这个类 这是我 Aura 里面所有技能的基础模板
 */
UCLASS()
class TOPDOWNVIEWDEMO_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	//增加一个“这个技能对应哪个输入 Tag”的属性 也就是给每个技能指定一个默认的输入标签
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;
};
