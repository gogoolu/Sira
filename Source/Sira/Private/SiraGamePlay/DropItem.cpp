#include "SiraGamePlay/DropItem.h"

#include "Component/InventoryComponent.h"
#include "Components/BoxComponent.h"
#include "Manager/Pool.h"
#include "Player/SiraPlayer.h"

ADropItem::ADropItem()
{
	PrimaryActorTick.bCanEverTick = false;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetCollisionProfileName("Trigger");
	BoxCollision->SetGenerateOverlapEvents(true);
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(
		this,
		&ThisClass::OnBoxBeginOverlap
	);
}

void ADropItem::SetDropInfo(FName ItemID)
{
	InventoryItemID = ItemID;
}

void ADropItem::SetDropInfo(int32 ItemID)
{
	InventoryItemID = FName{FString::FromInt(ItemID)};
}

void ADropItem::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto SiraPlayer = Cast<ASiraPlayer>(OtherActor))
	{
		SiraPlayer->GetInventoryComponent()->AddItem(InventoryItemID);
		auto DropItemPool = GetWorld()->GetSubsystem<USiraDropItemPool>();
		DropItemPool->Recycle(this);
	}
}

