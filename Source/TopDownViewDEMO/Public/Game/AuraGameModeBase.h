// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class UCharacterClassInfo;

/**
 * 
 */
UCLASS()
class TOPDOWNVIEWDEMO_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	//在 GameMode 上保存一个角色类配置资源的引用
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};
