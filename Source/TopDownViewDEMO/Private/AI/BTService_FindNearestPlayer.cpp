// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	//获取当前敌人控制的 Pawn
	APawn* OwningPawn = AIOwner->GetPawn();
	
	//判断自身是否是玩家友善方 如果是的话 那么我的敌人就是 Enemy 如果不是的话 那么我的敌人就是 Player
	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");
	
	//这个数组是用来存储拥有这个标签的 Actor 的
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);
}
