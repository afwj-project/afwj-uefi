#pragma once

#include "efigvars.h"

#define UefiMemCpy gBS->CopyMem // ALIAS: Memory Copy Function
#define UefiMemSet gBS->SetMem // ALIAS: Memory Set Function

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

extern EFI_MEMORY_MAP MemoryMap;
extern UINT64 NextAllocPage;

VOID UefiMemInit();
VOID* UefiMalloc(IN UINTN Size);
EFI_STATUS UefiRealloc(IN OUT VOID** Pool, IN UINTN Size);
UINT64 UefiGetMemSize(IN VOID* Pool, OUT EFI_STATUS* StatusRef);
VOID UefiFree(IN VOID* Pool);
VOID* UefiAllocPage();
