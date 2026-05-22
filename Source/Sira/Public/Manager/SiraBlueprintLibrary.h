#pragma once

#include <concepts>
#include "EngineUtils.h"
#include "Player/SiraPlayer.h"
#include "SiraPlayerController.h"


#include "SiraBlueprintLibrary.generated.h"

USTRUCT(BlueprintType)
struct FAttributeDisplayInfo
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayAttribute Attribute;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DisplayName;
};

class USiraUIGameInstance;

namespace SiraUtilities
{
	template <typename F, typename ActorType>
	concept CallForAllActorsFuncType = requires (F Func, ActorType* Actor)
	{
		{ Func(Actor) } ->std::same_as<void>;
	};
	
	template<typename ActorType>
	requires requires () {std::derived_from<ActorType, AActor>;}
	void CallForAllActors(const UObject* WorldContextObject, CallForAllActorsFuncType<ActorType> auto CallForAllActorsFunc)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			for (TActorIterator<ActorType> It(World); It; ++It)
			{
				CallForAllActorsFunc(*It);
			}
		}
	}
	
}

UCLASS(meta=(ScriptName="SiraBlueprintLibrary"))
class USiraBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	[[nodiscard]]
	static ASiraPlayerController* GetSiraPlayerController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	[[nodiscard]]
	static ASiraPlayer* GetSiraPlayer(const UObject* WorldContextObject);
	
	// this stupid function just use for handle reference parameter in blueprint function
	UFUNCTION(BlueprintPure, Category="Attribute")
	static FGameplayAttribute MakeGameplayAttribute(FGameplayAttribute Attribute);
	
	UFUNCTION(BlueprintPure, Category="Attribute")
	static float GetSiraPlayerAttribute(const ASiraPlayer* TargetPlayer, FGameplayAttribute Attribute);
	
	UFUNCTION(BlueprintPure, Category="Attribute")
	static FString GetAttributeName(const FGameplayAttribute& Attribute);
	
	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	[[nodiscard]]
	static int32 GetActorsCount(const UObject* WorldContextObject, TSubclassOf<AActor> Class);
	
	template<typename ActorType>
	requires std::derived_from<ActorType, AActor>
	[[nodiscard]]
	static int32 GetActorsCount(const UObject* WorldContextObject)
	{
		int32 Result = 0;
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			for (TActorIterator<ActorType> It(World); It; ++It)
			{
				Result++;
			}
		}
		return Result;
	}
	
	template<typename ActorType>
	requires std::derived_from<ActorType, AActor>
	[[nodiscard]]
	static int32 GetNotHiddenActorsCount(const UObject* WorldContextObject)
	{
		int32 Result = 0;
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			for (TActorIterator<ActorType> It(World); It; ++It)
			{
				if (!It->IsHidden())
				{
					Result++;
				}
			}
		}
		return Result;
	}
	
	template<typename ActorType>
	requires std::derived_from<ActorType, AActor>
	[[nodiscard]]
	static TArray<ActorType> GetAllActorsOfClass(const UObject* WorldContextObject)
	{
		TArray<ActorType> Result;
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			for (TActorIterator<ActorType> It(World); It; ++It)
			{
				Result.Add(*It);
			}
		}
		return Result;
	}
	
	UFUNCTION(BlueprintCallable, Category="Game", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static void SimplyShowMouse(const UObject* WorldContextObject, bool bShow);
	
	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static APlayerCameraManager* GetCameraManger(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="Game")
	static TArray<FAttributeDisplayInfo> GetAllPlayerAttributeDisplayInfos();
	
};
