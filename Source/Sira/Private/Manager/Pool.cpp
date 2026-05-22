#include "Manager/Pool.h"

#include "EngineUtils.h"
#include "SiraGameMode.h"
#include "SiraLog.h"

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

