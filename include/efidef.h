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

typedef enum _EFI_LOCATE_SEARCH_TYPE {
	AllHandles,
	ByRegisterNotify,
	ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

typedef enum _EFI_TIMER_DELAY {
	TimerCancel,
	TimerPeriodic,
	TimerRelative
} EFI_TIMER_DELAY;

typedef enum _EFI_ALLOCATE_TYPE {
	AllocateAnyPages,
	AllocateMaxAddress,
	AllocateAddress,
	MaxAllocateType
} EFI_ALLOCATE_TYPE;

typedef enum _EFI_INTERFACE_TYPE {
	EFI_NATIVE_INTERFACE
} EFI_INTERFACE_TYPE;

typedef enum _EFI_MEMORY_TYPE {
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

typedef enum _EFI_RESET_TYPE {
	EfiResetCold,
	EfiResetWarm,
	EfiResetShutdown,
	EfiResetPlatformSpecific
} EFI_RESET_TYPE;

typedef enum _EFI_PARITY_TYPE {
	DefaultParity,
	NoParity,
	EvenParity,
	OddParity,
	MarkParity,
	SpaceParity
} EFI_PARITY_TYPE;

typedef enum _EFI_STOP_BITS_TYPE {
	DefaultStopBits,
	OneStopBit,
	OneFiveStopBits,
	TwoStopBits
} EFI_STOP_BITS_TYPE;

typedef struct _EFI_GUID {
	UINT32 Data1;
	UINT16 Data2;
	UINT16 Data3;
	UINT8 Data4[8];
} EFI_GUID;

typedef struct _EFI_MAC_ADDRESS {
	UINT8 Addr[32];
} EFI_MAC_ADDRESS;

typedef struct _EFI_IPv4_ADDRESS {
	UINT8 Addr[4];
} EFI_IPv4_ADDRESS;

typedef struct _EFI_IPv6_ADDRESS {
	UINT8 Addr[16];
} EFI_IPv6_ADDRESS;

typedef struct _EFI_TIME {
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

typedef struct _EFI_TIME_CAPABILITIES {
	UINT32 Resolution;
	UINT32 Accuracy;
	BOOLEAN SetsToZero;
} EFI_TIME_CAPABILITIES;

#define EFI_SYSTEM_TABLE_SIGNATURE 0x5453595320494249
#define EFI_2_80_SYSTEM_TABLE_REVISION ((2 << 16) | (80))
#define EFI_2_70_SYSTEM_TABLE_REVISION ((2 << 16) | (70))
#define EFI_2_60_SYSTEM_TABLE_REVISION ((2 << 16) | (60))
#define EFI_2_50_SYSTEM_TABLE_REVISION ((2 << 16) | (50))
#define EFI_2_40_SYSTEM_TABLE_REVISION ((2 << 16) | (40))
#define EFI_SYSTEM_TABLE_REVISION EFI_2_80_SYSTEM_TABLE_REVISION
#define EFI_SPECIFICATION_VERSION EFI_SYSTEM_TABLE_REVISION

typedef struct _EFI_TABLE_HEADER {
	UINT64 Signature;
	UINT32 Revision;
	UINT32 HeaderSize;
	UINT32 CRC32;
	UINT32 Reserved;
} EFI_TABLE_HEADER;

typedef struct _EFI_INPUT_KEY {
	UINT16 ScanCode;
	CHAR16 UnicodeChar;
} EFI_INPUT_KEY;

typedef struct _EFI_DEVICE_PATH_PROTOCOL {
	UINT8 Type;
	UINT8 SubType;
	UINT8 Length[2];
} EFI_DEVICE_PATH_PROTOCOL;

typedef struct _SIMPLE_TEXT_OUTPUT_MODE {
	INT32 MaxMode;
	INT32 Mode;
	INT32 Attribute;
	INT32 CursorColumn;
	INT32 CursorRow;
	BOOLEAN CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

typedef struct _EFI_CONFIGURATION_TABLE {
	EFI_GUID VendorGuid;
	VOID* VendorTable;
} EFI_CONFIGURATION_TABLE;

typedef struct _EFI_MEMORY_DESCRIPTOR {
	UINT32 Type;
	EFI_PHYSICAL_ADDRESS PhysicalStart;
	EFI_VIRTUAL_ADDRESS VirtualStart;
	UINT64 NumberOfPages;
	UINT64 Attribute;
} EFI_MEMORY_DESCRIPTOR;

typedef struct _EFI_OPEN_PROTOCOL_INFORMATION_ENTRY {
	EFI_HANDLE AgentHandle;
	EFI_HANDLE ControllerHandle;
	UINT32 Attributes;
	UINT32 OpenCount;
} EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;

typedef struct _EFI_HII_PACKAGE_LIST_HEADER {
	EFI_GUID PackageListGuid;
	UINT32 PackageLength;
} EFI_HII_PACKAGE_LIST_HEADER, *EFI_HII_PACKAGE_LIST_PROTOCOL;

typedef struct _EFI_CAPSULE_BLOCK_DESCRIPTOR {
	UINT64 Length;
	union _EFI_CAPSULE_BLOCK_ADDRESS {
		EFI_PHYSICAL_ADDRESS DataBlock;
		EFI_PHYSICAL_ADDRESS ContinuationPointer;
	} Union;
} EFI_CAPSULE_BLOCK_DESCRIPTOR;

typedef struct _EFI_CAPSULE_HEADER {
	EFI_GUID CapsuleGuid;
	UINT32 HeaderSize;
	UINT32 Flags;
	UINT32 CapsuleImageSize;
} EFI_CAPSULE_HEADER;

typedef struct _EFI_CAPSULE_TABLE {
	UINT32 CapsuleArrayNumber;
	VOID* CapsulePtr[1];
} EFI_CAPSULE_TABLE;

typedef struct _EFI_MEMORY_RANGE {
	EFI_PHYSICAL_ADDRESS Address;
	UINT64 Length;
} EFI_MEMORY_RANGE;

typedef struct _EFI_CAPSULE_RESULT_VARIABLE_HEADER {
	UINT32 VariableTotalSize;
	UINT32 Reserved;
	EFI_GUID CapsuleGuid;
	EFI_TIME CapsuleProcessed;
	EFI_STATUS CapsuleStatus;
} EFI_CAPSULE_RESULT_VARIABLE_HEADER;

typedef struct _SERIAL_IO_MODE {
	UINT32 ControlMask;
	UINT32 Timeout;
	UINT64 BaudRate;
	UINT32 ReceiveFifoDepth;
	UINT32 DataBits;
	UINT32 Parity;
	UINT32 StopBits;
} SERIAL_IO_MODE;

struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
struct _EFI_BOOT_SERVICES;
typedef struct _EFI_BOOT_SERVICES EFI_BOOT_SERVICES;
struct _EFI_SYSTEM_TABLE;
typedef struct _EFI_SYSTEM_TABLE EFI_SYSTEM_TABLE;
struct _EFI_SERIAL_IO_PROTOCOL;
typedef struct _EFI_SERIAL_IO_PROTOCOL EFI_SERIAL_IO_PROTOCOL;

typedef EFI_STATUS(*EFI_IMAGE_ENTRY_POINT)(
	IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable);

#define EFI_SUCCESS 0
#define EFI_NOT_READY ((EFI_STATUS)(MAX_BIT | 6))

typedef VOID(*EFI_EVENT_NOTIFY)(
	IN EFI_EVENT Event, IN VOID* Context);

#define EVT_TIMER 0x80000000
#define EVT_RUNTIME 0x40000000
#define EVT_NOTIFY_WAIT 0x00000100
#define EVT_NOTIFY_SIGNAL 0x00000200
#define EVT_SIGNAL_EXIT_BOOT_SERVICES 0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE 0x60000202

typedef EFI_STATUS(*EFI_CREATE_EVENT)(
	IN UINT32 Type, IN EFI_TPL NotifyTpl,
	IN EFI_EVENT_NOTIFY NotifyFunction OPTIONAL,
	IN VOID* NotifyContext OPTIONAL, OUT EFI_EVENT* Event);

typedef EFI_STATUS(*EFI_CREATE_EVENT_EX)(
	IN UINT32 Type, IN EFI_TPL NotifyTpl,
	IN EFI_EVENT_NOTIFY NotifyFunction OPTIONAL,
	IN CONST VOID* NotifyContext OPTIONAL,
	IN CONST EFI_GUID* EventGroup OPTIONAL,
	OUT EFI_EVENT* Event);

#define EFI_EVENT_GROUP_EXIT_BOOT_SERVICES { 0x27ABF055, 0xB1B8, 0x4C26, { 0x80, 0x48, 0x74, 0x8F, 0x37, 0xBA, 0xA2, 0xDF } }
#define EFI_EVENT_GROUP_VIRTUAL_ADDRESS_CHANGE { 0x13FA7698, 0xC831, 0x49C7, { 0x87, 0xEA, 0x8F, 0x43, 0xFC, 0xC2, 0x51, 0x96 } }
#define EFI_EVENT_GROUP_MEMORY_MAP_CHANGE { 0x78BEE926, 0x692F, 0x48FD, { 0x9E, 0xDB, 0x01, 0x42, 0x2E, 0xF0, 0xD7, 0xAB } }
#define EFI_EVENT_GROUP_READY_TO_BOOT { 0x7CE88FB3, 0x4BD7, 0x4679, { 0x87, 0xA8, 0xA8, 0xD8, 0xDE, 0xE5, 0x0D, 0x2B } }
#define EFI_EVENT_GROUP_RESET_SYSTEM { 0x62DA6A56, 0x13FB, 0x485A, { 0xA8, 0xDA, 0xA3, 0xDD, 0x79, 0x12, 0xCB, 0x6B } }

typedef EFI_STATUS(*EFI_WAIT_FOR_EVENT)(
	IN UINTN NumberOfEvents,
	IN EFI_EVENT* Event, OUT UINTN* Index);

typedef EFI_STATUS(*EFI_SET_TIMER)(
	IN EFI_EVENT Event,
	IN EFI_TIMER_DELAY Type,
	IN UINT64 TriggerTime);

typedef EFI_STATUS(*EFI_EXIT)(
	IN EFI_HANDLE ImageHandle, IN EFI_STATUS ExitStatus,
	IN UINTN ExitDataSize, IN CHAR16* ExitData OPTIONAL);

typedef EFI_STATUS(*EFI_EXIT_BOOT_SERVICES)(
	IN EFI_HANDLE ImageHandle, IN UINTN MapKey);

#define TPL_APPLICATION 4
#define TPL_CALLBACK 8
#define TPL_NOTIFY 16
#define TPL_HIGH_LEVEL 31

typedef EFI_STATUS(*EFI_FREE_POOL)(IN VOID* Buffer);
typedef EFI_STATUS(*EFI_IMAGE_UNLOAD)(IN EFI_HANDLE ImageHandle);
typedef EFI_TPL(*EFI_RAISE_TPL)(IN EFI_TPL NewTpl);
typedef VOID(*EFI_RESTORE_TPL)(IN EFI_TPL OldTpl);
typedef EFI_STATUS(*EFI_CLOSE_EVENT)(IN EFI_EVENT Event);
typedef EFI_STATUS(*EFI_SIGNAL_EVENT)(IN EFI_EVENT Event);
typedef EFI_STATUS(*EFI_CHECK_EVENT)(IN EFI_EVENT Event);
typedef EFI_STATUS(*EFI_STALL)(IN UINTN MicroSeconds);
typedef EFI_STATUS(*EFI_GET_NEXT_MONOTONIC_COUNT)(OUT UINT64* Count);
typedef EFI_STATUS(*EFI_SET_TIME)(IN EFI_TIME* Time);
typedef EFI_STATUS(*EFI_GET_NEXT_HIGH_MONO_COUNT)(OUT UINT32* HighCount);
typedef EFI_STATUS(*EFI_SERIAL_RESET)(IN EFI_SERIAL_IO_PROTOCOL* This);

typedef EFI_STATUS(*EFI_ALLOCATE_PAGES)(
	IN EFI_ALLOCATE_TYPE Type, IN EFI_MEMORY_TYPE MemoryType,
	IN UINTN Pages, IN OUT EFI_PHYSICAL_ADDRESS* Memory);

typedef EFI_STATUS(*EFI_FREE_PAGES)(
	IN EFI_PHYSICAL_ADDRESS Memory, IN UINTN Pages);

typedef EFI_STATUS(*EFI_ALLOCATE_POOL)(
	IN EFI_MEMORY_TYPE PoolType,
	IN UINTN Size, OUT VOID** Buffer);

typedef EFI_STATUS(*EFI_GET_MEMORY_MAP)(
	IN OUT UINTN* MemoryMapSize, OUT EFI_MEMORY_DESCRIPTOR* MemoryMap,
	OUT UINTN* MapKey, OUT UINTN* DescriptorSize, OUT UINT32* DescriptorVersion);

#define EFI_MEMORY_UC 0x0000000000000001
#define EFI_MEMORY_WC 0x0000000000000002
#define EFI_MEMORY_WT 0x0000000000000004
#define EFI_MEMORY_WB 0x0000000000000008
#define EFI_MEMORY_UCE 0x0000000000000010
#define EFI_MEMORY_WP 0x0000000000001000
#define EFI_MEMORY_RP 0x0000000000002000
#define EFI_MEMORY_XP 0x0000000000004000
#define EFI_MEMORY_NV 0x0000000000008000
#define EFI_MEMORY_MORE_RELIABLE 0x0000000000010000
#define EFI_MEMORY_RO 0x0000000000020000
#define EFI_MEMORY_SP 0x0000000000040000
#define EFI_MEMORY_CPU_CRYPTO 0x0000000000080000
#define EFI_MEMORY_RUNTIME 0x8000000000000000
#define EFI_MEMORY_DESCRIPTOR_VERSION 1

typedef EFI_STATUS(*EFI_INSTALL_PROTOCOL_INTERFACE)(
	IN OUT EFI_HANDLE* Handle, IN EFI_GUID* Protocol,
	IN EFI_INTERFACE_TYPE InterfaceType, IN VOID* Interface);

typedef EFI_STATUS(*EFI_UNINSTALL_PROTOCOL_INTERFACE)(
	IN EFI_HANDLE Handle, IN EFI_GUID* Protocol, IN VOID* Interface);

typedef EFI_STATUS(*EFI_REINSTALL_PROTOCOL_INTERFACE)(
	IN EFI_HANDLE Handle, IN EFI_GUID* Protocol,
	IN VOID* OldInterface, IN VOID* NewInterface);

typedef EFI_STATUS(*EFI_REGISTER_PROTOCOL_NOTIFY)(
	IN EFI_GUID* Protocol, IN EFI_EVENT Event, OUT VOID** Registration);

typedef EFI_STATUS(*EFI_LOCATE_HANDLE)(
	IN EFI_LOCATE_SEARCH_TYPE SearchType, IN EFI_GUID* Protocol OPTIONAL,
	IN VOID* SearchKey OPTIONAL, IN OUT UINTN* BufferSize, OUT EFI_HANDLE* Buffer);

typedef EFI_STATUS(*EFI_HANDLE_PROTOCOL)(
	IN EFI_HANDLE Handle, IN EFI_GUID* Protocol, OUT VOID** Interface);

typedef EFI_STATUS(*EFI_LOCATE_DEVICE_PATH)(
	IN EFI_GUID* Protocol,
	IN OUT EFI_DEVICE_PATH_PROTOCOL** DevicePath,
	OUT EFI_HANDLE* Device);

typedef EFI_STATUS(*EFI_OPEN_PROTOCOL)(
	IN EFI_HANDLE Handle, IN EFI_GUID* Protocol, OUT VOID** Interface OPTIONAL,
	IN EFI_HANDLE AgentHandle, IN EFI_HANDLE ControllerHandle, IN UINT32 Attributes);

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL 0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL 0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL 0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER 0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE 0x00000020

typedef EFI_STATUS(*EFI_CLOSE_PROTOCOL)(
	IN EFI_HANDLE Handle, IN EFI_GUID* Protocol,
	IN EFI_HANDLE AgentHandle, IN EFI_HANDLE ControllerHandle);

typedef EFI_STATUS(*EFI_OPEN_PROTOCOL_INFORMATION)(
	IN EFI_HANDLE Handle, IN EFI_GUID* Protocol,
	OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY** EntryBuffer,
	OUT UINTN* EntryCount);

typedef EFI_STATUS(*EFI_CONNECT_CONTROLLER)(
	IN EFI_HANDLE ControllerHandle, IN EFI_HANDLE* DriverImageHandle OPTIONAL,
	IN EFI_DEVICE_PATH_PROTOCOL* RemainingDevicePath OPTIONAL, IN BOOLEAN Recursive);

typedef EFI_STATUS(*EFI_DISCONNECT_CONTROLLER)(
	IN EFI_HANDLE ControllerHandle,
	IN EFI_HANDLE DriverImageHandle OPTIONAL,
	IN EFI_HANDLE ChildHandle OPTIONAL);

typedef EFI_STATUS(*EFI_PROTOCOLS_PER_HANDLE)(
	IN EFI_HANDLE Handle,
	OUT EFI_GUID*** ProtocolBuffer,
	OUT UINTN* ProtocolBufferCount);

typedef EFI_STATUS(*EFI_LOCATE_HANDLE_BUFFER)(
	IN EFI_LOCATE_SEARCH_TYPE SearchType, IN EFI_GUID* Protocol OPTIONAL,
	IN VOID* SearchKey OPTIONAL, OUT UINTN* NoHandles, OUT EFI_HANDLE** Buffer);

typedef EFI_STATUS(*EFI_LOCATE_PROTOCOL)(
	IN EFI_GUID* Protocol,
	IN VOID* Registration OPTIONAL,
	OUT VOID** Interface);

typedef EFI_STATUS(*EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES)(
	IN OUT EFI_HANDLE* Handle, ...);

typedef EFI_STATUS(*EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES)(
	IN EFI_HANDLE Handle, ...);

typedef EFI_STATUS(*EFI_IMAGE_LOAD)(
	IN BOOLEAN BootPolicy,
	IN EFI_HANDLE ParentImageHandle,
	IN EFI_DEVICE_PATH_PROTOCOL* DevicePath,
	IN VOID* SourceBuffer OPTIONAL,
	IN UINTN SourceSize,
	OUT EFI_HANDLE* ImageHandle);

#define EFI_HII_PACKAGE_LIST_PROTOCOL_GUID { 0x6A1EE763, 0xD47A, 0x43B4, { 0xAA, 0xBE, 0xEF, 0x1D, 0xE2, 0xAB, 0x56, 0xFC } }
#define EFI_BOOT_SERVICES_SIGNATURE 0x56524553544F4F42
#define EFI_BOOT_SERVICES_REVISION EFI_SPECIFICATION_VERSION

typedef EFI_STATUS(*EFI_IMAGE_START)(
	IN EFI_HANDLE ImageHandle,
	OUT UINTN* ExitDataSize,
	OUT CHAR16** ExitData OPTIONAL);

typedef EFI_STATUS(*EFI_SET_WATCHDOG_TIMER)(
	IN UINTN Timeout, IN UINT64 WatchdogCode,
	IN UINTN DataSize, IN CHAR16* WatchdogData OPTIONAL);

typedef VOID(*EFI_COPY_MEM)(
	IN VOID* Destination,
	IN VOID* Source,
	IN UINTN Length);

typedef VOID(*EFI_SET_MEM)(
	IN VOID* Buffer,
	IN UINTN Size,
	IN UINT8 Value);

typedef EFI_STATUS(*EFI_INSTALL_CONFIGURATION_TABLE)(
	IN EFI_GUID* Guid, IN VOID* Table);

typedef EFI_STATUS(*EFI_CALCULATE_CRC32)(
	IN VOID* Data, IN UINTN DataSize, OUT UINT32* Crc32);

struct _EFI_BOOT_SERVICES {
	EFI_TABLE_HEADER Hdr;
	EFI_RAISE_TPL RaiseTPL;
	EFI_RESTORE_TPL RestoreTPL;
	EFI_ALLOCATE_PAGES AllocatePages;
	EFI_FREE_PAGES FreePages;
	EFI_GET_MEMORY_MAP GetMemoryMap;
	EFI_ALLOCATE_POOL AllocatePool;
	EFI_FREE_POOL FreePool;
	EFI_CREATE_EVENT CreateEvent;
	EFI_SET_TIMER SetTimer;
	EFI_WAIT_FOR_EVENT WaitForEvent;
	EFI_SIGNAL_EVENT SignalEvent;
	EFI_CLOSE_EVENT CloseEvent;
	EFI_CHECK_EVENT CheckEvent;
	EFI_INSTALL_PROTOCOL_INTERFACE InstallProtocolInterface;
	EFI_REINSTALL_PROTOCOL_INTERFACE ReinstallProtocolInterface;
	EFI_UNINSTALL_PROTOCOL_INTERFACE UninstallProtocolInterface;
	EFI_HANDLE_PROTOCOL HandleProtocol;
	VOID* Reserved;
	EFI_REGISTER_PROTOCOL_NOTIFY RegisterProtocolNotify;
	EFI_LOCATE_HANDLE LocateHandle;
	EFI_LOCATE_DEVICE_PATH LocateDevicePath;
	EFI_INSTALL_CONFIGURATION_TABLE InstallConfigurationTable;
	EFI_IMAGE_LOAD LoadImage;
	EFI_IMAGE_START StartImage;
	EFI_EXIT Exit;
	EFI_IMAGE_UNLOAD UnloadImage;
	EFI_EXIT_BOOT_SERVICES ExitBootServices;
	EFI_GET_NEXT_MONOTONIC_COUNT GetNextMonotonicCount;
	EFI_STALL Stall;
	EFI_SET_WATCHDOG_TIMER SetWatchdogTimer;
	EFI_CONNECT_CONTROLLER ConnectController;
	EFI_DISCONNECT_CONTROLLER DisconnectController;
	EFI_OPEN_PROTOCOL OpenProtocol;
	EFI_CLOSE_PROTOCOL CloseProtocol;
	EFI_OPEN_PROTOCOL_INFORMATION OpenProtocolInformation;
	EFI_PROTOCOLS_PER_HANDLE ProtocolsPerHandle;
	EFI_LOCATE_HANDLE_BUFFER LocateHandleBuffer;
	EFI_LOCATE_PROTOCOL LocateProtocol;
	EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES InstallMultipleProtocolInterfaces;
	EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES UninstallMultipleProtocolInterfaces;
	EFI_CALCULATE_CRC32 CalculateCrc32;
	EFI_COPY_MEM CopyMem;
	EFI_SET_MEM SetMem;
	EFI_CREATE_EVENT_EX CreateEventEx;
};

typedef EFI_STATUS(*EFI_GET_VARIABLE)(
	IN CHAR16* VariableName,
	IN EFI_GUID* VendorGuid,
	OUT UINT32* Attributes OPTIONAL,
	IN OUT UINTN* DataSize,
	OUT VOID* Data OPTIONAL);

#define EFI_VARIABLE_NON_VOLATILE 0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS 0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS 0x00000004
#define EFI_VARIABLE_HARDWARE_ERROR_RECORD 0x00000008
#define EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS 0x00000010
#define EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS 0x00000020
#define EFI_VARIABLE_APPEND_WRITE 0x00000040
#define EFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS 0x00000080
#define EFI_OPTIONAL_PTR 0x00000001

typedef EFI_STATUS(*EFI_GET_NEXT_VARIABLE_NAME)(
	IN OUT UINTN* VariableNameSize,
	IN OUT CHAR16* VariableName,
	IN OUT EFI_GUID* VendorGuid);

typedef EFI_STATUS(*EFI_SET_VARIABLE)(
	IN CHAR16* VariableName, IN EFI_GUID* VendorGuid,
	IN UINT32 Attributes, IN UINTN DataSize, IN VOID* Data);

typedef EFI_STATUS(*EFI_QUERY_VARIABLE_INFO)(
	IN UINT32 Attributes,
	OUT UINT64* MaximumVariableStorageSize,
	OUT UINT64* RemainingVariableStorageSize,
	OUT UINT64* MaximumVariableSize);

typedef EFI_STATUS(*EFI_GET_TIME)(
	OUT EFI_TIME* Time, OUT EFI_TIME_CAPABILITIES* Capabilities OPTIONAL);

typedef EFI_STATUS(*EFI_GET_WAKEUP_TIME)(
	OUT BOOLEAN* Enabled, OUT BOOLEAN* Pending, OUT EFI_TIME* Time);

typedef EFI_STATUS(*EFI_SET_WAKEUP_TIME)(
	IN BOOLEAN Enable, IN EFI_TIME* Time OPTIONAL);

typedef EFI_STATUS(*EFI_SET_VIRTUAL_ADDRESS_MAP)(
	IN UINTN MemorySize, IN UINTN DescriptorSize,
	IN UINT32 DescriptorVersion,
	IN EFI_MEMORY_DESCRIPTOR* VirtualMap);

typedef EFI_STATUS(*EFI_CONVERT_POINTER)(
	IN UINTN DebugDisposition, IN VOID** Address);

typedef VOID(*EFI_RESET_SYSTEM)(
	IN EFI_RESET_TYPE ResetType, IN EFI_STATUS ResetStatus,
	IN UINTN DataSize, IN VOID* ResetData OPTIONAL);

#define CAPSULE_FLAGS_PERSIST_ACROSS_RESET 0x00010000
#define CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE 0x00020000
#define CAPSULE_FLAGS_INITIATE_RESET 0x00040000
#define EFI_MEMORY_RANGE_CAPSULE_GUID { 0xDE9F0EC, 0x88B6, 0x428F, { 0x97, 0x7A, 0x25, 0x8F, 0x1D, 0x0E, 0x5E, 0x72 } }

typedef EFI_STATUS(*EFI_UPDATE_CAPSULE)(
	IN EFI_CAPSULE_HEADER** CapsuleHeaderArray, IN UINTN CapsuleCount,
	IN EFI_PHYSICAL_ADDRESS ScatterGatherList OPTIONAL);

typedef EFI_STATUS(*EFI_QUERY_CAPSULE_CAPABILITIES)(
	IN EFI_CAPSULE_HEADER** CapsuleHeaderArray, IN UINTN CapsuleCount,
	OUT UINT64* MaximumCapsuleSize, OUT EFI_RESET_TYPE* ResetType);

#define EFI_OS_INDICATIONS_BOOT_TO_FW_UI 0x0000000000000001
#define EFI_OS_INDICATIONS_TIMESTAMP_REVOCATION 0x0000000000000002
#define EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED 0x0000000000000004
#define EFI_OS_INDICATIONS_FMP_CAPSULE_SUPPORTED 0x0000000000000008
#define EFI_OS_INDICATIONS_CAPSULE_RESULT_VAR_SUPPORTED 0x0000000000000010
#define EFI_OS_INDICATIONS_START_OS_RECOVERY 0x0000000000000020
#define EFI_OS_INDICATIONS_START_PLATFORM_RECOVERY 0x0000000000000040
#define EFI_OS_INDICATIONS_JSON_CONFIG_DATA_REFRESH 0x0000000000000080
#define EFI_RUNTIME_SERVICES_SIGNATURE 0x56524553544E5552
#define EFI_RUNTIME_SERVICES_REVISION EFI_SPECIFICATION_VERSION

typedef struct _EFI_RUNTIME_SERVICES {
	EFI_TABLE_HEADER Hdr;
	EFI_GET_TIME GetTime;
	EFI_SET_TIME SetTime;
	EFI_GET_WAKEUP_TIME GetWakeupTime;
	EFI_SET_WAKEUP_TIME SetWakeupTime;
	EFI_SET_VIRTUAL_ADDRESS_MAP SetVirtualAddressMap;
	EFI_CONVERT_POINTER ConvertPointer;
	EFI_GET_VARIABLE GetVariable;
	EFI_GET_NEXT_VARIABLE_NAME GetNextVariableName;
	EFI_SET_VARIABLE SetVariable;
	EFI_GET_NEXT_HIGH_MONO_COUNT GetNextHighMonotonicCount;
	EFI_RESET_SYSTEM ResetSystem;
	EFI_UPDATE_CAPSULE UpdateCapsule;
	EFI_QUERY_CAPSULE_CAPABILITIES QueryCapsuleCapabilities;
	EFI_QUERY_VARIABLE_INFO QueryVariableInfo;
} EFI_RUNTIME_SERVICES;

#define EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID { 0x387477C2, 0x69C7, 0x11D2, { 0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B } }
#define EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID { 0x387477C1, 0x69C7, 0x11D2, { 0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B } }

typedef EFI_STATUS(*EFI_INPUT_RESET)(
	IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL* This,
	IN BOOLEAN ExtendedVerification);

typedef EFI_STATUS(*EFI_INPUT_READ_KEY)(
	IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL* This,
	OUT EFI_INPUT_KEY* Key);

struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
	EFI_INPUT_RESET Reset;
	EFI_INPUT_READ_KEY ReadKeyStroke;
	EFI_EVENT WaitForKey;
};

typedef EFI_STATUS(*EFI_TEXT_RESET)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This,
	IN BOOLEAN ExtendedVerification);

typedef EFI_STATUS(*EFI_TEXT_STRING)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This,
	IN CHAR16* String);

typedef EFI_STATUS(*EFI_TEXT_TEST_STRING)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This,
	IN CHAR16* String);

