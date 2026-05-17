#include "Character/WWCharacter.h"
#include "Stat/WWStatComponent.h"

AWWCharacter::AWWCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    // SetDefaultSubobjectClass를 통해 자식이 이 컴포넌트 클래스를 바꿀 수 있도록 여지를 줍니다.
    //Stat = ObjectInitializer.CreateDefaultSubobject<UWWStatComponent>(this, TEXT("Stat"));
}