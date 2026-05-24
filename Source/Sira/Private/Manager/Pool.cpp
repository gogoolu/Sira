#include "Manager/Pool.h"

#include "EngineUtils.h"
#include "SiraGameMode.h"
#include "SiraLog.h"
#include "Manager/SiraBlueprintLibrary.h"
#include "SiraGamePlay/Projectile.h"

ADropItem* USiraDropItemPool::Get()
{
	for (auto Actor: Pool)
	{
		if (Actor->IsHidden())
		{
			Actor->SetActorHiddenInGame(false);
			Actor->SetActorEnableCollision(true);
			return Actor;
		}
	}
	auto GameMode = Cast<ASiraGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode || !GameMode->DropItemClass)
	{
		UE_LOG(LogSira, Error, TEXT("[USiraDropItemPool::Get] GameMode or DropItemClass is nullptr"));
		return nullptr;
	}
	auto NewActor = GetWorld()->SpawnActor<ADropItem>(GameMode->DropItemClass);
	Pool.Add(NewActor);
	return NewActor;
}

ADropItem* USiraDropItemPool::Get(const FVector& Location)
{
	auto Actor = Get();
	Actor->SetActorLocation(Location);
	return Actor;
}

void USiraDropItemPool::Recycle(ADropItem* DropItem)
{
	if (!IsValid(DropItem))
	{
		UE_LOG(LogSira, Warning, TEXT("[UPool::Recycle] DropItem is InValid"));
		return;
	}
	if (!DropItem->IsHidden() || !Pool.Contains(DropItem))
	{
		DropItem->SetActorHiddenInGame(true);
		DropItem->SetActorEnableCollision(false);
		Pool.Add(DropItem);
	}
}

void USiraDropItemPool::RecycleAll()
{
	for (TActorIterator<ADropItem> It(GetWorld()); It; ++It)
	{
		if (!It->IsHidden())
		{
			Recycle(*It);
		}
	}
}

void UProjectilePool::RecycleAll()
{
	SiraUtilities::CallForAllActors<AProjectile>(GetWorld(), [this](AProjectile* Projectile)
	{
		Recycle(Projectile);
	});
}

void UProjectilePool::Recycle(AProjectile* Projectile)
{
	if (!Projectile)
	{
		return;
	}
	
	auto Class = Projectile->GetClass();
	auto& Projectiles = Pool.FindOrAdd(Class);
	Projectile->SetActorHiddenInGame(true);
	Projectile->SetActorEnableCollision(false);
	if (!Projectiles.Contains(Projectile))
	{
		Projectiles.Add(Projectile);
	}
}

AProjectile* UProjectilePool::Get(TSubclassOf<AProjectile> ProjectileClass, const FActorSpawnParameters& Parameters,
	const FVector& Location, const FRotator& Rotation)
{
	if (!ProjectileClass)
	{
		return nullptr;
	}
	if (auto Projectiles = Pool.Find(ProjectileClass))
	{
		for (auto Projectile : *Projectiles)
		{
			if (Projectile && Projectile->IsHidden())
			{
				Projectile->SetActorLocation(Location);
				Projectile->SetActorRotation(Rotation);
				Projectile->SetActorHiddenInGame(false);
				Projectile->SetActorEnableCollision(true);
				Projectile->ResetProjectile();
				Projectile->SetOwner(Parameters.Owner);
				Projectile->SetInstigator(Parameters.Instigator);
				return Projectile;
			}
		}
	}
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation, Parameters);
	if (Projectile)
	{
		if (!Pool.Find(ProjectileClass))
		{
			Pool.Add(ProjectileClass);
		}
		auto Projectiles = Pool.Find(ProjectileClass);
		Projectiles->Add(Projectile);
	}
	return Projectile;
}

