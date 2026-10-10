#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
		
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	/** 如果这是服务器或者是单人游戏则返回真 */
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	/** GetAvatarActorFromActorInfo 返回执行此能力的物理角色对象 该对象可能为空 */
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		SocketLocation.Z = 40.f;
		Rotation.Pitch = 0.f;
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
				
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		   ProjectileClass, 
		   SpawnTransform, 
		   GetOwningActorFromActorInfo(), 
		   Cast<APawn>(GetAvatarActorFromActorInfo()), 
		   ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		//TODO: 给投射物设置一个用于造成伤害的游戏效果规格(GameplayEffectSpec)
		
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
