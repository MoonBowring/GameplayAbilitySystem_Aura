// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	//获取当前敌人控制的 Pawn 也就是获取这个 AI 正在控制的 Pawn
	APawn* OwningPawn = AIOwner->GetPawn();
	
	//判断自身是否是玩家友善方 如果是的话 那么我的敌人就是 Enemy 如果不是的话 那么我的敌人就是 Player
	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");
	
	//这个数组是用来存储拥有这个标签的 Actor 的
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);
	
	//当前最近距离
	float ClosestDistance = TNumericLimits<float>::Max();
	//当前最近 Actor
	AActor* ClosestActor = nullptr;
	
	for (AActor* Actor : ActorsWithTag)
	{
		if (IsValid(Actor) && IsValid(OwningPawn))
		{
			//获取 for 循环到的 Actor 到自身的距离
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			//判断这个距离是否大于当前的最进距离
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	}
	
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
}
