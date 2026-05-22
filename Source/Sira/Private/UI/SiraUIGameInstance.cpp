#include "Manager/SiraUIGameInstance.h"

#include "SiraLog.h"
#include "Blueprint/UserWidget.h"
#include "DataTable/SiraDataTable.h"
#include "Manager/SiraBlueprintLibrary.h"
#include "UI/SiraUniquePanel.h"


// void USiraUIGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
// {
// 	Super::Initialize(Collection);
// 	UniquePanelTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/DataTable/DT_UniquePanel.DT_UniquePanel'"));
// }

// USiraUniquePanel* USiraUIGameSubsystem::GetOrCreateWidget(FName Key)
// {
// 	if (!UniquePanelTable)
// 	{
// 		UE_LOG(LogSira, Warning, TEXT("[USiraUIManager::GetOrCreateWidget] UniquePanelTable is nullptr"));
// 		return nullptr;
// 	}
// 	
// 	TSubclassOf<USiraUniquePanel> WidgetClass;
// 	if (TSubclassOf<USiraUniquePanel>* FoundClass = WidgetClassMap.Find(Key))
// 	{
// 		WidgetClass = *FoundClass;
// 	} else
// 	{
// 		auto Row = UniquePanelTable->FindRow<FUniquePanelRow>(Key, __func__);
// 		if (!Row)
// 		{
// 			UE_LOG(LogSira, Error, TEXT("[USiraUIManager::GetOrCreateWidget] Widget Not Found, Key: %s"), *Key.ToString());
// 			return nullptr;
// 		}
// 		WidgetClass = Row->UniquePanelSoftClass.LoadSynchronous();
// 		if (!WidgetClass)
// 		{
// 			UE_LOG(LogSira, Error, TEXT("[USiraUIManager::GetOrCreateWidget] Widget Class Load Failed, Key: %s"), *Key.ToString());
// 			return nullptr;
// 		}
// 		WidgetClassMap.Add(Key, WidgetClass);
// 	}
// 	
// 	if (auto FoundWidget = WidgetMap.Find(Key))
// 	{
// 		return *FoundWidget;
// 	}
// 	
// 	USiraUniquePanel* NewWidget = CreateWidget<USiraUniquePanel>(USiraBlueprintLibrary::GetSiraPlayerController(GetWorld()), WidgetClass);
// 	if (!NewWidget)
// 	{
// 		UE_LOG(LogSira, Warning, TEXT("[USiraUIManager::GetOrCreateWidget] Create Widget Failed, Key: %s"), *Key.ToString());
// 		return nullptr;
// 	}
// 	WidgetMap.Add(Key, NewWidget);
// 	NewWidget->SetUIKey(Key);
// 	return NewWidget;
// }
