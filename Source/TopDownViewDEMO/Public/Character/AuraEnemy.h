// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNVIEWDEMO_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();
	
	/** Enemy Interface */
	virtual void HightlightActor() override;
	virtual void UnHighlightActor() override;
	/**End Enemy Interface */
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
};