typedef EFI_STATUS(*EFI_TEXT_QUERY_MODE)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This,
	IN UINTN ModeNumber, OUT UINTN* Columns, OUT UINTN* Rows);

typedef EFI_STATUS(*EFI_TEXT_SET_MODE)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, IN UINTN ModeNumber);

typedef EFI_STATUS(*EFI_TEXT_SET_ATTRIBUTE)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, IN UINTN Attribute);

typedef EFI_STATUS(*EFI_TEXT_CLEAR_SCREEN)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This);

typedef EFI_STATUS(*EFI_TEXT_SET_CURSOR_POSITION)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This,
	IN UINTN Column, IN UINTN Row);

typedef EFI_STATUS(*EFI_TEXT_ENABLE_CURSOR)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, IN BOOLEAN Visible);

struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
	EFI_TEXT_RESET Reset;
	EFI_TEXT_STRING OutputString;
	EFI_TEXT_TEST_STRING TestString;
	EFI_TEXT_QUERY_MODE QueryMode;
	EFI_TEXT_SET_MODE SetMode;
	EFI_TEXT_SET_ATTRIBUTE SetAttribute;
	EFI_TEXT_CLEAR_SCREEN ClearScreen;
	EFI_TEXT_SET_CURSOR_POSITION SetCursorPosition;
	EFI_TEXT_ENABLE_CURSOR EnableCursor;
	SIMPLE_TEXT_OUTPUT_MODE* Mode;
};

