#include "DataTable/SiraDataTable.h"

#include "Misc/DataValidation.h"

#define LOCTEXT_NAMESPACE "SiraDataTable"

FInventoryItem FInventoryItem::None {};
#if WITH_EDITOR
EDataValidationResult FEnemyConfig::IsDataValid(FDataValidationContext& Context) const
{
	if (DropRate < 0.f || DropRate > 1.f)
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("DropRate Should in [0, 1]"))));
		return EDataValidationResult::Invalid;
	}
	return EDataValidationResult::Valid;
}

EDataValidationResult FUniquePanelRow::IsDataValid(FDataValidationContext& Context) const
{
	if (UniquePanelSoftClass.IsNull())
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("[FUniquePanelRow::IsDataValid] UniquePanelSoftClass is Null, UIName: %s"), *UIName.ToString())));
		return EDataValidationResult::Invalid;
	}
	auto AssetName = UniquePanelSoftClass.GetAssetName();
	// auto PathName = UniquePanelSoftClass->GetPathName();
	// auto FullName = UniquePanelSoftClass->GetFullName();
	// auto PackageName = UniquePanelSoftClass->GetPackage()->GetName();
	// UE_LOG(LogSira, Log, TEXT("[FUniquePanelRow::IsDataValid] AssetName: %s, PathName: %s, FullName: %s, PackageName: %s"), *AssetName, *PathName, *FullName, *PackageName);
	
	if (AssetName.IsEmpty())
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("[FUniquePanelRow::IsDataValid] AssetName is Empty, UIName: %s"), *UIName.ToString())));
		return EDataValidationResult::Invalid;
	}
	
	FStringView AssetNameView {GetData(AssetName), FMath::Max(AssetName.Len() - 2, 0)};
	if (UIName != AssetNameView)
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("[FUniquePanelRow::IsDataValid] UIName must Match AssetName, UIName: %s, AssetName: %s"), *UIName.ToString(), *AssetName)));
		return EDataValidationResult::Invalid;
	}
	
	return EDataValidationResult::Valid;
}
#endif
