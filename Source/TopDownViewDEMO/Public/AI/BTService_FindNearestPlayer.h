// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FindNearestPlayer.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNVIEWDEMO_API UBTService_FindNearestPlayer : public UBTService_BlueprintBase
{
	GENERATED_BODY()
	
protected:
	
	//当这个 Service 到了执行时间，Unreal 就会调用 TickNode，执行里面写的具体的 AI 逻辑
	//OwnerComp 当前正在运行这个 Service 的行为树组件
	//NodeMemory 这个 Service 可以使用的节点运行时内存
	//DeltaSeconds 距离上一次 Tick 经过的时间
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	/*
	 * 黑板键选择器
	 */
	//跟随目标
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector TargetToFollowSelector;
	
	//目标距离
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector DistanceToTargetSelector;
};
