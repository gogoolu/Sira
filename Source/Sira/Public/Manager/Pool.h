#pragma once

#include "SiraGamePlay/DropItem.h"

#include "Pool.generated.h"

UCLASS()
class USiraDropItemPool : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	ADropItem* Get();
	ADropItem* Get(const FVector& Location);
	void Recycle(ADropItem* DropItem);
	void RecycleAll();
	
private:
	
	TArray<TObjectPtr<ADropItem>> Pool;
};