struct _EFI_SYSTEM_TABLE {
	EFI_TABLE_HEADER Hdr;
	CHAR16* FirmwareVendor;
	UINT32 FirmwareRevision;
	EFI_HANDLE ConsoleInHandle;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL* ConIn;
	EFI_HANDLE ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut;
	EFI_HANDLE StandardErrorHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* StdErr;
	EFI_RUNTIME_SERVICES* RuntimeServices;
	EFI_BOOT_SERVICES* BootServices;
	UINTN NumberOfTableEntries;
	EFI_CONFIGURATION_TABLE* ConfigurationTable;
};

#define EFI_SERIAL_IO_PROTOCOL_GUID { 0xBB25CF6F, 0xF1D4, 0x11D2, { 0x9A, 0x0C, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0xFD } }
#define EFI_SERIAL_IO_PROTOCOL_REVISION 0x00010000
#define EFI_SERIAL_IO_PROTOCOL_REVISION1p1 0x00010001

typedef EFI_STATUS(*EFI_SERIAL_SET_ATTRIBUTES)(
	IN EFI_SERIAL_IO_PROTOCOL* This, IN UINT64 BaudRate,
	IN UINT32 ReceiveFifoDepth, IN UINT32 Timeout, IN EFI_PARITY_TYPE Parity,
	IN UINT8 DataBits, IN EFI_STOP_BITS_TYPE StopBits);

