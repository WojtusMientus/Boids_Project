
#include "Core/BoidGameInstance.h"
#include "Utilities/Macros/DebugMacros.h"
#include "Utilities/Subsystems/BoidDataManagerSubsystem.h"


void UBoidGameInstance::Init()
{
	Super::Init();
	
	UBoidDataManagerSubsystem* BoidDataManagerSubsystem = GetSubsystem<UBoidDataManagerSubsystem>();
	ENSURE_ALWAYS_RETURN(IsValid(BoidDataManagerSubsystem))
	BoidDataManagerSubsystem->LoadSimulationDataAssets();
}
