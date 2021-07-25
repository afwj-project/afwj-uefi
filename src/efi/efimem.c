#include "efi/efimem.h"

EFI_MEMORY_TUPLE PoolTable[2048];

VOID UefiMemInit() {
	for (UINTN i = 0; i < 2048; i++) {
		PoolTable[i].Size = 0;
		PoolTable[i].Ptr = NULL;
	}
}

VOID* UefiMalloc(IN UINTN Size) {
	VOID* Pool;
	EFI_STATUS Status = gBS->AllocatePool(EfiLoaderData, Size, &Pool);
	if (Status != EFI_SUCCESS) Pool = NULL;
	for (UINTN i = 0; i < 2048; i++) {
		if (PoolTable[i].Ptr == NULL && PoolTable[i].Size == 0) {
			PoolTable[i].Size = Size;
			PoolTable[i].Ptr = Pool;
			break;
		}
	}
	return Pool;
}

EFI_STATUS UefiRealloc(IN OUT VOID** Pool, IN UINTN Size) {
	VOID* Temp = *Pool;
	EFI_STATUS Status = gBS->AllocatePool(EfiLoaderData, Size, Pool);
	if (Status != EFI_SUCCESS) return Status;
	UINT64 TempSize = UefiGetMemSize(Temp, &Status);
	if (Status != EFI_SUCCESS) return Status;
	UefiMemCpy(*Pool, Temp, TempSize);
	for (UINTN i = 0; i < 2048; i++) {
		if (PoolTable[i].Ptr == Temp && PoolTable[i].Size == TempSize) {
			PoolTable[i].Size = Size;
			PoolTable[i].Ptr = *Pool;
			break;
		}
	}
	return EFI_SUCCESS;
}

UINT64 UefiGetMemSize(IN VOID* Pool, OUT EFI_STATUS* StatusRef) {
	for (UINTN i = 0; i < 2048; i++) {
		if (PoolTable[i].Ptr == Pool) {
			*StatusRef = EFI_SUCCESS;
			return PoolTable[i].Size;
		}
	}
	*StatusRef = EFI_NOT_FOUND;
	return 0;
}

VOID UefiFree(IN VOID* Pool) {
	for (UINTN i = 0; i < 2048; i++) {
		if (PoolTable[i].Ptr == Pool) {
			PoolTable[i].Size = 0;
			PoolTable[i].Ptr = NULL;
			break;
		}
	}
	gBS->FreePool(Pool);
}

VOID UefiMemCpy(OUT VOID* Destination, IN CONST VOID* Source, IN UINTN Length) {
	UINT8* DestinationRaw = (UINT8*)Destination;
	UINT8* SourceRaw = (UINT8*)Source;
	while (Length--) *DestinationRaw++ = *SourceRaw++;
}
