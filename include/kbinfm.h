#pragma once

#include "efi/efidef.h"

#define KERNEL_GEORGE_BOOLE_01 { 0x0000, 0x0000, 0x0001 }
#define KERNEL_START_CODE_SIZE 48

#pragma pack(push, 1)
/// @brief Format of kernel binary file
typedef struct {
	UINT8 StartCode[KERNEL_START_CODE_SIZE];
	UINT8 DummyCode[32];
} KERNEL_BINARY;

typedef struct {
	UINT64 FoundHandleIndex;
	KERNEL_BINARY KernelBuffer;
} FOUND_KERNEL_INFO;
#pragma pack(pop)
