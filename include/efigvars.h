#pragma once

#include "efidef.h"

extern EFI_SYSTEM_TABLE* ST;
#define gST ST
extern EFI_BOOT_SERVICES* BS;
#define gBS BS
extern EFI_RUNTIME_SERVICES* RT;
#define gRT RT
