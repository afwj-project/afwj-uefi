#pragma once

#include "efibase.h"

BOOLEAN IsZeroGUID(IN EFI_GUID TargetGUID);
BOOLEAN IsZeroSpace(IN CONST VOID* TargetData, IN UINT64 TargetSize);
INT32 UefiWideStrCmp(IN CONST CHAR16* Src1, IN CONST CHAR16* Src2);
INT32 UefiStrCmp(IN CONST CHAR8* Src1, IN CONST CHAR8* Src2);
