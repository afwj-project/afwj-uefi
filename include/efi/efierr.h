#pragma once

#include "efigvars.h"

VOID UefiErrorPrint(IN EFI_STATUS ErrorCode, IN CHAR16* ErrorSymbol);
VOID UefiErrorShutdown(IN EFI_STATUS ErrorCode, IN CHAR16* ErrorSymbol);
