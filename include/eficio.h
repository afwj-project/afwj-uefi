#pragma once

#include "efigvars.h"

EFI_STATUS UefiInitializeConsole();
EFI_STATUS UefiScanSecretText(OUT CHAR16* StringBuffer, IN UINTN StringCount);
