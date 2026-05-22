#pragma once

#include "DropItem.generated.h"

class UBoxComponent;

UCLASS()
class ADropItem : public AActor
{
	GENERATED_BODY()
	
public:
	
	ADropItem();
	
	UFUNCTION(BlueprintCallable)
	void SetDropInfo(FName ItemID);
	
	void SetDropInfo(int32 ItemID);
	
	UFUNCTION()
	void OnBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;
	
	FName InventoryItemID;
};
