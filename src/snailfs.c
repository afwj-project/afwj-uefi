#include "efi/efigvars.h"
#include "efi/efimem.h"
#include "efi/efiutils.h"
#include "snailfs.h"

#define SNAILFS_FILE_READ 0x0000000000000001
#define SNAILFS_FILE_WRITE 0x0000000000000002
#define SNAILFS_ENDING_LINK 0xFFFFFFFFFFFFFFFF

SNAILFS_BOOT_RECORD* BootRecord = NULL;
SNAILFS_TABLE_HEADER* TableHdr = NULL;
SNAILFS_SECTOR* ZeroSector = NULL;
UINT64 NextFileDescriptor = 3;

EFI_STATUS UefiSnailFileSystemInit() {
	gZeroSector = (SNAILFS_SECTOR*)UefiMalloc(sizeof(SNAILFS_SECTOR));
	for (UINTN i = 0; i < sizeof(SNAILFS_SECTOR); i++) gZeroSector->SectorBuffer[i] = 0;
	return EFI_SUCCESS;
}

VOID UefiCheckSnailBootRecord(IN SNAILFS_BOOT_RECORD* BootRecord, OUT EFI_STATUS* StatusRef) {
	if (BootRecord->Signature != SNAILFS_BOOT_SIGNATURE) {
		gST->ConOut->OutputString(gST->ConOut, L"Boot signature was not found.\r\n");
		*StatusRef = EFI_NOT_FOUND;
	} else {
		gST->ConOut->OutputString(gST->ConOut, L"Boot signature was found.\r\n");
		*StatusRef = EFI_SUCCESS;
	}
}

VOID UefiCheckSnailTableHdr(IN SNAILFS_TABLE_HEADER* TableHdr, OUT EFI_STATUS* StatusRef) {
	if (TableHdr->FileSystemChecking != SNAILFS_MAGIC_CODE) {
		gST->ConOut->OutputString(gST->ConOut, L"Unsupported file system.\r\n");
		goto COMPATIBILITY_ERROR;
	}
	if (TableHdr->VersionInformation != SNAILFS_0_10_REVISION) {
		gST->ConOut->OutputString(gST->ConOut, L"Version of file system is not compatible with operating system.\r\n");
		goto COMPATIBILITY_ERROR;
	}
	if (TableHdr->OperatingSystemChecking != SNAILFS_AFWJOS_CHECK) {
		gST->ConOut->OutputString(gST->ConOut, L"Failed to check operating system.\r\n");
		goto COMPATIBILITY_ERROR;
	}
	gST->ConOut->OutputString(gST->ConOut, L"Supported file system was found.\r\n");
	goto CHECKING_PROFIT;
COMPATIBILITY_ERROR:
	*StatusRef = EFI_UNSUPPORTED;
	return;
CHECKING_PROFIT:
	*StatusRef = EFI_SUCCESS;
}

VOID UefiCheckSnailTableSize(
	IN SNAILFS_TABLE_HEADER* TableHdr,
	IN EFI_PARTITION_ENTRY* PartitionEntry,
	OUT EFI_STATUS* StatusRef
) {
	UINT64 FromEntry = PartitionEntry->EndingLBA - PartitionEntry->StartingLBA + 1;
	UINT64 FromHdr = 2 + TableHdr->MaximumTableLength * 8 + TableHdr->MaximumTotalBlocks;
	if (FromEntry != FromHdr) {
		gST->ConOut->OutputString(gST->ConOut, L"Failed to check size of partition.\r\n");
		goto INVALID_HEADER;
	}
	if (TableHdr->EndingDataAddress - TableHdr->StartingDataAddress + 1 != TableHdr->MaximumTotalBlocks) {
		gST->ConOut->OutputString(gST->ConOut, L"Failed to check size of data area.\r\n");
		goto INVALID_HEADER;
	}
	gST->ConOut->OutputString(gST->ConOut, L"Table header is normal.\r\n");
	goto CHECKING_PROFIT;
INVALID_HEADER:
	*StatusRef = EFI_DEVICE_ERROR;
	return;
CHECKING_PROFIT:
	*StatusRef = EFI_SUCCESS;
}

EFI_STATUS UefiGetSnailZeroIndex(OUT UINT64* IndexRef) {
	SNAILFS_DATA_TABLE DataTable = NULL;
	UINT64 SectorsPerTuple = sizeof(SNAILFS_DATA_TUPLE) / gOperatingSystemBlockIo->Media->BlockSize;
	UINT8* DataTableBuffer = (UINT8*)UefiMalloc(sizeof(SNAILFS_DATA_TUPLE));
	for (UINTN i = 0; i < gTableHdr->MaximumTableLength; i++) {
		gOperatingSystemBlockIo->ReadBlocks(
			gOperatingSystemBlockIo, gOperatingSystemBlockIo->Media->MediaId,
			gOperatingSystemEntry->StartingLBA + 2 + SectorsPerTuple * i,
			sizeof(SNAILFS_DATA_TUPLE), DataTableBuffer);
		DataTable = (SNAILFS_DATA_TABLE)DataTableBuffer;
		if (IsZeroSpace(DataTable, sizeof(SNAILFS_DATA_TUPLE))) {
			*IndexRef = i;
			UefiFree(DataTableBuffer);
			return EFI_SUCCESS;
		}
	}
	*IndexRef = 0;
	UefiFree(DataTableBuffer);
	return EFI_NOT_FOUND;
}

