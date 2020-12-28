#include "efimem.h"

VOID* UefiMalloc(UINTN Size) {
	VOID* Pool;
	EFI_STATUS Status = gBS->AllocatePool(EfiLoaderData, Size, &Pool);
	if (Status != EFI_SUCCESS) Pool = NULL;
	return Pool;
}

VOID UefiFree(VOID* Pool) {
	gBS->FreePool(Pool);
}
