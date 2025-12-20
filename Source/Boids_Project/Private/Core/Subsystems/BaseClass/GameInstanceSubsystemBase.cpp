
#include "Core/Subsystems/BaseClass/GameInstanceSubsystemBase.h"
#include "Utilities/Subsystems/RuntimeDataLoaderSubsystem.h"

void UGameInstanceSubsystemBase::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Collection.InitializeDependency(URuntimeDataLoaderSubsystem::StaticClass());
}
