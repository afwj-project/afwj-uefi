#include "efi/efimem.h"

VOID* UefiMalloc(IN UINTN Size) {
	VOID* Pool;
	EFI_STATUS Status = gBS->AllocatePool(EfiLoaderData, Size, &Pool);
	if (Status != EFI_SUCCESS) Pool = NULL;
	return Pool;
}

VOID UefiFree(IN VOID* Pool) {
	gBS->FreePool(Pool);
}

VOID UefiMemCpy(OUT VOID* Destination, IN CONST VOID* Source, IN UINTN Length) {
	UINT8* DestinationRaw = (UINT8*)Destination;
	UINT8* SourceRaw = (UINT8*)Source;
	while (Length--) *DestinationRaw++ = *SourceRaw++;
}
