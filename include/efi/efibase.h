#pragma once

typedef void VOID;
typedef unsigned char BOOLEAN;
typedef char INT8;
typedef unsigned char UINT8;
typedef short INT16;
typedef unsigned short UINT16;
typedef int INT32;
typedef unsigned int UINT32;
typedef long long INT64;
typedef unsigned long long UINT64;
typedef char CHAR8;
typedef unsigned short CHAR16;
typedef INT64 INTN;
typedef UINT64 UINTN;
typedef UINTN EFI_STATUS;
typedef VOID* EFI_HANDLE;
typedef VOID* EFI_EVENT;
typedef UINT64 EFI_LBA;
typedef UINTN EFI_TPL;
typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;

#define FALSE ((BOOLEAN)(1 == 0))
#define TRUE ((BOOLEAN)(1 == 1))
#define IN
#define OUT
#define OPTIONAL
#define CONST const
#define NULL ((VOID*)0)
#define MAX_BIT 0x8000000000000000
#define EFI_DEVICE_PATH_PROTOCOL_GUID { 0x09576E91, 0x6D3F, 0x11D2, { 0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B } }

typedef enum {
	AllHandles,
	ByRegisterNotify,
	ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

typedef enum {
	TimerCancel,
	TimerPeriodic,
	TimerRelative
} EFI_TIMER_DELAY;

typedef enum {
	AllocateAnyPages,
	AllocateMaxAddress,
	AllocateAddress,
	MaxAllocateType
} EFI_ALLOCATE_TYPE;

typedef enum {
	EFI_NATIVE_INTERFACE
} EFI_INTERFACE_TYPE;

// TYPE: Memory Usages for UEFI
typedef enum {
	EfiReservedMemoryType,
	EfiLoaderCode,
	EfiLoaderData,
	EfiBootServicesCode,
	EfiBootServicesData,
	EfiRuntimeServicesCode,
	EfiRuntimeServicesData,
	EfiConventionalMemory,
	EfiUnusableMemory,
	EfiACPIReclaimMemory,
	EfiACPIMemoryNVS,
	EfiMemoryMappedIO,
	EfiMemoryMappedIOPortSpace,
	EfiPalCode,
	EfiPersistentMemory,
	EfiMaxMemoryType
} EFI_MEMORY_TYPE;

// TYPE: Computer Reset Methods for UEFI
typedef enum {
	EfiResetCold,
	EfiResetWarm,
	EfiResetShutdown,
	EfiResetPlatformSpecific
} EFI_RESET_TYPE;

// TYPE: Parity Bit Check Methods for UEFI
typedef enum {
	DefaultParity,
	NoParity,
	EvenParity,
	OddParity,
	MarkParity,
	SpaceParity
} EFI_PARITY_TYPE;

typedef enum {
	DefaultStopBits,
	OneStopBit,
	OneFiveStopBits,
	TwoStopBits
} EFI_STOP_BITS_TYPE;

// TYPE: GUID for UEFI
typedef struct {
	UINT32 Data1;
	UINT16 Data2;
	UINT16 Data3;
	UINT8 Data4[8];
} EFI_GUID;

typedef struct {
	UINT8 Addr[32];
} EFI_MAC_ADDRESS;

// TYPE: IPv4 Address for UEFI
typedef struct {
	UINT8 Addr[4];
} EFI_IPv4_ADDRESS;

// TYPE: IPv6 Address for UEFI
typedef struct {
	UINT8 Addr[16];
} EFI_IPv6_ADDRESS;

// TYPE: Data of Date & Time
typedef struct {
	UINT16 Year;
	UINT8 Month;
	UINT8 Day;
	UINT8 Hour;
	UINT8 Minute;
	UINT8 Second;
	UINT8 Pad1;
	UINT32 Nanosecond;
	INT16 TimeZone;
	UINT8 Daylight;
	UINT8 Pad2;
} EFI_TIME;

typedef struct {
	UINT32 Resolution;
	UINT32 Accuracy;
	BOOLEAN SetsToZero;
} EFI_TIME_CAPABILITIES;

// TYPE: Table Header for UEFI
typedef struct {
	UINT64 Signature;
	UINT32 Revision;
	UINT32 HeaderSize;
	UINT32 CRC32;
	UINT32 Reserved;
} EFI_TABLE_HEADER;

typedef struct {
	UINT16 ScanCode;
	CHAR16 UnicodeChar;
} EFI_INPUT_KEY;

/*
 * GUID: 09576E91-6D3F-11D2-8E3900A0C969723B
 * TYPE: Device Path Data
 */
typedef struct {
	UINT8 Type;
	UINT8 SubType;
	UINT8 Length[2];
} EFI_DEVICE_PATH_PROTOCOL;

typedef struct {
	INT32 MaxMode;
	INT32 Mode;
	INT32 Attribute;
	INT32 CursorColumn;
	INT32 CursorRow;
	BOOLEAN CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

typedef struct {
	EFI_GUID VendorGuid;
	VOID* VendorTable;
} EFI_CONFIGURATION_TABLE;

typedef struct {
	UINT32 Type;
	EFI_PHYSICAL_ADDRESS PhysicalStart;
	EFI_VIRTUAL_ADDRESS VirtualStart;
	UINT64 NumberOfPages;
	UINT64 Attribute;
} EFI_MEMORY_DESCRIPTOR;

typedef struct {
	EFI_HANDLE AgentHandle;
	EFI_HANDLE ControllerHandle;
	UINT32 Attributes;
	UINT32 OpenCount;
} EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;

typedef struct {
	EFI_GUID PackageListGuid;
	UINT32 PackageLength;
} EFI_HII_PACKAGE_LIST_HEADER, *EFI_HII_PACKAGE_LIST_PROTOCOL;

typedef struct {
	UINT64 Length;
	union {
		EFI_PHYSICAL_ADDRESS DataBlock;
		EFI_PHYSICAL_ADDRESS ContinuationPointer;
	} Union;
} EFI_CAPSULE_BLOCK_DESCRIPTOR;

typedef struct {
	EFI_GUID CapsuleGuid;
	UINT32 HeaderSize;
	UINT32 Flags;
	UINT32 CapsuleImageSize;
} EFI_CAPSULE_HEADER;

typedef struct {
	UINT32 CapsuleArrayNumber;
	VOID* CapsulePtr[1];
} EFI_CAPSULE_TABLE;

typedef struct {
	EFI_PHYSICAL_ADDRESS Address;
	UINT64 Length;
} EFI_MEMORY_RANGE;

typedef struct {
	UINT32 VariableTotalSize;
	UINT32 Reserved;
	EFI_GUID CapsuleGuid;
	EFI_TIME CapsuleProcessed;
	EFI_STATUS CapsuleStatus;
} EFI_CAPSULE_RESULT_VARIABLE_HEADER;

typedef struct {
	UINT32 ControlMask;
	UINT32 Timeout;
	UINT64 BaudRate;
	UINT32 ReceiveFifoDepth;
	UINT32 DataBits;
	UINT32 Parity;
	UINT32 StopBits;
} SERIAL_IO_MODE;
