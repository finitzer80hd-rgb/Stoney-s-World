#include "StoneyWorldGameMode.h"
#include "StoneyWorldCharacter.h"

AStoneyWorldGameMode::AStoneyWorldGameMode()
{
    DefaultPawnClass = AStoneyWorldCharacter::StaticClass();
}
