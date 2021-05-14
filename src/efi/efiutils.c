#include "efi/efiutils.h"

BOOLEAN IsZeroGUID(IN EFI_GUID TargetGUID) {
	if (TargetGUID.Data1 != 0) return FALSE;
	if (TargetGUID.Data2 != 0) return FALSE;
	if (TargetGUID.Data3 != 0) return FALSE;
	for (UINTN i = 0; i < 8; i++) if (TargetGUID.Data4[i] != 0) return FALSE;
	return TRUE;
}
