#pragma once

#include "efi/efifpt.h"

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

/// @brief Begin of node
typedef struct {
	EFI_LBA PreviousAddress; ///< Must be 0xFFFFFFFFFFFFFFFF if it is the first node.
	CHAR16 FilePath[512];
	UINT64 SectorCount;
	UINT8 Reserved[1008];
} BON_SECTION;

/// @brief End of node
typedef struct {
	EFI_LBA NextAddress; ///< Must be 0xFFFFFFFFFFFFFFFF if it is the last node.
	CHAR16 FilePath[512];
	UINT8 Reserved[1016];
} EON_SECTION;

/// @brief Link of node
typedef union {
	BON_SECTION PreviousLink;
	EON_SECTION NextLink;
} LINK_SECTION;

typedef struct {
	CHAR16 FilePath[512];
	EFI_LBA FirstNodeAddress;
	UINT64 FileSize;
	UINT64 FileAccess;
	UINT8 Reserved[1000];
} SNAILFS_DATA_TUPLE, *SNAILFS_DATA_TABLE;

typedef struct {
	UINT8 SectorBuffer[512];
} SNAILFS_SECTOR;

typedef struct {
	UINT64 FileDescriptor;
	CHAR16 FilePath[512];
	UINT64 FileAddress;
	UINT64 FileSize;
	UINT64 FileControl;
	EFI_LBA FirstNodeAddress;
	UINT64 CurrentNodeIdx;
	EFI_LBA CurrentNodeAddress;
	UINT8* ReadBuffer;
	UINT8* WriteBuffer;
} SNAILFS_FILE;
#pragma pack(pop)

extern SNAILFS_BOOT_RECORD* BootRecord;
#define gBootRecord BootRecord
extern SNAILFS_TABLE_HEADER* TableHdr;
#define gTableHdr TableHdr
extern SNAILFS_SECTOR* ZeroSector;
#define gZeroSector ZeroSector

EFI_STATUS UefiSnailFileSystemInit();
VOID UefiCheckSnailBootRecord(IN SNAILFS_BOOT_RECORD* BootRecord, OUT EFI_STATUS* StatusRef);
VOID UefiCheckSnailTableHdr(IN SNAILFS_TABLE_HEADER* TableHdr, OUT EFI_STATUS* StatusRef);
VOID UefiCheckSnailTableSize(
	IN SNAILFS_TABLE_HEADER* TableHdr,
	IN EFI_PARTITION_ENTRY* PartitionEntry,
	OUT EFI_STATUS* StatusRef);

EFI_STATUS UefiGetSnailZeroIndex(OUT UINT64* IndexRef);
EFI_STATUS UefiGetZeroSectorIndex(OUT UINT64* IndexRef);

EFI_STATUS UefiGetDataTable(OUT SNAILFS_DATA_TABLE AllTableDst);
EFI_STATUS UefiSnailFileSearch(IN CONST CHAR16* FilePath, OUT SNAILFS_DATA_TABLE SelectedTable);
SNAILFS_FILE* UefiSnailFileOpen(IN CONST CHAR16* FilePath, IN CHAR8 OpenType, OUT EFI_STATUS* StatusRef);
EFI_STATUS UefiSnailFileClose(IN SNAILFS_FILE* Stream);

UINTN UefiSnailFileRead(OUT VOID* DstBuffer, IN UINTN PartSize, IN OUT SNAILFS_FILE* Stream);
UINTN UefiSnailFileWrite(IN CONST VOID* SrcBuffer, IN UINTN PartSize, IN OUT SNAILFS_FILE* Stream);