EFI_STATUS UefiGetZeroSectorIndex(OUT UINT64* IndexRef) {
	SNAILFS_SECTOR* DataSector = NULL;
	DataSector = (SNAILFS_SECTOR*)UefiMalloc(sizeof(SNAILFS_SECTOR));
	for (UINTN i = 0; i < gTableHdr->MaximumTotalBlocks; i++) {
		gOperatingSystemBlockIo->ReadBlocks(
			gOperatingSystemBlockIo, gOperatingSystemBlockIo->Media->MediaId,
			gTableHdr->StartingDataAddress, sizeof(SNAILFS_SECTOR), DataSector->SectorBuffer);
		if (IsZeroSpace(DataSector->SectorBuffer, sizeof(SNAILFS_SECTOR))) {
			*IndexRef = i;
			UefiFree(DataSector);
			return EFI_SUCCESS;
		}
	}
	*IndexRef = 0;
	UefiFree(DataSector);
	return EFI_NOT_FOUND;
}

EFI_STATUS UefiSnailFileSearch(IN CONST CHAR16* FilePath, OUT SNAILFS_DATA_TABLE SelectedTable) {
	SNAILFS_DATA_TABLE DataTable = NULL;
	UINT64 SectorsPerTuple = sizeof(SNAILFS_DATA_TUPLE) / gOperatingSystemBlockIo->Media->BlockSize;
	UINT8* DataTableBuffer = (UINT8*)UefiMalloc(sizeof(SNAILFS_DATA_TUPLE));
	for (UINTN i = 0; i < gTableHdr->MaximumTableLength; i++) {
		gOperatingSystemBlockIo->ReadBlocks(
			gOperatingSystemBlockIo, gOperatingSystemBlockIo->Media->MediaId,
			gOperatingSystemEntry->StartingLBA + 2 + SectorsPerTuple * i,
			sizeof(SNAILFS_DATA_TUPLE), DataTableBuffer);
		DataTable = (SNAILFS_DATA_TABLE)DataTableBuffer;
		if (IsZeroSpace(DataTable, sizeof(SNAILFS_DATA_TUPLE))) continue;
		if (!UefiWideStrCmp(FilePath, DataTable[i].FilePath)) {
			gBS->CopyMem(SelectedTable, DataTable, sizeof(SNAILFS_DATA_TUPLE));
			UefiFree(DataTableBuffer);
			return EFI_SUCCESS;
		}
	}
	UefiFree(DataTableBuffer);
	return EFI_NOT_FOUND;
}

SNAILFS_FILE* UefiSnailFileOpen(IN CONST CHAR16* FilePath, IN CHAR8 OpenType, OUT EFI_STATUS* StatusRef) {
	SNAILFS_FILE* Stream = NULL;
	SNAILFS_DATA_TABLE DataTable = (SNAILFS_DATA_TABLE)UefiMalloc(sizeof(SNAILFS_DATA_TUPLE));
	*StatusRef = UefiSnailFileSearch(FilePath, DataTable);
	Stream = (SNAILFS_FILE*)UefiMalloc(sizeof(SNAILFS_FILE));
	gBS->CopyMem((VOID*)Stream->FilePath, (VOID*)FilePath, sizeof(CHAR16) * 1024);
	if (OpenType == 'w') {
		Stream->FileDescriptor = NextFileDescriptor++;
		Stream->FileAddress = 0;
		Stream->FileSize = 0;
		Stream->FileControl = SNAILFS_FILE_WRITE;
		Stream->WriteBuffer = (UINT8*)UefiMalloc(504);
		if (*StatusRef == EFI_NOT_FOUND) Stream->CurrentLBA = 0;
		else Stream->CurrentLBA = DataTable->StartingLBA;
		goto END_OF_FUNCTION;
	} else if (OpenType == 'r') {
		if (*StatusRef == EFI_NOT_FOUND) goto ERROR_OF_FUNCTION;
		Stream->FileDescriptor = NextFileDescriptor++;
		Stream->FileAddress = 0;
		Stream->FileSize = DataTable->FileSize;
		Stream->FileControl = SNAILFS_FILE_READ;
		Stream->CurrentLBA = DataTable->StartingLBA;
		Stream->WriteBuffer = NULL;
		goto END_OF_FUNCTION;
	} else {
		*StatusRef = EFI_INVALID_PARAMETER;
		goto ERROR_OF_FUNCTION;
	}
ERROR_OF_FUNCTION:
	return NULL;
END_OF_FUNCTION:
	return Stream;
}

EFI_STATUS UefiSnailFileClose(IN SNAILFS_FILE* Stream) {
	SNAILFS_DATA_TABLE DataTable = (SNAILFS_DATA_TABLE)UefiMalloc(sizeof(SNAILFS_DATA_TUPLE));
	EFI_STATUS Status = UefiSnailFileSearch(Stream->FilePath, DataTable);
	if (Stream->FileControl == SNAILFS_FILE_WRITE && Status == EFI_NOT_FOUND) {
	} else if (Stream->FileControl == SNAILFS_FILE_WRITE && Status == EFI_SUCCESS) {
	}
	UefiFree(Stream);
	return EFI_SUCCESS;
}
