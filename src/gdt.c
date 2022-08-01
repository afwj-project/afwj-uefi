#include "gdt.h"

__attribute__((aligned(4096)))
GLOBAL_DESCRIPTOR_TABLE GDT = {
	{0, 0, 0, 0x00, 0x00, 0},
	{0, 0, 0, 0x9A, 0xA0, 0},
	{0, 0, 0, 0x92, 0xA0, 0},
	{0, 0, 0, 0x00, 0x00, 0},
	{0, 0, 0, 0x92, 0xA0, 0},
	{0, 0, 0, 0x9A, 0xA0, 0},
	{0, 0, 0, 0x92, 0xA0, 0},
	{0, 0, 0, 0x9A, 0xA0, 0},
	{0, 0, 0, 0x89, 0xA0, 0},
	{0, 0, 0, 0x00, 0x00, 0},
};

TASK_STATE_SEGMENT TSS;

VOID SetupGlobalDescriptorTable() {
	for (UINTN i = 0; i < sizeof(TASK_STATE_SEGMENT); i++) ((UINT8*)&TSS)[i] = 0;
	UINT64 TssBase = (UINT64)&TSS;
	GDT.TssLow.BaseAddress1 = TssBase & 0xFFFF;
	GDT.TssLow.BaseAddress2 = (TssBase >> 16) & 0xFF;
	GDT.TssLow.BaseAddress3 = (TssBase >> 24) & 0xFF;
	GDT.TssLow.SegmentLimit1 = sizeof(TSS);
	GDT.TssHigh.SegmentLimit1 = (TssBase >> 32) & 0xFFFF;
	GDT.TssHigh.BaseAddress1 = (TssBase >> 48) & 0xFFFF;
	TABLE_POINTER GdtPtr = { sizeof(GDT) - 1, (UINT64)&GDT };
	LoadGdt(&GdtPtr);
}