typedef EFI_STATUS(*EFI_SERIAL_SET_CONTROL_BITS)(
	IN EFI_SERIAL_IO_PROTOCOL* This, IN UINT32 Control);

#define EFI_SERIAL_CLEAR_TO_SEND 0x0010
#define EFI_SERIAL_DATA_SET_READY 0x0020
#define EFI_SERIAL_RING_INDICATE 0x0040
#define EFI_SERIAL_CARRIER_DETECT 0x0080
#define EFI_SERIAL_REQUEST_TO_SEND 0x0002
#define EFI_SERIAL_DATA_TERMINAL_READY 0x0001
#define EFI_SERIAL_INPUT_BUFFER_EMPTY 0x0100
#define EFI_SERIAL_OUTPUT_BUFFER_EMPTY 0x0200
#define EFI_SERIAL_HARDWARE_LOOPBACK_ENABLE 0x1000
#define EFI_SERIAL_SOFTWARE_LOOPBACK_ENABLE 0x2000
#define EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE 0x4000

typedef EFI_STATUS(*EFI_SERIAL_GET_CONTROL_BITS)(
	IN EFI_SERIAL_IO_PROTOCOL* This, OUT UINT32* Control);

typedef EFI_STATUS(*EFI_SERIAL_WRITE)(
	IN EFI_SERIAL_IO_PROTOCOL* This,
	IN OUT UINTN* BufferSize, IN VOID* Buffer);

