#pragma once

#include "efigvars.h"

#define UefiMemCpy gBS->CopyMem // ALIAS: Memory Copy Function
#define UefiMemSet gBS->SetMem // ALIAS: Memory Set Function

#define PAGE_BIT_P (1 << 0)
#define PAGE_BIT_RW (1 << 1)
#define PAGE_BIT_US (1 << 2)
#define PAGE_BIT_A (1 << 5)
#define PAGE_BIT_D (1 << 6)
#define PAGE_XD_NX (1 << 63)
#define PAGE_ADDR_MASK 0x000FFFFFFFFFF000

typedef struct {
	UINT64 Size;
	VOID* Ptr;
} EFI_MEMORY_TUPLE;

typedef struct {
	UINTN MemoryMapSize;
	UINT8 MemoryMapBuffer[EFI_MEMORY_MAP_SIZE];
	UINTN MapKey;
	UINTN DescriptorSize;
	UINT32 DescriptorVersion;
} EFI_MEMORY_MAP;

#pragma pack(push, 1)
typedef struct {
	UINT64 Entries[512];
} MAPPING_TABLE;
#pragma pack(pop)

extern EFI_MEMORY_MAP MemoryMap;
extern UINT64 NextAllocPage;
extern MAPPING_TABLE Pml4;

VOID UefiMemInit();
VOID* UefiMalloc(IN UINTN Size);
EFI_STATUS UefiRealloc(IN OUT VOID** Pool, IN UINTN Size);
UINT64 UefiGetMemSize(IN VOID* Pool, OUT EFI_STATUS* StatusRef);
VOID UefiFree(IN VOID* Pool);
VOID* UefiAllocPage();
VOID IdentityMap4kb(UINT64 Logical);
extern VOID LoadPml4(MAPPING_TABLE* Pml4);
VOID SetupPaging();
