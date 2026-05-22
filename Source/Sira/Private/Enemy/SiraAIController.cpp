#include "Enemy/SiraAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Enemy/NormalEnemy.h"
#include "Manager/SiraConfigSubsystem.h"

// void ASiraAIController::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// }

void ASiraAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	auto Config = GetGameInstance()->GetSubsystem<USiraConfigSubsystem>();
	auto Enemy = Cast<ANormalEnemy>(InPawn);
	if (!Enemy)
	{
		return;
	}
	if (auto EnemyConfig = Config->GetEnemyConfig(Enemy->GetEnemyID()))
	{
		auto PendingLoadBehaviorTree = EnemyConfig->BehaviorTree;
		if (!PendingLoadBehaviorTree)
		{
			return;
		}
		EnemyConfig->BehaviorTree.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateLambda(
		[
			WeakThis = MakeWeakObjectPtr(this)
		]
			(const FSoftObjectPath& SoftPtr, UObject* LoadedAsset)
		{
			if (!WeakThis.IsValid())
			{
				return;
			}
			if (auto BehaviorTree = Cast<UBehaviorTree>(LoadedAsset))
			{
				WeakThis->RunBehaviorTree(BehaviorTree);
			}
		}));
	}
}
