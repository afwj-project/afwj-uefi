#pragma once

#include "efi/efidef.h"

#define KERNEL_BINARY_TEXT 0x5E96
#define KERNEL_BINARY_DATA 0xA911
#define KERNEL_BINARY_RDATA 0xFD54
#define KERNEL_BINARY_PDATA 0xF50D
#define KERNEL_BINARY_XDATA 0x3D39
#define KERNEL_BINARY_BSS 0xC554
#define KERNEL_BINARY_EDATA 0x1FBD
#define KERNEL_BINARY_IDATA 0xEF39
#define KERNEL_BINARY_RELOC 0xC9E5

#pragma pack(push, 1)
typedef struct {
	UINT16 SectionCode;
	UINT64 SectionSize;
} KERNEL_BINARY_SECTION_INFO;
#pragma pack(pop)
