// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UAuraUserWidget;
class UOverlayWidgetController;
struct FWidgetControllerParams;	

/*
 * HUD指的是游戏过程中屏幕上覆盖的状态和信息 可以理解为UI的总管
 * HUD的目的 是通知玩家当前游戏状态，即比分、生命值、剩余时间，等等
 * HUD通常是非交互式的，玩家不会点击HUD中的元素
 */

UCLASS()
class TOPDOWNVIEWDEMO_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	
	//如果我还没有 OverlayWidgetController，就创建一个并初始化；如果已经有了，就直接把原来的返回给你
	UFUNCTION()
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	
	void InitOverlay(APlayerController* PC,APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
protected:
	
	
private:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;//这个是游戏运行时 真正创建出来的对象 可以想象成图纸造出来的东西
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;//这个保存的是我要创建哪一种 Widget 可以把它想象成图纸
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;//运行时真正创建出来的那个 Controller 对象 可以把它想象成真正的施工人员
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;//我要创建哪一种 OverlayWidgetController 可以把它想象成施工方案
	
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
