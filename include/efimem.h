#pragma once

#include "efigvars.h"

VOID* UefiMalloc(IN UINTN Size);
VOID UefiFree(IN VOID* Pool);
VOID UefiMemCpy(OUT VOID* Destination, IN CONST VOID* Source, IN UINTN Length);