typedef EFI_STATUS(*EFI_SERIAL_READ)(
	IN EFI_SERIAL_IO_PROTOCOL* This,
	IN OUT UINTN* BufferSize, OUT VOID* Buffer);

struct _EFI_SERIAL_IO_PROTOCOL {
	UINT32 Revision;
	EFI_SERIAL_RESET Reset;
	EFI_SERIAL_SET_ATTRIBUTES SetAttributes;
	EFI_SERIAL_SET_CONTROL_BITS SetControl;
	EFI_SERIAL_GET_CONTROL_BITS GetControl;
	EFI_SERIAL_WRITE Write;
	EFI_SERIAL_READ Read;
	SERIAL_IO_MODE* Mode;
	CONST EFI_GUID* DeviceTypeGuid;
};

#define EFI_LOADED_IMAGE_PROTOCOL_GUID { 0x5B1B31A1, 0x9562, 0x11D2, { 0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B } }
#define EFI_LOADED_IMAGE_PROTOCOL_REVISION 0x1000
#define EFI_LOADED_IMAGE_DEVICE_PATH_PROTOCOL_GUID { 0xBC62157E, 0x3E33, 0x4FEC, { 0x99, 0x20, 0x2D, 0x3B, 0x36, 0xD7, 0x50, 0xDF } }
#define EFI_LOAD_FILE_PROTOCOL_GUID { 0x56EC3091, 0x954C, 0x11D2, { 0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } }

