#include "efi/efimem.h"

EFI_MEMORY_TUPLE PoolTable[2048];
EFI_MEMORY_MAP MemoryMap;
UINT64 NextAllocPage;

__attribute__((aligned(4096)))
MAPPING_TABLE Pml4;

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
	gBS->CopyMem(*Pool, Temp, TempSize);
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

VOID* UefiAllocPage() {
	VOID* Page = (VOID*)NextAllocPage;
	NextAllocPage += 4096;
	return Page;
}

VOID IdentityMap4kb(UINT64 Logical) {
	INT32 Flags = PAGE_BIT_P | PAGE_BIT_RW | PAGE_BIT_US;
	INT32 Pml4Index = (Logical >> 39) & 0x1FF;
	INT32 PdpIndex = (Logical >> 30) & 0x1FF;
	INT32 PdIndex = (Logical >> 21) & 0x1FF;
	INT32 PtIndex = (Logical >> 12) & 0x1FF;
	INT32 PoIndex = Logical & 0x7FF;
	UINT64 PdptAlloc;
	UINT64 PdtAlloc;
	UINT64 PtAlloc;
	if (!(Pml4.Entries[Pml4Index] & PAGE_BIT_P)) {
		PdptAlloc = (UINT64)UefiAllocPage();
		for (UINTN i = 0; i < 4096; i++) ((UINT8*)PdptAlloc)[i] = 0;
		Pml4.Entries[Pml4Index] = (PdptAlloc & PAGE_ADDR_MASK) | Flags;
		IdentityMap4kb(PdptAlloc);
	}
	MAPPING_TABLE* Pdpt = (MAPPING_TABLE*)(Pml4.Entries[Pml4Index] & PAGE_ADDR_MASK);
	if (!(Pdpt->Entries[PdpIndex] & PAGE_BIT_P)) {
		PdtAlloc = (UINT64)UefiAllocPage();
		for (UINTN i = 0; i < 4096; i++) ((UINT8*)PdtAlloc)[i] = 0;
		Pdpt->Entries[PdpIndex] = (PdtAlloc & PAGE_ADDR_MASK) | Flags;
		IdentityMap4kb(PdtAlloc);
	}
	MAPPING_TABLE* Pdt = (MAPPING_TABLE*)(Pdpt->Entries[PdpIndex] & PAGE_ADDR_MASK);
	if (!(Pdt->Entries[PdIndex] & PAGE_BIT_P)) {
		PtAlloc = (UINT64)UefiAllocPage();
		for (UINTN i = 0; i < 4096; i++) ((UINT8*)PtAlloc)[i] = 0;
		Pdt->Entries[PdIndex] = (PtAlloc & PAGE_ADDR_MASK) | Flags;
		IdentityMap4kb(PtAlloc);
	}
	MAPPING_TABLE* Pt = (MAPPING_TABLE*)(Pdt->Entries[PdIndex] & PAGE_ADDR_MASK);
	if (!(Pt->Entries[PtIndex] & PAGE_BIT_P)) Pt->Entries[PtIndex] = (Logical & PAGE_ADDR_MASK) | Flags;
}

VOID SetupPaging() {
	for (UINTN i = 0; i < 4096; i++) ((UINT8*)&Pml4)[i] = 0;
	for (UINTN i = 0; i < 134217728; i += 4096) IdentityMap4kb(i);
	LoadPml4(&Pml4);
}
