#include "efi/efiutils.h"

BOOLEAN IsZeroGUID(IN EFI_GUID TargetGUID) {
	if (TargetGUID.Data1 != 0) return FALSE;
	if (TargetGUID.Data2 != 0) return FALSE;
	if (TargetGUID.Data3 != 0) return FALSE;
	for (UINTN i = 0; i < 8; i++) if (TargetGUID.Data4[i] != 0) return FALSE;
	return TRUE;
}

BOOLEAN IsZeroSpace(IN CONST VOID* TargetData, IN UINT64 TargetSize) {
	CONST UINT8* Ptr = TargetData;
	UINT8 Value;
	for (UINTN i = 0; i < TargetSize; i++) {
		Value = (UINT8)*Ptr++;
		if (Value != 0) return FALSE;
	}
	return TRUE;
}

INT32 UefiWideStrCmp(IN CONST CHAR16* Src1, IN CONST CHAR16* Src2) {
	CONST CHAR16 *Ptr1 = Src1, *Ptr2 = Src2;
	CHAR16 WC1, WC2;
	do {
		WC1 = (CHAR16)*Ptr1++;
		WC2 = (CHAR16)*Ptr2++;
		if (WC1 == L'\0') return WC1 - WC2;
	} while (WC1 == WC2);
	return WC1 - WC2;
}

INT32 UefiStrCmp(IN CONST CHAR8* Src1, IN CONST CHAR8* Src2) {
	CONST CHAR8 *Ptr1 = Src1, *Ptr2 = Src2;
	CHAR8 C1, C2;
	do {
		C1 = (CHAR8)*Ptr1++;
		C2 = (CHAR8)*Ptr2++;
		if (C1 == '\0') return C1 - C2;
	} while (C1 == C2);
	return C1 - C2;
}