typedef struct _EFI_LOADED_IMAGE_PROTOCOL {
	UINT32 Revision;
	EFI_HANDLE ParentHandle;
	EFI_SYSTEM_TABLE* SystemTable;
	EFI_HANDLE DeviceHandle;
	EFI_DEVICE_PATH_PROTOCOL* FilePath;
	VOID* Reserved;
	UINT32 LoadOptionsSize;
	VOID* LoadOptions;
	VOID* ImageBase;
	UINT64 ImageSize;
	EFI_MEMORY_TYPE ImageCodeType;
	EFI_MEMORY_TYPE ImageDataType;
	EFI_IMAGE_UNLOAD Unload;
} EFI_LOADED_IMAGE_PROTOCOL;

#define EFI_DEVICE_PATH_TO_TEXT_PROTOCOL_GUID { 0x8B843E20, 0x8132, 0x4852, { 0x90, 0xCC, 0x55 ,0x1A, 0x4E, 0x4A, 0x7F, 0x1C } }
#define EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID { 0x5C99A21, 0xC70F, 0x4AD2, { 0x8A, 0x5F, 0x35, 0xDF, 0x33, 0x43, 0xF5, 0x1E } }

typedef CHAR16*(*EFI_DEVICE_PATH_TO_TEXT_NODE)(
	IN CONST EFI_DEVICE_PATH_PROTOCOL* DeviceNode,
	IN BOOLEAN DisplayOnly, IN BOOLEAN AllowShortcuts);

