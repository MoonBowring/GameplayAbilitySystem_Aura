// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNVIEWDEMO_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();
	
	//在服务器和客户端各自建立正确的玩家状态与能力系统AbilitySystem的关联
	//当服务器让玩家控制器开始控制角色时 此函数开始调用
	//服务器刚刚把这个角色交给一个玩家控制器了，现在我要把这个角色和能力系统连接起来 也就是说这个角色现在在服务器被玩家控制了
	virtual void PossessedBy(AController* NewController) override;
	//当服务器通过网络复制把玩家状态赋值给客户端的时候 客户端触发OnRep_PlayerState 也就是说服务器把这个玩家状态发给我了
	virtual void OnRep_PlayerState() override;
	
private:
	virtual  void InitAbilityActorInfo() override;//初始化能力角色信息
};
