#include "efi/efigvars.h"
#include "snailfs.h"

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
