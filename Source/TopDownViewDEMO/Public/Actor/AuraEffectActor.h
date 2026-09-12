#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;
class UGameplayEffect;

//这个宏是应用规则
UENUM(BlueprintType)
enum class EEffectApplicationPolicy : UINT8//Gameplay Effect 应该什么时候应用
{
	ApplyOnOverlap,//进入碰撞范围的时候应用
	ApplyOnEndOverlap,//离开碰撞范围的时候应用
	DoNotApply//这个 Effect Actor 不自动应用这个 Gameplay Effect
};

//这个宏是移除规则
UENUM(BlueprintType)
enum class EEffectRemovalPolicy : UINT8//Gameplay Effect 应该什么时候移除
{
	RemoveOnEndOverlap,//这个是当结束重叠时 移除GameplayEffect
	DoNotRemove//这个就是字面意思 不移除GameplayEffect
};

UCLASS()
class TOPDOWNVIEWDEMO_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;
	
	//把指定的 Gameplay Effect 应用到指定的 Target 身上   Target:效果要作用到谁   GameplayEffectClass:要施加什么 Gameplay Effect
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);
	
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectRemoval = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;//Instant Gameplay Effect 应该什么时候应用
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	//这个东西就是暴露给蓝图的 让设计师判断是否在离开范围时取消效果 默认是移除的
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
	
	//这里的TMap键是 ActiveGameplayEffect句柄 值是 AbilitySystemComponent 这样就可以不用从 TargetActor 再去获取了
	//这个TMap的作用就是将句柄映射到能力系统组件
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	float ActorLevel = 1.f;
};
