// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

/*
 * 现在版本 获取完整的鼠标梦中结果 把它包装成 GAS 认识的目标数据 然后再尝试发送给服务器 
 * 也就是 当我在客户端点击鼠标的时候 我会检测鼠标指向哪里 然后拿到鼠标的命中结果 吧目标数据交给服务器 让服务器知道火球应该朝哪里发射
 * 因为在多人游戏里 通常由服务器负责权威地处理技能和生成投射物 客户端知道自己的鼠标指向哪里 但服务器并不能直接读取你本地的鼠标位置
 * 所以现在的 UTargetDataUnderMouse 除了获取目标 还开始承担一项新工作 就是把客户端获取到的目标信息 通过 GAS 的机制传递给服务器
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

UCLASS()
class TOPDOWNVIEWDEMO_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);
	
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;
	
private:
	
	virtual void Activate() override;
	void SendMouseCursorData();
	
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