typedef CHAR16*(*EFI_DEVICE_PATH_TO_TEXT_PATH)(
	IN CONST EFI_DEVICE_PATH_PROTOCOL* DevicePath,
	IN BOOLEAN DisplayOnly, IN BOOLEAN AllowShortcuts);

typedef struct _EFI_DEVICE_PATH_TO_TEXT_PROTOCOL {
	EFI_DEVICE_PATH_TO_TEXT_NODE ConvertDeviceNodeToText;
	EFI_DEVICE_PATH_TO_TEXT_PATH ConvertDevicePathToText;
} EFI_DEVICE_PATH_TO_TEXT_PROTOCOL;

typedef EFI_DEVICE_PATH_PROTOCOL*(*EFI_DEVICE_PATH_FROM_TEXT_NODE)(IN CONST CHAR16* TextDeviceNode);
typedef EFI_DEVICE_PATH_PROTOCOL*(*EFI_DEVICE_PATH_FROM_TEXT_PATH)(IN CONST CHAR16* TextDevicePath);

typedef struct _EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL {
	EFI_DEVICE_PATH_FROM_TEXT_NODE ConvertTextToDeviceNode;
	EFI_DEVICE_PATH_FROM_TEXT_PATH ConvertTextToDevicePath;
} EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL;

typedef struct _EFI_LOAD_FILE_PROTOCOL EFI_LOAD_FILE_PROTOCOL;
typedef EFI_STATUS(*EFI_LOAD_FILE)(
	IN EFI_LOAD_FILE_PROTOCOL* This, IN EFI_DEVICE_PATH_PROTOCOL *FilePath,
	IN BOOLEAN BootPolicy, IN OUT UINTN* BufferSize, IN VOID* Buffer OPTIONAL);

struct _EFI_LOAD_FILE_PROTOCOL {
	EFI_LOAD_FILE LoadFile;
};
