#pragma once

#include "efibase.h"

#define SNAILFS_0_10_REVISION ((0 << 16) | (10))
#define SNAILFS_SPECIFICATION_VERSION SNAILFS_0_10_REVISION
#define SNAILFS_BOOT_SIGNATURE 0xAA55
#define SNAILFS_MAGIC_CODE 0x3053464C49414E53
#define SNAILFS_AFWJOS_CHECK 0x4E4B534F4A574641

#pragma pack(push, 1)
typedef struct {
	UINT8 Reserved[510];
	UINT16 Signature;
} SNAILFS_BOOT_RECORD;

typedef struct {
	UINT64 FileSystemChecking;
	UINT32 VersionInformation;
	UINT64 CurrentTableLength;
	UINT64 MaximumTableLength;
	EFI_LBA StartingDataAddress;
	EFI_LBA EndingDataAddress;
	UINT64 UsedTotalBlocks;
	UINT64 MaximumTotalBlocks;
	UINT64 OperatingSystemChecking;
	UINT8 Reserved[436];
} SNAILFS_TABLE_HEADER;

typedef struct {
	CHAR16 FilePath[1024];
	EFI_LBA StartingLBA;
	EFI_LBA EndingLBA;
	UINT64 FileSize;
	UINT64 FileAccess;
	UINT8 Reserved[2016];
} SNAILFS_DATA_TUPLE, *SNAILFS_DATA_TABLE;
#pragma pack(pop)
