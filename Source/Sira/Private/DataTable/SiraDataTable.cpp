#include "DataTable/SiraDataTable.h"

#include "Misc/DataValidation.h"

#define LOCTEXT_NAMESPACE "SiraDataTable"

EDataValidationResult FUniquePanelRow::IsDataValid(FDataValidationContext& Context) const
{
	FTableRowBase::IsDataValid(Context);
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
