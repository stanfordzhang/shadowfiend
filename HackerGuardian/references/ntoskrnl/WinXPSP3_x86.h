#ifdef __cplusplus
extern "C" {
#endif

//////////////////
//   C Define   //
//////////////////

#define VOID void
#define STDCALL __stdcall
#define FASTCALL __fastcall
#define CDECL __cdecl
#define THISCALL __thiscall
#define NEAR 
#define FAR 
//---------------------------------------------------------------------

//////////////////
// C Basic Type //
//////////////////

typedef signed char INT8;
typedef signed char CHAR;
typedef signed short INT16;
typedef signed int INT32;
typedef signed int BOOL;
typedef signed __int64 INT64;
typedef signed __int64 LONG64;
typedef signed long LONG32;
typedef unsigned char UINT8;
typedef unsigned char BYTE;
typedef unsigned short UINT16;
typedef unsigned short WCHAR;
typedef unsigned int UINT32;
typedef unsigned __int64 UINT64;
typedef unsigned __int64 ULONG64;
typedef unsigned long ULONG32;
typedef float FLOAT32;
typedef double FLOAT64;
typedef struct {unsigned short W[5];} FLOAT80;
typedef struct { __int64 LowPart;__int64 HighPart;} FLOAT128;
typedef double DATE;
typedef signed long HRESULT;
typedef union { struct {unsigned long Lo; long Hi;}; __int64 int64;} CURRENCY;
typedef wchar_t OLECHAR;
//---------------------------------------------------------------------

//------------------------------------------------------------------------
//------------------------------------------------------------------------
// Declaration of Enum types

typedef enum _ALTERNATIVE_ARCHITECTURE_TYPE  // 3 elements, 0x4 bytes
{
    StandardDesign  = 0 /*0x0*/,
    NEC98x86        = 1 /*0x1*/,
    EndAlternatives = 2 /*0x2*/
}ALTERNATIVE_ARCHITECTURE_TYPE, *PALTERNATIVE_ARCHITECTURE_TYPE;

typedef enum _ARBITER_ACTION  // 10 elements, 0x4 bytes
{
    ArbiterActionTestAllocation          = 0 /*0x0*/,
    ArbiterActionRetestAllocation        = 1 /*0x1*/,
    ArbiterActionCommitAllocation        = 2 /*0x2*/,
    ArbiterActionRollbackAllocation      = 3 /*0x3*/,
    ArbiterActionQueryAllocatedResources = 4 /*0x4*/,
    ArbiterActionWriteReservedResources  = 5 /*0x5*/,
    ArbiterActionQueryConflict           = 6 /*0x6*/,
    ArbiterActionQueryArbitrate          = 7 /*0x7*/,
    ArbiterActionAddReserved             = 8 /*0x8*/,
    ArbiterActionBootAllocation          = 9 /*0x9*/
}ARBITER_ACTION, *PARBITER_ACTION;

typedef enum _ARBITER_REQUEST_SOURCE  // 6 elements, 0x4 bytes
{
    ArbiterRequestUndefined      = -1 /*0xFF*/,
    ArbiterRequestLegacyReported = 0 /*0x0*/,
    ArbiterRequestHalReported    = 1 /*0x1*/,
    ArbiterRequestLegacyAssigned = 2 /*0x2*/,
    ArbiterRequestPnpDetected    = 3 /*0x3*/,
    ArbiterRequestPnpEnumerated  = 4 /*0x4*/
}ARBITER_REQUEST_SOURCE, *PARBITER_REQUEST_SOURCE;

typedef enum _ARBITER_RESULT  // 4 elements, 0x4 bytes
{
    ArbiterResultUndefined        = -1 /*0xFF*/,
    ArbiterResultSuccess          = 0 /*0x0*/,
    ArbiterResultExternalConflict = 1 /*0x1*/,
    ArbiterResultNullRequest      = 2 /*0x2*/
}ARBITER_RESULT, *PARBITER_RESULT;

typedef enum _BUS_DATA_TYPE  // 14 elements, 0x4 bytes
{
    ConfigurationSpaceUndefined = -1 /*0xFF*/,
    Cmos                        = 0 /*0x0*/,
    EisaConfiguration           = 1 /*0x1*/,
    Pos                         = 2 /*0x2*/,
    CbusConfiguration           = 3 /*0x3*/,
    PCIConfiguration            = 4 /*0x4*/,
    VMEConfiguration            = 5 /*0x5*/,
    NuBusConfiguration          = 6 /*0x6*/,
    PCMCIAConfiguration         = 7 /*0x7*/,
    MPIConfiguration            = 8 /*0x8*/,
    MPSAConfiguration           = 9 /*0x9*/,
    PNPISAConfiguration         = 10 /*0xA*/,
    SgiInternalConfiguration    = 11 /*0xB*/,
    MaximumBusDataType          = 12 /*0xC*/
}BUS_DATA_TYPE, *PBUS_DATA_TYPE;

typedef enum _BUS_QUERY_ID_TYPE  // 5 elements, 0x4 bytes
{
    BusQueryDeviceID           = 0 /*0x0*/,
    BusQueryHardwareIDs        = 1 /*0x1*/,
    BusQueryCompatibleIDs      = 2 /*0x2*/,
    BusQueryInstanceID         = 3 /*0x3*/,
    BusQueryDeviceSerialNumber = 4 /*0x4*/
}BUS_QUERY_ID_TYPE, *PBUS_QUERY_ID_TYPE;

typedef enum _CPU_VENDORS  // 8 elements, 0x4 bytes
{
    CPU_NONE      = 0 /*0x0*/,
    CPU_INTEL     = 1 /*0x1*/,
    CPU_AMD       = 2 /*0x2*/,
    CPU_CYRIX     = 3 /*0x3*/,
    CPU_TRANSMETA = 4 /*0x4*/,
    CPU_CENTAUR   = 5 /*0x5*/,
    CPU_RISE      = 6 /*0x6*/,
    CPU_UNKNOWN   = 7 /*0x7*/
}CPU_VENDORS, *PCPU_VENDORS;

typedef enum _DEVICE_POWER_STATE  // 6 elements, 0x4 bytes
{
    PowerDeviceUnspecified = 0 /*0x0*/,
    PowerDeviceD0          = 1 /*0x1*/,
    PowerDeviceD1          = 2 /*0x2*/,
    PowerDeviceD2          = 3 /*0x3*/,
    PowerDeviceD3          = 4 /*0x4*/,
    PowerDeviceMaximum     = 5 /*0x5*/
}DEVICE_POWER_STATE, *PDEVICE_POWER_STATE;

typedef enum _DEVICE_RELATION_TYPE  // 6 elements, 0x4 bytes
{
    BusRelations         = 0 /*0x0*/,
    EjectionRelations    = 1 /*0x1*/,
    PowerRelations       = 2 /*0x2*/,
    RemovalRelations     = 3 /*0x3*/,
    TargetDeviceRelation = 4 /*0x4*/,
    SingleBusRelations   = 5 /*0x5*/
}DEVICE_RELATION_TYPE, *PDEVICE_RELATION_TYPE;

typedef enum _DEVICE_TEXT_TYPE  // 2 elements, 0x4 bytes
{
    DeviceTextDescription         = 0 /*0x0*/,
    DeviceTextLocationInformation = 1 /*0x1*/
}DEVICE_TEXT_TYPE, *PDEVICE_TEXT_TYPE;

typedef enum _DEVICE_USAGE_NOTIFICATION_TYPE  // 4 elements, 0x4 bytes
{
    DeviceUsageTypeUndefined   = 0 /*0x0*/,
    DeviceUsageTypePaging      = 1 /*0x1*/,
    DeviceUsageTypeHibernation = 2 /*0x2*/,
    DeviceUsageTypeDumpFile    = 3 /*0x3*/
}DEVICE_USAGE_NOTIFICATION_TYPE, *PDEVICE_USAGE_NOTIFICATION_TYPE;

typedef enum _EXCEPTION_DISPOSITION  // 4 elements, 0x4 bytes
{
    ExceptionContinueExecution = 0 /*0x0*/,
    ExceptionContinueSearch    = 1 /*0x1*/,
    ExceptionNestedException   = 2 /*0x2*/,
    ExceptionCollidedUnwind    = 3 /*0x3*/
}EXCEPTION_DISPOSITION, *PEXCEPTION_DISPOSITION;

typedef enum _FILE_INFORMATION_CLASS  // 41 elements, 0x4 bytes
{
    FileDirectoryInformation       = 1 /*0x1*/,
    FileFullDirectoryInformation   = 2 /*0x2*/,
    FileBothDirectoryInformation   = 3 /*0x3*/,
    FileBasicInformation           = 4 /*0x4*/,
    FileStandardInformation        = 5 /*0x5*/,
    FileInternalInformation        = 6 /*0x6*/,
    FileEaInformation              = 7 /*0x7*/,
    FileAccessInformation          = 8 /*0x8*/,
    FileNameInformation            = 9 /*0x9*/,
    FileRenameInformation          = 10 /*0xA*/,
    FileLinkInformation            = 11 /*0xB*/,
    FileNamesInformation           = 12 /*0xC*/,
    FileDispositionInformation     = 13 /*0xD*/,
    FilePositionInformation        = 14 /*0xE*/,
    FileFullEaInformation          = 15 /*0xF*/,
    FileModeInformation            = 16 /*0x10*/,
    FileAlignmentInformation       = 17 /*0x11*/,
    FileAllInformation             = 18 /*0x12*/,
    FileAllocationInformation      = 19 /*0x13*/,
    FileEndOfFileInformation       = 20 /*0x14*/,
    FileAlternateNameInformation   = 21 /*0x15*/,
    FileStreamInformation          = 22 /*0x16*/,
    FilePipeInformation            = 23 /*0x17*/,
    FilePipeLocalInformation       = 24 /*0x18*/,
    FilePipeRemoteInformation      = 25 /*0x19*/,
    FileMailslotQueryInformation   = 26 /*0x1A*/,
    FileMailslotSetInformation     = 27 /*0x1B*/,
    FileCompressionInformation     = 28 /*0x1C*/,
    FileObjectIdInformation        = 29 /*0x1D*/,
    FileCompletionInformation      = 30 /*0x1E*/,
    FileMoveClusterInformation     = 31 /*0x1F*/,
    FileQuotaInformation           = 32 /*0x20*/,
    FileReparsePointInformation    = 33 /*0x21*/,
    FileNetworkOpenInformation     = 34 /*0x22*/,
    FileAttributeTagInformation    = 35 /*0x23*/,
    FileTrackingInformation        = 36 /*0x24*/,
    FileIdBothDirectoryInformation = 37 /*0x25*/,
    FileIdFullDirectoryInformation = 38 /*0x26*/,
    FileValidDataLengthInformation = 39 /*0x27*/,
    FileShortNameInformation       = 40 /*0x28*/,
    FileMaximumInformation         = 41 /*0x29*/
}FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

typedef enum _FS_FILTER_SECTION_SYNC_TYPE  // 2 elements, 0x4 bytes
{
    SyncTypeOther         = 0 /*0x0*/,
    SyncTypeCreateSection = 1 /*0x1*/
}FS_FILTER_SECTION_SYNC_TYPE, *PFS_FILTER_SECTION_SYNC_TYPE;

typedef enum _FSINFOCLASS  // 10 elements, 0x4 bytes
{
    FileFsVolumeInformation     = 1 /*0x1*/,
    FileFsLabelInformation      = 2 /*0x2*/,
    FileFsSizeInformation       = 3 /*0x3*/,
    FileFsDeviceInformation     = 4 /*0x4*/,
    FileFsAttributeInformation  = 5 /*0x5*/,
    FileFsControlInformation    = 6 /*0x6*/,
    FileFsFullSizeInformation   = 7 /*0x7*/,
    FileFsObjectIdInformation   = 8 /*0x8*/,
    FileFsDriverPathInformation = 9 /*0x9*/,
    FileFsMaximumInformation    = 10 /*0xA*/
}FSINFOCLASS, *PFSINFOCLASS;

typedef enum _INTERFACE_TYPE  // 18 elements, 0x4 bytes
{
    InterfaceTypeUndefined = -1 /*0xFF*/,
    Internal               = 0 /*0x0*/,
    Isa                    = 1 /*0x1*/,
    Eisa                   = 2 /*0x2*/,
    MicroChannel           = 3 /*0x3*/,
    TurboChannel           = 4 /*0x4*/,
    PCIBus                 = 5 /*0x5*/,
    VMEBus                 = 6 /*0x6*/,
    NuBus                  = 7 /*0x7*/,
    PCMCIABus              = 8 /*0x8*/,
    CBus                   = 9 /*0x9*/,
    MPIBus                 = 10 /*0xA*/,
    MPSABus                = 11 /*0xB*/,
    ProcessorInternal      = 12 /*0xC*/,
    InternalPowerBus       = 13 /*0xD*/,
    PNPISABus              = 14 /*0xE*/,
    PNPBus                 = 15 /*0xF*/,
    MaximumInterfaceType   = 16 /*0x10*/
}INTERFACE_TYPE, *PINTERFACE_TYPE;

typedef enum _IO_ALLOCATION_ACTION  // 3 elements, 0x4 bytes
{
    KeepObject                    = 1 /*0x1*/,
    DeallocateObject              = 2 /*0x2*/,
    DeallocateObjectKeepRegisters = 3 /*0x3*/
}IO_ALLOCATION_ACTION, *PIO_ALLOCATION_ACTION;

typedef enum _KINTERRUPT_MODE  // 2 elements, 0x4 bytes
{
    LevelSensitive = 0 /*0x0*/,
    Latched        = 1 /*0x1*/
}KINTERRUPT_MODE, *PKINTERRUPT_MODE;

typedef enum _KSPIN_LOCK_QUEUE_NUMBER  // 16 elements, 0x4 bytes
{
    LockQueueDispatcherLock   = 0 /*0x0*/,
    LockQueueContextSwapLock  = 1 /*0x1*/,
    LockQueuePfnLock          = 2 /*0x2*/,
    LockQueueSystemSpaceLock  = 3 /*0x3*/,
    LockQueueVacbLock         = 4 /*0x4*/,
    LockQueueMasterLock       = 5 /*0x5*/,
    LockQueueNonPagedPoolLock = 6 /*0x6*/,
    LockQueueIoCancelLock     = 7 /*0x7*/,
    LockQueueWorkQueueLock    = 8 /*0x8*/,
    LockQueueIoVpbLock        = 9 /*0x9*/,
    LockQueueIoDatabaseLock   = 10 /*0xA*/,
    LockQueueIoCompletionLock = 11 /*0xB*/,
    LockQueueNtfsStructLock   = 12 /*0xC*/,
    LockQueueAfdWorkQueueLock = 13 /*0xD*/,
    LockQueueBcbLock          = 14 /*0xE*/,
    LockQueueMaximumLock      = 15 /*0xF*/
}KSPIN_LOCK_QUEUE_NUMBER, *PKSPIN_LOCK_QUEUE_NUMBER;

typedef enum _KWAIT_REASON  // 28 elements, 0x4 bytes
{
    Executive         = 0 /*0x0*/,
    FreePage          = 1 /*0x1*/,
    PageIn            = 2 /*0x2*/,
    PoolAllocation    = 3 /*0x3*/,
    DelayExecution    = 4 /*0x4*/,
    Suspended         = 5 /*0x5*/,
    UserRequest       = 6 /*0x6*/,
    WrExecutive       = 7 /*0x7*/,
    WrFreePage        = 8 /*0x8*/,
    WrPageIn          = 9 /*0x9*/,
    WrPoolAllocation  = 10 /*0xA*/,
    WrDelayExecution  = 11 /*0xB*/,
    WrSuspended       = 12 /*0xC*/,
    WrUserRequest     = 13 /*0xD*/,
    WrEventPair       = 14 /*0xE*/,
    WrQueue           = 15 /*0xF*/,
    WrLpcReceive      = 16 /*0x10*/,
    WrLpcReply        = 17 /*0x11*/,
    WrVirtualMemory   = 18 /*0x12*/,
    WrPageOut         = 19 /*0x13*/,
    WrRendezvous      = 20 /*0x14*/,
    Spare2            = 21 /*0x15*/,
    Spare3            = 22 /*0x16*/,
    Spare4            = 23 /*0x17*/,
    Spare5            = 24 /*0x18*/,
    Spare6            = 25 /*0x19*/,
    WrKernel          = 26 /*0x1A*/,
    MaximumWaitReason = 27 /*0x1B*/
}KWAIT_REASON, *PKWAIT_REASON;

typedef enum _LSA_FOREST_TRUST_RECORD_TYPE  // 4 elements, 0x4 bytes
{
    ForestTrustTopLevelName   = 0 /*0x0*/,
    ForestTrustTopLevelNameEx = 1 /*0x1*/,
    ForestTrustDomainInfo     = 2 /*0x2*/,
    ForestTrustRecordTypeLast = 2 /*0x2*/
}LSA_FOREST_TRUST_RECORD_TYPE, *PLSA_FOREST_TRUST_RECORD_TYPE;

typedef enum _MEMORY_CACHING_TYPE  // 7 elements, 0x4 bytes
{
    MmNonCached              = 0 /*0x0*/,
    MmCached                 = 1 /*0x1*/,
    MmWriteCombined          = 2 /*0x2*/,
    MmHardwareCoherentCached = 3 /*0x3*/,
    MmNonCachedUnordered     = 4 /*0x4*/,
    MmUSWCCached             = 5 /*0x5*/,
    MmMaximumCacheType       = 6 /*0x6*/
}MEMORY_CACHING_TYPE, *PMEMORY_CACHING_TYPE;

typedef enum _MEMORY_CACHING_TYPE_ORIG  // 1 elements, 0x4 bytes
{
    MmFrameBufferCached = 2 /*0x2*/
}MEMORY_CACHING_TYPE_ORIG, *PMEMORY_CACHING_TYPE_ORIG;

typedef enum _MEMORY_TYPE  // 10 elements, 0x4 bytes
{
    MemoryExceptionBlock    = 0 /*0x0*/,
    MemorySystemBlock       = 1 /*0x1*/,
    MemoryFree              = 2 /*0x2*/,
    MemoryBad               = 3 /*0x3*/,
    MemoryLoadedProgram     = 4 /*0x4*/,
    MemoryFirmwareTemporary = 5 /*0x5*/,
    MemoryFirmwarePermanent = 6 /*0x6*/,
    MemoryFreeContiguous    = 7 /*0x7*/,
    MemorySpecialMemory     = 8 /*0x8*/,
    MemoryMaximum           = 9 /*0x9*/
}MEMORY_TYPE, *PMEMORY_TYPE;

typedef enum _MI_PFN_CACHE_ATTRIBUTE  // 4 elements, 0x4 bytes
{
    MiNonCached     = 0 /*0x0*/,
    MiCached        = 1 /*0x1*/,
    MiWriteCombined = 2 /*0x2*/,
    MiNotMapped     = 3 /*0x3*/
}MI_PFN_CACHE_ATTRIBUTE, *PMI_PFN_CACHE_ATTRIBUTE;

typedef enum _MMLISTS  // 8 elements, 0x4 bytes
{
    ZeroedPageList          = 0 /*0x0*/,
    FreePageList            = 1 /*0x1*/,
    StandbyPageList         = 2 /*0x2*/,
    ModifiedPageList        = 3 /*0x3*/,
    ModifiedNoWritePageList = 4 /*0x4*/,
    BadPageList             = 5 /*0x5*/,
    ActiveAndValid          = 6 /*0x6*/,
    TransitionPage          = 7 /*0x7*/
}MMLISTS, *PMMLISTS;

typedef enum _MMSYSTEM_PTE_POOL_TYPE  // 3 elements, 0x4 bytes
{
    SystemPteSpace        = 0 /*0x0*/,
    NonPagedPoolExpansion = 1 /*0x1*/,
    MaximumPtePoolTypes   = 2 /*0x2*/
}MMSYSTEM_PTE_POOL_TYPE, *PMMSYSTEM_PTE_POOL_TYPE;

typedef enum _MODE  // 3 elements, 0x4 bytes
{
    KernelMode  = 0 /*0x0*/,
    UserMode    = 1 /*0x1*/,
    MaximumMode = 2 /*0x2*/
}MODE, *PMODE;

typedef enum _NT_PRODUCT_TYPE  // 3 elements, 0x4 bytes
{
    NtProductWinNt    = 1 /*0x1*/,
    NtProductLanManNt = 2 /*0x2*/,
    NtProductServer   = 3 /*0x3*/
}NT_PRODUCT_TYPE, *PNT_PRODUCT_TYPE;

typedef enum _OB_OPEN_REASON  // 5 elements, 0x4 bytes
{
    ObCreateHandle    = 0 /*0x0*/,
    ObOpenHandle      = 1 /*0x1*/,
    ObDuplicateHandle = 2 /*0x2*/,
    ObInheritHandle   = 3 /*0x3*/,
    ObMaxOpenReason   = 4 /*0x4*/
}OB_OPEN_REASON, *POB_OPEN_REASON;

typedef enum _PCI_DISPATCH_STYLE  // 4 elements, 0x4 bytes
{
    IRP_COMPLETE = 0 /*0x0*/,
    IRP_DOWNWARD = 1 /*0x1*/,
    IRP_UPWARD   = 2 /*0x2*/,
    IRP_DISPATCH = 3 /*0x3*/
}PCI_DISPATCH_STYLE, *PPCI_DISPATCH_STYLE;

typedef enum _PCI_SIGNATURE  // 15 elements, 0x4 bytes
{
    PciPdoExtensionType                = 1768116272 /*0x69635030*/  /*0Pci*/,
    PciFdoExtensionType                = 1768116273 /*0x69635031*/  /*1Pci*/,
    PciArb_Io                          = 1768116274 /*0x69635032*/  /*2Pci*/,
    PciArb_Memory                      = 1768116275 /*0x69635033*/  /*3Pci*/,
    PciArb_Interrupt                   = 1768116276 /*0x69635034*/  /*4Pci*/,
    PciArb_BusNumber                   = 1768116277 /*0x69635035*/  /*5Pci*/,
    PciTrans_Interrupt                 = 1768116278 /*0x69635036*/  /*6Pci*/,
    PciInterface_BusHandler            = 1768116279 /*0x69635037*/  /*7Pci*/,
    PciInterface_IntRouteHandler       = 1768116280 /*0x69635038*/  /*8Pci*/,
    PciInterface_PciCb                 = 1768116281 /*0x69635039*/  /*9Pci*/,
    PciInterface_LegacyDeviceDetection = 1768116282 /*0x6963503A*/,
    PciInterface_PmeHandler            = 1768116283 /*0x6963503B*/,
    PciInterface_DevicePresent         = 1768116284 /*0x6963503C*/,
    PciInterface_NativeIde             = 1768116285 /*0x6963503D*/,
    PciInterface_AgpTarget             = 1768116286 /*0x6963503E*/
}PCI_SIGNATURE, *PPCI_SIGNATURE;

typedef enum _PF_SCENARIO_TYPE  // 3 elements, 0x4 bytes
{
    PfApplicationLaunchScenarioType = 0 /*0x0*/,
    PfSystemBootScenarioType        = 1 /*0x1*/,
    PfMaxScenarioType               = 2 /*0x2*/
}PF_SCENARIO_TYPE, *PPF_SCENARIO_TYPE;

typedef enum _PLUGPLAY_EVENT_CATEGORY  // 10 elements, 0x4 bytes
{
    HardwareProfileChangeEvent = 0 /*0x0*/,
    TargetDeviceChangeEvent    = 1 /*0x1*/,
    DeviceClassChangeEvent     = 2 /*0x2*/,
    CustomDeviceEvent          = 3 /*0x3*/,
    DeviceInstallEvent         = 4 /*0x4*/,
    DeviceArrivalEvent         = 5 /*0x5*/,
    PowerEvent                 = 6 /*0x6*/,
    VetoEvent                  = 7 /*0x7*/,
    BlockedDriverEvent         = 8 /*0x8*/,
    MaxPlugEventCategory       = 9 /*0x9*/
}PLUGPLAY_EVENT_CATEGORY, *PPLUGPLAY_EVENT_CATEGORY;

typedef enum _PNP_DEVNODE_STATE  // 21 elements, 0x4 bytes
{
    DeviceNodeUnspecified            = 768 /*0x300*/,
    DeviceNodeUninitialized          = 769 /*0x301*/,
    DeviceNodeInitialized            = 770 /*0x302*/,
    DeviceNodeDriversAdded           = 771 /*0x303*/,
    DeviceNodeResourcesAssigned      = 772 /*0x304*/,
    DeviceNodeStartPending           = 773 /*0x305*/,
    DeviceNodeStartCompletion        = 774 /*0x306*/,
    DeviceNodeStartPostWork          = 775 /*0x307*/,
    DeviceNodeStarted                = 776 /*0x308*/,
    DeviceNodeQueryStopped           = 777 /*0x309*/,
    DeviceNodeStopped                = 778 /*0x30A*/,
    DeviceNodeRestartCompletion      = 779 /*0x30B*/,
    DeviceNodeEnumeratePending       = 780 /*0x30C*/,
    DeviceNodeEnumerateCompletion    = 781 /*0x30D*/,
    DeviceNodeAwaitingQueuedDeletion = 782 /*0x30E*/,
    DeviceNodeAwaitingQueuedRemoval  = 783 /*0x30F*/,
    DeviceNodeQueryRemoved           = 784 /*0x310*/,
    DeviceNodeRemovePendingCloses    = 785 /*0x311*/,
    DeviceNodeRemoved                = 786 /*0x312*/,
    DeviceNodeDeletePendingCloses    = 787 /*0x313*/,
    DeviceNodeDeleted                = 788 /*0x314*/
}PNP_DEVNODE_STATE, *PPNP_DEVNODE_STATE;

typedef enum _PNP_VETO_TYPE  // 13 elements, 0x4 bytes
{
    PNP_VetoTypeUnknown          = 0 /*0x0*/,
    PNP_VetoLegacyDevice         = 1 /*0x1*/,
    PNP_VetoPendingClose         = 2 /*0x2*/,
    PNP_VetoWindowsApp           = 3 /*0x3*/,
    PNP_VetoWindowsService       = 4 /*0x4*/,
    PNP_VetoOutstandingOpen      = 5 /*0x5*/,
    PNP_VetoDevice               = 6 /*0x6*/,
    PNP_VetoDriver               = 7 /*0x7*/,
    PNP_VetoIllegalDeviceRequest = 8 /*0x8*/,
    PNP_VetoInsufficientPower    = 9 /*0x9*/,
    PNP_VetoNonDisableable       = 10 /*0xA*/,
    PNP_VetoLegacyDriver         = 11 /*0xB*/,
    PNP_VetoInsufficientRights   = 12 /*0xC*/
}PNP_VETO_TYPE, *PPNP_VETO_TYPE;

typedef enum _POLICY_AUDIT_EVENT_TYPE  // 9 elements, 0x4 bytes
{
    AuditCategorySystem                 = 0 /*0x0*/,
    AuditCategoryLogon                  = 1 /*0x1*/,
    AuditCategoryObjectAccess           = 2 /*0x2*/,
    AuditCategoryPrivilegeUse           = 3 /*0x3*/,
    AuditCategoryDetailedTracking       = 4 /*0x4*/,
    AuditCategoryPolicyChange           = 5 /*0x5*/,
    AuditCategoryAccountManagement      = 6 /*0x6*/,
    AuditCategoryDirectoryServiceAccess = 7 /*0x7*/,
    AuditCategoryAccountLogon           = 8 /*0x8*/
}POLICY_AUDIT_EVENT_TYPE, *PPOLICY_AUDIT_EVENT_TYPE;

typedef enum _POOL_TYPE  // 15 elements, 0x4 bytes
{
    NonPagedPool                         = 0 /*0x0*/,
    PagedPool                            = 1 /*0x1*/,
    NonPagedPoolMustSucceed              = 2 /*0x2*/,
    DontUseThisType                      = 3 /*0x3*/,
    NonPagedPoolCacheAligned             = 4 /*0x4*/,
    PagedPoolCacheAligned                = 5 /*0x5*/,
    NonPagedPoolCacheAlignedMustS        = 6 /*0x6*/,
    MaxPoolType                          = 7 /*0x7*/,
    NonPagedPoolSession                  = 32 /*0x20*/,
    PagedPoolSession                     = 33 /*0x21*/,
    NonPagedPoolMustSucceedSession       = 34 /*0x22*/,
    DontUseThisTypeSession               = 35 /*0x23*/,
    NonPagedPoolCacheAlignedSession      = 36 /*0x24*/,
    PagedPoolCacheAlignedSession         = 37 /*0x25*/,
    NonPagedPoolCacheAlignedMustSSession = 38 /*0x26*/
}POOL_TYPE, *PPOOL_TYPE;

typedef enum _POP_POLICY_DEVICE_TYPE  // 7 elements, 0x4 bytes
{
    PolicyDeviceSystemButton     = 0 /*0x0*/,
    PolicyDeviceThermalZone      = 1 /*0x1*/,
    PolicyDeviceBattery          = 2 /*0x2*/,
    PolicyInitiatePowerActionAPI = 3 /*0x3*/,
    PolicySetPowerStateAPI       = 4 /*0x4*/,
    PolicyImmediateDozeS4        = 5 /*0x5*/,
    PolicySystemIdle             = 6 /*0x6*/
}POP_POLICY_DEVICE_TYPE, *PPOP_POLICY_DEVICE_TYPE;

typedef enum _POWER_ACTION  // 8 elements, 0x4 bytes
{
    PowerActionNone          = 0 /*0x0*/,
    PowerActionReserved      = 1 /*0x1*/,
    PowerActionSleep         = 2 /*0x2*/,
    PowerActionHibernate     = 3 /*0x3*/,
    PowerActionShutdown      = 4 /*0x4*/,
    PowerActionShutdownReset = 5 /*0x5*/,
    PowerActionShutdownOff   = 6 /*0x6*/,
    PowerActionWarmEject     = 7 /*0x7*/
}POWER_ACTION, *PPOWER_ACTION;

typedef enum _POWER_STATE_TYPE  // 2 elements, 0x4 bytes
{
    SystemPowerState = 0 /*0x0*/,
    DevicePowerState = 1 /*0x1*/
}POWER_STATE_TYPE, *PPOWER_STATE_TYPE;

typedef enum _PP_NPAGED_LOOKASIDE_NUMBER  // 8 elements, 0x4 bytes
{
    LookasideSmallIrpList   = 0 /*0x0*/,
    LookasideLargeIrpList   = 1 /*0x1*/,
    LookasideMdlList        = 2 /*0x2*/,
    LookasideCreateInfoList = 3 /*0x3*/,
    LookasideNameBufferList = 4 /*0x4*/,
    LookasideTwilightList   = 5 /*0x5*/,
    LookasideCompletionList = 6 /*0x6*/,
    LookasideMaximumList    = 7 /*0x7*/
}PP_NPAGED_LOOKASIDE_NUMBER, *PPP_NPAGED_LOOKASIDE_NUMBER;

typedef enum _PROFILE_STATUS  // 5 elements, 0x4 bytes
{
    DOCK_NOTDOCKDEVICE      = 0 /*0x0*/,
    DOCK_QUIESCENT          = 1 /*0x1*/,
    DOCK_ARRIVING           = 2 /*0x2*/,
    DOCK_DEPARTING          = 3 /*0x3*/,
    DOCK_EJECTIRP_COMPLETED = 4 /*0x4*/
}PROFILE_STATUS, *PPROFILE_STATUS;

typedef enum _PROXY_CLASS  // 4 elements, 0x4 bytes
{
    ProxyFull      = 0 /*0x0*/,
    ProxyService   = 1 /*0x1*/,
    ProxyTree      = 2 /*0x2*/,
    ProxyDirectory = 3 /*0x3*/
}PROXY_CLASS, *PPROXY_CLASS;

typedef enum _PS_QUOTA_TYPE  // 4 elements, 0x4 bytes
{
    PsNonPagedPool = 0 /*0x0*/,
    PsPagedPool    = 1 /*0x1*/,
    PsPageFile     = 2 /*0x2*/,
    PsQuotaTypes   = 3 /*0x3*/
}PS_QUOTA_TYPE, *PPS_QUOTA_TYPE;

typedef enum _ReplacesCorHdrNumericDefines  // 22 elements, 0x4 bytes
{
    COMIMAGE_FLAGS_ILONLY                = 1 /*0x1*/,
    COMIMAGE_FLAGS_32BITREQUIRED         = 2 /*0x2*/,
    COMIMAGE_FLAGS_IL_LIBRARY            = 4 /*0x4*/,
    COMIMAGE_FLAGS_STRONGNAMESIGNED      = 8 /*0x8*/,
    COMIMAGE_FLAGS_TRACKDEBUGDATA        = 65536 /*0x10000*/,
    COR_VERSION_MAJOR_V2                 = 2 /*0x2*/,
    COR_VERSION_MAJOR                    = 2 /*0x2*/,
    COR_VERSION_MINOR                    = 0 /*0x0*/,
    COR_DELETED_NAME_LENGTH              = 8 /*0x8*/,
    COR_VTABLEGAP_NAME_LENGTH            = 8 /*0x8*/,
    NATIVE_TYPE_MAX_CB                   = 1 /*0x1*/,
    COR_ILMETHOD_SECT_SMALL_MAX_DATASIZE = 255 /*0xFF*/,
    IMAGE_COR_MIH_METHODRVA              = 1 /*0x1*/,
    IMAGE_COR_MIH_EHRVA                  = 2 /*0x2*/,
    IMAGE_COR_MIH_BASICBLOCK             = 8 /*0x8*/,
    COR_VTABLE_32BIT                     = 1 /*0x1*/,
    COR_VTABLE_64BIT                     = 2 /*0x2*/,
    COR_VTABLE_FROM_UNMANAGED            = 4 /*0x4*/,
    COR_VTABLE_CALL_MOST_DERIVED         = 16 /*0x10*/,
    IMAGE_COR_EATJ_THUNK_SIZE            = 32 /*0x20*/,
    MAX_CLASS_NAME                       = 1024 /*0x400*/,
    MAX_PACKAGE_NAME                     = 1024 /*0x400*/
}ReplacesCorHdrNumericDefines, *PReplacesCorHdrNumericDefines;

typedef enum _SECURITY_IMPERSONATION_LEVEL  // 4 elements, 0x4 bytes
{
    SecurityAnonymous      = 0 /*0x0*/,
    SecurityIdentification = 1 /*0x1*/,
    SecurityImpersonation  = 2 /*0x2*/,
    SecurityDelegation     = 3 /*0x3*/
}SECURITY_IMPERSONATION_LEVEL, *PSECURITY_IMPERSONATION_LEVEL;

typedef enum _SECURITY_OPERATION_CODE  // 4 elements, 0x4 bytes
{
    SetSecurityDescriptor    = 0 /*0x0*/,
    QuerySecurityDescriptor  = 1 /*0x1*/,
    DeleteSecurityDescriptor = 2 /*0x2*/,
    AssignSecurityDescriptor = 3 /*0x3*/
}SECURITY_OPERATION_CODE, *PSECURITY_OPERATION_CODE;

typedef enum _SYSTEM_POWER_STATE  // 8 elements, 0x4 bytes
{
    PowerSystemUnspecified = 0 /*0x0*/,
    PowerSystemWorking     = 1 /*0x1*/,
    PowerSystemSleeping1   = 2 /*0x2*/,
    PowerSystemSleeping2   = 3 /*0x3*/,
    PowerSystemSleeping3   = 4 /*0x4*/,
    PowerSystemHibernate   = 5 /*0x5*/,
    PowerSystemShutdown    = 6 /*0x6*/,
    PowerSystemMaximum     = 7 /*0x7*/
}SYSTEM_POWER_STATE, *PSYSTEM_POWER_STATE;

typedef enum _TOKEN_TYPE  // 2 elements, 0x4 bytes
{
    TokenPrimary       = 1 /*0x1*/,
    TokenImpersonation = 2 /*0x2*/
}TOKEN_TYPE, *PTOKEN_TYPE;

typedef enum _VI_DEADLOCK_RESOURCE_TYPE  // 7 elements, 0x4 bytes
{
    VfDeadlockUnknown         = 0 /*0x0*/,
    VfDeadlockMutex           = 1 /*0x1*/,
    VfDeadlockFastMutex       = 2 /*0x2*/,
    VfDeadlockFastMutexUnsafe = 3 /*0x3*/,
    VfDeadlockSpinLock        = 4 /*0x4*/,
    VfDeadlockQueuedSpinLock  = 5 /*0x5*/,
    VfDeadlockTypeMaximum     = 6 /*0x6*/
}VI_DEADLOCK_RESOURCE_TYPE, *PVI_DEADLOCK_RESOURCE_TYPE;

//------------------------------------------------------------------------
//------------------------------------------------------------------------
// Information about structure types

typedef struct _ACCESS_STATE;
typedef struct _ACL;
typedef struct _ACTIVATION_CONTEXT_STACK;
typedef struct _ADAPTER_OBJECT;
typedef struct _AMD64_DBGKD_CONTROL_SET;
typedef struct _ARBITER_ALLOCATION_STATE;
typedef struct _ARBITER_ALTERNATIVE;
typedef struct _ARBITER_CONFLICT_INFO;
typedef struct _ARBITER_INSTANCE;
typedef struct _ARBITER_INTERFACE;
typedef struct _ARBITER_LIST_ENTRY;
typedef struct _ARBITER_ORDERING;
typedef struct _ARBITER_ORDERING_LIST;
typedef struct _ARBITER_PARAMETERS;
typedef struct _BATTERY_REPORTING_SCALE;
typedef struct _BITMAP_RANGE;
typedef struct _BUS_EXTENSION_LIST;
typedef struct _BUS_HANDLER;
typedef struct _CACHE_MANAGER_CALLBACKS;
typedef struct _CACHE_UNINITIALIZE_EVENT;
typedef struct _CACHED_CHILD_LIST;
typedef struct _CALL_HASH_ENTRY;
typedef struct _CALL_PERFORMANCE_DATA;
typedef struct _CELL_DATA;
typedef struct _CHILD_LIST;
typedef struct _CLIENT_ID;
typedef struct _CM_BIG_DATA;
typedef struct _CM_CACHED_VALUE_INDEX;
typedef struct _CM_CELL_REMAP_BLOCK;
typedef struct _CM_FULL_RESOURCE_DESCRIPTOR;
typedef struct _CM_INDEX_HINT_BLOCK;
typedef struct _CM_KEY_BODY;
typedef struct _CM_KEY_CONTROL_BLOCK;
typedef struct _CM_KEY_HASH;
typedef struct _CM_KEY_INDEX;
typedef struct _CM_KEY_NODE;
typedef struct _CM_KEY_REFERENCE;
typedef struct _CM_KEY_SECURITY;
typedef struct _CM_KEY_SECURITY_CACHE;
typedef struct _CM_KEY_SECURITY_CACHE_ENTRY;
typedef struct _CM_KEY_VALUE;
typedef struct _CM_NAME_CONTROL_BLOCK;
typedef struct _CM_NAME_HASH;
typedef struct _CM_NOTIFY_BLOCK;
typedef struct _CM_PARTIAL_RESOURCE_DESCRIPTOR;
typedef struct _CM_PARTIAL_RESOURCE_LIST;
typedef struct _CM_RESOURCE_LIST;
typedef struct _CM_VIEW_OF_FILE;
typedef struct _CMHIVE;
typedef struct _CMP_OFFSET_ARRAY;
typedef struct _COMPRESSED_DATA_INFO;
typedef struct _CONTEXT;
typedef struct _CONTROL_AREA;
typedef struct _CURDIR;
typedef struct _DBGKD_ANY_CONTROL_SET;
typedef struct _DBGKD_BREAKPOINTEX;
typedef struct _DBGKD_CONTINUE;
typedef struct _DBGKD_CONTINUE2;
typedef struct _DBGKD_FILL_MEMORY;
typedef struct _DBGKD_GET_CONTEXT;
typedef struct _DBGKD_GET_INTERNAL_BREAKPOINT32;
typedef struct _DBGKD_GET_INTERNAL_BREAKPOINT64;
typedef struct _DBGKD_GET_SET_BUS_DATA;
typedef struct _DBGKD_GET_VERSION32;
typedef struct _DBGKD_GET_VERSION64;
typedef struct _DBGKD_LOAD_SYMBOLS32;
typedef struct _DBGKD_LOAD_SYMBOLS64;
typedef struct _DBGKD_MANIPULATE_STATE32;
typedef struct _DBGKD_MANIPULATE_STATE64;
typedef struct _DBGKD_QUERY_MEMORY;
typedef struct _DBGKD_QUERY_SPECIAL_CALLS;
typedef struct _DBGKD_READ_MEMORY32;
typedef struct _DBGKD_READ_MEMORY64;
typedef struct _DBGKD_READ_WRITE_IO_EXTENDED32;
typedef struct _DBGKD_READ_WRITE_IO_EXTENDED64;
typedef struct _DBGKD_READ_WRITE_IO32;
typedef struct _DBGKD_READ_WRITE_IO64;
typedef struct _DBGKD_READ_WRITE_MSR;
typedef struct _DBGKD_RESTORE_BREAKPOINT;
typedef struct _DBGKD_SEARCH_MEMORY;
typedef struct _DBGKD_SET_CONTEXT;
typedef struct _DBGKD_SET_INTERNAL_BREAKPOINT32;
typedef struct _DBGKD_SET_INTERNAL_BREAKPOINT64;
typedef struct _DBGKD_SET_SPECIAL_CALL32;
typedef struct _DBGKD_SET_SPECIAL_CALL64;
typedef struct _DBGKD_WRITE_BREAKPOINT32;
typedef struct _DBGKD_WRITE_BREAKPOINT64;
typedef struct _DBGKD_WRITE_MEMORY32;
typedef struct _DBGKD_WRITE_MEMORY64;
typedef struct _DBGKM_EXCEPTION32;
typedef struct _DBGKM_EXCEPTION64;
typedef struct _DEFERRED_WRITE;
typedef struct _DESCRIPTOR;
typedef struct _DEVICE_CAPABILITIES;
typedef struct _DEVICE_MAP;
typedef struct _DEVICE_NODE;
typedef struct _DEVICE_OBJECT;
typedef struct _DEVICE_OBJECT_POWER_EXTENSION;
typedef struct _DEVICE_RELATIONS;
typedef struct _DEVOBJ_EXTENSION;
typedef struct _DISPATCHER_HEADER;
typedef struct _DRIVER_EXTENSION;
typedef struct _DRIVER_OBJECT;
typedef struct _DUAL;
typedef struct _DUMP_INITIALIZATION_CONTEXT;
typedef struct _DUMP_STACK_CONTEXT;
typedef struct _EJOB;
typedef struct _EPROCESS;
typedef struct _EPROCESS_QUOTA_BLOCK;
typedef struct _EPROCESS_QUOTA_ENTRY;
typedef struct _ERESOURCE;
typedef struct _ETHREAD;
typedef struct _ETIMER;
typedef struct _EVENT_COUNTER;
typedef struct _EX_FAST_REF;
typedef struct _EX_PUSH_LOCK;
typedef struct _EX_PUSH_LOCK_CACHE_AWARE;
typedef struct _EX_PUSH_LOCK_WAIT_BLOCK;
typedef union _EX_QUEUE_WORKER_INFO;
typedef struct _EX_RUNDOWN_REF;
typedef struct _EX_WORK_QUEUE;
typedef struct _EXCEPTION_POINTERS;
typedef struct _EXCEPTION_RECORD;
typedef struct _EXCEPTION_RECORD32;
typedef struct _EXCEPTION_RECORD64;
typedef struct _EXCEPTION_REGISTRATION_RECORD;
typedef struct _FAST_IO_DISPATCH;
typedef struct _FAST_MUTEX;
typedef struct _FILE_BASIC_INFORMATION;
typedef struct _FILE_GET_QUOTA_INFORMATION;
typedef struct _FILE_NETWORK_OPEN_INFORMATION;
typedef struct _FILE_OBJECT;
typedef struct _FILE_STANDARD_INFORMATION;
typedef struct _flags;
typedef struct _FLOATING_SAVE_AREA;
typedef struct _FNSAVE_FORMAT;
typedef struct _FS_FILTER_CALLBACK_DATA;
typedef struct _FS_FILTER_CALLBACKS;
typedef union _FS_FILTER_PARAMETERS;
typedef struct _FX_SAVE_AREA;
typedef struct _FXSAVE_FORMAT;
typedef struct _GDI_TEB_BATCH;
typedef struct _GENERAL_LOOKASIDE;
typedef struct _GENERIC_MAPPING;
typedef struct _GUID;
typedef struct _HANDLE_TABLE;
typedef struct _HANDLE_TABLE_ENTRY;
typedef struct _HANDLE_TABLE_ENTRY_INFO;
typedef struct _HANDLE_TRACE_DB_ENTRY;
typedef struct _HANDLE_TRACE_DEBUG_INFO;
typedef struct _HARDWARE_PTE;
typedef struct _HBASE_BLOCK;
typedef struct _HEAP;
typedef struct _HEAP_ENTRY;
typedef struct _HEAP_ENTRY_EXTRA;
typedef struct _HEAP_FREE_ENTRY;
typedef struct _HEAP_FREE_ENTRY_EXTRA;
typedef struct _HEAP_LOCK;
typedef struct _HEAP_LOOKASIDE;
typedef struct _HEAP_PSEUDO_TAG_ENTRY;
typedef struct _HEAP_SEGMENT;
typedef struct _HEAP_STOP_ON_TAG;
typedef struct _HEAP_STOP_ON_VALUES;
typedef struct _HEAP_SUBSEGMENT;
typedef struct _HEAP_TAG_ENTRY;
typedef struct _HEAP_UCR_SEGMENT;
typedef struct _HEAP_UNCOMMMTTED_RANGE;
typedef struct _HEAP_USERDATA_HEADER;
typedef struct _HEAP_VIRTUAL_ALLOC_ENTRY;
typedef struct _HHIVE;
typedef struct _HIVE_LIST_ENTRY;
typedef struct _HMAP_DIRECTORY;
typedef struct _HMAP_ENTRY;
typedef struct _HMAP_TABLE;
typedef struct _IA64_DBGKD_CONTROL_SET;
typedef struct _IMAGE_DATA_DIRECTORY;
typedef struct _IMAGE_DEBUG_DIRECTORY;
typedef struct _IMAGE_DOS_HEADER;
typedef struct _IMAGE_FILE_HEADER;
typedef struct _IMAGE_NT_HEADERS;
typedef struct _IMAGE_OPTIONAL_HEADER;
typedef struct _IMAGE_ROM_OPTIONAL_HEADER;
typedef struct _IMAGE_SECTION_HEADER;
typedef struct _INITIAL_PRIVILEGE_SET;
typedef struct _INTERFACE;
typedef struct _INTERLOCK_SEQ;
typedef struct _IO_CLIENT_EXTENSION;
typedef struct _IO_COMPLETION_CONTEXT;
typedef struct _IO_COUNTERS;
typedef struct _IO_RESOURCE_DESCRIPTOR;
typedef struct _IO_RESOURCE_LIST;
typedef struct _IO_RESOURCE_REQUIREMENTS_LIST;
typedef struct _IO_SECURITY_CONTEXT;
typedef struct _IO_STACK_LOCATION;
typedef struct _IO_STATUS_BLOCK;
typedef struct _IO_TIMER;
typedef struct _IRP;
typedef struct _KAPC;
typedef struct _KAPC_STATE;
typedef struct _KDEVICE_QUEUE;
typedef struct _KDEVICE_QUEUE_ENTRY;
typedef struct _KDPC;
typedef struct _KEVENT;
typedef struct _KEXECUTE_OPTIONS;
typedef struct _KGDTENTRY;
typedef struct _KIDTENTRY;
typedef struct _KiIoAccessMap;
typedef struct _KINTERRUPT;
typedef struct _KLOCK_QUEUE_HANDLE;
typedef struct _KMUTANT;
typedef struct _KNODE;
typedef struct _KPCR;
typedef struct _KPRCB;
typedef struct _KPROCESS;
typedef struct _KPROCESSOR_STATE;
typedef struct _KQUEUE;
typedef struct _KSEMAPHORE;
typedef struct _KSPECIAL_REGISTERS;
typedef struct _KSPIN_LOCK_QUEUE;
typedef struct _KSYSTEM_TIME;
typedef struct _KTHREAD;
typedef struct _KTIMER;
typedef struct _KTRAP_FRAME;
typedef struct _KTSS;
typedef struct _KUSER_SHARED_DATA;
typedef struct _KWAIT_BLOCK;
typedef struct _LARGE_CONTROL_AREA;
typedef union _LARGE_INTEGER;
typedef struct _LDR_DATA_TABLE_ENTRY;
typedef struct _LIST_ENTRY;
typedef struct _LIST_ENTRY32;
typedef struct _LIST_ENTRY64;
typedef struct _LPCP_MESSAGE;
typedef struct _LPCP_NONPAGED_PORT_QUEUE;
typedef struct _LPCP_PORT_OBJECT;
typedef struct _LPCP_PORT_QUEUE;
typedef struct _LUID;
typedef struct _LUID_AND_ATTRIBUTES;
typedef struct _MAILSLOT_CREATE_PARAMETERS;
typedef struct _MBCB;
typedef struct _MDL;
typedef struct _MI_VERIFIER_DRIVER_ENTRY;
typedef struct _MI_VERIFIER_POOL_HEADER;
typedef struct _MM_DRIVER_VERIFIER_DATA;
typedef struct _MM_PAGED_POOL_INFO;
typedef struct _MM_SESSION_SPACE;
typedef struct _MM_SESSION_SPACE_FLAGS;
typedef struct _MMADDRESS_LIST;
typedef struct _MMBANKED_SECTION;
typedef struct _MMCOLOR_TABLES;
typedef struct _MMEXTEND_INFO;
typedef struct _MMFREE_POOL_ENTRY;
typedef struct _MMMOD_WRITER_LISTHEAD;
typedef struct _MMMOD_WRITER_MDL_ENTRY;
typedef struct _MMPAGING_FILE;
typedef struct _MMPFN;
typedef struct _MMPFNENTRY;
typedef struct _MMPFNLIST;
typedef struct _MMPTE;
typedef struct _MMPTE_HARDWARE;
typedef struct _MMPTE_HIGHLOW;
typedef struct _MMPTE_LIST;
typedef struct _MMPTE_PROTOTYPE;
typedef struct _MMPTE_SOFTWARE;
typedef struct _MMPTE_SUBSECTION;
typedef struct _MMPTE_TRANSITION;
typedef struct _MMSECTION_FLAGS;
typedef struct _MMSESSION;
typedef struct _MMSUBSECTION_FLAGS;
typedef struct _MMSUPPORT;
typedef struct _MMSUPPORT_FLAGS;
typedef struct _MMVAD;
typedef struct _MMVAD_FLAGS;
typedef struct _MMVAD_FLAGS2;
typedef struct _MMVAD_LONG;
typedef struct _MMVAD_SHORT;
typedef struct _MMVIEW;
typedef struct _MMWSL;
typedef struct _MMWSLE;
typedef struct _MMWSLE_HASH;
typedef struct _MMWSLENTRY;
typedef struct _NAMED_PIPE_CREATE_PARAMETERS;
typedef struct _NPAGED_LOOKASIDE_LIST;
typedef struct _NT_TIB;
typedef struct _OBJECT_ATTRIBUTES;
typedef struct _OBJECT_CREATE_INFORMATION;
typedef struct _OBJECT_DIRECTORY;
typedef struct _OBJECT_DIRECTORY_ENTRY;
typedef struct _OBJECT_DUMP_CONTROL;
typedef struct _OBJECT_HANDLE_INFORMATION;
typedef struct _OBJECT_HEADER;
typedef struct _OBJECT_HEADER_CREATOR_INFO;
typedef struct _OBJECT_HEADER_NAME_INFO;
typedef struct _OBJECT_NAME_INFORMATION;
typedef struct _OBJECT_SYMBOLIC_LINK;
typedef struct _OBJECT_TYPE;
typedef struct _OBJECT_TYPE_INITIALIZER;
typedef struct _OWNER_ENTRY;
typedef struct _PAGED_LOOKASIDE_LIST;
typedef struct _PAGEFAULT_HISTORY;
typedef struct _PCI_ARBITER_INSTANCE;
typedef struct _PCI_BUS_INTERFACE_STANDARD;
typedef struct _PCI_COMMON_CONFIG;
typedef struct _PCI_COMMON_EXTENSION;
typedef struct _PCI_FDO_EXTENSION;
typedef struct _PCI_FUNCTION_RESOURCES;
typedef struct _PCI_HEADER_TYPE_0;
typedef struct _PCI_HEADER_TYPE_1;
typedef struct _PCI_HEADER_TYPE_2;
typedef union _PCI_HEADER_TYPE_DEPENDENT;
typedef struct _PCI_INTERFACE;
typedef struct _PCI_LOCK;
typedef struct _PCI_MJ_DISPATCH_TABLE;
typedef struct _PCI_MN_DISPATCH_TABLE;
typedef struct _PCI_PDO_EXTENSION;
typedef struct _PCI_PMC;
typedef struct _PCI_POWER_STATE;
typedef struct _PCI_SECONDARY_EXTENSION;
typedef struct _PCI_SLOT_NUMBER;
typedef struct _PEB;
typedef struct _PEB_FREE_BLOCK;
typedef struct _PEB_LDR_DATA;
typedef struct _PHYSICAL_MEMORY_DESCRIPTOR;
typedef struct _PHYSICAL_MEMORY_RUN;
typedef struct _PI_BUS_EXTENSION;
typedef struct _PI_RESOURCE_ARBITER_ENTRY;
typedef struct _PLUGPLAY_EVENT_BLOCK;
typedef struct _PM_SUPPORT;
typedef struct _PNP_DEVICE_EVENT_ENTRY;
typedef struct _PNP_DEVICE_EVENT_LIST;
typedef struct _PO_DEVICE_NOTIFY;
typedef struct _PO_DEVICE_NOTIFY_ORDER;
typedef struct _PO_HIBER_PERF;
typedef struct _PO_MEMORY_IMAGE;
typedef struct _PO_MEMORY_RANGE_ARRAY;
typedef struct _PO_NOTIFY_ORDER_LEVEL;
typedef struct _POOL_BLOCK_HEAD;
typedef struct _POOL_DESCRIPTOR;
typedef struct _POOL_HACKER;
typedef struct _POOL_HEADER;
typedef struct _POOL_TRACKER_BIG_PAGES;
typedef struct _POOL_TRACKER_TABLE;
typedef struct _POP_ACTION_TRIGGER;
typedef struct _POP_DEVICE_POWER_IRP;
typedef struct _POP_DEVICE_SYS_STATE;
typedef struct _POP_HIBER_CONTEXT;
typedef struct _POP_IDLE_HANDLER;
typedef struct _POP_POWER_ACTION;
typedef struct _POP_SHUTDOWN_BUG_CHECK;
typedef struct _POP_THERMAL_ZONE;
typedef struct _POP_TRIGGER_WAIT;
typedef struct _PORT_MESSAGE;
typedef struct _POWER_ACTION_POLICY;
typedef struct _POWER_CHANNEL_SUMMARY;
typedef struct _POWER_SEQUENCE;
typedef union _POWER_STATE;
typedef struct _PP_LOOKASIDE_LIST;
typedef struct _PRIVATE_CACHE_MAP;
typedef struct _PRIVATE_CACHE_MAP_FLAGS;
typedef struct _PRIVILEGE_SET;
typedef struct _PROCESS_WS_WATCH_INFORMATION;
typedef struct _PROCESSOR_IDLE_TIMES;
typedef struct _PROCESSOR_PERF_STATE;
typedef struct _PROCESSOR_POWER_POLICY;
typedef struct _PROCESSOR_POWER_POLICY_INFO;
typedef struct _PROCESSOR_POWER_STATE;
typedef struct _PS_IMPERSONATION_INFORMATION;
typedef struct _PS_JOB_TOKEN_FILTER;
typedef struct _QUAD;
typedef struct _RTL_ATOM_TABLE;
typedef struct _RTL_ATOM_TABLE_ENTRY;
typedef struct _RTL_BITMAP;
typedef struct _RTL_CRITICAL_SECTION;
typedef struct _RTL_CRITICAL_SECTION_DEBUG;
typedef struct _RTL_DRIVE_LETTER_CURDIR;
typedef struct _RTL_HANDLE_TABLE;
typedef struct _RTL_HANDLE_TABLE_ENTRY;
typedef struct _RTL_RANGE;
typedef struct _RTL_RANGE_LIST;
typedef struct _RTL_USER_PROCESS_PARAMETERS;
typedef struct _RTLP_RANGE_LIST_ENTRY;
typedef struct _SCSI_REQUEST_BLOCK;
typedef struct _SE_AUDIT_PROCESS_CREATION_INFO;
typedef struct _SECTION_IMAGE_INFORMATION;
typedef struct _SECTION_OBJECT;
typedef struct _SECTION_OBJECT_POINTERS;
typedef struct _SECURITY_CLIENT_CONTEXT;
typedef struct _SECURITY_DESCRIPTOR;
typedef struct _SECURITY_DESCRIPTOR_RELATIVE;
typedef struct _SECURITY_QUALITY_OF_SERVICE;
typedef struct _SECURITY_SUBJECT_CONTEXT;
typedef struct _SECURITY_TOKEN_AUDIT_DATA;
typedef struct _SECURITY_TOKEN_PROXY_DATA;
typedef struct _SEGMENT;
typedef struct _SEGMENT_OBJECT;
typedef struct _SEP_AUDIT_POLICY;
typedef struct _SEP_AUDIT_POLICY_CATEGORIES;
typedef struct _SEP_AUDIT_POLICY_OVERLAY;
typedef struct _SHARED_CACHE_MAP;
typedef struct _SID;
typedef struct _SID_AND_ATTRIBUTES;
typedef struct _SID_IDENTIFIER_AUTHORITY;
typedef struct _SINGLE_LIST_ENTRY;
typedef union _SLIST_HEADER;
typedef struct _STRING;
typedef struct _SUBSECTION;
typedef struct _SUPPORTED_RANGE;
typedef struct _SUPPORTED_RANGES;
typedef struct _SYSPTES_HEADER;
typedef struct _SYSTEM_POWER_CAPABILITIES;
typedef struct _SYSTEM_POWER_LEVEL;
typedef struct _SYSTEM_POWER_POLICY;
typedef struct _TEB;
typedef struct _TEB_ACTIVE_FRAME;
typedef struct _TEB_ACTIVE_FRAME_CONTEXT;
typedef struct _TERMINATION_PORT;
typedef struct _THERMAL_INFORMATION;
typedef struct _TOKEN;
typedef struct _TOKEN_CONTROL;
typedef struct _TOKEN_SOURCE;
typedef struct _TRACE_ENABLE_FLAG_EXTENSION;
typedef union _ULARGE_INTEGER;
typedef struct _UNICODE_STRING;
typedef struct _VACB;
typedef struct _VACB_LEVEL_REFERENCE;
typedef struct _VI_DEADLOCK_GLOBALS;
typedef struct _VI_DEADLOCK_NODE;
typedef struct _VI_DEADLOCK_RESOURCE;
typedef struct _VI_DEADLOCK_THREAD;
typedef struct _VI_POOL_ENTRY;
typedef struct _VI_POOL_ENTRY_INUSE;
typedef struct _VPB;
typedef struct _WAIT_CONTEXT_BLOCK;
typedef struct _WMI_BUFFER_HEADER;
typedef struct _WMI_BUFFER_STATE;
typedef struct _WMI_CLIENT_CONTEXT;
typedef struct _WMI_LOGGER_CONTEXT;
typedef struct _WMI_LOGGER_MODE;
typedef struct _WNODE_HEADER;
typedef struct _WORK_QUEUE_ITEM;
typedef struct _Wx86ThreadState;
typedef struct _X86_DBGKD_CONTROL_SET;

//------------------------------------------------------------------------
//------------------------------------------------------------------------
// Declaration of Function types

typedef VOID (NEAR STDCALL FUNCT_00BC_0130_DeferredRoutine) (struct _KDPC*, VOID*, VOID*, VOID*);
typedef enum _EXCEPTION_DISPOSITION (NEAR STDCALL FUNCT_0161_0160_Handler) (struct _EXCEPTION_RECORD*, VOID*, struct _CONTEXT*, VOID*);
typedef VOID (NEAR STDCALL FUNCT_00BC_01AA_NormalRoutine) (VOID*, VOID*, VOID*);
typedef VOID (NEAR STDCALL FUNCT_00BC_01A1_KernelRoutine) (struct _KAPC*, FUNCT_00BC_01AA_NormalRoutine**, VOID**, VOID**, VOID**);
typedef VOID (NEAR STDCALL FUNCT_00BC_01AA_NormalRoutine) (VOID*, VOID*, VOID*);
typedef VOID (NEAR STDCALL FUNCT_00BC_01B0_RundownRoutine) (struct _KAPC*);
typedef VOID* (NEAR STDCALL FUNCT_00BB_0280_Allocate) (enum _POOL_TYPE, ULONG32, ULONG32);
typedef VOID (NEAR STDCALL FUNCT_00BC_0285_Free_InterfaceReference_InterfaceDereference_DeleteProcedure_ReleaseFromLazyWrite_ReleaseFromReadAhead_WorkerRoutine_Destructor_Callback) (VOID*);
typedef VOID (NEAR STDCALL FUNCT_00BC_028D_WorkerRoutine) (VOID*, VOID*, VOID*, VOID*);
typedef VOID (NEAR FASTCALL FUNCT_00BC_02E9_IdleFunction) (struct _PROCESSOR_POWER_STATE*);
typedef LONG32 (NEAR FASTCALL FUNCT_0049_0301_PerfSetThrottle) (UINT8);
typedef LONG32 (NEAR STDCALL FUNCT_0049_03BB_AddDevice) (struct _DRIVER_OBJECT*, struct _DEVICE_OBJECT*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_03D8_PreAcquireForSectionSynchronization_PreReleaseForSectionSynchronization_PreAcquireForCcFlush_PreReleaseForCcFlush_PreAcquireForModifiedPageWriter_PreReleaseForModifiedPageWriter) (struct _FS_FILTER_CALLBACK_DATA*, VOID**);
typedef VOID (NEAR STDCALL FUNCT_00BC_0445_PostAcquireForSectionSynchronization_PostReleaseForSectionSynchronization_PostAcquireForCcFlush_PostReleaseForCcFlush_PostAcquireForModifiedPageWriter_PostReleaseForModifiedPageWriter) (struct _FS_FILTER_CALLBACK_DATA*, LONG32, VOID*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_0469_FastIoCheckIfPossible) (struct _FILE_OBJECT*, union _LARGE_INTEGER*, ULONG32, UINT8, ULONG32, UINT8, struct _IO_STATUS_BLOCK*, struct _DEVICE_OBJECT*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_0478_FastIoRead_FastIoWrite) (struct _FILE_OBJECT*, union _LARGE_INTEGER*, ULONG32, UINT8, ULONG32, VOID*, struct _IO_STATUS_BLOCK*, struct _DEVICE_OBJECT*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_0482_FastIoQueryBasicInfo) (struct _FILE_OBJECT*, UINT8, struct _FILE_BASIC_INFORMATION*, struct _IO_STATUS_BLOCK*, struct _DEVICE_OBJECT*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_0490_FastIoQueryStandardInfo) (struct _FILE_OBJECT*, UINT8, struct _FILE_STANDARD_INFORMATION*, struct _IO_STATUS_BLOCK*, struct _DEVICE_OBJECT*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_049E_FastIoLock) (struct _FILE_OBJECT*, union _LARGE_INTEGER*, union _LARGE_INTEGER*, struct _EPROCESS*, ULONG32, UINT8, UINT8, struct _IO_STATUS_BLOCK*, struct _DEVICE_OBJECT*);
typedef VOID (NEAR STDCALL FUNCT_00BC_0631_PostProcessInitRoutine_DispatchAddress_FinishRoutine) ();
typedef UINT8 (NEAR STDCALL FUNCT_003D_067B_FastIoUnlockSingle) (struct _FILE_OBJECT*, union _LARGE_INTEGER*, union _LARGE_INTEGER*, struct _EPROCESS*, ULONG32, struct _IO_STATUS_BLOCK*, struct _DEVICE_OBJECT*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_0684_FastIoUnlockAll) (struct _FILE_OBJECT*, struct _EPROCESS*, struct _IO_STATUS_BLOCK*, struct _DEVICE_OBJECT*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_068A_FastIoUnlockAllByKey) (struct _FILE_OBJECT*, VOID*, ULONG32, struct _IO_STATUS_BLOCK*, struct _DEVICE_OBJECT*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_0691_FastIoDeviceControl) (struct _FILE_OBJECT*, UINT8, VOID*, ULONG32, VOID*, ULONG32, ULONG32, struct _IO_STATUS_BLOCK*, struct _DEVICE_OBJECT*);
typedef VOID (NEAR STDCALL FUNCT_00BC_069C_AcquireFileForNtCreateSection_ReleaseFileForNtCreateSection) (struct _FILE_OBJECT*);
typedef VOID (NEAR STDCALL FUNCT_00BC_069F_FastIoDetachDevice) (struct _DEVICE_OBJECT*, struct _DEVICE_OBJECT*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_06A3_FastIoQueryNetworkOpenInfo) (struct _FILE_OBJECT*, UINT8, struct _FILE_NETWORK_OPEN_INFORMATION*, struct _IO_STATUS_BLOCK*, struct _DEVICE_OBJECT*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_06B3_AcquireForModWrite) (struct _FILE_OBJECT*, union _LARGE_INTEGER*, struct _ERESOURCE**, struct _DEVICE_OBJECT*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_06B9_MdlRead_PrepareMdlWrite) (struct _FILE_OBJECT*, union _LARGE_INTEGER*, ULONG32, ULONG32, struct _MDL**, struct _IO_STATUS_BLOCK*, struct _DEVICE_OBJECT*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_06CD_MdlReadComplete_MdlReadCompleteCompressed) (struct _FILE_OBJECT*, struct _MDL*, struct _DEVICE_OBJECT*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_06D2_MdlWriteComplete_MdlWriteCompleteCompressed) (struct _FILE_OBJECT*, union _LARGE_INTEGER*, struct _MDL*, struct _DEVICE_OBJECT*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_06D8_FastIoReadCompressed_FastIoWriteCompressed) (struct _FILE_OBJECT*, union _LARGE_INTEGER*, ULONG32, ULONG32, VOID*, struct _MDL**, struct _IO_STATUS_BLOCK*, struct _COMPRESSED_DATA_INFO*, ULONG32, struct _DEVICE_OBJECT*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_06ED_FastIoQueryOpen) (struct _IRP*, struct _FILE_NETWORK_OPEN_INFORMATION*, struct _DEVICE_OBJECT*);
typedef VOID (NEAR STDCALL FUNCT_00BC_0713_UserApcRoutine) (VOID*, struct _IO_STATUS_BLOCK*, ULONG32);
typedef VOID (NEAR STDCALL FUNCT_00BC_0718_DriverStartIo_CancelRoutine_SavedCancelRoutine) (struct _DEVICE_OBJECT*, struct _IRP*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_0948_CompletionRoutine) (struct _DEVICE_OBJECT*, struct _IRP*, VOID*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_094D_ReleaseForModWrite) (struct _FILE_OBJECT*, struct _ERESOURCE*, struct _DEVICE_OBJECT*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_0952_AcquireForCcFlush_ReleaseForCcFlush) (struct _FILE_OBJECT*, struct _DEVICE_OBJECT*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_0956_DriverInit) (struct _DRIVER_OBJECT*, struct _UNICODE_STRING*);
typedef VOID (NEAR STDCALL FUNCT_00BC_095A_DriverUnload) (struct _DRIVER_OBJECT*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_095E_MajorFunction) (struct _DEVICE_OBJECT*, struct _IRP*);
typedef VOID (NEAR STDCALL FUNCT_00BC_0968_TimerRoutine) (struct _DEVICE_OBJECT*, VOID*);
typedef enum _IO_ALLOCATION_ACTION (NEAR STDCALL FUNCT_0978_0977_DeviceRoutine) (struct _DEVICE_OBJECT*, struct _IRP*, VOID*, VOID*);
typedef VOID (NEAR STDCALL FUNCT_00BC_09D8_DumpProcedure) (VOID*, struct _OBJECT_DUMP_CONTROL*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_09E0_OpenProcedure) (enum _OB_OPEN_REASON, struct _EPROCESS*, VOID*, ULONG32, ULONG32);
typedef VOID (NEAR STDCALL FUNCT_00BC_09ED_CloseProcedure) (struct _EPROCESS*, VOID*, ULONG32, ULONG32, ULONG32);
typedef LONG32 (NEAR STDCALL FUNCT_0049_09F4_ParseProcedure) (VOID*, VOID*, struct _ACCESS_STATE*, CHAR, ULONG32, struct _UNICODE_STRING*, struct _UNICODE_STRING*, VOID*, struct _SECURITY_QUALITY_OF_SERVICE*, VOID**);
typedef LONG32 (NEAR STDCALL FUNCT_0049_0A00_SecurityProcedure) (VOID*, enum _SECURITY_OPERATION_CODE, ULONG32*, VOID*, ULONG32*, VOID**, enum _POOL_TYPE, struct _GENERIC_MAPPING*, CHAR);
typedef LONG32 (NEAR STDCALL FUNCT_0049_0A11_QueryNameProcedure) (VOID*, UINT8, struct _OBJECT_NAME_INFORMATION*, ULONG32, ULONG32*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_0A18_OkayToCloseProcedure) (struct _EPROCESS*, VOID*, VOID*, CHAR);
typedef UINT8 (NEAR STDCALL FUNCT_003D_0BDB_AcquireForLazyWrite_AcquireForReadAhead) (VOID*, UINT8);
typedef VOID (NEAR STDCALL FUNCT_00BC_0BDF_FlushToLsnRoutine) (VOID*, union _LARGE_INTEGER);
typedef LONG32 (NEAR STDCALL FUNCT_0049_0C6E_CommitRoutine) (VOID*, VOID**, ULONG32*);
typedef INT64 (NEAR STDCALL FUNCT_004D_0E28_GetCpuClock) ();
typedef VOID (NEAR STDCALL FUNCT_00BC_0E44_BufferCallback) (struct _WMI_BUFFER_HEADER*, VOID*);
typedef VOID (NEAR STDCALL FUNCT_00BC_0EDA_PostRoutine) (VOID*, VOID*);
typedef struct _CELL_DATA* (NEAR STDCALL FUNCT_0F22_0F21_GetCellRoutine) (struct _HHIVE*, ULONG32);
typedef VOID (NEAR STDCALL FUNCT_00BC_0F6F_ReleaseCellRoutine) (struct _HHIVE*, ULONG32);
typedef VOID* (NEAR STDCALL FUNCT_00BB_0F71_Allocate) (ULONG32, UINT8, ULONG32);
typedef VOID (NEAR STDCALL FUNCT_00BC_0F76_Free) (VOID*, ULONG32);
typedef UINT8 (NEAR STDCALL FUNCT_003D_0F7A_FileSetSize) (struct _HHIVE*, ULONG32, ULONG32, ULONG32);
typedef UINT8 (NEAR STDCALL FUNCT_003D_0F80_FileWrite) (struct _HHIVE*, ULONG32, struct _CMP_OFFSET_ARRAY*, ULONG32, ULONG32*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_0F8C_FileRead) (struct _HHIVE*, ULONG32, ULONG32*, VOID*, ULONG32);
typedef UINT8 (NEAR STDCALL FUNCT_003D_0F93_FileFlush) (struct _HHIVE*, ULONG32, union _LARGE_INTEGER*, ULONG32);
typedef VOID (NEAR STDCALL FUNCT_00BC_130F_BankedRoutine) (ULONG32, ULONG32, VOID*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_138A_SystemControlIrpDispatchFunction_OtherIrpDispatchFunction_DispatchFunction) (struct _IRP*, struct _IO_STACK_LOCATION*, struct _PCI_COMMON_EXTENSION*);
typedef ULONG32 (NEAR STDCALL FUNCT_0009_13CC_ReadConfig_WriteConfig) (VOID*, UINT8, ULONG32, VOID*, ULONG32, ULONG32);
typedef VOID (NEAR STDCALL FUNCT_00BC_13D4_PinToLine) (VOID*, struct _PCI_COMMON_CONFIG*);
typedef VOID (NEAR STDCALL FUNCT_00BC_1422_LineToPin) (VOID*, struct _PCI_COMMON_CONFIG*, struct _PCI_COMMON_CONFIG*);
typedef ULONG32 (NEAR STDCALL FUNCT_0009_1464_GetBusData_SetBusData) (struct _BUS_HANDLER*, struct _BUS_HANDLER*, ULONG32, VOID*, ULONG32, ULONG32);
typedef LONG32 (NEAR STDCALL FUNCT_0049_146C_AdjustResourceList) (struct _BUS_HANDLER*, struct _BUS_HANDLER*, struct _IO_RESOURCE_REQUIREMENTS_LIST**);
typedef LONG32 (NEAR STDCALL FUNCT_0049_1472_AssignSlotResources) (struct _BUS_HANDLER*, struct _BUS_HANDLER*, struct _UNICODE_STRING*, struct _UNICODE_STRING*, struct _DRIVER_OBJECT*, struct _DEVICE_OBJECT*, ULONG32, struct _CM_RESOURCE_LIST**);
typedef ULONG32 (NEAR STDCALL FUNCT_0009_147D_GetInterruptVector) (struct _BUS_HANDLER*, struct _BUS_HANDLER*, ULONG32, ULONG32, UINT8*, ULONG32*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_1485_TranslateBusAddress) (struct _BUS_HANDLER*, struct _BUS_HANDLER*, union _LARGE_INTEGER, ULONG32*, union _LARGE_INTEGER*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_1544_ArbiterHandler) (VOID*, enum _ARBITER_ACTION, struct _ARBITER_PARAMETERS*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_15EE_ServiceRoutine) (struct _KINTERRUPT*, VOID*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_160F_Constructor) (VOID*, VOID*, VOID*, UINT16, UINT16, struct _INTERFACE*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_1617_Initializer) (struct _PCI_ARBITER_INSTANCE*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_1682_UnpackRequirement) (struct _IO_RESOURCE_DESCRIPTOR*, UINT64*, UINT64*, ULONG32*, ULONG32*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_168A_PackResource) (struct _IO_RESOURCE_DESCRIPTOR*, UINT64, struct _CM_PARTIAL_RESOURCE_DESCRIPTOR*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_168F_UnpackResource) (struct _CM_PARTIAL_RESOURCE_DESCRIPTOR*, UINT64*, ULONG32*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_1694_ScoreRequirement) (struct _IO_RESOURCE_DESCRIPTOR*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_1697_TestAllocation_RetestAllocation_BootAllocation_QueryArbitrate) (struct _ARBITER_INSTANCE*, struct _LIST_ENTRY*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_169C_CommitAllocation_RollbackAllocation) (struct _ARBITER_INSTANCE*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_169F_QueryConflict) (struct _ARBITER_INSTANCE*, struct _DEVICE_OBJECT*, struct _IO_RESOURCE_DESCRIPTOR*, ULONG32*, struct _ARBITER_CONFLICT_INFO**);
typedef LONG32 (NEAR STDCALL FUNCT_0049_16A6_AddReserved) (struct _ARBITER_INSTANCE*, struct _IO_RESOURCE_DESCRIPTOR*, struct _CM_PARTIAL_RESOURCE_DESCRIPTOR*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_16AB_StartArbiter) (struct _ARBITER_INSTANCE*, struct _CM_RESOURCE_LIST*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_16AF_PreprocessEntry_AllocateEntry) (struct _ARBITER_INSTANCE*, struct _ARBITER_ALLOCATION_STATE*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_16B3_GetNextAllocationRange_FindSuitableRange_OverrideConflict) (struct _ARBITER_INSTANCE*, struct _ARBITER_ALLOCATION_STATE*);
typedef VOID (NEAR STDCALL FUNCT_00BC_16B5_AddAllocation_BacktrackAllocation) (struct _ARBITER_INSTANCE*, struct _ARBITER_ALLOCATION_STATE*);
typedef UINT8 (NEAR STDCALL FUNCT_003D_16B7_ConflictCallback) (VOID*, struct _RTL_RANGE*);
typedef UINT8 (NEAR FASTCALL FUNCT_003D_1759_IdleFunction) (struct _PROCESSOR_IDLE_TIMES*);
typedef VOID (NEAR STDCALL FUNCT_00BC_18EF_StallRoutine) (ULONG32);
typedef UINT8 (NEAR STDCALL FUNCT_003D_18F2_OpenRoutine) (union _LARGE_INTEGER);
typedef LONG32 (NEAR STDCALL FUNCT_0049_18F5_WriteRoutine) (union _LARGE_INTEGER*, struct _MDL*);
typedef LONG32 (NEAR STDCALL FUNCT_0049_18FB_WritePendingRoutine) (LONG32, union _LARGE_INTEGER*, struct _MDL*, VOID*);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
// Declaration of Structure types

          typedef struct _LUID       // 2 elements, 0x8 bytes (sizeof) 
          {                                                            
/*0x000*/     ULONG32      LowPart;                                    
/*0x004*/     LONG32       HighPart;                                   
          }LUID, *PLUID;                                               
                                                                       
          typedef struct _SECURITY_SUBJECT_CONTEXT                   // 4 elements, 0x10 bytes (sizeof) 
          {                                                                                             
/*0x000*/     VOID*        ClientToken;                                                                 
/*0x004*/     enum _SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;                                    
/*0x008*/     VOID*        PrimaryToken;                                                                
/*0x00C*/     VOID*        ProcessAuditId;                                                              
          }SECURITY_SUBJECT_CONTEXT, *PSECURITY_SUBJECT_CONTEXT;                                        
                                                                                                        
          typedef struct _LUID_AND_ATTRIBUTES // 2 elements, 0xC bytes (sizeof) 
          {                                                                     
/*0x000*/     struct _LUID Luid;              // 2 elements, 0x8 bytes (sizeof) 
/*0x008*/     ULONG32      Attributes;                                          
          }LUID_AND_ATTRIBUTES, *PLUID_AND_ATTRIBUTES;                                  
                                                                                
          typedef struct _INITIAL_PRIVILEGE_SET         // 3 elements, 0x2C bytes (sizeof) 
          {                                                                                
/*0x000*/     ULONG32      PrivilegeCount;                                                 
/*0x004*/     ULONG32      Control;                                                        
/*0x008*/     struct _LUID_AND_ATTRIBUTES Privilege[3];                                    
          }INITIAL_PRIVILEGE_SET, *PINITIAL_PRIVILEGE_SET;                                   
                                                                                           
          typedef struct _PRIVILEGE_SET                 // 3 elements, 0x14 bytes (sizeof) 
          {                                                                                
/*0x000*/     ULONG32      PrivilegeCount;                                                 
/*0x004*/     ULONG32      Control;                                                        
/*0x008*/     struct _LUID_AND_ATTRIBUTES Privilege[1];                                    
          }PRIVILEGE_SET, *PPRIVILEGE_SET;                                                 
                                                                                           
          typedef struct _UNICODE_STRING  // 3 elements, 0x8 bytes (sizeof) 
          {                                                                 
/*0x000*/     UINT16       Length;                                          
/*0x002*/     UINT16       MaximumLength;                                   
/*0x004*/     UINT16*      Buffer;                                          
          }UNICODE_STRING, *PUNICODE_STRING;                                  
                                                                            
          typedef struct _ACCESS_STATE                                 // 16 elements, 0x74 bytes (sizeof) 
          {                                                                                                
/*0x000*/     struct _LUID OperationID;                                // 2 elements, 0x8 bytes (sizeof)   
/*0x008*/     UINT8        SecurityEvaluated;                                                              
/*0x009*/     UINT8        GenerateAudit;                                                                  
/*0x00A*/     UINT8        GenerateOnClose;                                                                
/*0x00B*/     UINT8        PrivilegesAllocated;                                                            
/*0x00C*/     ULONG32      Flags;                                                                          
/*0x010*/     ULONG32      RemainingDesiredAccess;                                                         
/*0x014*/     ULONG32      PreviouslyGrantedAccess;                                                        
/*0x018*/     ULONG32      OriginalDesiredAccess;                                                          
/*0x01C*/     struct _SECURITY_SUBJECT_CONTEXT SubjectSecurityContext; // 4 elements, 0x10 bytes (sizeof)  
/*0x02C*/     VOID*        SecurityDescriptor;                                                             
/*0x030*/     VOID*        AuxData;                                                                        
              union                                                    // 2 elements, 0x2C bytes (sizeof)  
              {                                                                                            
/*0x034*/         struct _INITIAL_PRIVILEGE_SET InitialPrivilegeSet;   // 3 elements, 0x2C bytes (sizeof)  
/*0x034*/         struct _PRIVILEGE_SET PrivilegeSet;                  // 3 elements, 0x14 bytes (sizeof)  
              }Privileges;                                                                                 
/*0x060*/     UINT8        AuditPrivileges;                                                                
/*0x061*/     UINT8        _PADDING0_[0x3];                                                                
/*0x064*/     struct _UNICODE_STRING ObjectName;                       // 3 elements, 0x8 bytes (sizeof)   
/*0x06C*/     struct _UNICODE_STRING ObjectTypeName;                   // 3 elements, 0x8 bytes (sizeof)   
          }ACCESS_STATE, *PACCESS_STATE;                                                                   
                                                                                                           
          typedef struct _ACL           // 5 elements, 0x8 bytes (sizeof) 
          {                                                               
/*0x000*/     UINT8        AclRevision;                                   
/*0x001*/     UINT8        Sbz1;                                          
/*0x002*/     UINT16       AclSize;                                       
/*0x004*/     UINT16       AceCount;                                      
/*0x006*/     UINT16       Sbz2;                                          
          }ACL, *PACL;                                                    
                                                                          
          typedef struct _LIST_ENTRY     // 2 elements, 0x8 bytes (sizeof) 
          {                                                                
/*0x000*/     struct _LIST_ENTRY* Flink;                                   
/*0x004*/     struct _LIST_ENTRY* Blink;                                   
          }LIST_ENTRY, *PLIST_ENTRY;                                       
                                                                           
          typedef struct _ACTIVATION_CONTEXT_STACK   // 4 elements, 0x14 bytes (sizeof) 
          {                                                                             
/*0x000*/     ULONG32      Flags;                                                       
/*0x004*/     ULONG32      NextCookieSequenceNumber;                                    
/*0x008*/     VOID*        ActiveFrame;                                                 
/*0x00C*/     struct _LIST_ENTRY FrameListCache;     // 2 elements, 0x8 bytes (sizeof)  
          }ACTIVATION_CONTEXT_STACK, *PACTIVATION_CONTEXT_STACK;                                   
                                                                                        
typedef struct _ADAPTER_OBJECT // 0 elements, 0x0 bytes (sizeof) 
{                                                                
}ADAPTER_OBJECT, *PADAPTER_OBJECT;                                  
                                                                 
          typedef struct _AMD64_DBGKD_CONTROL_SET // 4 elements, 0x1C bytes (sizeof) 
          {                                                                          
/*0x000*/     ULONG32      TraceFlag;                                                
/*0x004*/     UINT64       Dr7;                                                      
/*0x00C*/     UINT64       CurrentSymbolStart;                                       
/*0x014*/     UINT64       CurrentSymbolEnd;                                         
          }AMD64_DBGKD_CONTROL_SET, *PAMD64_DBGKD_CONTROL_SET;                                   
                                                                                     
          typedef struct _ARBITER_ALLOCATION_STATE             // 12 elements, 0x38 bytes (sizeof) 
          {                                                                                        
/*0x000*/     UINT64       Start;                                                                  
/*0x008*/     UINT64       End;                                                                    
/*0x010*/     UINT64       CurrentMinimum;                                                         
/*0x018*/     UINT64       CurrentMaximum;                                                         
/*0x020*/     struct _ARBITER_LIST_ENTRY* Entry;                                                   
/*0x024*/     struct _ARBITER_ALTERNATIVE* CurrentAlternative;                                     
/*0x028*/     ULONG32      AlternativeCount;                                                       
/*0x02C*/     struct _ARBITER_ALTERNATIVE* Alternatives;                                           
/*0x030*/     UINT16       Flags;                                                                  
/*0x032*/     UINT8        RangeAttributes;                                                        
/*0x033*/     UINT8        RangeAvailableAttributes;                                               
/*0x034*/     ULONG32      WorkSpace;                                                              
          }ARBITER_ALLOCATION_STATE, *PARBITER_ALLOCATION_STATE;                                    
                                                                                                   
          typedef struct _ARBITER_ALTERNATIVE             // 8 elements, 0x30 bytes (sizeof) 
          {                                                                                  
/*0x000*/     UINT64       Minimum;                                                          
/*0x008*/     UINT64       Maximum;                                                          
/*0x010*/     ULONG32      Length;                                                           
/*0x014*/     ULONG32      Alignment;                                                        
/*0x018*/     LONG32       Priority;                                                         
/*0x01C*/     ULONG32      Flags;                                                            
/*0x020*/     struct _IO_RESOURCE_DESCRIPTOR* Descriptor;                                    
/*0x024*/     ULONG32      Reserved[3];                                                      
          }ARBITER_ALTERNATIVE, *PARBITER_ALTERNATIVE;                                       
                                                                                             
          typedef struct _ARBITER_CONFLICT_INFO    // 3 elements, 0x18 bytes (sizeof) 
          {                                                                           
/*0x000*/     struct _DEVICE_OBJECT* OwningObject;                                    
/*0x004*/     UINT8        _PADDING0_[0x4];                                           
/*0x008*/     UINT64       Start;                                                     
/*0x010*/     UINT64       End;                                                       
          }ARBITER_CONFLICT_INFO, *PARBITER_CONFLICT_INFO;                                   
                                                                                      
          typedef struct _ARBITER_ORDERING_LIST    // 3 elements, 0x8 bytes (sizeof) 
          {                                                                          
/*0x000*/     UINT16       Count;                                                    
/*0x002*/     UINT16       Maximum;                                                  
/*0x004*/     struct _ARBITER_ORDERING* Orderings;                                   
          }ARBITER_ORDERING_LIST, *PARBITER_ORDERING_LIST;                                  
                                                                                     
          typedef struct _ARBITER_INSTANCE                                                                       // 37 elements, 0x9C bytes (sizeof) 
          {                                                                                                                                          
/*0x000*/     ULONG32      Signature;                                                                                                                
/*0x004*/     struct _KEVENT* MutexEvent;                                                                                                            
/*0x008*/     UINT16*      Name;                                                                                                                     
/*0x00C*/     INT32        ResourceType;                                                                                                             
/*0x010*/     struct _RTL_RANGE_LIST* Allocation;                                                                                                    
/*0x014*/     struct _RTL_RANGE_LIST* PossibleAllocation;                                                                                            
/*0x018*/     struct _ARBITER_ORDERING_LIST OrderingList;                                                        // 3 elements, 0x8 bytes (sizeof)   
/*0x020*/     struct _ARBITER_ORDERING_LIST ReservedList;                                                        // 3 elements, 0x8 bytes (sizeof)   
/*0x028*/     LONG32       ReferenceCount;                                                                                                           
/*0x02C*/     struct _ARBITER_INTERFACE* Interface;                                                                                                  
/*0x030*/     ULONG32      AllocationStackMaxSize;                                                                                                   
/*0x034*/     struct _ARBITER_ALLOCATION_STATE* AllocationStack;                                                                                     
/*0x038*/     FUNCT_0049_1682_UnpackRequirement* UnpackRequirement;                                                                                  
/*0x03C*/     FUNCT_0049_168A_PackResource* PackResource;                                                                                            
/*0x040*/     FUNCT_0049_168F_UnpackResource* UnpackResource;                                                                                        
/*0x044*/     FUNCT_0049_1694_ScoreRequirement* ScoreRequirement;                                                                                    
/*0x048*/     FUNCT_0049_1697_TestAllocation_RetestAllocation_BootAllocation_QueryArbitrate* TestAllocation;                                         
/*0x04C*/     FUNCT_0049_1697_TestAllocation_RetestAllocation_BootAllocation_QueryArbitrate* RetestAllocation;                                       
/*0x050*/     FUNCT_0049_169C_CommitAllocation_RollbackAllocation* CommitAllocation;                                                                 
/*0x054*/     FUNCT_0049_169C_CommitAllocation_RollbackAllocation* RollbackAllocation;                                                               
/*0x058*/     FUNCT_0049_1697_TestAllocation_RetestAllocation_BootAllocation_QueryArbitrate* BootAllocation;                                         
/*0x05C*/     FUNCT_0049_1697_TestAllocation_RetestAllocation_BootAllocation_QueryArbitrate* QueryArbitrate;                                         
/*0x060*/     FUNCT_0049_169F_QueryConflict* QueryConflict;                                                                                          
/*0x064*/     FUNCT_0049_16A6_AddReserved* AddReserved;                                                                                              
/*0x068*/     FUNCT_0049_16AB_StartArbiter* StartArbiter;                                                                                            
/*0x06C*/     FUNCT_0049_16AF_PreprocessEntry_AllocateEntry* PreprocessEntry;                                                                        
/*0x070*/     FUNCT_0049_16AF_PreprocessEntry_AllocateEntry* AllocateEntry;                                                                          
/*0x074*/     FUNCT_003D_16B3_GetNextAllocationRange_FindSuitableRange_OverrideConflict* GetNextAllocationRange;                                     
/*0x078*/     FUNCT_003D_16B3_GetNextAllocationRange_FindSuitableRange_OverrideConflict* FindSuitableRange;                                          
/*0x07C*/     FUNCT_00BC_16B5_AddAllocation_BacktrackAllocation* AddAllocation;                                                                      
/*0x080*/     FUNCT_00BC_16B5_AddAllocation_BacktrackAllocation* BacktrackAllocation;                                                                
/*0x084*/     FUNCT_003D_16B3_GetNextAllocationRange_FindSuitableRange_OverrideConflict* OverrideConflict;                                           
/*0x088*/     UINT8        TransactionInProgress;                                                                                                    
/*0x089*/     UINT8        _PADDING0_[0x3];                                                                                                          
/*0x08C*/     VOID*        Extension;                                                                                                                
/*0x090*/     struct _DEVICE_OBJECT* BusDeviceObject;                                                                                                
/*0x094*/     VOID*        ConflictCallbackContext;                                                                                                  
/*0x098*/     FUNCT_003D_16B7_ConflictCallback* ConflictCallback;                                                                                    
          }ARBITER_INSTANCE, *PARBITER_INSTANCE;                                                                                                     
                                                                                                                                                     
          typedef struct _ARBITER_INTERFACE                                                                                                                                                   // 7 elements, 0x18 bytes (sizeof) 
          {                                                                                                                                                                                                                      
/*0x000*/     UINT16       Size;                                                                                                                                                                                                 
/*0x002*/     UINT16       Version;                                                                                                                                                                                              
/*0x004*/     VOID*        Context;                                                                                                                                                                                              
/*0x008*/     FUNCT_00BC_0285_Free_InterfaceReference_InterfaceDereference_DeleteProcedure_ReleaseFromLazyWrite_ReleaseFromReadAhead_WorkerRoutine_Destructor_Callback* InterfaceReference;                                      
/*0x00C*/     FUNCT_00BC_0285_Free_InterfaceReference_InterfaceDereference_DeleteProcedure_ReleaseFromLazyWrite_ReleaseFromReadAhead_WorkerRoutine_Destructor_Callback* InterfaceDereference;                                    
/*0x010*/     FUNCT_0049_1544_ArbiterHandler* ArbiterHandler;                                                                                                                                                                    
/*0x014*/     ULONG32      Flags;                                                                                                                                                                                                
          }ARBITER_INTERFACE, *PARBITER_INTERFACE;                                                                                                                                                                               
                                                                                                                                                                                                                                 
          typedef struct _ARBITER_LIST_ENTRY                       // 13 elements, 0x38 bytes (sizeof) 
          {                                                                                            
/*0x000*/     struct _LIST_ENTRY ListEntry;                        // 2 elements, 0x8 bytes (sizeof)   
/*0x008*/     ULONG32      AlternativeCount;                                                           
/*0x00C*/     struct _IO_RESOURCE_DESCRIPTOR* Alternatives;                                            
/*0x010*/     struct _DEVICE_OBJECT* PhysicalDeviceObject;                                             
/*0x014*/     enum _ARBITER_REQUEST_SOURCE RequestSource;                                              
/*0x018*/     ULONG32      Flags;                                                                      
/*0x01C*/     LONG32       WorkSpace;                                                                  
/*0x020*/     enum _INTERFACE_TYPE InterfaceType;                                                      
/*0x024*/     ULONG32      SlotNumber;                                                                 
/*0x028*/     ULONG32      BusNumber;                                                                  
/*0x02C*/     struct _CM_PARTIAL_RESOURCE_DESCRIPTOR* Assignment;                                      
/*0x030*/     struct _IO_RESOURCE_DESCRIPTOR* SelectedAlternative;                                     
/*0x034*/     enum _ARBITER_RESULT Result;                                                             
          }ARBITER_LIST_ENTRY, *PARBITER_LIST_ENTRY;                                                   
                                                                                                       
          typedef struct _ARBITER_ORDERING // 2 elements, 0x10 bytes (sizeof) 
          {                                                                   
/*0x000*/     UINT64       Start;                                             
/*0x008*/     UINT64       End;                                               
          }ARBITER_ORDERING, *PARBITER_ORDERING;                                   
                                                                              
          typedef struct _ARBITER_PARAMETERS                                 // 1 elements, 0x10 bytes (sizeof) 
          {                                                                                                     
              union                                                          // 7 elements, 0x10 bytes (sizeof) 
              {                                                                                                 
                  struct                                                     // 3 elements, 0xC bytes (sizeof)  
                  {                                                                                             
/*0x000*/             struct _LIST_ENTRY* ArbitrationList;                                                      
/*0x004*/             ULONG32      AllocateFromCount;                                                           
/*0x008*/             struct _CM_PARTIAL_RESOURCE_DESCRIPTOR* AllocateFrom;                                     
                  }TestAllocation;                                                                              
                  struct                                                     // 3 elements, 0xC bytes (sizeof)  
                  {                                                                                             
/*0x000*/             struct _LIST_ENTRY* ArbitrationList;                                                      
/*0x004*/             ULONG32      AllocateFromCount;                                                           
/*0x008*/             struct _CM_PARTIAL_RESOURCE_DESCRIPTOR* AllocateFrom;                                     
                  }RetestAllocation;                                                                            
                  struct                                                     // 1 elements, 0x4 bytes (sizeof)  
                  {                                                                                             
/*0x000*/             struct _LIST_ENTRY* ArbitrationList;                                                      
                  }BootAllocation;                                                                              
                  struct                                                     // 1 elements, 0x4 bytes (sizeof)  
                  {                                                                                             
/*0x000*/             struct _CM_PARTIAL_RESOURCE_LIST** AllocatedResources;                                    
                  }QueryAllocatedResources;                                                                     
                  struct                                                     // 4 elements, 0x10 bytes (sizeof) 
                  {                                                                                             
/*0x000*/             struct _DEVICE_OBJECT* PhysicalDeviceObject;                                              
/*0x004*/             struct _IO_RESOURCE_DESCRIPTOR* ConflictingResource;                                      
/*0x008*/             ULONG32*     ConflictCount;                                                               
/*0x00C*/             struct _ARBITER_CONFLICT_INFO** Conflicts;                                                
                  }QueryConflict;                                                                               
                  struct                                                     // 1 elements, 0x4 bytes (sizeof)  
                  {                                                                                             
/*0x000*/             struct _LIST_ENTRY* ArbitrationList;                                                      
                  }QueryArbitrate;                                                                              
                  struct                                                     // 1 elements, 0x4 bytes (sizeof)  
                  {                                                                                             
/*0x000*/             struct _DEVICE_OBJECT* ReserveDevice;                                                     
                  }AddReserved;                                                                                 
              }Parameters;                                                                                      
          }ARBITER_PARAMETERS, *PARBITER_PARAMETERS;                                                            
                                                                                                                
          typedef struct _BATTERY_REPORTING_SCALE // 2 elements, 0x8 bytes (sizeof) 
          {                                                                         
/*0x000*/     ULONG32      Granularity;                                             
/*0x004*/     ULONG32      Capacity;                                                
          }BATTERY_REPORTING_SCALE, *PBATTERY_REPORTING_SCALE;                                  
                                                                                    
          typedef struct _BITMAP_RANGE     // 6 elements, 0x20 bytes (sizeof) 
          {                                                                   
/*0x000*/     struct _LIST_ENTRY Links;    // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/     INT64        BasePage;                                          
/*0x010*/     ULONG32      FirstDirtyPage;                                    
/*0x014*/     ULONG32      LastDirtyPage;                                     
/*0x018*/     ULONG32      DirtyPages;                                        
/*0x01C*/     ULONG32*     Bitmap;                                            
          }BITMAP_RANGE, *PBITMAP_RANGE;                                      
                                                                              
          typedef struct _BUS_EXTENSION_LIST          // 2 elements, 0x8 bytes (sizeof) 
          {                                                                             
/*0x000*/     VOID*        Next;                                                        
/*0x004*/     struct _PI_BUS_EXTENSION* BusExtension;                                   
          }BUS_EXTENSION_LIST, *PBUS_EXTENSION_LIST;                                    
                                                                                        
          typedef struct _BUS_HANDLER                                   // 24 elements, 0x6C bytes (sizeof) 
          {                                                                                                 
/*0x000*/     ULONG32      Version;                                                                         
/*0x004*/     enum _INTERFACE_TYPE InterfaceType;                                                           
/*0x008*/     enum _BUS_DATA_TYPE ConfigurationType;                                                        
/*0x00C*/     ULONG32      BusNumber;                                                                       
/*0x010*/     struct _DEVICE_OBJECT* DeviceObject;                                                          
/*0x014*/     struct _BUS_HANDLER* ParentHandler;                                                           
/*0x018*/     VOID*        BusData;                                                                         
/*0x01C*/     ULONG32      DeviceControlExtensionSize;                                                      
/*0x020*/     struct _SUPPORTED_RANGES* BusAddresses;                                                       
/*0x024*/     ULONG32      Reserved[4];                                                                     
/*0x034*/     FUNCT_0009_1464_GetBusData_SetBusData* GetBusData;                                            
/*0x038*/     FUNCT_0009_1464_GetBusData_SetBusData* SetBusData;                                            
/*0x03C*/     FUNCT_0049_146C_AdjustResourceList* AdjustResourceList;                                       
/*0x040*/     FUNCT_0049_1472_AssignSlotResources* AssignSlotResources;                                     
/*0x044*/     FUNCT_0009_147D_GetInterruptVector* GetInterruptVector;                                       
/*0x048*/     FUNCT_003D_1485_TranslateBusAddress* TranslateBusAddress;                                     
/*0x04C*/     VOID*        Spare1;                                                                          
/*0x050*/     VOID*        Spare2;                                                                          
/*0x054*/     VOID*        Spare3;                                                                          
/*0x058*/     VOID*        Spare4;                                                                          
/*0x05C*/     VOID*        Spare5;                                                                          
/*0x060*/     VOID*        Spare6;                                                                          
/*0x064*/     VOID*        Spare7;                                                                          
/*0x068*/     VOID*        Spare8;                                                                          
          }BUS_HANDLER, *PBUS_HANDLER;                                                                      
                                                                                                            
          typedef struct _CACHE_MANAGER_CALLBACKS                                                                                                                                             // 4 elements, 0x10 bytes (sizeof) 
          {                                                                                                                                                                                                                      
/*0x000*/     FUNCT_003D_0BDB_AcquireForLazyWrite_AcquireForReadAhead* AcquireForLazyWrite;                                                                                                                                      
/*0x004*/     FUNCT_00BC_0285_Free_InterfaceReference_InterfaceDereference_DeleteProcedure_ReleaseFromLazyWrite_ReleaseFromReadAhead_WorkerRoutine_Destructor_Callback* ReleaseFromLazyWrite;                                    
/*0x008*/     FUNCT_003D_0BDB_AcquireForLazyWrite_AcquireForReadAhead* AcquireForReadAhead;                                                                                                                                      
/*0x00C*/     FUNCT_00BC_0285_Free_InterfaceReference_InterfaceDereference_DeleteProcedure_ReleaseFromLazyWrite_ReleaseFromReadAhead_WorkerRoutine_Destructor_Callback* ReleaseFromReadAhead;                                    
          }CACHE_MANAGER_CALLBACKS, *PCACHE_MANAGER_CALLBACKS;                                                                                                                                                                   
                                                                                                                                                                                                                                 
          typedef struct _DISPATCHER_HEADER    // 6 elements, 0x10 bytes (sizeof) 
          {                                                                       
/*0x000*/     UINT8        Type;                                                  
/*0x001*/     UINT8        Absolute;                                              
/*0x002*/     UINT8        Size;                                                  
/*0x003*/     UINT8        Inserted;                                              
/*0x004*/     LONG32       SignalState;                                           
/*0x008*/     struct _LIST_ENTRY WaitListHead; // 2 elements, 0x8 bytes (sizeof)  
          }DISPATCHER_HEADER, *PDISPATCHER_HEADER;                                   
                                                                                  
          typedef struct _KEVENT                // 1 elements, 0x10 bytes (sizeof) 
          {                                                                        
/*0x000*/     struct _DISPATCHER_HEADER Header; // 6 elements, 0x10 bytes (sizeof) 
          }KEVENT, *PKEVENT;                                                       
                                                                                   
          typedef struct _CACHE_UNINITIALIZE_EVENT    // 2 elements, 0x14 bytes (sizeof) 
          {                                                                              
/*0x000*/     struct _CACHE_UNINITIALIZE_EVENT* Next;                                    
/*0x004*/     struct _KEVENT Event;                   // 1 elements, 0x10 bytes (sizeof) 
          }CACHE_UNINITIALIZE_EVENT, *PCACHE_UNINITIALIZE_EVENT;                                   
                                                                                         
          typedef struct _CACHED_CHILD_LIST              // 3 elements, 0x8 bytes (sizeof) 
          {                                                                                
/*0x000*/     ULONG32      Count;                                                          
              union                                      // 2 elements, 0x4 bytes (sizeof) 
              {                                                                            
/*0x004*/         ULONG32      ValueList;                                                  
/*0x004*/         struct _CM_KEY_CONTROL_BLOCK* RealKcb;                                   
              };                                                                           
          }CACHED_CHILD_LIST, *PCACHED_CHILD_LIST;                                         
                                                                                           
          typedef struct _CALL_HASH_ENTRY   // 4 elements, 0x14 bytes (sizeof) 
          {                                                                    
/*0x000*/     struct _LIST_ENTRY ListEntry; // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/     VOID*        CallersAddress;                                     
/*0x00C*/     VOID*        CallersCaller;                                      
/*0x010*/     ULONG32      CallCount;                                          
          }CALL_HASH_ENTRY, *PCALL_HASH_ENTRY;                                   
                                                                               
          typedef struct _CALL_PERFORMANCE_DATA // 2 elements, 0x204 bytes (sizeof) 
          {                                                                         
/*0x000*/     ULONG32      SpinLock;                                                
/*0x004*/     struct _LIST_ENTRY HashTable[64];                                     
          }CALL_PERFORMANCE_DATA, *PCALL_PERFORMANCE_DATA;                                    
                                                                                    
          typedef union _LARGE_INTEGER   // 4 elements, 0x8 bytes (sizeof) 
          {                                                                
              struct                     // 2 elements, 0x8 bytes (sizeof) 
              {                                                            
/*0x000*/         ULONG32      LowPart;                                    
/*0x004*/         LONG32       HighPart;                                   
              };                                                           
              struct                     // 2 elements, 0x8 bytes (sizeof) 
              {                                                            
/*0x000*/         ULONG32      LowPart;                                    
/*0x004*/         LONG32       HighPart;                                   
              }u;                                                          
/*0x000*/     INT64        QuadPart;                                       
          }LARGE_INTEGER, *PLARGE_INTEGER;                                  
                                                                           
          typedef struct _CHILD_LIST // 2 elements, 0x8 bytes (sizeof) 
          {                                                            
/*0x000*/     ULONG32      Count;                                      
/*0x004*/     ULONG32      List;                                       
          }CHILD_LIST, *PCHILD_LIST;                                   
                                                                       
          typedef struct _CM_KEY_REFERENCE // 2 elements, 0x8 bytes (sizeof) 
          {                                                                  
/*0x000*/     ULONG32      KeyCell;                                          
/*0x004*/     struct _HHIVE* KeyHive;                                        
          }CM_KEY_REFERENCE, *PCM_KEY_REFERENCE;                                  
                                                                             
          typedef struct _CM_KEY_NODE                          // 22 elements, 0x50 bytes (sizeof) 
          {                                                                                        
/*0x000*/     UINT16       Signature;                                                              
/*0x002*/     UINT16       Flags;                                                                  
/*0x004*/     union _LARGE_INTEGER LastWriteTime;              // 4 elements, 0x8 bytes (sizeof)   
/*0x00C*/     ULONG32      Spare;                                                                  
/*0x010*/     ULONG32      Parent;                                                                 
/*0x014*/     ULONG32      SubKeyCounts[2];                                                        
              union                                            // 2 elements, 0x10 bytes (sizeof)  
              {                                                                                    
                  struct                                       // 2 elements, 0x10 bytes (sizeof)  
                  {                                                                                
/*0x01C*/             ULONG32      SubKeyLists[2];                                                 
/*0x024*/             struct _CHILD_LIST ValueList;            // 2 elements, 0x8 bytes (sizeof)   
                  };                                                                               
/*0x01C*/         struct _CM_KEY_REFERENCE ChildHiveReference; // 2 elements, 0x8 bytes (sizeof)   
              };                                                                                   
/*0x02C*/     ULONG32      Security;                                                               
/*0x030*/     ULONG32      Class;                                                                  
              struct                                           // 4 elements, 0x4 bytes (sizeof)   
              {                                                                                    
/*0x034*/         ULONG32      MaxNameLen : 16;                // 0 BitPosition                    
/*0x034*/         ULONG32      UserFlags : 4;                  // 16 BitPosition                   
/*0x034*/         ULONG32      VirtControlFlags : 4;           // 20 BitPosition                   
/*0x034*/         ULONG32      Debug : 8;                      // 24 BitPosition                   
              };                                                                                   
/*0x038*/     ULONG32      MaxClassLen;                                                            
/*0x03C*/     ULONG32      MaxValueNameLen;                                                        
/*0x040*/     ULONG32      MaxValueDataLen;                                                        
/*0x044*/     ULONG32      WorkVar;                                                                
/*0x048*/     UINT16       NameLength;                                                             
/*0x04A*/     UINT16       ClassLength;                                                            
/*0x04C*/     UINT16       Name[1];                                                                
/*0x04E*/     UINT8        _PADDING0_[0x2];                                                        
          }CM_KEY_NODE, *PCM_KEY_NODE;                                                             
                                                                                                   
          typedef struct _CM_KEY_VALUE      // 8 elements, 0x18 bytes (sizeof) 
          {                                                                    
/*0x000*/     UINT16       Signature;                                          
/*0x002*/     UINT16       NameLength;                                         
/*0x004*/     ULONG32      DataLength;                                         
/*0x008*/     ULONG32      Data;                                               
/*0x00C*/     ULONG32      Type;                                               
/*0x010*/     UINT16       Flags;                                              
/*0x012*/     UINT16       Spare;                                              
/*0x014*/     UINT16       Name[1];                                            
/*0x016*/     UINT8        _PADDING0_[0x2];                                    
          }CM_KEY_VALUE, *PCM_KEY_VALUE;                                       
                                                                               
          typedef struct _SECURITY_DESCRIPTOR_RELATIVE // 7 elements, 0x14 bytes (sizeof) 
          {                                                                               
/*0x000*/     UINT8        Revision;                                                      
/*0x001*/     UINT8        Sbz1;                                                          
/*0x002*/     UINT16       Control;                                                       
/*0x004*/     ULONG32      Owner;                                                         
/*0x008*/     ULONG32      Group;                                                         
/*0x00C*/     ULONG32      Sacl;                                                          
/*0x010*/     ULONG32      Dacl;                                                          
          }SECURITY_DESCRIPTOR_RELATIVE, *PSECURITY_DESCRIPTOR_RELATIVE;                                   
                                                                                          
          typedef struct _CM_KEY_SECURITY                      // 7 elements, 0x28 bytes (sizeof) 
          {                                                                                       
/*0x000*/     UINT16       Signature;                                                             
/*0x002*/     UINT16       Reserved;                                                              
/*0x004*/     ULONG32      Flink;                                                                 
/*0x008*/     ULONG32      Blink;                                                                 
/*0x00C*/     ULONG32      ReferenceCount;                                                        
/*0x010*/     ULONG32      DescriptorLength;                                                      
/*0x014*/     struct _SECURITY_DESCRIPTOR_RELATIVE Descriptor; // 7 elements, 0x14 bytes (sizeof) 
          }CM_KEY_SECURITY, *PCM_KEY_SECURITY;                                                    
                                                                                                  
          typedef struct _CM_KEY_INDEX // 3 elements, 0x8 bytes (sizeof) 
          {                                                              
/*0x000*/     UINT16       Signature;                                    
/*0x002*/     UINT16       Count;                                        
/*0x004*/     ULONG32      List[1];                                      
          }CM_KEY_INDEX, *PCM_KEY_INDEX;                                  
                                                                         
          typedef struct _CM_BIG_DATA // 3 elements, 0x8 bytes (sizeof) 
          {                                                             
/*0x000*/     UINT16       Signature;                                   
/*0x002*/     UINT16       Count;                                       
/*0x004*/     ULONG32      List;                                        
          }CM_BIG_DATA, *PCM_BIG_DATA;                                  
                                                                        
          typedef struct _CELL_DATA                    // 1 elements, 0x50 bytes (sizeof)  
          {                                                                                
              union                                    // 7 elements, 0x50 bytes (sizeof)  
              {                                                                            
/*0x000*/         struct _CM_KEY_NODE KeyNode;         // 22 elements, 0x50 bytes (sizeof) 
/*0x000*/         struct _CM_KEY_VALUE KeyValue;       // 8 elements, 0x18 bytes (sizeof)  
/*0x000*/         struct _CM_KEY_SECURITY KeySecurity; // 7 elements, 0x28 bytes (sizeof)  
/*0x000*/         struct _CM_KEY_INDEX KeyIndex;       // 3 elements, 0x8 bytes (sizeof)   
/*0x000*/         struct _CM_BIG_DATA ValueData;       // 3 elements, 0x8 bytes (sizeof)   
/*0x000*/         ULONG32      KeyList[1];                                                 
/*0x000*/         UINT16       KeyString[1];                                               
              }u;                                                                          
          }CELL_DATA, *PCELL_DATA;                                                         
                                                                                           
          typedef struct _CLIENT_ID       // 2 elements, 0x8 bytes (sizeof) 
          {                                                                 
/*0x000*/     VOID*        UniqueProcess;                                   
/*0x004*/     VOID*        UniqueThread;                                    
          }CLIENT_ID, *PCLIENT_ID;                                          
                                                                            
          typedef struct _CM_CACHED_VALUE_INDEX // 2 elements, 0x54 bytes (sizeof) 
          {                                                                        
/*0x000*/     ULONG32      CellIndex;                                              
              union                             // 2 elements, 0x50 bytes (sizeof) 
              {                                                                    
/*0x004*/         struct _CELL_DATA CellData;   // 1 elements, 0x50 bytes (sizeof) 
/*0x004*/         ULONG32      List[1];                                            
              }Data;                                                               
          }CM_CACHED_VALUE_INDEX, *PCM_CACHED_VALUE_INDEX;                                   
                                                                                   
          typedef struct _CM_CELL_REMAP_BLOCK // 2 elements, 0x8 bytes (sizeof) 
          {                                                                     
/*0x000*/     ULONG32      OldCell;                                             
/*0x004*/     ULONG32      NewCell;                                             
          }CM_CELL_REMAP_BLOCK, *PCM_CELL_REMAP_BLOCK;                                  
                                                                                
          typedef struct _CM_PARTIAL_RESOURCE_DESCRIPTOR // 4 elements, 0x10 bytes (sizeof) 
          {                                                                                 
/*0x000*/     UINT8        Type;                                                            
/*0x001*/     UINT8        ShareDisposition;                                                
/*0x002*/     UINT16       Flags;                                                           
              union                                      // 8 elements, 0xC bytes (sizeof)  
              {                                                                             
                  struct                                 // 2 elements, 0xC bytes (sizeof)  
                  {                                                                         
/*0x004*/             union _LARGE_INTEGER Start;        // 4 elements, 0x8 bytes (sizeof)  
/*0x00C*/             ULONG32      Length;                                                  
                  }Generic;                                                                 
                  struct                                 // 2 elements, 0xC bytes (sizeof)  
                  {                                                                         
/*0x004*/             union _LARGE_INTEGER Start;        // 4 elements, 0x8 bytes (sizeof)  
/*0x00C*/             ULONG32      Length;                                                  
                  }Port;                                                                    
                  struct                                 // 3 elements, 0xC bytes (sizeof)  
                  {                                                                         
/*0x004*/             ULONG32      Level;                                                   
/*0x008*/             ULONG32      Vector;                                                  
/*0x00C*/             ULONG32      Affinity;                                                
                  }Interrupt;                                                               
                  struct                                 // 2 elements, 0xC bytes (sizeof)  
                  {                                                                         
/*0x004*/             union _LARGE_INTEGER Start;        // 4 elements, 0x8 bytes (sizeof)  
/*0x00C*/             ULONG32      Length;                                                  
                  }Memory;                                                                  
                  struct                                 // 3 elements, 0xC bytes (sizeof)  
                  {                                                                         
/*0x004*/             ULONG32      Channel;                                                 
/*0x008*/             ULONG32      Port;                                                    
/*0x00C*/             ULONG32      Reserved1;                                               
                  }Dma;                                                                     
                  struct                                 // 1 elements, 0xC bytes (sizeof)  
                  {                                                                         
/*0x004*/             ULONG32      Data[3];                                                 
                  }DevicePrivate;                                                           
                  struct                                 // 3 elements, 0xC bytes (sizeof)  
                  {                                                                         
/*0x004*/             ULONG32      Start;                                                   
/*0x008*/             ULONG32      Length;                                                  
/*0x00C*/             ULONG32      Reserved;                                                
                  }BusNumber;                                                               
                  struct                                 // 3 elements, 0xC bytes (sizeof)  
                  {                                                                         
/*0x004*/             ULONG32      DataSize;                                                
/*0x008*/             ULONG32      Reserved1;                                               
/*0x00C*/             ULONG32      Reserved2;                                               
                  }DeviceSpecificData;                                                      
              }u;                                                                           
          }CM_PARTIAL_RESOURCE_DESCRIPTOR, *PCM_PARTIAL_RESOURCE_DESCRIPTOR;                                   
                                                                                            
          typedef struct _CM_PARTIAL_RESOURCE_LIST                          // 4 elements, 0x18 bytes (sizeof) 
          {                                                                                                    
/*0x000*/     UINT16       Version;                                                                            
/*0x002*/     UINT16       Revision;                                                                           
/*0x004*/     ULONG32      Count;                                                                              
/*0x008*/     struct _CM_PARTIAL_RESOURCE_DESCRIPTOR PartialDescriptors[1];                                    
          }CM_PARTIAL_RESOURCE_LIST, *PCM_PARTIAL_RESOURCE_LIST;                                               
                                                                                                               
          typedef struct _CM_FULL_RESOURCE_DESCRIPTOR               // 3 elements, 0x20 bytes (sizeof) 
          {                                                                                            
/*0x000*/     enum _INTERFACE_TYPE InterfaceType;                                                      
/*0x004*/     ULONG32      BusNumber;                                                                  
/*0x008*/     struct _CM_PARTIAL_RESOURCE_LIST PartialResourceList; // 4 elements, 0x18 bytes (sizeof) 
          }CM_FULL_RESOURCE_DESCRIPTOR, *PCM_FULL_RESOURCE_DESCRIPTOR;                                   
                                                                                                       
          typedef struct _CM_INDEX_HINT_BLOCK // 2 elements, 0x8 bytes (sizeof) 
          {                                                                     
/*0x000*/     ULONG32      Count;                                               
/*0x004*/     ULONG32      HashKey[1];                                          
          }CM_INDEX_HINT_BLOCK, *PCM_INDEX_HINT_BLOCK;                                  
                                                                                
          typedef struct _CM_KEY_BODY                        // 7 elements, 0x44 bytes (sizeof) 
          {                                                                                     
/*0x000*/     ULONG32      Type;                                                                
/*0x004*/     struct _CM_KEY_CONTROL_BLOCK* KeyControlBlock;                                    
/*0x008*/     struct _CM_NOTIFY_BLOCK* NotifyBlock;                                             
/*0x00C*/     VOID*        ProcessID;                                                           
/*0x010*/     ULONG32      Callers;                                                             
/*0x014*/     VOID*        CallerAddress[10];                                                   
/*0x03C*/     struct _LIST_ENTRY KeyBodyList;                // 2 elements, 0x8 bytes (sizeof)  
          }CM_KEY_BODY, *PCM_KEY_BODY;                                                          
                                                                                                
          typedef struct _CM_KEY_HASH        // 4 elements, 0x10 bytes (sizeof) 
          {                                                                     
/*0x000*/     ULONG32      ConvKey;                                             
/*0x004*/     struct _CM_KEY_HASH* NextHash;                                    
/*0x008*/     struct _HHIVE* KeyHive;                                           
/*0x00C*/     ULONG32      KeyCell;                                             
          }CM_KEY_HASH, *PCM_KEY_HASH;                                          
                                                                                
          typedef struct _CM_KEY_CONTROL_BLOCK               // 28 elements, 0x50 bytes (sizeof) 
          {                                                                                      
/*0x000*/     ULONG32      RefCount;                                                             
              struct                                         // 5 elements, 0x4 bytes (sizeof)   
              {                                                                                  
/*0x004*/         ULONG32      ExtFlags : 8;                 // 0 BitPosition                    
/*0x004*/         ULONG32      PrivateAlloc : 1;             // 8 BitPosition                    
/*0x004*/         ULONG32      Delete : 1;                   // 9 BitPosition                    
/*0x004*/         ULONG32      DelayedCloseIndex : 12;       // 10 BitPosition                   
/*0x004*/         ULONG32      TotalLevels : 10;             // 22 BitPosition                   
              };                                                                                 
              union                                          // 2 elements, 0x10 bytes (sizeof)  
              {                                                                                  
/*0x008*/         struct _CM_KEY_HASH KeyHash;               // 4 elements, 0x10 bytes (sizeof)  
                  struct                                     // 4 elements, 0x10 bytes (sizeof)  
                  {                                                                              
/*0x008*/             ULONG32      ConvKey;                                                      
/*0x00C*/             struct _CM_KEY_HASH* NextHash;                                             
/*0x010*/             struct _HHIVE* KeyHive;                                                    
/*0x014*/             ULONG32      KeyCell;                                                      
                  };                                                                             
              };                                                                                 
/*0x018*/     struct _CM_KEY_CONTROL_BLOCK* ParentKcb;                                           
/*0x01C*/     struct _CM_NAME_CONTROL_BLOCK* NameBlock;                                          
/*0x020*/     struct _CM_KEY_SECURITY_CACHE* CachedSecurity;                                     
/*0x024*/     struct _CACHED_CHILD_LIST ValueCache;          // 3 elements, 0x8 bytes (sizeof)   
              union                                          // 3 elements, 0x4 bytes (sizeof)   
              {                                                                                  
/*0x02C*/         struct _CM_INDEX_HINT_BLOCK* IndexHint;                                        
/*0x02C*/         ULONG32      HashKey;                                                          
/*0x02C*/         ULONG32      SubKeyCount;                                                      
              };                                                                                 
              union                                          // 2 elements, 0x8 bytes (sizeof)   
              {                                                                                  
/*0x030*/         struct _LIST_ENTRY KeyBodyListHead;        // 2 elements, 0x8 bytes (sizeof)   
/*0x030*/         struct _LIST_ENTRY FreeListEntry;          // 2 elements, 0x8 bytes (sizeof)   
              };                                                                                 
/*0x038*/     union _LARGE_INTEGER KcbLastWriteTime;         // 4 elements, 0x8 bytes (sizeof)   
/*0x040*/     UINT16       KcbMaxNameLen;                                                        
/*0x042*/     UINT16       KcbMaxValueNameLen;                                                   
/*0x044*/     ULONG32      KcbMaxValueDataLen;                                                   
              struct                                         // 4 elements, 0x4 bytes (sizeof)   
              {                                                                                  
/*0x048*/         ULONG32      KcbUserFlags : 4;             // 0 BitPosition                    
/*0x048*/         ULONG32      KcbVirtControlFlags : 4;      // 4 BitPosition                    
/*0x048*/         ULONG32      KcbDebug : 8;                 // 8 BitPosition                    
/*0x048*/         ULONG32      Flags : 16;                   // 16 BitPosition                   
              };                                                                                 
          }CM_KEY_CONTROL_BLOCK, *PCM_KEY_CONTROL_BLOCK;                                         
                                                                                                 
          typedef struct _CM_KEY_SECURITY_CACHE                // 5 elements, 0x28 bytes (sizeof) 
          {                                                                                       
/*0x000*/     ULONG32      Cell;                                                                  
/*0x004*/     ULONG32      ConvKey;                                                               
/*0x008*/     struct _LIST_ENTRY List;                         // 2 elements, 0x8 bytes (sizeof)  
/*0x010*/     ULONG32      DescriptorLength;                                                      
/*0x014*/     struct _SECURITY_DESCRIPTOR_RELATIVE Descriptor; // 7 elements, 0x14 bytes (sizeof) 
          }CM_KEY_SECURITY_CACHE, *PCM_KEY_SECURITY_CACHE;                                        
                                                                                                  
          typedef struct _CM_KEY_SECURITY_CACHE_ENTRY        // 2 elements, 0x8 bytes (sizeof) 
          {                                                                                    
/*0x000*/     ULONG32      Cell;                                                               
/*0x004*/     struct _CM_KEY_SECURITY_CACHE* CachedSecurity;                                   
          }CM_KEY_SECURITY_CACHE_ENTRY, *PCM_KEY_SECURITY_CACHE_ENTRY;                                  
                                                                                               
          typedef struct _CM_NAME_HASH        // 4 elements, 0xC bytes (sizeof) 
          {                                                                     
/*0x000*/     ULONG32      ConvKey;                                             
/*0x004*/     struct _CM_NAME_HASH* NextHash;                                   
/*0x008*/     UINT16       NameLength;                                          
/*0x00A*/     UINT16       Name[1];                                             
          }CM_NAME_HASH, *PCM_NAME_HASH;                                        
                                                                                
          typedef struct _CM_NAME_CONTROL_BLOCK      // 7 elements, 0x10 bytes (sizeof) 
          {                                                                             
/*0x000*/     UINT8        Compressed;                                                  
/*0x001*/     UINT8        _PADDING0_[0x1];                                             
/*0x002*/     UINT16       RefCount;                                                    
              union                                  // 2 elements, 0xC bytes (sizeof)  
              {                                                                         
/*0x004*/         struct _CM_NAME_HASH NameHash;     // 4 elements, 0xC bytes (sizeof)  
                  struct                             // 4 elements, 0xC bytes (sizeof)  
                  {                                                                     
/*0x004*/             ULONG32      ConvKey;                                             
/*0x008*/             struct _CM_KEY_HASH* NextHash;                                    
/*0x00C*/             UINT16       NameLength;                                          
/*0x00E*/             UINT16       Name[1];                                             
                  };                                                                    
              };                                                                        
          }CM_NAME_CONTROL_BLOCK, *PCM_NAME_CONTROL_BLOCK;                                   
                                                                                        
          typedef struct _CM_NOTIFY_BLOCK                      // 8 elements, 0x2C bytes (sizeof) 
          {                                                                                       
/*0x000*/     struct _LIST_ENTRY HiveList;                     // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/     struct _LIST_ENTRY PostList;                     // 2 elements, 0x8 bytes (sizeof)  
/*0x010*/     struct _CM_KEY_CONTROL_BLOCK* KeyControlBlock;                                      
/*0x014*/     struct _CM_KEY_BODY* KeyBody;                                                       
              struct                                           // 3 elements, 0x4 bytes (sizeof)  
              {                                                                                   
/*0x018*/         ULONG32      Filter : 30;                    // 0 BitPosition                   
/*0x018*/         ULONG32      WatchTree : 1;                  // 30 BitPosition                  
/*0x018*/         ULONG32      NotifyPending : 1;              // 31 BitPosition                  
              };                                                                                  
/*0x01C*/     struct _SECURITY_SUBJECT_CONTEXT SubjectContext; // 4 elements, 0x10 bytes (sizeof) 
          }CM_NOTIFY_BLOCK, *PCM_NOTIFY_BLOCK;                                                    
                                                                                                  
          typedef struct _CM_RESOURCE_LIST                 // 2 elements, 0x24 bytes (sizeof) 
          {                                                                                   
/*0x000*/     ULONG32      Count;                                                             
/*0x004*/     struct _CM_FULL_RESOURCE_DESCRIPTOR List[1];                                    
          }CM_RESOURCE_LIST, *PCM_RESOURCE_LIST;                                              
                                                                                              
          typedef struct _CM_VIEW_OF_FILE     // 7 elements, 0x24 bytes (sizeof) 
          {                                                                      
/*0x000*/     struct _LIST_ENTRY LRUViewList; // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/     struct _LIST_ENTRY PinViewList; // 2 elements, 0x8 bytes (sizeof)  
/*0x010*/     ULONG32      FileOffset;                                           
/*0x014*/     ULONG32      Size;                                                 
/*0x018*/     ULONG32*     ViewAddress;                                          
/*0x01C*/     VOID*        Bcb;                                                  
/*0x020*/     ULONG32      UseCount;                                             
          }CM_VIEW_OF_FILE, *PCM_VIEW_OF_FILE;                                   
                                                                                 
          typedef struct _RTL_BITMAP     // 2 elements, 0x8 bytes (sizeof) 
          {                                                                
/*0x000*/     ULONG32      SizeOfBitMap;                                   
/*0x004*/     ULONG32*     Buffer;                                         
          }RTL_BITMAP, *PRTL_BITMAP;                                       
                                                                           
          typedef struct _DUAL                    // 7 elements, 0xDC bytes (sizeof) 
          {                                                                          
/*0x000*/     ULONG32      Length;                                                   
/*0x004*/     struct _HMAP_DIRECTORY* Map;                                           
/*0x008*/     struct _HMAP_TABLE* SmallDir;                                          
/*0x00C*/     ULONG32      Guard;                                                    
/*0x010*/     struct _RTL_BITMAP FreeDisplay[24];                                    
/*0x0D0*/     ULONG32      FreeSummary;                                              
/*0x0D4*/     struct _LIST_ENTRY FreeBins;        // 2 elements, 0x8 bytes (sizeof)  
          }DUAL, *PDUAL;                                                             
                                                                                     
          typedef struct _HHIVE                                       // 24 elements, 0x210 bytes (sizeof) 
          {                                                                                                
/*0x000*/     ULONG32      Signature;                                                                      
/*0x004*/     FUNCT_0F22_0F21_GetCellRoutine* GetCellRoutine;                                              
/*0x008*/     FUNCT_00BC_0F6F_ReleaseCellRoutine* ReleaseCellRoutine;                                      
/*0x00C*/     FUNCT_00BB_0F71_Allocate* Allocate;                                                          
/*0x010*/     FUNCT_00BC_0F76_Free* Free;                                                                  
/*0x014*/     FUNCT_003D_0F7A_FileSetSize* FileSetSize;                                                    
/*0x018*/     FUNCT_003D_0F80_FileWrite* FileWrite;                                                        
/*0x01C*/     FUNCT_003D_0F8C_FileRead* FileRead;                                                          
/*0x020*/     FUNCT_003D_0F93_FileFlush* FileFlush;                                                        
/*0x024*/     struct _HBASE_BLOCK* BaseBlock;                                                              
/*0x028*/     struct _RTL_BITMAP DirtyVector;                         // 2 elements, 0x8 bytes (sizeof)    
/*0x030*/     ULONG32      DirtyCount;                                                                     
/*0x034*/     ULONG32      DirtyAlloc;                                                                     
/*0x038*/     UINT8        RealWrites;                                                                     
/*0x039*/     UINT8        _PADDING0_[0x3];                                                                
/*0x03C*/     ULONG32      Cluster;                                                                        
/*0x040*/     UINT8        Flat;                                                                           
/*0x041*/     UINT8        ReadOnly;                                                                       
/*0x042*/     UINT8        Log;                                                                            
/*0x043*/     UINT8        _PADDING1_[0x1];                                                                
/*0x044*/     ULONG32      HiveFlags;                                                                      
/*0x048*/     ULONG32      LogSize;                                                                        
/*0x04C*/     ULONG32      RefreshCount;                                                                   
/*0x050*/     ULONG32      StorageTypeCount;                                                               
/*0x054*/     ULONG32      Version;                                                                        
/*0x058*/     struct _DUAL Storage[2];                                                                     
          }HHIVE, *PHHIVE;                                                                                 
                                                                                                           
          typedef struct _CMHIVE                                  // 28 elements, 0x49C bytes (sizeof) 
          {                                                                                            
/*0x000*/     struct _HHIVE Hive;                                 // 24 elements, 0x210 bytes (sizeof) 
/*0x210*/     VOID*        FileHandles[3];                                                             
/*0x21C*/     struct _LIST_ENTRY NotifyList;                      // 2 elements, 0x8 bytes (sizeof)    
/*0x224*/     struct _LIST_ENTRY HiveList;                        // 2 elements, 0x8 bytes (sizeof)    
/*0x22C*/     struct _FAST_MUTEX* HiveLock;                                                            
/*0x230*/     struct _FAST_MUTEX* ViewLock;                                                            
/*0x234*/     struct _LIST_ENTRY LRUViewListHead;                 // 2 elements, 0x8 bytes (sizeof)    
/*0x23C*/     struct _LIST_ENTRY PinViewListHead;                 // 2 elements, 0x8 bytes (sizeof)    
/*0x244*/     struct _FILE_OBJECT* FileObject;                                                         
/*0x248*/     struct _UNICODE_STRING FileFullPath;                // 3 elements, 0x8 bytes (sizeof)    
/*0x250*/     struct _UNICODE_STRING FileUserName;                // 3 elements, 0x8 bytes (sizeof)    
/*0x258*/     UINT16       MappedViews;                                                                
/*0x25A*/     UINT16       PinnedViews;                                                                
/*0x25C*/     ULONG32      UseCount;                                                                   
/*0x260*/     ULONG32      SecurityCount;                                                              
/*0x264*/     ULONG32      SecurityCacheSize;                                                          
/*0x268*/     LONG32       SecurityHitHint;                                                            
/*0x26C*/     struct _CM_KEY_SECURITY_CACHE_ENTRY* SecurityCache;                                      
/*0x270*/     struct _LIST_ENTRY SecurityHash[64];                                                     
/*0x470*/     struct _KEVENT* UnloadEvent;                                                             
/*0x474*/     struct _CM_KEY_CONTROL_BLOCK* RootKcb;                                                   
/*0x478*/     UINT8        Frozen;                                                                     
/*0x479*/     UINT8        _PADDING0_[0x3];                                                            
/*0x47C*/     struct _WORK_QUEUE_ITEM* UnloadWorkItem;                                                 
/*0x480*/     UINT8        GrowOnlyMode;                                                               
/*0x481*/     UINT8        _PADDING1_[0x3];                                                            
/*0x484*/     ULONG32      GrowOffset;                                                                 
/*0x488*/     struct _LIST_ENTRY KcbConvertListHead;              // 2 elements, 0x8 bytes (sizeof)    
/*0x490*/     struct _LIST_ENTRY KnodeConvertListHead;            // 2 elements, 0x8 bytes (sizeof)    
/*0x498*/     struct _CM_CELL_REMAP_BLOCK* CellRemapArray;                                             
          }CMHIVE, *PCMHIVE;                                                                           
                                                                                                       
          typedef struct _CMP_OFFSET_ARRAY // 3 elements, 0xC bytes (sizeof) 
          {                                                                  
/*0x000*/     ULONG32      FileOffset;                                       
/*0x004*/     VOID*        DataBuffer;                                       
/*0x008*/     ULONG32      DataLength;                                       
          }CMP_OFFSET_ARRAY, *PCMP_OFFSET_ARRAY;                                  
                                                                             
          typedef struct _COMPRESSED_DATA_INFO         // 7 elements, 0xC bytes (sizeof) 
          {                                                                              
/*0x000*/     UINT16       CompressionFormatAndEngine;                                   
/*0x002*/     UINT8        CompressionUnitShift;                                         
/*0x003*/     UINT8        ChunkShift;                                                   
/*0x004*/     UINT8        ClusterShift;                                                 
/*0x005*/     UINT8        Reserved;                                                     
/*0x006*/     UINT16       NumberOfChunks;                                               
/*0x008*/     ULONG32      CompressedChunkSizes[1];                                      
          }COMPRESSED_DATA_INFO, *PCOMPRESSED_DATA_INFO;                                  
                                                                                         
          typedef struct _FLOATING_SAVE_AREA // 9 elements, 0x70 bytes (sizeof) 
          {                                                                     
/*0x000*/     ULONG32      ControlWord;                                         
/*0x004*/     ULONG32      StatusWord;                                          
/*0x008*/     ULONG32      TagWord;                                             
/*0x00C*/     ULONG32      ErrorOffset;                                         
/*0x010*/     ULONG32      ErrorSelector;                                       
/*0x014*/     ULONG32      DataOffset;                                          
/*0x018*/     ULONG32      DataSelector;                                        
/*0x01C*/     UINT8        RegisterArea[80];                                    
/*0x06C*/     ULONG32      Cr0NpxState;                                         
          }FLOATING_SAVE_AREA, *PFLOATING_SAVE_AREA;                                   
                                                                                
          typedef struct _CONTEXT                   // 25 elements, 0x2CC bytes (sizeof) 
          {                                                                              
/*0x000*/     ULONG32      ContextFlags;                                                 
/*0x004*/     ULONG32      Dr0;                                                          
/*0x008*/     ULONG32      Dr1;                                                          
/*0x00C*/     ULONG32      Dr2;                                                          
/*0x010*/     ULONG32      Dr3;                                                          
/*0x014*/     ULONG32      Dr6;                                                          
/*0x018*/     ULONG32      Dr7;                                                          
/*0x01C*/     struct _FLOATING_SAVE_AREA FloatSave; // 9 elements, 0x70 bytes (sizeof)   
/*0x08C*/     ULONG32      SegGs;                                                        
/*0x090*/     ULONG32      SegFs;                                                        
/*0x094*/     ULONG32      SegEs;                                                        
/*0x098*/     ULONG32      SegDs;                                                        
/*0x09C*/     ULONG32      Edi;                                                          
/*0x0A0*/     ULONG32      Esi;                                                          
/*0x0A4*/     ULONG32      Ebx;                                                          
/*0x0A8*/     ULONG32      Edx;                                                          
/*0x0AC*/     ULONG32      Ecx;                                                          
/*0x0B0*/     ULONG32      Eax;                                                          
/*0x0B4*/     ULONG32      Ebp;                                                          
/*0x0B8*/     ULONG32      Eip;                                                          
/*0x0BC*/     ULONG32      SegCs;                                                        
/*0x0C0*/     ULONG32      EFlags;                                                       
/*0x0C4*/     ULONG32      Esp;                                                          
/*0x0C8*/     ULONG32      SegSs;                                                        
/*0x0CC*/     UINT8        ExtendedRegisters[512];                                       
          }CONTEXT, *PCONTEXT;                                                           
                                                                                         
          typedef struct _MMSECTION_FLAGS                // 31 elements, 0x4 bytes (sizeof) 
          {                                                                                 
/*0x000*/     UINT32       BeingDeleted : 1;             // 0 BitPosition                   
/*0x000*/     UINT32       BeingCreated : 1;             // 1 BitPosition                   
/*0x000*/     UINT32       BeingPurged : 1;              // 2 BitPosition                   
/*0x000*/     UINT32       NoModifiedWriting : 1;        // 3 BitPosition                   
/*0x000*/     UINT32       FailAllIo : 1;                // 4 BitPosition                   
/*0x000*/     UINT32       Image : 1;                    // 5 BitPosition                   
/*0x000*/     UINT32       Based : 1;                    // 6 BitPosition                   
/*0x000*/     UINT32       File : 1;                     // 7 BitPosition                   
/*0x000*/     UINT32       Networked : 1;                // 8 BitPosition                   
/*0x000*/     UINT32       NoCache : 1;                  // 9 BitPosition                   
/*0x000*/     UINT32       PhysicalMemory : 1;           // 10 BitPosition                  
/*0x000*/     UINT32       CopyOnWrite : 1;              // 11 BitPosition                  
/*0x000*/     UINT32       Reserve : 1;                  // 12 BitPosition                  
/*0x000*/     UINT32       Commit : 1;                   // 13 BitPosition                  
/*0x000*/     UINT32       FloppyMedia : 1;              // 14 BitPosition                  
/*0x000*/     UINT32       WasPurged : 1;                // 15 BitPosition                  
/*0x000*/     UINT32       UserReference : 1;            // 16 BitPosition                  
/*0x000*/     UINT32       GlobalMemory : 1;             // 17 BitPosition                  
/*0x000*/     UINT32       DeleteOnClose : 1;            // 18 BitPosition                  
/*0x000*/     UINT32       FilePointerNull : 1;          // 19 BitPosition                  
/*0x000*/     UINT32       DebugSymbolsLoaded : 1;       // 20 BitPosition                  
/*0x000*/     UINT32       SetMappedFileIoComplete : 1;  // 21 BitPosition                  
/*0x000*/     UINT32       CollidedFlush : 1;            // 22 BitPosition                  
/*0x000*/     UINT32       NoChange : 1;                 // 23 BitPosition                  
/*0x000*/     UINT32       HadUserReference : 1;         // 24 BitPosition                  
/*0x000*/     UINT32       ImageMappedInSystemSpace : 1; // 25 BitPosition                  
/*0x000*/     UINT32       UserWritable : 1;             // 26 BitPosition                  
/*0x000*/     UINT32       Accessed : 1;                 // 27 BitPosition                  
/*0x000*/     UINT32       GlobalOnlyPerSession : 1;     // 28 BitPosition                  
/*0x000*/     UINT32       Rom : 1;                      // 29 BitPosition                  
/*0x000*/     UINT32       filler : 2;                   // 30 BitPosition                  
          }MMSECTION_FLAGS, *PMMSECTION_FLAGS;                                              
                                                                                            
          typedef struct _CONTROL_AREA                   // 13 elements, 0x30 bytes (sizeof) 
          {                                                                                  
/*0x000*/     struct _SEGMENT* Segment;                                                      
/*0x004*/     struct _LIST_ENTRY DereferenceList;        // 2 elements, 0x8 bytes (sizeof)   
/*0x00C*/     ULONG32      NumberOfSectionReferences;                                        
/*0x010*/     ULONG32      NumberOfPfnReferences;                                            
/*0x014*/     ULONG32      NumberOfMappedViews;                                              
/*0x018*/     UINT16       NumberOfSubsections;                                              
/*0x01A*/     UINT16       FlushInProgressCount;                                             
/*0x01C*/     ULONG32      NumberOfUserReferences;                                           
              union                                      // 2 elements, 0x4 bytes (sizeof)   
              {                                                                              
/*0x020*/         ULONG32      LongFlags;                                                    
/*0x020*/         struct _MMSECTION_FLAGS Flags;         // 31 elements, 0x4 bytes (sizeof)  
              }u;                                                                            
/*0x024*/     struct _FILE_OBJECT* FilePointer;                                              
/*0x028*/     struct _EVENT_COUNTER* WaitingForDeletion;                                     
/*0x02C*/     UINT16       ModifiedWriteCount;                                               
/*0x02E*/     UINT16       NumberOfSystemCacheViews;                                         
          }CONTROL_AREA, *PCONTROL_AREA;                                                     
                                                                                             
          typedef struct _CURDIR              // 2 elements, 0xC bytes (sizeof) 
          {                                                                     
/*0x000*/     struct _UNICODE_STRING DosPath; // 3 elements, 0x8 bytes (sizeof) 
/*0x008*/     VOID*        Handle;                                              
          }CURDIR, *PCURDIR;                                                    
                                                                                
          typedef struct _X86_DBGKD_CONTROL_SET // 4 elements, 0x10 bytes (sizeof) 
          {                                                                        
/*0x000*/     ULONG32      TraceFlag;                                              
/*0x004*/     ULONG32      Dr7;                                                    
/*0x008*/     ULONG32      CurrentSymbolStart;                                     
/*0x00C*/     ULONG32      CurrentSymbolEnd;                                       
          }X86_DBGKD_CONTROL_SET, *PX86_DBGKD_CONTROL_SET;                                   
                                                                                   
          typedef struct _IA64_DBGKD_CONTROL_SET // 3 elements, 0x14 bytes (sizeof) 
          {                                                                         
/*0x000*/     ULONG32      Continue;                                                
/*0x004*/     UINT64       CurrentSymbolStart;                                      
/*0x00C*/     UINT64       CurrentSymbolEnd;                                        
          }IA64_DBGKD_CONTROL_SET, *PIA64_DBGKD_CONTROL_SET;                                   
                                                                                    
          typedef struct _DBGKD_ANY_CONTROL_SET                    // 4 elements, 0x1C bytes (sizeof) 
          {                                                                                           
              union                                                // 4 elements, 0x1C bytes (sizeof) 
              {                                                                                       
/*0x000*/         struct _X86_DBGKD_CONTROL_SET X86ControlSet;     // 4 elements, 0x10 bytes (sizeof) 
/*0x000*/         ULONG32      AlphaControlSet;                                                       
/*0x000*/         struct _IA64_DBGKD_CONTROL_SET IA64ControlSet;   // 3 elements, 0x14 bytes (sizeof) 
/*0x000*/         struct _AMD64_DBGKD_CONTROL_SET Amd64ControlSet; // 4 elements, 0x1C bytes (sizeof) 
              };                                                                                      
          }DBGKD_ANY_CONTROL_SET, *PDBGKD_ANY_CONTROL_SET;                                            
                                                                                                      
          typedef struct _DBGKD_BREAKPOINTEX // 2 elements, 0x8 bytes (sizeof) 
          {                                                                    
/*0x000*/     ULONG32      BreakPointCount;                                    
/*0x004*/     LONG32       ContinueStatus;                                     
          }DBGKD_BREAKPOINTEX, *PDBGKD_BREAKPOINTEX;                                  
                                                                               
          typedef struct _DBGKD_CONTINUE   // 1 elements, 0x4 bytes (sizeof) 
          {                                                                  
/*0x000*/     LONG32       ContinueStatus;                                   
          }DBGKD_CONTINUE, *PDBGKD_CONTINUE;                                  
                                                                             
          typedef struct _DBGKD_CONTINUE2                      // 3 elements, 0x20 bytes (sizeof) 
          {                                                                                       
/*0x000*/     LONG32       ContinueStatus;                                                        
              union                                            // 2 elements, 0x1C bytes (sizeof) 
              {                                                                                   
/*0x004*/         struct _X86_DBGKD_CONTROL_SET ControlSet;    // 4 elements, 0x10 bytes (sizeof) 
/*0x004*/         struct _DBGKD_ANY_CONTROL_SET AnyControlSet; // 4 elements, 0x1C bytes (sizeof) 
              };                                                                                  
          }DBGKD_CONTINUE2, *PDBGKD_CONTINUE2;                                                    
                                                                                                  
          typedef struct _DBGKD_FILL_MEMORY // 4 elements, 0x10 bytes (sizeof) 
          {                                                                    
/*0x000*/     UINT64       Address;                                            
/*0x008*/     ULONG32      Length;                                             
/*0x00C*/     UINT16       Flags;                                              
/*0x00E*/     UINT16       PatternLength;                                      
          }DBGKD_FILL_MEMORY, *PDBGKD_FILL_MEMORY;                                   
                                                                               
          typedef struct _DBGKD_GET_CONTEXT // 1 elements, 0x4 bytes (sizeof) 
          {                                                                   
/*0x000*/     ULONG32      Unused;                                            
          }DBGKD_GET_CONTEXT, *PDBGKD_GET_CONTEXT;                                  
                                                                              
          typedef struct _DBGKD_GET_INTERNAL_BREAKPOINT32 // 7 elements, 0x1C bytes (sizeof) 
          {                                                                                  
/*0x000*/     ULONG32      BreakpointAddress;                                                
/*0x004*/     ULONG32      Flags;                                                            
/*0x008*/     ULONG32      Calls;                                                            
/*0x00C*/     ULONG32      MaxCallsPerPeriod;                                                
/*0x010*/     ULONG32      MinInstructions;                                                  
/*0x014*/     ULONG32      MaxInstructions;                                                  
/*0x018*/     ULONG32      TotalInstructions;                                                
          }DBGKD_GET_INTERNAL_BREAKPOINT32, *PDBGKD_GET_INTERNAL_BREAKPOINT32;                                   
                                                                                             
          typedef struct _DBGKD_GET_INTERNAL_BREAKPOINT64 // 7 elements, 0x20 bytes (sizeof) 
          {                                                                                  
/*0x000*/     UINT64       BreakpointAddress;                                                
/*0x008*/     ULONG32      Flags;                                                            
/*0x00C*/     ULONG32      Calls;                                                            
/*0x010*/     ULONG32      MaxCallsPerPeriod;                                                
/*0x014*/     ULONG32      MinInstructions;                                                  
/*0x018*/     ULONG32      MaxInstructions;                                                  
/*0x01C*/     ULONG32      TotalInstructions;                                                
          }DBGKD_GET_INTERNAL_BREAKPOINT64, *PDBGKD_GET_INTERNAL_BREAKPOINT64;                                   
                                                                                             
          typedef struct _DBGKD_GET_SET_BUS_DATA // 5 elements, 0x14 bytes (sizeof) 
          {                                                                         
/*0x000*/     ULONG32      BusDataType;                                             
/*0x004*/     ULONG32      BusNumber;                                               
/*0x008*/     ULONG32      SlotNumber;                                              
/*0x00C*/     ULONG32      Offset;                                                  
/*0x010*/     ULONG32      Length;                                                  
          }DBGKD_GET_SET_BUS_DATA, *PDBGKD_GET_SET_BUS_DATA;                                   
                                                                                    
          typedef struct _DBGKD_GET_VERSION32        // 14 elements, 0x28 bytes (sizeof) 
          {                                                                              
/*0x000*/     UINT16       MajorVersion;                                                 
/*0x002*/     UINT16       MinorVersion;                                                 
/*0x004*/     UINT16       ProtocolVersion;                                              
/*0x006*/     UINT16       Flags;                                                        
/*0x008*/     ULONG32      KernBase;                                                     
/*0x00C*/     ULONG32      PsLoadedModuleList;                                           
/*0x010*/     UINT16       MachineType;                                                  
/*0x012*/     UINT16       ThCallbackStack;                                              
/*0x014*/     UINT16       NextCallback;                                                 
/*0x016*/     UINT16       FramePointer;                                                 
/*0x018*/     ULONG32      KiCallUserMode;                                               
/*0x01C*/     ULONG32      KeUserCallbackDispatcher;                                     
/*0x020*/     ULONG32      BreakpointWithStatus;                                         
/*0x024*/     ULONG32      DebuggerDataList;                                             
          }DBGKD_GET_VERSION32, *PDBGKD_GET_VERSION32;                                    
                                                                                         
          typedef struct _DBGKD_GET_VERSION64  // 13 elements, 0x28 bytes (sizeof) 
          {                                                                        
/*0x000*/     UINT16       MajorVersion;                                           
/*0x002*/     UINT16       MinorVersion;                                           
/*0x004*/     UINT16       ProtocolVersion;                                        
/*0x006*/     UINT16       Flags;                                                  
/*0x008*/     UINT16       MachineType;                                            
/*0x00A*/     UINT8        MaxPacketType;                                          
/*0x00B*/     UINT8        MaxStateChange;                                         
/*0x00C*/     UINT8        MaxManipulate;                                          
/*0x00D*/     UINT8        Simulation;                                             
/*0x00E*/     UINT16       Unused[1];                                              
/*0x010*/     UINT64       KernBase;                                               
/*0x018*/     UINT64       PsLoadedModuleList;                                     
/*0x020*/     UINT64       DebuggerDataList;                                       
          }DBGKD_GET_VERSION64, *PDBGKD_GET_VERSION64;                                    
                                                                                   
          typedef struct _DBGKD_LOAD_SYMBOLS32 // 6 elements, 0x18 bytes (sizeof) 
          {                                                                       
/*0x000*/     ULONG32      PathNameLength;                                        
/*0x004*/     ULONG32      BaseOfDll;                                             
/*0x008*/     ULONG32      ProcessId;                                             
/*0x00C*/     ULONG32      CheckSum;                                              
/*0x010*/     ULONG32      SizeOfImage;                                           
/*0x014*/     UINT8        UnloadSymbols;                                         
/*0x015*/     UINT8        _PADDING0_[0x3];                                       
          }DBGKD_LOAD_SYMBOLS32, *PDBGKD_LOAD_SYMBOLS32;                                   
                                                                                  
          typedef struct _DBGKD_LOAD_SYMBOLS64 // 6 elements, 0x28 bytes (sizeof) 
          {                                                                       
/*0x000*/     ULONG32      PathNameLength;                                        
/*0x004*/     UINT8        _PADDING0_[0x4];                                       
/*0x008*/     UINT64       BaseOfDll;                                             
/*0x010*/     UINT64       ProcessId;                                             
/*0x018*/     ULONG32      CheckSum;                                              
/*0x01C*/     ULONG32      SizeOfImage;                                           
/*0x020*/     UINT8        UnloadSymbols;                                         
/*0x021*/     UINT8        _PADDING1_[0x7];                                       
          }DBGKD_LOAD_SYMBOLS64, *PDBGKD_LOAD_SYMBOLS64;                                   
                                                                                  
          typedef struct _DBGKD_READ_MEMORY32 // 3 elements, 0xC bytes (sizeof) 
          {                                                                     
/*0x000*/     ULONG32      TargetBaseAddress;                                   
/*0x004*/     ULONG32      TransferCount;                                       
/*0x008*/     ULONG32      ActualBytesRead;                                     
          }DBGKD_READ_MEMORY32, *PDBGKD_READ_MEMORY32;                                  
                                                                                
          typedef struct _DBGKD_WRITE_MEMORY32 // 3 elements, 0xC bytes (sizeof) 
          {                                                                      
/*0x000*/     ULONG32      TargetBaseAddress;                                    
/*0x004*/     ULONG32      TransferCount;                                        
/*0x008*/     ULONG32      ActualBytesWritten;                                   
          }DBGKD_WRITE_MEMORY32, *PDBGKD_WRITE_MEMORY32;                                  
                                                                                 
          typedef struct _DBGKD_READ_MEMORY64 // 3 elements, 0x10 bytes (sizeof) 
          {                                                                      
/*0x000*/     UINT64       TargetBaseAddress;                                    
/*0x008*/     ULONG32      TransferCount;                                        
/*0x00C*/     ULONG32      ActualBytesRead;                                      
          }DBGKD_READ_MEMORY64, *PDBGKD_READ_MEMORY64;                                   
                                                                                 
          typedef struct _DBGKD_WRITE_MEMORY64 // 3 elements, 0x10 bytes (sizeof) 
          {                                                                       
/*0x000*/     UINT64       TargetBaseAddress;                                     
/*0x008*/     ULONG32      TransferCount;                                         
/*0x00C*/     ULONG32      ActualBytesWritten;                                    
          }DBGKD_WRITE_MEMORY64, *PDBGKD_WRITE_MEMORY64;                                   
                                                                                  
          typedef struct _DBGKD_SET_CONTEXT // 1 elements, 0x4 bytes (sizeof) 
          {                                                                   
/*0x000*/     ULONG32      ContextFlags;                                      
          }DBGKD_SET_CONTEXT, *PDBGKD_SET_CONTEXT;                                  
                                                                              
          typedef struct _DBGKD_WRITE_BREAKPOINT32 // 2 elements, 0x8 bytes (sizeof) 
          {                                                                          
/*0x000*/     ULONG32      BreakPointAddress;                                        
/*0x004*/     ULONG32      BreakPointHandle;                                         
          }DBGKD_WRITE_BREAKPOINT32, *PDBGKD_WRITE_BREAKPOINT32;                                  
                                                                                     
          typedef struct _DBGKD_RESTORE_BREAKPOINT // 1 elements, 0x4 bytes (sizeof) 
          {                                                                          
/*0x000*/     ULONG32      BreakPointHandle;                                         
          }DBGKD_RESTORE_BREAKPOINT, *PDBGKD_RESTORE_BREAKPOINT;                                  
                                                                                     
          typedef struct _DBGKD_READ_WRITE_IO32 // 3 elements, 0xC bytes (sizeof) 
          {                                                                       
/*0x000*/     ULONG32      DataSize;                                              
/*0x004*/     ULONG32      IoAddress;                                             
/*0x008*/     ULONG32      DataValue;                                             
          }DBGKD_READ_WRITE_IO32, *PDBGKD_READ_WRITE_IO32;                                  
                                                                                  
          typedef struct _DBGKD_READ_WRITE_IO_EXTENDED32 // 6 elements, 0x18 bytes (sizeof) 
          {                                                                                 
/*0x000*/     ULONG32      DataSize;                                                        
/*0x004*/     ULONG32      InterfaceType;                                                   
/*0x008*/     ULONG32      BusNumber;                                                       
/*0x00C*/     ULONG32      AddressSpace;                                                    
/*0x010*/     ULONG32      IoAddress;                                                       
/*0x014*/     ULONG32      DataValue;                                                       
          }DBGKD_READ_WRITE_IO_EXTENDED32, *PDBGKD_READ_WRITE_IO_EXTENDED32;                                   
                                                                                            
          typedef struct _DBGKD_QUERY_SPECIAL_CALLS // 1 elements, 0x4 bytes (sizeof) 
          {                                                                           
/*0x000*/     ULONG32      NumberOfSpecialCalls;                                      
          }DBGKD_QUERY_SPECIAL_CALLS, *PDBGKD_QUERY_SPECIAL_CALLS;                                  
                                                                                      
          typedef struct _DBGKD_SET_SPECIAL_CALL32 // 1 elements, 0x4 bytes (sizeof) 
          {                                                                          
/*0x000*/     ULONG32      SpecialCall;                                              
          }DBGKD_SET_SPECIAL_CALL32, *PDBGKD_SET_SPECIAL_CALL32;                                  
                                                                                     
          typedef struct _DBGKD_SET_INTERNAL_BREAKPOINT32 // 2 elements, 0x8 bytes (sizeof) 
          {                                                                                 
/*0x000*/     ULONG32      BreakpointAddress;                                               
/*0x004*/     ULONG32      Flags;                                                           
          }DBGKD_SET_INTERNAL_BREAKPOINT32, *PDBGKD_SET_INTERNAL_BREAKPOINT32;                                  
                                                                                            
          typedef struct _DBGKD_READ_WRITE_MSR // 3 elements, 0xC bytes (sizeof) 
          {                                                                      
/*0x000*/     ULONG32      Msr;                                                  
/*0x004*/     ULONG32      DataValueLow;                                         
/*0x008*/     ULONG32      DataValueHigh;                                        
          }DBGKD_READ_WRITE_MSR, *PDBGKD_READ_WRITE_MSR;                                  
                                                                                 
          typedef struct _DBGKD_SEARCH_MEMORY // 4 elements, 0x18 bytes (sizeof) 
          {                                                                      
              union                           // 2 elements, 0x8 bytes (sizeof)  
              {                                                                  
/*0x000*/         UINT64       SearchAddress;                                    
/*0x000*/         UINT64       FoundAddress;                                     
              };                                                                 
/*0x008*/     UINT64       SearchLength;                                         
/*0x010*/     ULONG32      PatternLength;                                        
/*0x014*/     UINT8        _PADDING0_[0x4];                                      
          }DBGKD_SEARCH_MEMORY, *PDBGKD_SEARCH_MEMORY;                                   
                                                                                 
          typedef struct _DBGKD_MANIPULATE_STATE32                               // 5 elements, 0x34 bytes (sizeof)  
          {                                                                                                          
/*0x000*/     ULONG32      ApiNumber;                                                                                
/*0x004*/     UINT16       ProcessorLevel;                                                                           
/*0x006*/     UINT16       Processor;                                                                                
/*0x008*/     LONG32       ReturnStatus;                                                                             
              union                                                              // 20 elements, 0x28 bytes (sizeof) 
              {                                                                                                      
/*0x00C*/         struct _DBGKD_READ_MEMORY32 ReadMemory;                        // 3 elements, 0xC bytes (sizeof)   
/*0x00C*/         struct _DBGKD_WRITE_MEMORY32 WriteMemory;                      // 3 elements, 0xC bytes (sizeof)   
/*0x00C*/         struct _DBGKD_READ_MEMORY64 ReadMemory64;                      // 3 elements, 0x10 bytes (sizeof)  
/*0x00C*/         struct _DBGKD_WRITE_MEMORY64 WriteMemory64;                    // 3 elements, 0x10 bytes (sizeof)  
/*0x00C*/         struct _DBGKD_GET_CONTEXT GetContext;                          // 1 elements, 0x4 bytes (sizeof)   
/*0x00C*/         struct _DBGKD_SET_CONTEXT SetContext;                          // 1 elements, 0x4 bytes (sizeof)   
/*0x00C*/         struct _DBGKD_WRITE_BREAKPOINT32 WriteBreakPoint;              // 2 elements, 0x8 bytes (sizeof)   
/*0x00C*/         struct _DBGKD_RESTORE_BREAKPOINT RestoreBreakPoint;            // 1 elements, 0x4 bytes (sizeof)   
/*0x00C*/         struct _DBGKD_CONTINUE Continue;                               // 1 elements, 0x4 bytes (sizeof)   
/*0x00C*/         struct _DBGKD_CONTINUE2 Continue2;                             // 3 elements, 0x20 bytes (sizeof)  
/*0x00C*/         struct _DBGKD_READ_WRITE_IO32 ReadWriteIo;                     // 3 elements, 0xC bytes (sizeof)   
/*0x00C*/         struct _DBGKD_READ_WRITE_IO_EXTENDED32 ReadWriteIoExtended;    // 6 elements, 0x18 bytes (sizeof)  
/*0x00C*/         struct _DBGKD_QUERY_SPECIAL_CALLS QuerySpecialCalls;           // 1 elements, 0x4 bytes (sizeof)   
/*0x00C*/         struct _DBGKD_SET_SPECIAL_CALL32 SetSpecialCall;               // 1 elements, 0x4 bytes (sizeof)   
/*0x00C*/         struct _DBGKD_SET_INTERNAL_BREAKPOINT32 SetInternalBreakpoint; // 2 elements, 0x8 bytes (sizeof)   
/*0x00C*/         struct _DBGKD_GET_INTERNAL_BREAKPOINT32 GetInternalBreakpoint; // 7 elements, 0x1C bytes (sizeof)  
/*0x00C*/         struct _DBGKD_GET_VERSION32 GetVersion32;                      // 14 elements, 0x28 bytes (sizeof) 
/*0x00C*/         struct _DBGKD_BREAKPOINTEX BreakPointEx;                       // 2 elements, 0x8 bytes (sizeof)   
/*0x00C*/         struct _DBGKD_READ_WRITE_MSR ReadWriteMsr;                     // 3 elements, 0xC bytes (sizeof)   
/*0x00C*/         struct _DBGKD_SEARCH_MEMORY SearchMemory;                      // 4 elements, 0x18 bytes (sizeof)  
              }u;                                                                                                    
          }DBGKD_MANIPULATE_STATE32, *PDBGKD_MANIPULATE_STATE32;                                                     
                                                                                                                     
          typedef struct _DBGKD_WRITE_BREAKPOINT64 // 2 elements, 0x10 bytes (sizeof) 
          {                                                                           
/*0x000*/     UINT64       BreakPointAddress;                                         
/*0x008*/     ULONG32      BreakPointHandle;                                          
/*0x00C*/     UINT8        _PADDING0_[0x4];                                           
          }DBGKD_WRITE_BREAKPOINT64, *PDBGKD_WRITE_BREAKPOINT64;                                   
                                                                                      
          typedef struct _DBGKD_READ_WRITE_IO64 // 3 elements, 0x10 bytes (sizeof) 
          {                                                                        
/*0x000*/     UINT64       IoAddress;                                              
/*0x008*/     ULONG32      DataSize;                                               
/*0x00C*/     ULONG32      DataValue;                                              
          }DBGKD_READ_WRITE_IO64, *PDBGKD_READ_WRITE_IO64;                                   
                                                                                   
          typedef struct _DBGKD_READ_WRITE_IO_EXTENDED64 // 6 elements, 0x20 bytes (sizeof) 
          {                                                                                 
/*0x000*/     ULONG32      DataSize;                                                        
/*0x004*/     ULONG32      InterfaceType;                                                   
/*0x008*/     ULONG32      BusNumber;                                                       
/*0x00C*/     ULONG32      AddressSpace;                                                    
/*0x010*/     UINT64       IoAddress;                                                       
/*0x018*/     ULONG32      DataValue;                                                       
/*0x01C*/     UINT8        _PADDING0_[0x4];                                                 
          }DBGKD_READ_WRITE_IO_EXTENDED64, *PDBGKD_READ_WRITE_IO_EXTENDED64;                                   
                                                                                            
          typedef struct _DBGKD_SET_SPECIAL_CALL64 // 1 elements, 0x8 bytes (sizeof) 
          {                                                                          
/*0x000*/     UINT64       SpecialCall;                                              
          }DBGKD_SET_SPECIAL_CALL64, *PDBGKD_SET_SPECIAL_CALL64;                                  
                                                                                     
          typedef struct _DBGKD_SET_INTERNAL_BREAKPOINT64 // 2 elements, 0x10 bytes (sizeof) 
          {                                                                                  
/*0x000*/     UINT64       BreakpointAddress;                                                
/*0x008*/     ULONG32      Flags;                                                            
/*0x00C*/     UINT8        _PADDING0_[0x4];                                                  
          }DBGKD_SET_INTERNAL_BREAKPOINT64, *PDBGKD_SET_INTERNAL_BREAKPOINT64;                                   
                                                                                             
          typedef struct _DBGKD_QUERY_MEMORY // 4 elements, 0x18 bytes (sizeof) 
          {                                                                     
/*0x000*/     UINT64       Address;                                             
/*0x008*/     UINT64       Reserved;                                            
/*0x010*/     ULONG32      AddressSpace;                                        
/*0x014*/     ULONG32      Flags;                                               
          }DBGKD_QUERY_MEMORY, *PDBGKD_QUERY_MEMORY;                                   
                                                                                
          typedef struct _DBGKD_MANIPULATE_STATE64                               // 5 elements, 0x38 bytes (sizeof)  
          {                                                                                                          
/*0x000*/     ULONG32      ApiNumber;                                                                                
/*0x004*/     UINT16       ProcessorLevel;                                                                           
/*0x006*/     UINT16       Processor;                                                                                
/*0x008*/     LONG32       ReturnStatus;                                                                             
/*0x00C*/     UINT8        _PADDING0_[0x4];                                                                          
              union                                                              // 21 elements, 0x28 bytes (sizeof) 
              {                                                                                                      
/*0x010*/         struct _DBGKD_READ_MEMORY64 ReadMemory;                        // 3 elements, 0x10 bytes (sizeof)  
/*0x010*/         struct _DBGKD_WRITE_MEMORY64 WriteMemory;                      // 3 elements, 0x10 bytes (sizeof)  
/*0x010*/         struct _DBGKD_GET_CONTEXT GetContext;                          // 1 elements, 0x4 bytes (sizeof)   
/*0x010*/         struct _DBGKD_SET_CONTEXT SetContext;                          // 1 elements, 0x4 bytes (sizeof)   
/*0x010*/         struct _DBGKD_WRITE_BREAKPOINT64 WriteBreakPoint;              // 2 elements, 0x10 bytes (sizeof)  
/*0x010*/         struct _DBGKD_RESTORE_BREAKPOINT RestoreBreakPoint;            // 1 elements, 0x4 bytes (sizeof)   
/*0x010*/         struct _DBGKD_CONTINUE Continue;                               // 1 elements, 0x4 bytes (sizeof)   
/*0x010*/         struct _DBGKD_CONTINUE2 Continue2;                             // 3 elements, 0x20 bytes (sizeof)  
/*0x010*/         struct _DBGKD_READ_WRITE_IO64 ReadWriteIo;                     // 3 elements, 0x10 bytes (sizeof)  
/*0x010*/         struct _DBGKD_READ_WRITE_IO_EXTENDED64 ReadWriteIoExtended;    // 6 elements, 0x20 bytes (sizeof)  
/*0x010*/         struct _DBGKD_QUERY_SPECIAL_CALLS QuerySpecialCalls;           // 1 elements, 0x4 bytes (sizeof)   
/*0x010*/         struct _DBGKD_SET_SPECIAL_CALL64 SetSpecialCall;               // 1 elements, 0x8 bytes (sizeof)   
/*0x010*/         struct _DBGKD_SET_INTERNAL_BREAKPOINT64 SetInternalBreakpoint; // 2 elements, 0x10 bytes (sizeof)  
/*0x010*/         struct _DBGKD_GET_INTERNAL_BREAKPOINT64 GetInternalBreakpoint; // 7 elements, 0x20 bytes (sizeof)  
/*0x010*/         struct _DBGKD_GET_VERSION64 GetVersion64;                      // 13 elements, 0x28 bytes (sizeof) 
/*0x010*/         struct _DBGKD_BREAKPOINTEX BreakPointEx;                       // 2 elements, 0x8 bytes (sizeof)   
/*0x010*/         struct _DBGKD_READ_WRITE_MSR ReadWriteMsr;                     // 3 elements, 0xC bytes (sizeof)   
/*0x010*/         struct _DBGKD_SEARCH_MEMORY SearchMemory;                      // 4 elements, 0x18 bytes (sizeof)  
/*0x010*/         struct _DBGKD_GET_SET_BUS_DATA GetSetBusData;                  // 5 elements, 0x14 bytes (sizeof)  
/*0x010*/         struct _DBGKD_FILL_MEMORY FillMemory;                          // 4 elements, 0x10 bytes (sizeof)  
/*0x010*/         struct _DBGKD_QUERY_MEMORY QueryMemory;                        // 4 elements, 0x18 bytes (sizeof)  
              }u;                                                                                                    
          }DBGKD_MANIPULATE_STATE64, *PDBGKD_MANIPULATE_STATE64;                                                     
                                                                                                                     
          typedef struct _EXCEPTION_RECORD32         // 6 elements, 0x50 bytes (sizeof) 
          {                                                                             
/*0x000*/     LONG32       ExceptionCode;                                               
/*0x004*/     ULONG32      ExceptionFlags;                                              
/*0x008*/     ULONG32      ExceptionRecord;                                             
/*0x00C*/     ULONG32      ExceptionAddress;                                            
/*0x010*/     ULONG32      NumberParameters;                                            
/*0x014*/     ULONG32      ExceptionInformation[15];                                    
          }EXCEPTION_RECORD32, *PEXCEPTION_RECORD32;                                    
                                                                                        
          typedef struct _DBGKM_EXCEPTION32               // 2 elements, 0x54 bytes (sizeof) 
          {                                                                                  
/*0x000*/     struct _EXCEPTION_RECORD32 ExceptionRecord; // 6 elements, 0x50 bytes (sizeof) 
/*0x050*/     ULONG32      FirstChance;                                                      
          }DBGKM_EXCEPTION32, *PDBGKM_EXCEPTION32;                                           
                                                                                             
          typedef struct _EXCEPTION_RECORD64         // 7 elements, 0x98 bytes (sizeof) 
          {                                                                             
/*0x000*/     LONG32       ExceptionCode;                                               
/*0x004*/     ULONG32      ExceptionFlags;                                              
/*0x008*/     UINT64       ExceptionRecord;                                             
/*0x010*/     UINT64       ExceptionAddress;                                            
/*0x018*/     ULONG32      NumberParameters;                                            
/*0x01C*/     ULONG32      __unusedAlignment;                                           
/*0x020*/     UINT64       ExceptionInformation[15];                                    
          }EXCEPTION_RECORD64, *PEXCEPTION_RECORD64;                                    
                                                                                        
          typedef struct _DBGKM_EXCEPTION64               // 2 elements, 0xA0 bytes (sizeof) 
          {                                                                                  
/*0x000*/     struct _EXCEPTION_RECORD64 ExceptionRecord; // 7 elements, 0x98 bytes (sizeof) 
/*0x098*/     ULONG32      FirstChance;                                                      
/*0x09C*/     UINT8        _PADDING0_[0x4];                                                  
          }DBGKM_EXCEPTION64, *PDBGKM_EXCEPTION64;                                           
                                                                                             
          typedef struct _DEFERRED_WRITE                // 10 elements, 0x28 bytes (sizeof) 
          {                                                                                 
/*0x000*/     INT16        NodeTypeCode;                                                    
/*0x002*/     INT16        NodeByteSize;                                                    
/*0x004*/     struct _FILE_OBJECT* FileObject;                                              
/*0x008*/     ULONG32      BytesToWrite;                                                    
/*0x00C*/     struct _LIST_ENTRY DeferredWriteLinks;    // 2 elements, 0x8 bytes (sizeof)   
/*0x014*/     struct _KEVENT* Event;                                                        
/*0x018*/     FUNCT_00BC_0EDA_PostRoutine* PostRoutine;                                     
/*0x01C*/     VOID*        Context1;                                                        
/*0x020*/     VOID*        Context2;                                                        
/*0x024*/     UINT8        LimitModifiedPages;                                              
/*0x025*/     UINT8        _PADDING0_[0x3];                                                 
          }DEFERRED_WRITE, *PDEFERRED_WRITE;                                                
                                                                                            
          typedef struct _DESCRIPTOR // 3 elements, 0x8 bytes (sizeof) 
          {                                                            
/*0x000*/     UINT16       Pad;                                        
/*0x002*/     UINT16       Limit;                                      
/*0x004*/     ULONG32      Base;                                       
          }DESCRIPTOR, *PDESCRIPTOR;                                   
                                                                       
          typedef struct _DEVICE_CAPABILITIES          // 29 elements, 0x40 bytes (sizeof) 
          {                                                                                
/*0x000*/     UINT16       Size;                                                           
/*0x002*/     UINT16       Version;                                                        
              struct                                   // 19 elements, 0x4 bytes (sizeof)  
              {                                                                            
/*0x004*/         ULONG32      DeviceD1 : 1;           // 0 BitPosition                    
/*0x004*/         ULONG32      DeviceD2 : 1;           // 1 BitPosition                    
/*0x004*/         ULONG32      LockSupported : 1;      // 2 BitPosition                    
/*0x004*/         ULONG32      EjectSupported : 1;     // 3 BitPosition                    
/*0x004*/         ULONG32      Removable : 1;          // 4 BitPosition                    
/*0x004*/         ULONG32      DockDevice : 1;         // 5 BitPosition                    
/*0x004*/         ULONG32      UniqueID : 1;           // 6 BitPosition                    
/*0x004*/         ULONG32      SilentInstall : 1;      // 7 BitPosition                    
/*0x004*/         ULONG32      RawDeviceOK : 1;        // 8 BitPosition                    
/*0x004*/         ULONG32      SurpriseRemovalOK : 1;  // 9 BitPosition                    
/*0x004*/         ULONG32      WakeFromD0 : 1;         // 10 BitPosition                   
/*0x004*/         ULONG32      WakeFromD1 : 1;         // 11 BitPosition                   
/*0x004*/         ULONG32      WakeFromD2 : 1;         // 12 BitPosition                   
/*0x004*/         ULONG32      WakeFromD3 : 1;         // 13 BitPosition                   
/*0x004*/         ULONG32      HardwareDisabled : 1;   // 14 BitPosition                   
/*0x004*/         ULONG32      NonDynamic : 1;         // 15 BitPosition                   
/*0x004*/         ULONG32      WarmEjectSupported : 1; // 16 BitPosition                   
/*0x004*/         ULONG32      NoDisplayInUI : 1;      // 17 BitPosition                   
/*0x004*/         ULONG32      Reserved : 14;          // 18 BitPosition                   
              };                                                                           
/*0x008*/     ULONG32      Address;                                                        
/*0x00C*/     ULONG32      UINumber;                                                       
/*0x010*/     enum _DEVICE_POWER_STATE DeviceState[7];                                     
/*0x02C*/     enum _SYSTEM_POWER_STATE SystemWake;                                         
/*0x030*/     enum _DEVICE_POWER_STATE DeviceWake;                                         
/*0x034*/     ULONG32      D1Latency;                                                      
/*0x038*/     ULONG32      D2Latency;                                                      
/*0x03C*/     ULONG32      D3Latency;                                                      
          }DEVICE_CAPABILITIES, *PDEVICE_CAPABILITIES;                                     
                                                                                           
          typedef struct _DEVICE_MAP                               // 5 elements, 0x30 bytes (sizeof) 
          {                                                                                           
/*0x000*/     struct _OBJECT_DIRECTORY* DosDevicesDirectory;                                          
/*0x004*/     struct _OBJECT_DIRECTORY* GlobalDosDevicesDirectory;                                    
/*0x008*/     ULONG32      ReferenceCount;                                                            
/*0x00C*/     ULONG32      DriveMap;                                                                  
/*0x010*/     UINT8        DriveType[32];                                                             
          }DEVICE_MAP, *PDEVICE_MAP;                                                                  
                                                                                                      
          typedef struct _DEVICE_NODE                                      // 45 elements, 0x118 bytes (sizeof) 
          {                                                                                                     
/*0x000*/     struct _DEVICE_NODE* Sibling;                                                                     
/*0x004*/     struct _DEVICE_NODE* Child;                                                                       
/*0x008*/     struct _DEVICE_NODE* Parent;                                                                      
/*0x00C*/     struct _DEVICE_NODE* LastChild;                                                                   
/*0x010*/     ULONG32      Level;                                                                               
/*0x014*/     struct _PO_DEVICE_NOTIFY* Notify;                                                                 
/*0x018*/     enum _PNP_DEVNODE_STATE State;                                                                    
/*0x01C*/     enum _PNP_DEVNODE_STATE PreviousState;                                                            
/*0x020*/     enum _PNP_DEVNODE_STATE StateHistory[20];                                                         
/*0x070*/     ULONG32      StateHistoryEntry;                                                                   
/*0x074*/     LONG32       CompletionStatus;                                                                    
/*0x078*/     struct _IRP* PendingIrp;                                                                          
/*0x07C*/     ULONG32      Flags;                                                                               
/*0x080*/     ULONG32      UserFlags;                                                                           
/*0x084*/     ULONG32      Problem;                                                                             
/*0x088*/     struct _DEVICE_OBJECT* PhysicalDeviceObject;                                                      
/*0x08C*/     struct _CM_RESOURCE_LIST* ResourceList;                                                           
/*0x090*/     struct _CM_RESOURCE_LIST* ResourceListTranslated;                                                 
/*0x094*/     struct _UNICODE_STRING InstancePath;                         // 3 elements, 0x8 bytes (sizeof)    
/*0x09C*/     struct _UNICODE_STRING ServiceName;                          // 3 elements, 0x8 bytes (sizeof)    
/*0x0A4*/     struct _DEVICE_OBJECT* DuplicatePDO;                                                              
/*0x0A8*/     struct _IO_RESOURCE_REQUIREMENTS_LIST* ResourceRequirements;                                      
/*0x0AC*/     enum _INTERFACE_TYPE InterfaceType;                                                               
/*0x0B0*/     ULONG32      BusNumber;                                                                           
/*0x0B4*/     enum _INTERFACE_TYPE ChildInterfaceType;                                                          
/*0x0B8*/     ULONG32      ChildBusNumber;                                                                      
/*0x0BC*/     UINT16       ChildBusTypeIndex;                                                                   
/*0x0BE*/     UINT8        RemovalPolicy;                                                                       
/*0x0BF*/     UINT8        HardwareRemovalPolicy;                                                               
/*0x0C0*/     struct _LIST_ENTRY TargetDeviceNotify;                       // 2 elements, 0x8 bytes (sizeof)    
/*0x0C8*/     struct _LIST_ENTRY DeviceArbiterList;                        // 2 elements, 0x8 bytes (sizeof)    
/*0x0D0*/     struct _LIST_ENTRY DeviceTranslatorList;                     // 2 elements, 0x8 bytes (sizeof)    
/*0x0D8*/     UINT16       NoTranslatorMask;                                                                    
/*0x0DA*/     UINT16       QueryTranslatorMask;                                                                 
/*0x0DC*/     UINT16       NoArbiterMask;                                                                       
/*0x0DE*/     UINT16       QueryArbiterMask;                                                                    
              union                                                        // 2 elements, 0x4 bytes (sizeof)    
              {                                                                                                 
/*0x0E0*/         struct _DEVICE_NODE* LegacyDeviceNode;                                                        
/*0x0E0*/         struct _DEVICE_RELATIONS* PendingDeviceRelations;                                             
              }OverUsed1;                                                                                       
              union                                                        // 1 elements, 0x4 bytes (sizeof)    
              {                                                                                                 
/*0x0E4*/         struct _DEVICE_NODE* NextResourceDeviceNode;                                                  
              }OverUsed2;                                                                                       
/*0x0E8*/     struct _CM_RESOURCE_LIST* BootResources;                                                          
/*0x0EC*/     ULONG32      CapabilityFlags;                                                                     
              struct                                                       // 3 elements, 0x10 bytes (sizeof)   
              {                                                                                                 
/*0x0F0*/         enum _PROFILE_STATUS DockStatus;                                                              
/*0x0F4*/         struct _LIST_ENTRY ListEntry;                            // 2 elements, 0x8 bytes (sizeof)    
/*0x0FC*/         UINT16*      SerialNumber;                                                                    
              }DockInfo;                                                                                        
/*0x100*/     ULONG32      DisableableDepends;                                                                  
/*0x104*/     struct _LIST_ENTRY PendedSetInterfaceState;                  // 2 elements, 0x8 bytes (sizeof)    
/*0x10C*/     struct _LIST_ENTRY LegacyBusListEntry;                       // 2 elements, 0x8 bytes (sizeof)    
/*0x114*/     ULONG32      DriverUnloadRetryCount;                                                              
          }DEVICE_NODE, *PDEVICE_NODE;                                                                          
                                                                                                                
          typedef struct _KDEVICE_QUEUE_ENTRY     // 3 elements, 0x10 bytes (sizeof) 
          {                                                                          
/*0x000*/     struct _LIST_ENTRY DeviceListEntry; // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/     ULONG32      SortKey;                                                  
/*0x00C*/     UINT8        Inserted;                                                 
/*0x00D*/     UINT8        _PADDING0_[0x3];                                          
          }KDEVICE_QUEUE_ENTRY, *PKDEVICE_QUEUE_ENTRY;                                   
                                                                                     
          typedef struct _WAIT_CONTEXT_BLOCK                // 7 elements, 0x28 bytes (sizeof) 
          {                                                                                    
/*0x000*/     struct _KDEVICE_QUEUE_ENTRY WaitQueueEntry;   // 3 elements, 0x10 bytes (sizeof) 
/*0x010*/     FUNCT_0978_0977_DeviceRoutine* DeviceRoutine;                                    
/*0x014*/     VOID*        DeviceContext;                                                      
/*0x018*/     ULONG32      NumberOfMapRegisters;                                               
/*0x01C*/     VOID*        DeviceObject;                                                       
/*0x020*/     VOID*        CurrentIrp;                                                         
/*0x024*/     struct _KDPC* BufferChainingDpc;                                                 
          }WAIT_CONTEXT_BLOCK, *PWAIT_CONTEXT_BLOCK;                                           
                                                                                               
          typedef struct _KDEVICE_QUEUE          // 5 elements, 0x14 bytes (sizeof) 
          {                                                                         
/*0x000*/     INT16        Type;                                                    
/*0x002*/     INT16        Size;                                                    
/*0x004*/     struct _LIST_ENTRY DeviceListHead; // 2 elements, 0x8 bytes (sizeof)  
/*0x00C*/     ULONG32      Lock;                                                    
/*0x010*/     UINT8        Busy;                                                    
/*0x011*/     UINT8        _PADDING0_[0x3];                                         
          }KDEVICE_QUEUE, *PKDEVICE_QUEUE;                                          
                                                                                    
          typedef struct _KDPC                                  // 9 elements, 0x20 bytes (sizeof) 
          {                                                                                        
/*0x000*/     INT16        Type;                                                                   
/*0x002*/     UINT8        Number;                                                                 
/*0x003*/     UINT8        Importance;                                                             
/*0x004*/     struct _LIST_ENTRY DpcListEntry;                  // 2 elements, 0x8 bytes (sizeof)  
/*0x00C*/     FUNCT_00BC_0130_DeferredRoutine* DeferredRoutine;                                    
/*0x010*/     VOID*        DeferredContext;                                                        
/*0x014*/     VOID*        SystemArgument1;                                                        
/*0x018*/     VOID*        SystemArgument2;                                                        
/*0x01C*/     ULONG32*     Lock;                                                                   
          }KDPC, *PKDPC;                                                                           
                                                                                                   
          typedef struct _DEVICE_OBJECT                        // 25 elements, 0xB8 bytes (sizeof) 
          {                                                                                        
/*0x000*/     INT16        Type;                                                                   
/*0x002*/     UINT16       Size;                                                                   
/*0x004*/     LONG32       ReferenceCount;                                                         
/*0x008*/     struct _DRIVER_OBJECT* DriverObject;                                                 
/*0x00C*/     struct _DEVICE_OBJECT* NextDevice;                                                   
/*0x010*/     struct _DEVICE_OBJECT* AttachedDevice;                                               
/*0x014*/     struct _IRP* CurrentIrp;                                                             
/*0x018*/     struct _IO_TIMER* Timer;                                                             
/*0x01C*/     ULONG32      Flags;                                                                  
/*0x020*/     ULONG32      Characteristics;                                                        
/*0x024*/     struct _VPB* Vpb;                                                                    
/*0x028*/     VOID*        DeviceExtension;                                                        
/*0x02C*/     ULONG32      DeviceType;                                                             
/*0x030*/     CHAR         StackSize;                                                              
/*0x031*/     UINT8        _PADDING0_[0x3];                                                        
              union                                            // 2 elements, 0x28 bytes (sizeof)  
              {                                                                                    
/*0x034*/         struct _LIST_ENTRY ListEntry;                // 2 elements, 0x8 bytes (sizeof)   
/*0x034*/         struct _WAIT_CONTEXT_BLOCK Wcb;              // 7 elements, 0x28 bytes (sizeof)  
              }Queue;                                                                              
/*0x05C*/     ULONG32      AlignmentRequirement;                                                   
/*0x060*/     struct _KDEVICE_QUEUE DeviceQueue;               // 5 elements, 0x14 bytes (sizeof)  
/*0x074*/     struct _KDPC Dpc;                                // 9 elements, 0x20 bytes (sizeof)  
/*0x094*/     ULONG32      ActiveThreadCount;                                                      
/*0x098*/     VOID*        SecurityDescriptor;                                                     
/*0x09C*/     struct _KEVENT DeviceLock;                       // 1 elements, 0x10 bytes (sizeof)  
/*0x0AC*/     UINT16       SectorSize;                                                             
/*0x0AE*/     UINT16       Spare1;                                                                 
/*0x0B0*/     struct _DEVOBJ_EXTENSION* DeviceObjectExtension;                                     
/*0x0B4*/     VOID*        Reserved;                                                               
          }DEVICE_OBJECT, *PDEVICE_OBJECT;                                                         
                                                                                                   
          typedef struct _POWER_CHANNEL_SUMMARY // 4 elements, 0x14 bytes (sizeof) 
          {                                                                        
/*0x000*/     ULONG32      Signature;                                              
/*0x004*/     ULONG32      TotalCount;                                             
/*0x008*/     ULONG32      D0Count;                                                
/*0x00C*/     struct _LIST_ENTRY NotifyList;    // 2 elements, 0x8 bytes (sizeof)  
          }POWER_CHANNEL_SUMMARY, *PPOWER_CHANNEL_SUMMARY;                                   
                                                                                   
          typedef struct _DEVICE_OBJECT_POWER_EXTENSION          // 11 elements, 0x4C bytes (sizeof) 
          {                                                                                          
/*0x000*/     ULONG32      IdleCount;                                                                
/*0x004*/     ULONG32      ConservationIdleTime;                                                     
/*0x008*/     ULONG32      PerformanceIdleTime;                                                      
/*0x00C*/     struct _DEVICE_OBJECT* DeviceObject;                                                   
/*0x010*/     struct _LIST_ENTRY IdleList;                       // 2 elements, 0x8 bytes (sizeof)   
/*0x018*/     UINT8        DeviceType;                                                               
/*0x019*/     UINT8        _PADDING0_[0x3];                                                          
/*0x01C*/     enum _DEVICE_POWER_STATE State;                                                        
/*0x020*/     struct _LIST_ENTRY NotifySourceList;               // 2 elements, 0x8 bytes (sizeof)   
/*0x028*/     struct _LIST_ENTRY NotifyTargetList;               // 2 elements, 0x8 bytes (sizeof)   
/*0x030*/     struct _POWER_CHANNEL_SUMMARY PowerChannelSummary; // 4 elements, 0x14 bytes (sizeof)  
/*0x044*/     struct _LIST_ENTRY Volume;                         // 2 elements, 0x8 bytes (sizeof)   
          }DEVICE_OBJECT_POWER_EXTENSION, *PDEVICE_OBJECT_POWER_EXTENSION;                                    
                                                                                                     
          typedef struct _DEVICE_RELATIONS       // 2 elements, 0x8 bytes (sizeof) 
          {                                                                        
/*0x000*/     ULONG32      Count;                                                  
/*0x004*/     struct _DEVICE_OBJECT* Objects[1];                                   
          }DEVICE_RELATIONS, *PDEVICE_RELATIONS;                                   
                                                                                   
          typedef struct _DEVOBJ_EXTENSION                 // 12 elements, 0x2C bytes (sizeof) 
          {                                                                                    
/*0x000*/     INT16        Type;                                                               
/*0x002*/     UINT16       Size;                                                               
/*0x004*/     struct _DEVICE_OBJECT* DeviceObject;                                             
/*0x008*/     ULONG32      PowerFlags;                                                         
/*0x00C*/     struct _DEVICE_OBJECT_POWER_EXTENSION* Dope;                                     
/*0x010*/     ULONG32      ExtensionFlags;                                                     
/*0x014*/     VOID*        DeviceNode;                                                         
/*0x018*/     struct _DEVICE_OBJECT* AttachedTo;                                               
/*0x01C*/     LONG32       StartIoCount;                                                       
/*0x020*/     LONG32       StartIoKey;                                                         
/*0x024*/     ULONG32      StartIoFlags;                                                       
/*0x028*/     struct _VPB* Vpb;                                                                
          }DEVOBJ_EXTENSION, *PDEVOBJ_EXTENSION;                                               
                                                                                               
          typedef struct _DRIVER_EXTENSION                        // 6 elements, 0x1C bytes (sizeof) 
          {                                                                                          
/*0x000*/     struct _DRIVER_OBJECT* DriverObject;                                                   
/*0x004*/     FUNCT_0049_03BB_AddDevice* AddDevice;                                                  
/*0x008*/     ULONG32      Count;                                                                    
/*0x00C*/     struct _UNICODE_STRING ServiceKeyName;              // 3 elements, 0x8 bytes (sizeof)  
/*0x014*/     struct _IO_CLIENT_EXTENSION* ClientDriverExtension;                                    
/*0x018*/     struct _FS_FILTER_CALLBACKS* FsFilterCallbacks;                                        
          }DRIVER_EXTENSION, *PDRIVER_EXTENSION;                                                     
                                                                                                     
          typedef struct _DRIVER_OBJECT                                                      // 15 elements, 0xA8 bytes (sizeof) 
          {                                                                                                                      
/*0x000*/     INT16        Type;                                                                                                 
/*0x002*/     INT16        Size;                                                                                                 
/*0x004*/     struct _DEVICE_OBJECT* DeviceObject;                                                                               
/*0x008*/     ULONG32      Flags;                                                                                                
/*0x00C*/     VOID*        DriverStart;                                                                                          
/*0x010*/     ULONG32      DriverSize;                                                                                           
/*0x014*/     VOID*        DriverSection;                                                                                        
/*0x018*/     struct _DRIVER_EXTENSION* DriverExtension;                                                                         
/*0x01C*/     struct _UNICODE_STRING DriverName;                                             // 3 elements, 0x8 bytes (sizeof)   
/*0x024*/     struct _UNICODE_STRING* HardwareDatabase;                                                                          
/*0x028*/     struct _FAST_IO_DISPATCH* FastIoDispatch;                                                                          
/*0x02C*/     FUNCT_0049_0956_DriverInit* DriverInit;                                                                            
/*0x030*/     FUNCT_00BC_0718_DriverStartIo_CancelRoutine_SavedCancelRoutine* DriverStartIo;                                     
/*0x034*/     FUNCT_00BC_095A_DriverUnload* DriverUnload;                                                                        
/*0x038*/     FUNCT_0049_095E_MajorFunction* MajorFunction[28];                                                                  
          }DRIVER_OBJECT, *PDRIVER_OBJECT;                                                                                       
                                                                                                                                 
          typedef struct _GUID       // 4 elements, 0x10 bytes (sizeof) 
          {                                                             
/*0x000*/     ULONG32      Data1;                                       
/*0x004*/     UINT16       Data2;                                       
/*0x006*/     UINT16       Data3;                                       
/*0x008*/     UINT8        Data4[8];                                    
          }GUID, *PGUID;                                                
                                                                        
          typedef struct _DUMP_INITIALIZATION_CONTEXT                                              // 19 elements, 0x70 bytes (sizeof) 
          {                                                                                                                            
/*0x000*/     ULONG32      Length;                                                                                                     
/*0x004*/     ULONG32      Reserved;                                                                                                   
/*0x008*/     VOID*        MemoryBlock;                                                                                                
/*0x00C*/     VOID*        CommonBuffer[2];                                                                                            
/*0x014*/     UINT8        _PADDING0_[0x4];                                                                                            
/*0x018*/     union _LARGE_INTEGER PhysicalAddress[2];                                                                                 
/*0x028*/     FUNCT_00BC_18EF_StallRoutine* StallRoutine;                                                                              
/*0x02C*/     FUNCT_003D_18F2_OpenRoutine* OpenRoutine;                                                                                
/*0x030*/     FUNCT_0049_18F5_WriteRoutine* WriteRoutine;                                                                              
/*0x034*/     FUNCT_00BC_0631_PostProcessInitRoutine_DispatchAddress_FinishRoutine* FinishRoutine;                                     
/*0x038*/     struct _ADAPTER_OBJECT* AdapterObject;                                                                                   
/*0x03C*/     VOID*        MappedRegisterBase;                                                                                         
/*0x040*/     VOID*        PortConfiguration;                                                                                          
/*0x044*/     UINT8        CrashDump;                                                                                                  
/*0x045*/     UINT8        _PADDING1_[0x3];                                                                                            
/*0x048*/     ULONG32      MaximumTransferSize;                                                                                        
/*0x04C*/     ULONG32      CommonBufferSize;                                                                                           
/*0x050*/     VOID*        TargetAddress;                                                                                              
/*0x054*/     FUNCT_0049_18FB_WritePendingRoutine* WritePendingRoutine;                                                                
/*0x058*/     ULONG32      PartitionStyle;                                                                                             
              union                                                                                // 2 elements, 0x10 bytes (sizeof)  
              {                                                                                                                        
                  struct                                                                           // 2 elements, 0x8 bytes (sizeof)   
                  {                                                                                                                    
/*0x05C*/             ULONG32      Signature;                                                                                          
/*0x060*/             ULONG32      CheckSum;                                                                                           
                  }Mbr;                                                                                                                
                  struct                                                                           // 1 elements, 0x10 bytes (sizeof)  
                  {                                                                                                                    
/*0x05C*/             struct _GUID DiskId;                                                         // 4 elements, 0x10 bytes (sizeof)  
                  }Gpt;                                                                                                                
              }DiskInfo;                                                                                                               
          }DUMP_INITIALIZATION_CONTEXT, *PDUMP_INITIALIZATION_CONTEXT;                                                                 
                                                                                                                                       
          typedef struct _STRING          // 3 elements, 0x8 bytes (sizeof) 
          {                                                                 
/*0x000*/     UINT16       Length;                                          
/*0x002*/     UINT16       MaximumLength;                                   
/*0x004*/     CHAR*        Buffer;                                          
          }STRING, *PSTRING;                                                
                                                                            
          typedef struct _DUMP_STACK_CONTEXT                  // 11 elements, 0xB0 bytes (sizeof) 
          {                                                                                       
/*0x000*/     struct _DUMP_INITIALIZATION_CONTEXT Init;       // 19 elements, 0x70 bytes (sizeof) 
/*0x070*/     union _LARGE_INTEGER PartitionOffset;           // 4 elements, 0x8 bytes (sizeof)   
/*0x078*/     VOID*        DumpPointers;                                                          
/*0x07C*/     ULONG32      PointersLength;                                                        
/*0x080*/     UINT16*      ModulePrefix;                                                          
/*0x084*/     struct _LIST_ENTRY DriverList;                  // 2 elements, 0x8 bytes (sizeof)   
/*0x08C*/     struct _STRING InitMsg;                         // 3 elements, 0x8 bytes (sizeof)   
/*0x094*/     struct _STRING ProgMsg;                         // 3 elements, 0x8 bytes (sizeof)   
/*0x09C*/     struct _STRING DoneMsg;                         // 3 elements, 0x8 bytes (sizeof)   
/*0x0A4*/     VOID*        FileObject;                                                            
/*0x0A8*/     enum _DEVICE_USAGE_NOTIFICATION_TYPE UsageType;                                     
/*0x0AC*/     UINT8        _PADDING0_[0x4];                                                       
          }DUMP_STACK_CONTEXT, *PDUMP_STACK_CONTEXT;                                              
                                                                                                  
          typedef struct _OWNER_ENTRY      // 3 elements, 0x8 bytes (sizeof) 
          {                                                                  
/*0x000*/     ULONG32      OwnerThread;                                      
              union                        // 2 elements, 0x4 bytes (sizeof) 
              {                                                              
/*0x004*/         LONG32       OwnerCount;                                   
/*0x004*/         ULONG32      TableSize;                                    
              };                                                             
          }OWNER_ENTRY, *POWNER_ENTRY;                                       
                                                                             
          typedef struct _ERESOURCE                   // 13 elements, 0x38 bytes (sizeof) 
          {                                                                               
/*0x000*/     struct _LIST_ENTRY SystemResourcesList; // 2 elements, 0x8 bytes (sizeof)   
/*0x008*/     struct _OWNER_ENTRY* OwnerTable;                                            
/*0x00C*/     INT16        ActiveCount;                                                   
/*0x00E*/     UINT16       Flag;                                                          
/*0x010*/     struct _KSEMAPHORE* SharedWaiters;                                          
/*0x014*/     struct _KEVENT* ExclusiveWaiters;                                           
/*0x018*/     struct _OWNER_ENTRY OwnerThreads[2];                                        
/*0x028*/     ULONG32      ContentionCount;                                               
/*0x02C*/     UINT16       NumberOfSharedWaiters;                                         
/*0x02E*/     UINT16       NumberOfExclusiveWaiters;                                      
              union                                   // 2 elements, 0x4 bytes (sizeof)   
              {                                                                           
/*0x030*/         VOID*        Address;                                                   
/*0x030*/         ULONG32      CreatorBackTraceIndex;                                     
              };                                                                          
/*0x034*/     ULONG32      SpinLock;                                                      
          }ERESOURCE, *PERESOURCE;                                                        
                                                                                          
          typedef struct _IO_COUNTERS           // 6 elements, 0x30 bytes (sizeof) 
          {                                                                        
/*0x000*/     UINT64       ReadOperationCount;                                     
/*0x008*/     UINT64       WriteOperationCount;                                    
/*0x010*/     UINT64       OtherOperationCount;                                    
/*0x018*/     UINT64       ReadTransferCount;                                      
/*0x020*/     UINT64       WriteTransferCount;                                     
/*0x028*/     UINT64       OtherTransferCount;                                     
          }IO_COUNTERS, *PIO_COUNTERS;                                             
                                                                                   
          typedef struct _FAST_MUTEX   // 5 elements, 0x20 bytes (sizeof) 
          {                                                               
/*0x000*/     LONG32       Count;                                         
/*0x004*/     struct _KTHREAD* Owner;                                     
/*0x008*/     ULONG32      Contention;                                    
/*0x00C*/     struct _KEVENT Event;    // 1 elements, 0x10 bytes (sizeof) 
/*0x01C*/     ULONG32      OldIrql;                                       
          }FAST_MUTEX, *PFAST_MUTEX;                                      
                                                                          
          typedef struct _EJOB                                // 45 elements, 0x180 bytes (sizeof) 
          {                                                                                        
/*0x000*/     struct _KEVENT Event;                           // 1 elements, 0x10 bytes (sizeof)   
/*0x010*/     struct _LIST_ENTRY JobLinks;                    // 2 elements, 0x8 bytes (sizeof)    
/*0x018*/     struct _LIST_ENTRY ProcessListHead;             // 2 elements, 0x8 bytes (sizeof)    
/*0x020*/     struct _ERESOURCE JobLock;                      // 13 elements, 0x38 bytes (sizeof)  
/*0x058*/     union _LARGE_INTEGER TotalUserTime;             // 4 elements, 0x8 bytes (sizeof)    
/*0x060*/     union _LARGE_INTEGER TotalKernelTime;           // 4 elements, 0x8 bytes (sizeof)    
/*0x068*/     union _LARGE_INTEGER ThisPeriodTotalUserTime;   // 4 elements, 0x8 bytes (sizeof)    
/*0x070*/     union _LARGE_INTEGER ThisPeriodTotalKernelTime; // 4 elements, 0x8 bytes (sizeof)    
/*0x078*/     ULONG32      TotalPageFaultCount;                                                    
/*0x07C*/     ULONG32      TotalProcesses;                                                         
/*0x080*/     ULONG32      ActiveProcesses;                                                        
/*0x084*/     ULONG32      TotalTerminatedProcesses;                                               
/*0x088*/     union _LARGE_INTEGER PerProcessUserTimeLimit;   // 4 elements, 0x8 bytes (sizeof)    
/*0x090*/     union _LARGE_INTEGER PerJobUserTimeLimit;       // 4 elements, 0x8 bytes (sizeof)    
/*0x098*/     ULONG32      LimitFlags;                                                             
/*0x09C*/     ULONG32      MinimumWorkingSetSize;                                                  
/*0x0A0*/     ULONG32      MaximumWorkingSetSize;                                                  
/*0x0A4*/     ULONG32      ActiveProcessLimit;                                                     
/*0x0A8*/     ULONG32      Affinity;                                                               
/*0x0AC*/     UINT8        PriorityClass;                                                          
/*0x0AD*/     UINT8        _PADDING0_[0x3];                                                        
/*0x0B0*/     ULONG32      UIRestrictionsClass;                                                    
/*0x0B4*/     ULONG32      SecurityLimitFlags;                                                     
/*0x0B8*/     VOID*        Token;                                                                  
/*0x0BC*/     struct _PS_JOB_TOKEN_FILTER* Filter;                                                 
/*0x0C0*/     ULONG32      EndOfJobTimeAction;                                                     
/*0x0C4*/     VOID*        CompletionPort;                                                         
/*0x0C8*/     VOID*        CompletionKey;                                                          
/*0x0CC*/     ULONG32      SessionId;                                                              
/*0x0D0*/     ULONG32      SchedulingClass;                                                        
/*0x0D4*/     UINT8        _PADDING1_[0x4];                                                        
/*0x0D8*/     UINT64       ReadOperationCount;                                                     
/*0x0E0*/     UINT64       WriteOperationCount;                                                    
/*0x0E8*/     UINT64       OtherOperationCount;                                                    
/*0x0F0*/     UINT64       ReadTransferCount;                                                      
/*0x0F8*/     UINT64       WriteTransferCount;                                                     
/*0x100*/     UINT64       OtherTransferCount;                                                     
/*0x108*/     struct _IO_COUNTERS IoInfo;                     // 6 elements, 0x30 bytes (sizeof)   
/*0x138*/     ULONG32      ProcessMemoryLimit;                                                     
/*0x13C*/     ULONG32      JobMemoryLimit;                                                         
/*0x140*/     ULONG32      PeakProcessMemoryUsed;                                                  
/*0x144*/     ULONG32      PeakJobMemoryUsed;                                                      
/*0x148*/     ULONG32      CurrentJobMemoryUsed;                                                   
/*0x14C*/     struct _FAST_MUTEX MemoryLimitsLock;            // 5 elements, 0x20 bytes (sizeof)   
/*0x16C*/     struct _LIST_ENTRY JobSetLinks;                 // 2 elements, 0x8 bytes (sizeof)    
/*0x174*/     ULONG32      MemberLevel;                                                            
/*0x178*/     ULONG32      JobFlags;                                                               
/*0x17C*/     UINT8        _PADDING2_[0x4];                                                        
          }EJOB, *PEJOB;                                                                           
                                                                                                   
          typedef struct _KGDTENTRY                 // 3 elements, 0x8 bytes (sizeof)  
          {                                                                            
/*0x000*/     UINT16       LimitLow;                                                   
/*0x002*/     UINT16       BaseLow;                                                    
              union                                 // 2 elements, 0x4 bytes (sizeof)  
              {                                                                        
                  struct                            // 4 elements, 0x4 bytes (sizeof)  
                  {                                                                    
/*0x004*/             UINT8        BaseMid;                                            
/*0x005*/             UINT8        Flags1;                                             
/*0x006*/             UINT8        Flags2;                                             
/*0x007*/             UINT8        BaseHi;                                             
                  }Bytes;                                                              
                  struct                            // 10 elements, 0x4 bytes (sizeof) 
                  {                                                                    
/*0x004*/             ULONG32      BaseMid : 8;     // 0 BitPosition                   
/*0x004*/             ULONG32      Type : 5;        // 8 BitPosition                   
/*0x004*/             ULONG32      Dpl : 2;         // 13 BitPosition                  
/*0x004*/             ULONG32      Pres : 1;        // 15 BitPosition                  
/*0x004*/             ULONG32      LimitHi : 4;     // 16 BitPosition                  
/*0x004*/             ULONG32      Sys : 1;         // 20 BitPosition                  
/*0x004*/             ULONG32      Reserved_0 : 1;  // 21 BitPosition                  
/*0x004*/             ULONG32      Default_Big : 1; // 22 BitPosition                  
/*0x004*/             ULONG32      Granularity : 1; // 23 BitPosition                  
/*0x004*/             ULONG32      BaseHi : 8;      // 24 BitPosition                  
                  }Bits;                                                               
              }HighWord;                                                               
          }KGDTENTRY, *PKGDTENTRY;                                                     
                                                                                       
          typedef struct _KIDTENTRY        // 4 elements, 0x8 bytes (sizeof) 
          {                                                                  
/*0x000*/     UINT16       Offset;                                           
/*0x002*/     UINT16       Selector;                                         
/*0x004*/     UINT16       Access;                                           
/*0x006*/     UINT16       ExtendedOffset;                                   
          }KIDTENTRY, *PKIDTENTRY;                                           
                                                                             
          typedef struct _SINGLE_LIST_ENTRY    // 1 elements, 0x4 bytes (sizeof) 
          {                                                                      
/*0x000*/     struct _SINGLE_LIST_ENTRY* Next;                                   
          }SINGLE_LIST_ENTRY, *PSINGLE_LIST_ENTRY;                                  
                                                                                 
          typedef struct _KEXECUTE_OPTIONS            // 7 elements, 0x1 bytes (sizeof) 
          {                                                                             
/*0x000*/     UINT8        ExecuteDisable : 1;        // 0 BitPosition                  
/*0x000*/     UINT8        ExecuteEnable : 1;         // 1 BitPosition                  
/*0x000*/     UINT8        DisableThunkEmulation : 1; // 2 BitPosition                  
/*0x000*/     UINT8        Permanent : 1;             // 3 BitPosition                  
/*0x000*/     UINT8        ExecuteDispatchEnable : 1; // 4 BitPosition                  
/*0x000*/     UINT8        ImageDispatchEnable : 1;   // 5 BitPosition                  
/*0x000*/     UINT8        Spare : 2;                 // 6 BitPosition                  
          }KEXECUTE_OPTIONS, *PKEXECUTE_OPTIONS;                                        
                                                                                        
          typedef struct _KPROCESS                     // 29 elements, 0x6C bytes (sizeof) 
          {                                                                                
/*0x000*/     struct _DISPATCHER_HEADER Header;        // 6 elements, 0x10 bytes (sizeof)  
/*0x010*/     struct _LIST_ENTRY ProfileListHead;      // 2 elements, 0x8 bytes (sizeof)   
/*0x018*/     ULONG32      DirectoryTableBase[2];                                          
/*0x020*/     struct _KGDTENTRY LdtDescriptor;         // 3 elements, 0x8 bytes (sizeof)   
/*0x028*/     struct _KIDTENTRY Int21Descriptor;       // 4 elements, 0x8 bytes (sizeof)   
/*0x030*/     UINT16       IopmOffset;                                                     
/*0x032*/     UINT8        Iopl;                                                           
/*0x033*/     UINT8        Unused;                                                         
/*0x034*/     ULONG32      ActiveProcessors;                                               
/*0x038*/     ULONG32      KernelTime;                                                     
/*0x03C*/     ULONG32      UserTime;                                                       
/*0x040*/     struct _LIST_ENTRY ReadyListHead;        // 2 elements, 0x8 bytes (sizeof)   
/*0x048*/     struct _SINGLE_LIST_ENTRY SwapListEntry; // 1 elements, 0x4 bytes (sizeof)   
/*0x04C*/     VOID*        VdmTrapcHandler;                                                
/*0x050*/     struct _LIST_ENTRY ThreadListHead;       // 2 elements, 0x8 bytes (sizeof)   
/*0x058*/     ULONG32      ProcessLock;                                                    
/*0x05C*/     ULONG32      Affinity;                                                       
/*0x060*/     UINT16       StackCount;                                                     
/*0x062*/     CHAR         BasePriority;                                                   
/*0x063*/     CHAR         ThreadQuantum;                                                  
/*0x064*/     UINT8        AutoAlignment;                                                  
/*0x065*/     UINT8        State;                                                          
/*0x066*/     UINT8        ThreadSeed;                                                     
/*0x067*/     UINT8        DisableBoost;                                                   
/*0x068*/     UINT8        PowerState;                                                     
/*0x069*/     UINT8        DisableQuantum;                                                 
/*0x06A*/     UINT8        IdealNode;                                                      
              union                                    // 2 elements, 0x1 bytes (sizeof)   
              {                                                                            
/*0x06B*/         struct _KEXECUTE_OPTIONS Flags;      // 7 elements, 0x1 bytes (sizeof)   
/*0x06B*/         UINT8        ExecuteOptions;                                             
              };                                                                           
          }KPROCESS, *PKPROCESS;                                                           
                                                                                           
          typedef struct _EX_PUSH_LOCK            // 5 elements, 0x4 bytes (sizeof) 
          {                                                                         
              union                               // 3 elements, 0x4 bytes (sizeof) 
              {                                                                     
                  struct                          // 3 elements, 0x4 bytes (sizeof) 
                  {                                                                 
/*0x000*/             ULONG32      Waiting : 1;   // 0 BitPosition                  
/*0x000*/             ULONG32      Exclusive : 1; // 1 BitPosition                  
/*0x000*/             ULONG32      Shared : 30;   // 2 BitPosition                  
                  };                                                                
/*0x000*/         ULONG32      Value;                                               
/*0x000*/         VOID*        Ptr;                                                 
              };                                                                    
          }EX_PUSH_LOCK, *PEX_PUSH_LOCK;                                            
                                                                                    
          typedef struct _EX_RUNDOWN_REF // 2 elements, 0x4 bytes (sizeof) 
          {                                                                
              union                      // 2 elements, 0x4 bytes (sizeof) 
              {                                                            
/*0x000*/         ULONG32      Count;                                      
/*0x000*/         VOID*        Ptr;                                        
              };                                                           
          }EX_RUNDOWN_REF, *PEX_RUNDOWN_REF;                                  
                                                                           
          typedef struct _EX_FAST_REF      // 3 elements, 0x4 bytes (sizeof) 
          {                                                                  
              union                        // 3 elements, 0x4 bytes (sizeof) 
              {                                                              
/*0x000*/         VOID*        Object;                                       
/*0x000*/         ULONG32      RefCnt : 3; // 0 BitPosition                  
/*0x000*/         ULONG32      Value;                                        
              };                                                             
          }EX_FAST_REF, *PEX_FAST_REF;                                       
                                                                             
          typedef struct _HARDWARE_PTE                   // 16 elements, 0x8 bytes (sizeof) 
          {                                                                                 
              union                                      // 2 elements, 0x8 bytes (sizeof)  
              {                                                                             
                  struct                                 // 14 elements, 0x8 bytes (sizeof) 
                  {                                                                         
/*0x000*/             UINT64       Valid : 1;            // 0 BitPosition                   
/*0x000*/             UINT64       Write : 1;            // 1 BitPosition                   
/*0x000*/             UINT64       Owner : 1;            // 2 BitPosition                   
/*0x000*/             UINT64       WriteThrough : 1;     // 3 BitPosition                   
/*0x000*/             UINT64       CacheDisable : 1;     // 4 BitPosition                   
/*0x000*/             UINT64       Accessed : 1;         // 5 BitPosition                   
/*0x000*/             UINT64       Dirty : 1;            // 6 BitPosition                   
/*0x000*/             UINT64       LargePage : 1;        // 7 BitPosition                   
/*0x000*/             UINT64       Global : 1;           // 8 BitPosition                   
/*0x000*/             UINT64       CopyOnWrite : 1;      // 9 BitPosition                   
/*0x000*/             UINT64       Prototype : 1;        // 10 BitPosition                  
/*0x000*/             UINT64       reserved0 : 1;        // 11 BitPosition                  
/*0x000*/             UINT64       PageFrameNumber : 26; // 12 BitPosition                  
/*0x000*/             UINT64       reserved1 : 26;       // 38 BitPosition                  
                  };                                                                        
                  struct                                 // 2 elements, 0x8 bytes (sizeof)  
                  {                                                                         
/*0x000*/             ULONG32      LowPart;                                                 
/*0x004*/             ULONG32      HighPart;                                                
                  };                                                                        
              };                                                                            
          }HARDWARE_PTE, *PHARDWARE_PTE;                                                    
                                                                                            
          typedef struct _SE_AUDIT_PROCESS_CREATION_INFO      // 1 elements, 0x4 bytes (sizeof) 
          {                                                                                     
/*0x000*/     struct _OBJECT_NAME_INFORMATION* ImageFileName;                                   
          }SE_AUDIT_PROCESS_CREATION_INFO, *PSE_AUDIT_PROCESS_CREATION_INFO;                                  
                                                                                                
          typedef struct _MMSUPPORT_FLAGS                 // 9 elements, 0x4 bytes (sizeof) 
          {                                                                                 
/*0x000*/     UINT32       SessionSpace : 1;              // 0 BitPosition                  
/*0x000*/     UINT32       BeingTrimmed : 1;              // 1 BitPosition                  
/*0x000*/     UINT32       SessionLeader : 1;             // 2 BitPosition                  
/*0x000*/     UINT32       TrimHard : 1;                  // 3 BitPosition                  
/*0x000*/     UINT32       WorkingSetHard : 1;            // 4 BitPosition                  
/*0x000*/     UINT32       AddressSpaceBeingDeleted : 1;  // 5 BitPosition                  
/*0x000*/     UINT32       Available : 10;                // 6 BitPosition                  
/*0x000*/     UINT32       AllowWorkingSetAdjustment : 8; // 16 BitPosition                 
/*0x000*/     UINT32       MemoryPriority : 8;            // 24 BitPosition                 
          }MMSUPPORT_FLAGS, *PMMSUPPORT_FLAGS;                                              
                                                                                            
          typedef struct _MMSUPPORT                        // 14 elements, 0x40 bytes (sizeof) 
          {                                                                                    
/*0x000*/     union _LARGE_INTEGER LastTrimTime;           // 4 elements, 0x8 bytes (sizeof)   
/*0x008*/     struct _MMSUPPORT_FLAGS Flags;               // 9 elements, 0x4 bytes (sizeof)   
/*0x00C*/     ULONG32      PageFaultCount;                                                     
/*0x010*/     ULONG32      PeakWorkingSetSize;                                                 
/*0x014*/     ULONG32      WorkingSetSize;                                                     
/*0x018*/     ULONG32      MinimumWorkingSetSize;                                              
/*0x01C*/     ULONG32      MaximumWorkingSetSize;                                              
/*0x020*/     struct _MMWSL* VmWorkingSetList;                                                 
/*0x024*/     struct _LIST_ENTRY WorkingSetExpansionLinks; // 2 elements, 0x8 bytes (sizeof)   
/*0x02C*/     ULONG32      Claim;                                                              
/*0x030*/     ULONG32      NextEstimationSlot;                                                 
/*0x034*/     ULONG32      NextAgingSlot;                                                      
/*0x038*/     ULONG32      EstimatedAvailable;                                                 
/*0x03C*/     ULONG32      GrowthSinceLastEstimate;                                            
          }MMSUPPORT, *PMMSUPPORT;                                                             
                                                                                               
          typedef struct _EPROCESS                                               // 107 elements, 0x260 bytes (sizeof) 
          {                                                                                                            
/*0x000*/     struct _KPROCESS Pcb;                                              // 29 elements, 0x6C bytes (sizeof)   
/*0x06C*/     struct _EX_PUSH_LOCK ProcessLock;                                  // 5 elements, 0x4 bytes (sizeof)     
/*0x070*/     union _LARGE_INTEGER CreateTime;                                   // 4 elements, 0x8 bytes (sizeof)     
/*0x078*/     union _LARGE_INTEGER ExitTime;                                     // 4 elements, 0x8 bytes (sizeof)     
/*0x080*/     struct _EX_RUNDOWN_REF RundownProtect;                             // 2 elements, 0x4 bytes (sizeof)     
/*0x084*/     VOID*        UniqueProcessId;                                                                            
/*0x088*/     struct _LIST_ENTRY ActiveProcessLinks;                             // 2 elements, 0x8 bytes (sizeof)     
/*0x090*/     ULONG32      QuotaUsage[3];                                                                              
/*0x09C*/     ULONG32      QuotaPeak[3];                                                                               
/*0x0A8*/     ULONG32      CommitCharge;                                                                               
/*0x0AC*/     ULONG32      PeakVirtualSize;                                                                            
/*0x0B0*/     ULONG32      VirtualSize;                                                                                
/*0x0B4*/     struct _LIST_ENTRY SessionProcessLinks;                            // 2 elements, 0x8 bytes (sizeof)     
/*0x0BC*/     VOID*        DebugPort;                                                                                  
/*0x0C0*/     VOID*        ExceptionPort;                                                                              
/*0x0C4*/     struct _HANDLE_TABLE* ObjectTable;                                                                       
/*0x0C8*/     struct _EX_FAST_REF Token;                                         // 3 elements, 0x4 bytes (sizeof)     
/*0x0CC*/     struct _FAST_MUTEX WorkingSetLock;                                 // 5 elements, 0x20 bytes (sizeof)    
/*0x0EC*/     ULONG32      WorkingSetPage;                                                                             
/*0x0F0*/     struct _FAST_MUTEX AddressCreationLock;                            // 5 elements, 0x20 bytes (sizeof)    
/*0x110*/     ULONG32      HyperSpaceLock;                                                                             
/*0x114*/     struct _ETHREAD* ForkInProgress;                                                                         
/*0x118*/     ULONG32      HardwareTrigger;                                                                            
/*0x11C*/     VOID*        VadRoot;                                                                                    
/*0x120*/     VOID*        VadHint;                                                                                    
/*0x124*/     VOID*        CloneRoot;                                                                                  
/*0x128*/     ULONG32      NumberOfPrivatePages;                                                                       
/*0x12C*/     ULONG32      NumberOfLockedPages;                                                                        
/*0x130*/     VOID*        Win32Process;                                                                               
/*0x134*/     struct _EJOB* Job;                                                                                       
/*0x138*/     VOID*        SectionObject;                                                                              
/*0x13C*/     VOID*        SectionBaseAddress;                                                                         
/*0x140*/     struct _EPROCESS_QUOTA_BLOCK* QuotaBlock;                                                                
/*0x144*/     struct _PAGEFAULT_HISTORY* WorkingSetWatch;                                                              
/*0x148*/     VOID*        Win32WindowStation;                                                                         
/*0x14C*/     VOID*        InheritedFromUniqueProcessId;                                                               
/*0x150*/     VOID*        LdtInformation;                                                                             
/*0x154*/     VOID*        VadFreeHint;                                                                                
/*0x158*/     VOID*        VdmObjects;                                                                                 
/*0x15C*/     VOID*        DeviceMap;                                                                                  
/*0x160*/     struct _LIST_ENTRY PhysicalVadList;                                // 2 elements, 0x8 bytes (sizeof)     
              union                                                              // 2 elements, 0x8 bytes (sizeof)     
              {                                                                                                        
/*0x168*/         struct _HARDWARE_PTE PageDirectoryPte;                         // 16 elements, 0x8 bytes (sizeof)    
/*0x168*/         UINT64       Filler;                                                                                 
              };                                                                                                       
/*0x170*/     VOID*        Session;                                                                                    
/*0x174*/     UINT8        ImageFileName[16];                                                                          
/*0x184*/     struct _LIST_ENTRY JobLinks;                                       // 2 elements, 0x8 bytes (sizeof)     
/*0x18C*/     VOID*        LockedPagesList;                                                                            
/*0x190*/     struct _LIST_ENTRY ThreadListHead;                                 // 2 elements, 0x8 bytes (sizeof)     
/*0x198*/     VOID*        SecurityPort;                                                                               
/*0x19C*/     VOID*        PaeTop;                                                                                     
/*0x1A0*/     ULONG32      ActiveThreads;                                                                              
/*0x1A4*/     ULONG32      GrantedAccess;                                                                              
/*0x1A8*/     ULONG32      DefaultHardErrorProcessing;                                                                 
/*0x1AC*/     LONG32       LastThreadExitStatus;                                                                       
/*0x1B0*/     struct _PEB* Peb;                                                                                        
/*0x1B4*/     struct _EX_FAST_REF PrefetchTrace;                                 // 3 elements, 0x4 bytes (sizeof)     
/*0x1B8*/     union _LARGE_INTEGER ReadOperationCount;                           // 4 elements, 0x8 bytes (sizeof)     
/*0x1C0*/     union _LARGE_INTEGER WriteOperationCount;                          // 4 elements, 0x8 bytes (sizeof)     
/*0x1C8*/     union _LARGE_INTEGER OtherOperationCount;                          // 4 elements, 0x8 bytes (sizeof)     
/*0x1D0*/     union _LARGE_INTEGER ReadTransferCount;                            // 4 elements, 0x8 bytes (sizeof)     
/*0x1D8*/     union _LARGE_INTEGER WriteTransferCount;                           // 4 elements, 0x8 bytes (sizeof)     
/*0x1E0*/     union _LARGE_INTEGER OtherTransferCount;                           // 4 elements, 0x8 bytes (sizeof)     
/*0x1E8*/     ULONG32      CommitChargeLimit;                                                                          
/*0x1EC*/     ULONG32      CommitChargePeak;                                                                           
/*0x1F0*/     VOID*        AweInfo;                                                                                    
/*0x1F4*/     struct _SE_AUDIT_PROCESS_CREATION_INFO SeAuditProcessCreationInfo; // 1 elements, 0x4 bytes (sizeof)     
/*0x1F8*/     struct _MMSUPPORT Vm;                                              // 14 elements, 0x40 bytes (sizeof)   
/*0x238*/     ULONG32      LastFaultCount;                                                                             
/*0x23C*/     ULONG32      ModifiedPageCount;                                                                          
/*0x240*/     ULONG32      NumberOfVads;                                                                               
/*0x244*/     ULONG32      JobStatus;                                                                                  
              union                                                              // 2 elements, 0x4 bytes (sizeof)     
              {                                                                                                        
/*0x248*/         ULONG32      Flags;                                                                                  
                  struct                                                         // 27 elements, 0x4 bytes (sizeof)    
                  {                                                                                                    
/*0x248*/             ULONG32      CreateReported : 1;                           // 0 BitPosition                      
/*0x248*/             ULONG32      NoDebugInherit : 1;                           // 1 BitPosition                      
/*0x248*/             ULONG32      ProcessExiting : 1;                           // 2 BitPosition                      
/*0x248*/             ULONG32      ProcessDelete : 1;                            // 3 BitPosition                      
/*0x248*/             ULONG32      Wow64SplitPages : 1;                          // 4 BitPosition                      
/*0x248*/             ULONG32      VmDeleted : 1;                                // 5 BitPosition                      
/*0x248*/             ULONG32      OutswapEnabled : 1;                           // 6 BitPosition                      
/*0x248*/             ULONG32      Outswapped : 1;                               // 7 BitPosition                      
/*0x248*/             ULONG32      ForkFailed : 1;                               // 8 BitPosition                      
/*0x248*/             ULONG32      HasPhysicalVad : 1;                           // 9 BitPosition                      
/*0x248*/             ULONG32      AddressSpaceInitialized : 2;                  // 10 BitPosition                     
/*0x248*/             ULONG32      SetTimerResolution : 1;                       // 12 BitPosition                     
/*0x248*/             ULONG32      BreakOnTermination : 1;                       // 13 BitPosition                     
/*0x248*/             ULONG32      SessionCreationUnderway : 1;                  // 14 BitPosition                     
/*0x248*/             ULONG32      WriteWatch : 1;                               // 15 BitPosition                     
/*0x248*/             ULONG32      ProcessInSession : 1;                         // 16 BitPosition                     
/*0x248*/             ULONG32      OverrideAddressSpace : 1;                     // 17 BitPosition                     
/*0x248*/             ULONG32      HasAddressSpace : 1;                          // 18 BitPosition                     
/*0x248*/             ULONG32      LaunchPrefetched : 1;                         // 19 BitPosition                     
/*0x248*/             ULONG32      InjectInpageErrors : 1;                       // 20 BitPosition                     
/*0x248*/             ULONG32      VmTopDown : 1;                                // 21 BitPosition                     
/*0x248*/             ULONG32      Unused3 : 1;                                  // 22 BitPosition                     
/*0x248*/             ULONG32      Unused4 : 1;                                  // 23 BitPosition                     
/*0x248*/             ULONG32      VdmAllowed : 1;                               // 24 BitPosition                     
/*0x248*/             ULONG32      Unused : 5;                                   // 25 BitPosition                     
/*0x248*/             ULONG32      Unused1 : 1;                                  // 30 BitPosition                     
/*0x248*/             ULONG32      Unused2 : 1;                                  // 31 BitPosition                     
                  };                                                                                                   
              };                                                                                                       
/*0x24C*/     LONG32       ExitStatus;                                                                                 
/*0x250*/     UINT16       NextPageColor;                                                                              
              union                                                              // 2 elements, 0x2 bytes (sizeof)     
              {                                                                                                        
                  struct                                                         // 2 elements, 0x2 bytes (sizeof)     
                  {                                                                                                    
/*0x252*/             UINT8        SubSystemMinorVersion;                                                              
/*0x253*/             UINT8        SubSystemMajorVersion;                                                              
                  };                                                                                                   
/*0x252*/         UINT16       SubSystemVersion;                                                                       
              };                                                                                                       
/*0x254*/     UINT8        PriorityClass;                                                                              
/*0x255*/     UINT8        WorkingSetAcquiredUnsafe;                                                                   
/*0x256*/     UINT8        _PADDING0_[0x2];                                                                            
/*0x258*/     ULONG32      Cookie;                                                                                     
/*0x25C*/     UINT8        _PADDING1_[0x4];                                                                            
          }EPROCESS, *PEPROCESS;                                                                                       
                                                                                                                       
          typedef struct _EPROCESS_QUOTA_ENTRY // 4 elements, 0x10 bytes (sizeof) 
          {                                                                       
/*0x000*/     ULONG32      Usage;                                                 
/*0x004*/     ULONG32      Limit;                                                 
/*0x008*/     ULONG32      Peak;                                                  
/*0x00C*/     ULONG32      Return;                                                
          }EPROCESS_QUOTA_ENTRY, *PEPROCESS_QUOTA_ENTRY;                                   
                                                                                  
          typedef struct _EPROCESS_QUOTA_BLOCK            // 4 elements, 0x40 bytes (sizeof) 
          {                                                                                  
/*0x000*/     struct _EPROCESS_QUOTA_ENTRY QuotaEntry[3];                                    
/*0x030*/     struct _LIST_ENTRY QuotaList;               // 2 elements, 0x8 bytes (sizeof)  
/*0x038*/     ULONG32      ReferenceCount;                                                   
/*0x03C*/     ULONG32      ProcessCount;                                                     
          }EPROCESS_QUOTA_BLOCK, *PEPROCESS_QUOTA_BLOCK;                                     
                                                                                             
          typedef struct _KAPC_STATE             // 5 elements, 0x18 bytes (sizeof) 
          {                                                                         
/*0x000*/     struct _LIST_ENTRY ApcListHead[2];                                    
/*0x010*/     struct _KPROCESS* Process;                                            
/*0x014*/     UINT8        KernelApcInProgress;                                     
/*0x015*/     UINT8        KernelApcPending;                                        
/*0x016*/     UINT8        UserApcPending;                                          
/*0x017*/     UINT8        _PADDING0_[0x1];                                         
          }KAPC_STATE, *PKAPC_STATE;                                                
                                                                                    
          typedef struct _KWAIT_BLOCK             // 6 elements, 0x18 bytes (sizeof) 
          {                                                                          
/*0x000*/     struct _LIST_ENTRY WaitListEntry;   // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/     struct _KTHREAD* Thread;                                               
/*0x00C*/     VOID*        Object;                                                   
/*0x010*/     struct _KWAIT_BLOCK* NextWaitBlock;                                    
/*0x014*/     UINT16       WaitKey;                                                  
/*0x016*/     UINT16       WaitType;                                                 
          }KWAIT_BLOCK, *PKWAIT_BLOCK;                                               
                                                                                     
          typedef union _ULARGE_INTEGER  // 4 elements, 0x8 bytes (sizeof) 
          {                                                                
              struct                     // 2 elements, 0x8 bytes (sizeof) 
              {                                                            
/*0x000*/         ULONG32      LowPart;                                    
/*0x004*/         ULONG32      HighPart;                                   
              };                                                           
              struct                     // 2 elements, 0x8 bytes (sizeof) 
              {                                                            
/*0x000*/         ULONG32      LowPart;                                    
/*0x004*/         ULONG32      HighPart;                                   
              }u;                                                          
/*0x000*/     UINT64       QuadPart;                                       
          }ULARGE_INTEGER, *PULARGE_INTEGER;                                  
                                                                           
          typedef struct _KTIMER                 // 5 elements, 0x28 bytes (sizeof) 
          {                                                                         
/*0x000*/     struct _DISPATCHER_HEADER Header;  // 6 elements, 0x10 bytes (sizeof) 
/*0x010*/     union _ULARGE_INTEGER DueTime;     // 4 elements, 0x8 bytes (sizeof)  
/*0x018*/     struct _LIST_ENTRY TimerListEntry; // 2 elements, 0x8 bytes (sizeof)  
/*0x020*/     struct _KDPC* Dpc;                                                    
/*0x024*/     LONG32       Period;                                                  
          }KTIMER, *PKTIMER;                                                        
                                                                                    
          typedef struct _KAPC                                // 14 elements, 0x30 bytes (sizeof) 
          {                                                                                       
/*0x000*/     INT16        Type;                                                                  
/*0x002*/     INT16        Size;                                                                  
/*0x004*/     ULONG32      Spare0;                                                                
/*0x008*/     struct _KTHREAD* Thread;                                                            
/*0x00C*/     struct _LIST_ENTRY ApcListEntry;                // 2 elements, 0x8 bytes (sizeof)   
/*0x014*/     FUNCT_00BC_01A1_KernelRoutine* KernelRoutine;                                       
/*0x018*/     FUNCT_00BC_01B0_RundownRoutine* RundownRoutine;                                     
/*0x01C*/     FUNCT_00BC_01AA_NormalRoutine* NormalRoutine;                                       
/*0x020*/     VOID*        NormalContext;                                                         
/*0x024*/     VOID*        SystemArgument1;                                                       
/*0x028*/     VOID*        SystemArgument2;                                                       
/*0x02C*/     CHAR         ApcStateIndex;                                                         
/*0x02D*/     CHAR         ApcMode;                                                               
/*0x02E*/     UINT8        Inserted;                                                              
/*0x02F*/     UINT8        _PADDING0_[0x1];                                                       
          }KAPC, *PKAPC;                                                                          
                                                                                                  
          typedef struct _KSEMAPHORE            // 2 elements, 0x14 bytes (sizeof) 
          {                                                                        
/*0x000*/     struct _DISPATCHER_HEADER Header; // 6 elements, 0x10 bytes (sizeof) 
/*0x010*/     LONG32       Limit;                                                  
          }KSEMAPHORE, *PKSEMAPHORE;                                               
                                                                                   
          typedef struct _KTHREAD                          // 73 elements, 0x1C0 bytes (sizeof) 
          {                                                                                     
/*0x000*/     struct _DISPATCHER_HEADER Header;            // 6 elements, 0x10 bytes (sizeof)   
/*0x010*/     struct _LIST_ENTRY MutantListHead;           // 2 elements, 0x8 bytes (sizeof)    
/*0x018*/     VOID*        InitialStack;                                                        
/*0x01C*/     VOID*        StackLimit;                                                          
/*0x020*/     VOID*        Teb;                                                                 
/*0x024*/     VOID*        TlsArray;                                                            
/*0x028*/     VOID*        KernelStack;                                                         
/*0x02C*/     UINT8        DebugActive;                                                         
/*0x02D*/     UINT8        State;                                                               
/*0x02E*/     UINT8        Alerted[2];                                                          
/*0x030*/     UINT8        Iopl;                                                                
/*0x031*/     UINT8        NpxState;                                                            
/*0x032*/     CHAR         Saturation;                                                          
/*0x033*/     CHAR         Priority;                                                            
/*0x034*/     struct _KAPC_STATE ApcState;                 // 5 elements, 0x18 bytes (sizeof)   
/*0x04C*/     ULONG32      ContextSwitches;                                                     
/*0x050*/     UINT8        IdleSwapBlock;                                                       
/*0x051*/     UINT8        Spare0[3];                                                           
/*0x054*/     LONG32       WaitStatus;                                                          
/*0x058*/     UINT8        WaitIrql;                                                            
/*0x059*/     CHAR         WaitMode;                                                            
/*0x05A*/     UINT8        WaitNext;                                                            
/*0x05B*/     UINT8        WaitReason;                                                          
/*0x05C*/     struct _KWAIT_BLOCK* WaitBlockList;                                               
              union                                        // 2 elements, 0x8 bytes (sizeof)    
              {                                                                                 
/*0x060*/         struct _LIST_ENTRY WaitListEntry;        // 2 elements, 0x8 bytes (sizeof)    
/*0x060*/         struct _SINGLE_LIST_ENTRY SwapListEntry; // 1 elements, 0x4 bytes (sizeof)    
              };                                                                                
/*0x068*/     ULONG32      WaitTime;                                                            
/*0x06C*/     CHAR         BasePriority;                                                        
/*0x06D*/     UINT8        DecrementCount;                                                      
/*0x06E*/     CHAR         PriorityDecrement;                                                   
/*0x06F*/     CHAR         Quantum;                                                             
/*0x070*/     struct _KWAIT_BLOCK WaitBlock[4];                                                 
/*0x0D0*/     VOID*        LegoData;                                                            
/*0x0D4*/     ULONG32      KernelApcDisable;                                                    
/*0x0D8*/     ULONG32      UserAffinity;                                                        
/*0x0DC*/     UINT8        SystemAffinityActive;                                                
/*0x0DD*/     UINT8        PowerState;                                                          
/*0x0DE*/     UINT8        NpxIrql;                                                             
/*0x0DF*/     UINT8        InitialNode;                                                         
/*0x0E0*/     VOID*        ServiceTable;                                                        
/*0x0E4*/     struct _KQUEUE* Queue;                                                            
/*0x0E8*/     ULONG32      ApcQueueLock;                                                        
/*0x0EC*/     UINT8        _PADDING0_[0x4];                                                     
/*0x0F0*/     struct _KTIMER Timer;                        // 5 elements, 0x28 bytes (sizeof)   
/*0x118*/     struct _LIST_ENTRY QueueListEntry;           // 2 elements, 0x8 bytes (sizeof)    
/*0x120*/     ULONG32      SoftAffinity;                                                        
/*0x124*/     ULONG32      Affinity;                                                            
/*0x128*/     UINT8        Preempted;                                                           
/*0x129*/     UINT8        ProcessReadyQueue;                                                   
/*0x12A*/     UINT8        KernelStackResident;                                                 
/*0x12B*/     UINT8        NextProcessor;                                                       
/*0x12C*/     VOID*        CallbackStack;                                                       
/*0x130*/     VOID*        Win32Thread;                                                         
/*0x134*/     struct _KTRAP_FRAME* TrapFrame;                                                   
/*0x138*/     struct _KAPC_STATE* ApcStatePointer[2];                                           
/*0x140*/     CHAR         PreviousMode;                                                        
/*0x141*/     UINT8        EnableStackSwap;                                                     
/*0x142*/     UINT8        LargeStack;                                                          
/*0x143*/     UINT8        ResourceIndex;                                                       
/*0x144*/     ULONG32      KernelTime;                                                          
/*0x148*/     ULONG32      UserTime;                                                            
/*0x14C*/     struct _KAPC_STATE SavedApcState;            // 5 elements, 0x18 bytes (sizeof)   
/*0x164*/     UINT8        Alertable;                                                           
/*0x165*/     UINT8        ApcStateIndex;                                                       
/*0x166*/     UINT8        ApcQueueable;                                                        
/*0x167*/     UINT8        AutoAlignment;                                                       
/*0x168*/     VOID*        StackBase;                                                           
/*0x16C*/     struct _KAPC SuspendApc;                     // 14 elements, 0x30 bytes (sizeof)  
/*0x19C*/     struct _KSEMAPHORE SuspendSemaphore;         // 2 elements, 0x14 bytes (sizeof)   
/*0x1B0*/     struct _LIST_ENTRY ThreadListEntry;          // 2 elements, 0x8 bytes (sizeof)    
/*0x1B8*/     CHAR         FreezeCount;                                                         
/*0x1B9*/     CHAR         SuspendCount;                                                        
/*0x1BA*/     UINT8        IdealProcessor;                                                      
/*0x1BB*/     UINT8        DisableBoost;                                                        
/*0x1BC*/     UINT8        _PADDING1_[0x4];                                                     
          }KTHREAD, *PKTHREAD;                                                                  
                                                                                                
          typedef struct _ETHREAD                                      // 54 elements, 0x258 bytes (sizeof) 
          {                                                                                                 
/*0x000*/     struct _KTHREAD Tcb;                                     // 73 elements, 0x1C0 bytes (sizeof) 
              union                                                    // 2 elements, 0x8 bytes (sizeof)    
              {                                                                                             
/*0x1C0*/         union _LARGE_INTEGER CreateTime;                     // 4 elements, 0x8 bytes (sizeof)    
                  struct                                               // 2 elements, 0x4 bytes (sizeof)    
                  {                                                                                         
/*0x1C0*/             UINT32       NestedFaultCount : 2;               // 0 BitPosition                     
/*0x1C0*/             UINT32       ApcNeeded : 1;                      // 2 BitPosition                     
                  };                                                                                        
              };                                                                                            
              union                                                    // 3 elements, 0x8 bytes (sizeof)    
              {                                                                                             
/*0x1C8*/         union _LARGE_INTEGER ExitTime;                       // 4 elements, 0x8 bytes (sizeof)    
/*0x1C8*/         struct _LIST_ENTRY LpcReplyChain;                    // 2 elements, 0x8 bytes (sizeof)    
/*0x1C8*/         struct _LIST_ENTRY KeyedWaitChain;                   // 2 elements, 0x8 bytes (sizeof)    
              };                                                                                            
              union                                                    // 2 elements, 0x4 bytes (sizeof)    
              {                                                                                             
/*0x1D0*/         LONG32       ExitStatus;                                                                  
/*0x1D0*/         VOID*        OfsChain;                                                                    
              };                                                                                            
/*0x1D4*/     struct _LIST_ENTRY PostBlockList;                        // 2 elements, 0x8 bytes (sizeof)    
              union                                                    // 3 elements, 0x4 bytes (sizeof)    
              {                                                                                             
/*0x1DC*/         struct _TERMINATION_PORT* TerminationPort;                                                
/*0x1DC*/         struct _ETHREAD* ReaperLink;                                                              
/*0x1DC*/         VOID*        KeyedWaitValue;                                                              
              };                                                                                            
/*0x1E0*/     ULONG32      ActiveTimerListLock;                                                             
/*0x1E4*/     struct _LIST_ENTRY ActiveTimerListHead;                  // 2 elements, 0x8 bytes (sizeof)    
/*0x1EC*/     struct _CLIENT_ID Cid;                                   // 2 elements, 0x8 bytes (sizeof)    
              union                                                    // 2 elements, 0x14 bytes (sizeof)   
              {                                                                                             
/*0x1F4*/         struct _KSEMAPHORE LpcReplySemaphore;                // 2 elements, 0x14 bytes (sizeof)   
/*0x1F4*/         struct _KSEMAPHORE KeyedWaitSemaphore;               // 2 elements, 0x14 bytes (sizeof)   
              };                                                                                            
              union                                                    // 2 elements, 0x4 bytes (sizeof)    
              {                                                                                             
/*0x208*/         VOID*        LpcReplyMessage;                                                             
/*0x208*/         VOID*        LpcWaitingOnPort;                                                            
              };                                                                                            
/*0x20C*/     struct _PS_IMPERSONATION_INFORMATION* ImpersonationInfo;                                      
/*0x210*/     struct _LIST_ENTRY IrpList;                              // 2 elements, 0x8 bytes (sizeof)    
/*0x218*/     ULONG32      TopLevelIrp;                                                                     
/*0x21C*/     struct _DEVICE_OBJECT* DeviceToVerify;                                                        
/*0x220*/     struct _EPROCESS* ThreadsProcess;                                                             
/*0x224*/     VOID*        StartAddress;                                                                    
              union                                                    // 2 elements, 0x4 bytes (sizeof)    
              {                                                                                             
/*0x228*/         VOID*        Win32StartAddress;                                                           
/*0x228*/         ULONG32      LpcReceivedMessageId;                                                        
              };                                                                                            
/*0x22C*/     struct _LIST_ENTRY ThreadListEntry;                      // 2 elements, 0x8 bytes (sizeof)    
/*0x234*/     struct _EX_RUNDOWN_REF RundownProtect;                   // 2 elements, 0x4 bytes (sizeof)    
/*0x238*/     struct _EX_PUSH_LOCK ThreadLock;                         // 5 elements, 0x4 bytes (sizeof)    
/*0x23C*/     ULONG32      LpcReplyMessageId;                                                               
/*0x240*/     ULONG32      ReadClusterSize;                                                                 
/*0x244*/     ULONG32      GrantedAccess;                                                                   
              union                                                    // 2 elements, 0x4 bytes (sizeof)    
              {                                                                                             
/*0x248*/         ULONG32      CrossThreadFlags;                                                            
                  struct                                               // 9 elements, 0x4 bytes (sizeof)    
                  {                                                                                         
/*0x248*/             ULONG32      Terminated : 1;                     // 0 BitPosition                     
/*0x248*/             ULONG32      DeadThread : 1;                     // 1 BitPosition                     
/*0x248*/             ULONG32      HideFromDebugger : 1;               // 2 BitPosition                     
/*0x248*/             ULONG32      ActiveImpersonationInfo : 1;        // 3 BitPosition                     
/*0x248*/             ULONG32      SystemThread : 1;                   // 4 BitPosition                     
/*0x248*/             ULONG32      HardErrorsAreDisabled : 1;          // 5 BitPosition                     
/*0x248*/             ULONG32      BreakOnTermination : 1;             // 6 BitPosition                     
/*0x248*/             ULONG32      SkipCreationMsg : 1;                // 7 BitPosition                     
/*0x248*/             ULONG32      SkipTerminationMsg : 1;             // 8 BitPosition                     
                  };                                                                                        
              };                                                                                            
              union                                                    // 2 elements, 0x4 bytes (sizeof)    
              {                                                                                             
/*0x24C*/         ULONG32      SameThreadPassiveFlags;                                                      
                  struct                                               // 3 elements, 0x4 bytes (sizeof)    
                  {                                                                                         
/*0x24C*/             ULONG32      ActiveExWorker : 1;                 // 0 BitPosition                     
/*0x24C*/             ULONG32      ExWorkerCanWaitUser : 1;            // 1 BitPosition                     
/*0x24C*/             ULONG32      MemoryMaker : 1;                    // 2 BitPosition                     
                  };                                                                                        
              };                                                                                            
              union                                                    // 2 elements, 0x4 bytes (sizeof)    
              {                                                                                             
/*0x250*/         ULONG32      SameThreadApcFlags;                                                          
                  struct                                               // 3 elements, 0x1 bytes (sizeof)    
                  {                                                                                         
/*0x250*/             UINT8        LpcReceivedMsgIdValid : 1;          // 0 BitPosition                     
/*0x250*/             UINT8        LpcExitThreadCalled : 1;            // 1 BitPosition                     
/*0x250*/             UINT8        AddressSpaceOwner : 1;              // 2 BitPosition                     
                  };                                                                                        
              };                                                                                            
/*0x254*/     UINT8        ForwardClusterOnly;                                                              
/*0x255*/     UINT8        DisablePageFaultClustering;                                                      
/*0x256*/     UINT8        _PADDING0_[0x2];                                                                 
          }ETHREAD, *PETHREAD;                                                                              
                                                                                                            
          typedef struct _ETIMER                       // 9 elements, 0x98 bytes (sizeof)  
          {                                                                                
/*0x000*/     struct _KTIMER KeTimer;                  // 5 elements, 0x28 bytes (sizeof)  
/*0x028*/     struct _KAPC TimerApc;                   // 14 elements, 0x30 bytes (sizeof) 
/*0x058*/     struct _KDPC TimerDpc;                   // 9 elements, 0x20 bytes (sizeof)  
/*0x078*/     struct _LIST_ENTRY ActiveTimerListEntry; // 2 elements, 0x8 bytes (sizeof)   
/*0x080*/     ULONG32      Lock;                                                           
/*0x084*/     LONG32       Period;                                                         
/*0x088*/     UINT8        ApcAssociated;                                                  
/*0x089*/     UINT8        WakeTimer;                                                      
/*0x08A*/     UINT8        _PADDING0_[0x2];                                                
/*0x08C*/     struct _LIST_ENTRY WakeTimerListEntry;   // 2 elements, 0x8 bytes (sizeof)   
/*0x094*/     UINT8        _PADDING1_[0x4];                                                
          }ETIMER, *PETIMER;                                                               
                                                                                           
          typedef struct _EVENT_COUNTER            // 3 elements, 0x18 bytes (sizeof) 
          {                                                                           
/*0x000*/     struct _SINGLE_LIST_ENTRY ListEntry; // 1 elements, 0x4 bytes (sizeof)  
/*0x004*/     ULONG32      RefCount;                                                  
/*0x008*/     struct _KEVENT Event;                // 1 elements, 0x10 bytes (sizeof) 
          }EVENT_COUNTER, *PEVENT_COUNTER;                                            
                                                                                      
          typedef struct _EX_PUSH_LOCK_CACHE_AWARE // 1 elements, 0x80 bytes (sizeof) 
          {                                                                           
/*0x000*/     struct _EX_PUSH_LOCK* Locks[32];                                        
          }EX_PUSH_LOCK_CACHE_AWARE, *PEX_PUSH_LOCK_CACHE_AWARE;                                   
                                                                                      
          typedef struct _EX_PUSH_LOCK_WAIT_BLOCK    // 4 elements, 0x1C bytes (sizeof) 
          {                                                                             
/*0x000*/     struct _KEVENT WakeEvent;              // 1 elements, 0x10 bytes (sizeof) 
/*0x010*/     struct _EX_PUSH_LOCK_WAIT_BLOCK* Next;                                    
/*0x014*/     ULONG32      ShareCount;                                                  
/*0x018*/     UINT8        Exclusive;                                                   
/*0x019*/     UINT8        _PADDING0_[0x3];                                             
          }EX_PUSH_LOCK_WAIT_BLOCK, *PEX_PUSH_LOCK_WAIT_BLOCK;                                   
                                                                                        
          typedef union _EX_QUEUE_WORKER_INFO              // 5 elements, 0x4 bytes (sizeof) 
          {                                                                                  
              struct                                       // 4 elements, 0x4 bytes (sizeof) 
              {                                                                              
/*0x000*/         ULONG32      QueueDisabled : 1;          // 0 BitPosition                  
/*0x000*/         ULONG32      MakeThreadsAsNecessary : 1; // 1 BitPosition                  
/*0x000*/         ULONG32      WaitMode : 1;               // 2 BitPosition                  
/*0x000*/         ULONG32      WorkerCount : 29;           // 3 BitPosition                  
              };                                                                             
/*0x000*/     LONG32       QueueWorkerInfo;                                                  
          }EX_QUEUE_WORKER_INFO, *PEX_QUEUE_WORKER_INFO;                                     
                                                                                             
          typedef struct _KQUEUE                 // 5 elements, 0x28 bytes (sizeof) 
          {                                                                         
/*0x000*/     struct _DISPATCHER_HEADER Header;  // 6 elements, 0x10 bytes (sizeof) 
/*0x010*/     struct _LIST_ENTRY EntryListHead;  // 2 elements, 0x8 bytes (sizeof)  
/*0x018*/     ULONG32      CurrentCount;                                            
/*0x01C*/     ULONG32      MaximumCount;                                            
/*0x020*/     struct _LIST_ENTRY ThreadListHead; // 2 elements, 0x8 bytes (sizeof)  
          }KQUEUE, *PKQUEUE;                                                        
                                                                                    
          typedef struct _EX_WORK_QUEUE                // 6 elements, 0x3C bytes (sizeof) 
          {                                                                               
/*0x000*/     struct _KQUEUE WorkerQueue;              // 5 elements, 0x28 bytes (sizeof) 
/*0x028*/     ULONG32      DynamicThreadCount;                                            
/*0x02C*/     ULONG32      WorkItemsProcessed;                                            
/*0x030*/     ULONG32      WorkItemsProcessedLastPass;                                    
/*0x034*/     ULONG32      QueueDepthLastPass;                                            
/*0x038*/     union _EX_QUEUE_WORKER_INFO Info;        // 5 elements, 0x4 bytes (sizeof)  
          }EX_WORK_QUEUE, *PEX_WORK_QUEUE;                                                
                                                                                          
          typedef struct _EXCEPTION_POINTERS             // 2 elements, 0x8 bytes (sizeof) 
          {                                                                                
/*0x000*/     struct _EXCEPTION_RECORD* ExceptionRecord;                                   
/*0x004*/     struct _CONTEXT* ContextRecord;                                              
          }EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;                                       
                                                                                           
          typedef struct _EXCEPTION_RECORD               // 6 elements, 0x50 bytes (sizeof) 
          {                                                                                 
/*0x000*/     LONG32       ExceptionCode;                                                   
/*0x004*/     ULONG32      ExceptionFlags;                                                  
/*0x008*/     struct _EXCEPTION_RECORD* ExceptionRecord;                                    
/*0x00C*/     VOID*        ExceptionAddress;                                                
/*0x010*/     ULONG32      NumberParameters;                                                
/*0x014*/     ULONG32      ExceptionInformation[15];                                        
          }EXCEPTION_RECORD, *PEXCEPTION_RECORD;                                            
                                                                                            
          typedef struct _EXCEPTION_REGISTRATION_RECORD    // 2 elements, 0x8 bytes (sizeof) 
          {                                                                                  
/*0x000*/     struct _EXCEPTION_REGISTRATION_RECORD* Next;                                   
/*0x004*/     FUNCT_0161_0160_Handler* Handler;                                              
          }EXCEPTION_REGISTRATION_RECORD, *PEXCEPTION_REGISTRATION_RECORD;                                  
                                                                                             
          typedef struct _FAST_IO_DISPATCH                                                                                // 28 elements, 0x70 bytes (sizeof) 
          {                                                                                                                                                   
/*0x000*/     ULONG32      SizeOfFastIoDispatch;                                                                                                              
/*0x004*/     FUNCT_003D_0469_FastIoCheckIfPossible* FastIoCheckIfPossible;                                                                                   
/*0x008*/     FUNCT_003D_0478_FastIoRead_FastIoWrite* FastIoRead;                                                                                             
/*0x00C*/     FUNCT_003D_0478_FastIoRead_FastIoWrite* FastIoWrite;                                                                                            
/*0x010*/     FUNCT_003D_0482_FastIoQueryBasicInfo* FastIoQueryBasicInfo;                                                                                     
/*0x014*/     FUNCT_003D_0490_FastIoQueryStandardInfo* FastIoQueryStandardInfo;                                                                               
/*0x018*/     FUNCT_003D_049E_FastIoLock* FastIoLock;                                                                                                         
/*0x01C*/     FUNCT_003D_067B_FastIoUnlockSingle* FastIoUnlockSingle;                                                                                         
/*0x020*/     FUNCT_003D_0684_FastIoUnlockAll* FastIoUnlockAll;                                                                                               
/*0x024*/     FUNCT_003D_068A_FastIoUnlockAllByKey* FastIoUnlockAllByKey;                                                                                     
/*0x028*/     FUNCT_003D_0691_FastIoDeviceControl* FastIoDeviceControl;                                                                                       
/*0x02C*/     FUNCT_00BC_069C_AcquireFileForNtCreateSection_ReleaseFileForNtCreateSection* AcquireFileForNtCreateSection;                                     
/*0x030*/     FUNCT_00BC_069C_AcquireFileForNtCreateSection_ReleaseFileForNtCreateSection* ReleaseFileForNtCreateSection;                                     
/*0x034*/     FUNCT_00BC_069F_FastIoDetachDevice* FastIoDetachDevice;                                                                                         
/*0x038*/     FUNCT_003D_06A3_FastIoQueryNetworkOpenInfo* FastIoQueryNetworkOpenInfo;                                                                         
/*0x03C*/     FUNCT_0049_06B3_AcquireForModWrite* AcquireForModWrite;                                                                                         
/*0x040*/     FUNCT_003D_06B9_MdlRead_PrepareMdlWrite* MdlRead;                                                                                               
/*0x044*/     FUNCT_003D_06CD_MdlReadComplete_MdlReadCompleteCompressed* MdlReadComplete;                                                                     
/*0x048*/     FUNCT_003D_06B9_MdlRead_PrepareMdlWrite* PrepareMdlWrite;                                                                                       
/*0x04C*/     FUNCT_003D_06D2_MdlWriteComplete_MdlWriteCompleteCompressed* MdlWriteComplete;                                                                  
/*0x050*/     FUNCT_003D_06D8_FastIoReadCompressed_FastIoWriteCompressed* FastIoReadCompressed;                                                               
/*0x054*/     FUNCT_003D_06D8_FastIoReadCompressed_FastIoWriteCompressed* FastIoWriteCompressed;                                                              
/*0x058*/     FUNCT_003D_06CD_MdlReadComplete_MdlReadCompleteCompressed* MdlReadCompleteCompressed;                                                           
/*0x05C*/     FUNCT_003D_06D2_MdlWriteComplete_MdlWriteCompleteCompressed* MdlWriteCompleteCompressed;                                                        
/*0x060*/     FUNCT_003D_06ED_FastIoQueryOpen* FastIoQueryOpen;                                                                                               
/*0x064*/     FUNCT_0049_094D_ReleaseForModWrite* ReleaseForModWrite;                                                                                         
/*0x068*/     FUNCT_0049_0952_AcquireForCcFlush_ReleaseForCcFlush* AcquireForCcFlush;                                                                         
/*0x06C*/     FUNCT_0049_0952_AcquireForCcFlush_ReleaseForCcFlush* ReleaseForCcFlush;                                                                         
          }FAST_IO_DISPATCH, *PFAST_IO_DISPATCH;                                                                                                              
                                                                                                                                                              
          typedef struct _FILE_BASIC_INFORMATION   // 5 elements, 0x28 bytes (sizeof) 
          {                                                                           
/*0x000*/     union _LARGE_INTEGER CreationTime;   // 4 elements, 0x8 bytes (sizeof)  
/*0x008*/     union _LARGE_INTEGER LastAccessTime; // 4 elements, 0x8 bytes (sizeof)  
/*0x010*/     union _LARGE_INTEGER LastWriteTime;  // 4 elements, 0x8 bytes (sizeof)  
/*0x018*/     union _LARGE_INTEGER ChangeTime;     // 4 elements, 0x8 bytes (sizeof)  
/*0x020*/     ULONG32      FileAttributes;                                            
/*0x024*/     UINT8        _PADDING0_[0x4];                                           
          }FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;                                   
                                                                                      
          typedef struct _SID_IDENTIFIER_AUTHORITY // 1 elements, 0x6 bytes (sizeof) 
          {                                                                          
/*0x000*/     UINT8        Value[6];                                                 
          }SID_IDENTIFIER_AUTHORITY, *PSID_IDENTIFIER_AUTHORITY;                                  
                                                                                     
          typedef struct _SID                                       // 4 elements, 0xC bytes (sizeof) 
          {                                                                                           
/*0x000*/     UINT8        Revision;                                                                  
/*0x001*/     UINT8        SubAuthorityCount;                                                         
/*0x002*/     struct _SID_IDENTIFIER_AUTHORITY IdentifierAuthority; // 1 elements, 0x6 bytes (sizeof) 
/*0x008*/     ULONG32      SubAuthority[1];                                                           
          }SID, *PSID;                                                                                
                                                                                                      
          typedef struct _FILE_GET_QUOTA_INFORMATION // 3 elements, 0x14 bytes (sizeof) 
          {                                                                             
/*0x000*/     ULONG32      NextEntryOffset;                                             
/*0x004*/     ULONG32      SidLength;                                                   
/*0x008*/     struct _SID  Sid;                      // 4 elements, 0xC bytes (sizeof)  
          }FILE_GET_QUOTA_INFORMATION, *PFILE_GET_QUOTA_INFORMATION;                                   
                                                                                        
          typedef struct _FILE_NETWORK_OPEN_INFORMATION // 7 elements, 0x38 bytes (sizeof) 
          {                                                                                
/*0x000*/     union _LARGE_INTEGER CreationTime;        // 4 elements, 0x8 bytes (sizeof)  
/*0x008*/     union _LARGE_INTEGER LastAccessTime;      // 4 elements, 0x8 bytes (sizeof)  
/*0x010*/     union _LARGE_INTEGER LastWriteTime;       // 4 elements, 0x8 bytes (sizeof)  
/*0x018*/     union _LARGE_INTEGER ChangeTime;          // 4 elements, 0x8 bytes (sizeof)  
/*0x020*/     union _LARGE_INTEGER AllocationSize;      // 4 elements, 0x8 bytes (sizeof)  
/*0x028*/     union _LARGE_INTEGER EndOfFile;           // 4 elements, 0x8 bytes (sizeof)  
/*0x030*/     ULONG32      FileAttributes;                                                 
/*0x034*/     UINT8        _PADDING0_[0x4];                                                
          }FILE_NETWORK_OPEN_INFORMATION, *PFILE_NETWORK_OPEN_INFORMATION;                                   
                                                                                           
          typedef struct _FILE_OBJECT                                // 27 elements, 0x70 bytes (sizeof) 
          {                                                                                              
/*0x000*/     INT16        Type;                                                                         
/*0x002*/     INT16        Size;                                                                         
/*0x004*/     struct _DEVICE_OBJECT* DeviceObject;                                                       
/*0x008*/     struct _VPB* Vpb;                                                                          
/*0x00C*/     VOID*        FsContext;                                                                    
/*0x010*/     VOID*        FsContext2;                                                                   
/*0x014*/     struct _SECTION_OBJECT_POINTERS* SectionObjectPointer;                                     
/*0x018*/     VOID*        PrivateCacheMap;                                                              
/*0x01C*/     LONG32       FinalStatus;                                                                  
/*0x020*/     struct _FILE_OBJECT* RelatedFileObject;                                                    
/*0x024*/     UINT8        LockOperation;                                                                
/*0x025*/     UINT8        DeletePending;                                                                
/*0x026*/     UINT8        ReadAccess;                                                                   
/*0x027*/     UINT8        WriteAccess;                                                                  
/*0x028*/     UINT8        DeleteAccess;                                                                 
/*0x029*/     UINT8        SharedRead;                                                                   
/*0x02A*/     UINT8        SharedWrite;                                                                  
/*0x02B*/     UINT8        SharedDelete;                                                                 
/*0x02C*/     ULONG32      Flags;                                                                        
/*0x030*/     struct _UNICODE_STRING FileName;                       // 3 elements, 0x8 bytes (sizeof)   
/*0x038*/     union _LARGE_INTEGER CurrentByteOffset;                // 4 elements, 0x8 bytes (sizeof)   
/*0x040*/     ULONG32      Waiters;                                                                      
/*0x044*/     ULONG32      Busy;                                                                         
/*0x048*/     VOID*        LastLock;                                                                     
/*0x04C*/     struct _KEVENT Lock;                                   // 1 elements, 0x10 bytes (sizeof)  
/*0x05C*/     struct _KEVENT Event;                                  // 1 elements, 0x10 bytes (sizeof)  
/*0x06C*/     struct _IO_COMPLETION_CONTEXT* CompletionContext;                                          
          }FILE_OBJECT, *PFILE_OBJECT;                                                                   
                                                                                                         
          typedef struct _FILE_STANDARD_INFORMATION // 5 elements, 0x18 bytes (sizeof) 
          {                                                                            
/*0x000*/     union _LARGE_INTEGER AllocationSize;  // 4 elements, 0x8 bytes (sizeof)  
/*0x008*/     union _LARGE_INTEGER EndOfFile;       // 4 elements, 0x8 bytes (sizeof)  
/*0x010*/     ULONG32      NumberOfLinks;                                              
/*0x014*/     UINT8        DeletePending;                                              
/*0x015*/     UINT8        Directory;                                                  
/*0x016*/     UINT8        _PADDING0_[0x2];                                            
          }FILE_STANDARD_INFORMATION, *PFILE_STANDARD_INFORMATION;                                   
                                                                                       
          typedef struct _flags       // 1 elements, 0x1 bytes (sizeof) 
          {                                                             
/*0x000*/     UINT8        Removable;                                   
          }flags, *Pflags;                                              
                                                                        
          typedef struct _FNSAVE_FORMAT      // 8 elements, 0x6C bytes (sizeof) 
          {                                                                     
/*0x000*/     ULONG32      ControlWord;                                         
/*0x004*/     ULONG32      StatusWord;                                          
/*0x008*/     ULONG32      TagWord;                                             
/*0x00C*/     ULONG32      ErrorOffset;                                         
/*0x010*/     ULONG32      ErrorSelector;                                       
/*0x014*/     ULONG32      DataOffset;                                          
/*0x018*/     ULONG32      DataSelector;                                        
/*0x01C*/     UINT8        RegisterArea[80];                                    
          }FNSAVE_FORMAT, *PFNSAVE_FORMAT;                                      
                                                                                
          typedef union _FS_FILTER_PARAMETERS                 // 4 elements, 0x14 bytes (sizeof) 
          {                                                                                      
              struct                                          // 2 elements, 0x8 bytes (sizeof)  
              {                                                                                  
/*0x000*/         union _LARGE_INTEGER* EndingOffset;                                            
/*0x004*/         struct _ERESOURCE** ResourceToRelease;                                         
              }AcquireForModifiedPageWriter;                                                     
              struct                                          // 1 elements, 0x4 bytes (sizeof)  
              {                                                                                  
/*0x000*/         struct _ERESOURCE* ResourceToRelease;                                          
              }ReleaseForModifiedPageWriter;                                                     
              struct                                          // 2 elements, 0x8 bytes (sizeof)  
              {                                                                                  
/*0x000*/         enum _FS_FILTER_SECTION_SYNC_TYPE SyncType;                                    
/*0x004*/         ULONG32      PageProtection;                                                   
              }AcquireForSectionSynchronization;                                                 
              struct                                          // 5 elements, 0x14 bytes (sizeof) 
              {                                                                                  
/*0x000*/         VOID*        Argument1;                                                        
/*0x004*/         VOID*        Argument2;                                                        
/*0x008*/         VOID*        Argument3;                                                        
/*0x00C*/         VOID*        Argument4;                                                        
/*0x010*/         VOID*        Argument5;                                                        
              }Others;                                                                           
          }FS_FILTER_PARAMETERS, *PFS_FILTER_PARAMETERS;                                         
                                                                                                 
          typedef struct _FS_FILTER_CALLBACK_DATA      // 6 elements, 0x24 bytes (sizeof) 
          {                                                                               
/*0x000*/     ULONG32      SizeOfFsFilterCallbackData;                                    
/*0x004*/     UINT8        Operation;                                                     
/*0x005*/     UINT8        Reserved;                                                      
/*0x006*/     UINT8        _PADDING0_[0x2];                                               
/*0x008*/     struct _DEVICE_OBJECT* DeviceObject;                                        
/*0x00C*/     struct _FILE_OBJECT* FileObject;                                            
/*0x010*/     union _FS_FILTER_PARAMETERS Parameters;  // 4 elements, 0x14 bytes (sizeof) 
          }FS_FILTER_CALLBACK_DATA, *PFS_FILTER_CALLBACK_DATA;                                   
                                                                                          
          typedef struct _FS_FILTER_CALLBACKS                                                                                                                                                                                                                // 14 elements, 0x38 bytes (sizeof) 
          {                                                                                                                                                                                                                                                                                      
/*0x000*/     ULONG32      SizeOfFsFilterCallbacks;                                                                                                                                                                                                                                              
/*0x004*/     ULONG32      Reserved;                                                                                                                                                                                                                                                             
/*0x008*/     FUNCT_0049_03D8_PreAcquireForSectionSynchronization_PreReleaseForSectionSynchronization_PreAcquireForCcFlush_PreReleaseForCcFlush_PreAcquireForModifiedPageWriter_PreReleaseForModifiedPageWriter* PreAcquireForSectionSynchronization;                                            
/*0x00C*/     FUNCT_00BC_0445_PostAcquireForSectionSynchronization_PostReleaseForSectionSynchronization_PostAcquireForCcFlush_PostReleaseForCcFlush_PostAcquireForModifiedPageWriter_PostReleaseForModifiedPageWriter* PostAcquireForSectionSynchronization;                                     
/*0x010*/     FUNCT_0049_03D8_PreAcquireForSectionSynchronization_PreReleaseForSectionSynchronization_PreAcquireForCcFlush_PreReleaseForCcFlush_PreAcquireForModifiedPageWriter_PreReleaseForModifiedPageWriter* PreReleaseForSectionSynchronization;                                            
/*0x014*/     FUNCT_00BC_0445_PostAcquireForSectionSynchronization_PostReleaseForSectionSynchronization_PostAcquireForCcFlush_PostReleaseForCcFlush_PostAcquireForModifiedPageWriter_PostReleaseForModifiedPageWriter* PostReleaseForSectionSynchronization;                                     
/*0x018*/     FUNCT_0049_03D8_PreAcquireForSectionSynchronization_PreReleaseForSectionSynchronization_PreAcquireForCcFlush_PreReleaseForCcFlush_PreAcquireForModifiedPageWriter_PreReleaseForModifiedPageWriter* PreAcquireForCcFlush;                                                           
/*0x01C*/     FUNCT_00BC_0445_PostAcquireForSectionSynchronization_PostReleaseForSectionSynchronization_PostAcquireForCcFlush_PostReleaseForCcFlush_PostAcquireForModifiedPageWriter_PostReleaseForModifiedPageWriter* PostAcquireForCcFlush;                                                    
/*0x020*/     FUNCT_0049_03D8_PreAcquireForSectionSynchronization_PreReleaseForSectionSynchronization_PreAcquireForCcFlush_PreReleaseForCcFlush_PreAcquireForModifiedPageWriter_PreReleaseForModifiedPageWriter* PreReleaseForCcFlush;                                                           
/*0x024*/     FUNCT_00BC_0445_PostAcquireForSectionSynchronization_PostReleaseForSectionSynchronization_PostAcquireForCcFlush_PostReleaseForCcFlush_PostAcquireForModifiedPageWriter_PostReleaseForModifiedPageWriter* PostReleaseForCcFlush;                                                    
/*0x028*/     FUNCT_0049_03D8_PreAcquireForSectionSynchronization_PreReleaseForSectionSynchronization_PreAcquireForCcFlush_PreReleaseForCcFlush_PreAcquireForModifiedPageWriter_PreReleaseForModifiedPageWriter* PreAcquireForModifiedPageWriter;                                                
/*0x02C*/     FUNCT_00BC_0445_PostAcquireForSectionSynchronization_PostReleaseForSectionSynchronization_PostAcquireForCcFlush_PostReleaseForCcFlush_PostAcquireForModifiedPageWriter_PostReleaseForModifiedPageWriter* PostAcquireForModifiedPageWriter;                                         
/*0x030*/     FUNCT_0049_03D8_PreAcquireForSectionSynchronization_PreReleaseForSectionSynchronization_PreAcquireForCcFlush_PreReleaseForCcFlush_PreAcquireForModifiedPageWriter_PreReleaseForModifiedPageWriter* PreReleaseForModifiedPageWriter;                                                
/*0x034*/     FUNCT_00BC_0445_PostAcquireForSectionSynchronization_PostReleaseForSectionSynchronization_PostAcquireForCcFlush_PostReleaseForCcFlush_PostAcquireForModifiedPageWriter_PostReleaseForModifiedPageWriter* PostReleaseForModifiedPageWriter;                                         
          }FS_FILTER_CALLBACKS, *PFS_FILTER_CALLBACKS;                                                                                                                                                                                                                                           
                                                                                                                                                                                                                                                                                                 
          typedef struct _FXSAVE_FORMAT       // 14 elements, 0x208 bytes (sizeof) 
          {                                                                        
/*0x000*/     UINT16       ControlWord;                                            
/*0x002*/     UINT16       StatusWord;                                             
/*0x004*/     UINT16       TagWord;                                                
/*0x006*/     UINT16       ErrorOpcode;                                            
/*0x008*/     ULONG32      ErrorOffset;                                            
/*0x00C*/     ULONG32      ErrorSelector;                                          
/*0x010*/     ULONG32      DataOffset;                                             
/*0x014*/     ULONG32      DataSelector;                                           
/*0x018*/     ULONG32      MXCsr;                                                  
/*0x01C*/     ULONG32      MXCsrMask;                                              
/*0x020*/     UINT8        RegisterArea[128];                                      
/*0x0A0*/     UINT8        Reserved3[128];                                         
/*0x120*/     UINT8        Reserved4[224];                                         
/*0x200*/     UINT8        Align16Byte[8];                                         
          }FXSAVE_FORMAT, *PFXSAVE_FORMAT;                                         
                                                                                   
          typedef struct _FX_SAVE_AREA          // 3 elements, 0x210 bytes (sizeof)  
          {                                                                          
              union                             // 2 elements, 0x208 bytes (sizeof)  
              {                                                                      
/*0x000*/         struct _FNSAVE_FORMAT FnArea; // 8 elements, 0x6C bytes (sizeof)   
/*0x000*/         struct _FXSAVE_FORMAT FxArea; // 14 elements, 0x208 bytes (sizeof) 
              }U;                                                                    
/*0x208*/     ULONG32      NpxSavedCpu;                                              
/*0x20C*/     ULONG32      Cr0NpxState;                                              
          }FX_SAVE_AREA, *PFX_SAVE_AREA;                                             
                                                                                     
          typedef struct _GDI_TEB_BATCH // 3 elements, 0x4E0 bytes (sizeof) 
          {                                                                 
/*0x000*/     ULONG32      Offset;                                          
/*0x004*/     ULONG32      HDC;                                             
/*0x008*/     ULONG32      Buffer[310];                                     
          }GDI_TEB_BATCH, *PGDI_TEB_BATCH;                                    
                                                                            
          typedef union _SLIST_HEADER             // 4 elements, 0x8 bytes (sizeof) 
          {                                                                         
/*0x000*/     UINT64       Alignment;                                               
              struct                              // 3 elements, 0x8 bytes (sizeof) 
              {                                                                     
/*0x000*/         struct _SINGLE_LIST_ENTRY Next; // 1 elements, 0x4 bytes (sizeof) 
/*0x004*/         UINT16       Depth;                                               
/*0x006*/         UINT16       Sequence;                                            
              };                                                                    
          }SLIST_HEADER, *PSLIST_HEADER;                                            
                                                                                    
          typedef struct _GENERAL_LOOKASIDE                                                                                                                                   // 19 elements, 0x80 bytes (sizeof) 
          {                                                                                                                                                                                                       
/*0x000*/     union _SLIST_HEADER ListHead;                                                                                                                                   // 4 elements, 0x8 bytes (sizeof)   
/*0x008*/     UINT16       Depth;                                                                                                                                                                                 
/*0x00A*/     UINT16       MaximumDepth;                                                                                                                                                                          
/*0x00C*/     ULONG32      TotalAllocates;                                                                                                                                                                        
              union                                                                                                                                                           // 2 elements, 0x4 bytes (sizeof)   
              {                                                                                                                                                                                                   
/*0x010*/         ULONG32      AllocateMisses;                                                                                                                                                                    
/*0x010*/         ULONG32      AllocateHits;                                                                                                                                                                      
              };                                                                                                                                                                                                  
/*0x014*/     ULONG32      TotalFrees;                                                                                                                                                                            
              union                                                                                                                                                           // 2 elements, 0x4 bytes (sizeof)   
              {                                                                                                                                                                                                   
/*0x018*/         ULONG32      FreeMisses;                                                                                                                                                                        
/*0x018*/         ULONG32      FreeHits;                                                                                                                                                                          
              };                                                                                                                                                                                                  
/*0x01C*/     enum _POOL_TYPE Type;                                                                                                                                                                               
/*0x020*/     ULONG32      Tag;                                                                                                                                                                                   
/*0x024*/     ULONG32      Size;                                                                                                                                                                                  
/*0x028*/     FUNCT_00BB_0280_Allocate* Allocate;                                                                                                                                                                 
/*0x02C*/     FUNCT_00BC_0285_Free_InterfaceReference_InterfaceDereference_DeleteProcedure_ReleaseFromLazyWrite_ReleaseFromReadAhead_WorkerRoutine_Destructor_Callback* Free;                                     
/*0x030*/     struct _LIST_ENTRY ListEntry;                                                                                                                                   // 2 elements, 0x8 bytes (sizeof)   
/*0x038*/     ULONG32      LastTotalAllocates;                                                                                                                                                                    
              union                                                                                                                                                           // 2 elements, 0x4 bytes (sizeof)   
              {                                                                                                                                                                                                   
/*0x03C*/         ULONG32      LastAllocateMisses;                                                                                                                                                                
/*0x03C*/         ULONG32      LastAllocateHits;                                                                                                                                                                  
              };                                                                                                                                                                                                  
/*0x040*/     ULONG32      Future[2];                                                                                                                                                                             
/*0x048*/     UINT8        _PADDING0_[0x38];                                                                                                                                                                      
          }GENERAL_LOOKASIDE, *PGENERAL_LOOKASIDE;                                                                                                                                                                
                                                                                                                                                                                                                  
          typedef struct _GENERIC_MAPPING  // 4 elements, 0x10 bytes (sizeof) 
          {                                                                   
/*0x000*/     ULONG32      GenericRead;                                       
/*0x004*/     ULONG32      GenericWrite;                                      
/*0x008*/     ULONG32      GenericExecute;                                    
/*0x00C*/     ULONG32      GenericAll;                                        
          }GENERIC_MAPPING, *PGENERIC_MAPPING;                                   
                                                                              
          typedef struct _HANDLE_TABLE                    // 14 elements, 0x44 bytes (sizeof) 
          {                                                                                   
/*0x000*/     ULONG32      TableCode;                                                         
/*0x004*/     struct _EPROCESS* QuotaProcess;                                                 
/*0x008*/     VOID*        UniqueProcessId;                                                   
/*0x00C*/     struct _EX_PUSH_LOCK HandleTableLock[4];                                        
/*0x01C*/     struct _LIST_ENTRY HandleTableList;         // 2 elements, 0x8 bytes (sizeof)   
/*0x024*/     struct _EX_PUSH_LOCK HandleContentionEvent; // 5 elements, 0x4 bytes (sizeof)   
/*0x028*/     struct _HANDLE_TRACE_DEBUG_INFO* DebugInfo;                                     
/*0x02C*/     LONG32       ExtraInfoPages;                                                    
/*0x030*/     ULONG32      FirstFree;                                                         
/*0x034*/     ULONG32      LastFree;                                                          
/*0x038*/     ULONG32      NextHandleNeedingPool;                                             
/*0x03C*/     LONG32       HandleCount;                                                       
              union                                       // 2 elements, 0x4 bytes (sizeof)   
              {                                                                               
/*0x040*/         ULONG32      Flags;                                                         
/*0x040*/         UINT8        StrictFIFO : 1;            // 0 BitPosition                    
              };                                                                              
          }HANDLE_TABLE, *PHANDLE_TABLE;                                                      
                                                                                              
          typedef struct _HANDLE_TABLE_ENTRY                  // 8 elements, 0x8 bytes (sizeof) 
          {                                                                                     
              union                                           // 4 elements, 0x4 bytes (sizeof) 
              {                                                                                 
/*0x000*/         VOID*        Object;                                                          
/*0x000*/         ULONG32      ObAttributes;                                                    
/*0x000*/         struct _HANDLE_TABLE_ENTRY_INFO* InfoTable;                                   
/*0x000*/         ULONG32      Value;                                                           
              };                                                                                
              union                                           // 3 elements, 0x4 bytes (sizeof) 
              {                                                                                 
/*0x004*/         ULONG32      GrantedAccess;                                                   
                  struct                                      // 2 elements, 0x4 bytes (sizeof) 
                  {                                                                             
/*0x004*/             UINT16       GrantedAccessIndex;                                          
/*0x006*/             UINT16       CreatorBackTraceIndex;                                       
                  };                                                                            
/*0x004*/         LONG32       NextFreeTableEntry;                                              
              };                                                                                
          }HANDLE_TABLE_ENTRY, *PHANDLE_TABLE_ENTRY;                                            
                                                                                                
          typedef struct _HANDLE_TABLE_ENTRY_INFO // 1 elements, 0x4 bytes (sizeof) 
          {                                                                         
/*0x000*/     ULONG32      AuditMask;                                               
          }HANDLE_TABLE_ENTRY_INFO, *PHANDLE_TABLE_ENTRY_INFO;                                  
                                                                                    
          typedef struct _HANDLE_TRACE_DB_ENTRY // 4 elements, 0x50 bytes (sizeof) 
          {                                                                        
/*0x000*/     struct _CLIENT_ID ClientId;       // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/     VOID*        Handle;                                                 
/*0x00C*/     ULONG32      Type;                                                   
/*0x010*/     VOID*        StackTrace[16];                                         
          }HANDLE_TRACE_DB_ENTRY, *PHANDLE_TRACE_DB_ENTRY;                                   
                                                                                   
          typedef struct _HANDLE_TRACE_DEBUG_INFO          // 2 elements, 0x50004 bytes (sizeof) 
          {                                                                                      
/*0x000*/     ULONG32      CurrentStackIndex;                                                    
/*0x004*/     struct _HANDLE_TRACE_DB_ENTRY TraceDb[4096];                                       
          }HANDLE_TRACE_DEBUG_INFO, *PHANDLE_TRACE_DEBUG_INFO;                                      
                                                                                                 
          typedef struct _HBASE_BLOCK         // 17 elements, 0x1000 bytes (sizeof) 
          {                                                                         
/*0x000*/     ULONG32      Signature;                                               
/*0x004*/     ULONG32      Sequence1;                                               
/*0x008*/     ULONG32      Sequence2;                                               
/*0x00C*/     union _LARGE_INTEGER TimeStamp; // 4 elements, 0x8 bytes (sizeof)     
/*0x014*/     ULONG32      Major;                                                   
/*0x018*/     ULONG32      Minor;                                                   
/*0x01C*/     ULONG32      Type;                                                    
/*0x020*/     ULONG32      Format;                                                  
/*0x024*/     ULONG32      RootCell;                                                
/*0x028*/     ULONG32      Length;                                                  
/*0x02C*/     ULONG32      Cluster;                                                 
/*0x030*/     UINT8        FileName[64];                                            
/*0x070*/     ULONG32      Reserved1[99];                                           
/*0x1FC*/     ULONG32      CheckSum;                                                
/*0x200*/     ULONG32      Reserved2[894];                                          
/*0xFF8*/     ULONG32      BootType;                                                
/*0xFFC*/     ULONG32      BootRecover;                                             
          }HBASE_BLOCK, *PHBASE_BLOCK;                                              
                                                                                    
          typedef struct _HEAP_ENTRY             // 7 elements, 0x8 bytes (sizeof) 
          {                                                                        
              union                              // 2 elements, 0x4 bytes (sizeof) 
              {                                                                    
                  struct                         // 2 elements, 0x4 bytes (sizeof) 
                  {                                                                
/*0x000*/             UINT16       Size;                                           
/*0x002*/             UINT16       PreviousSize;                                   
                  };                                                               
/*0x000*/         VOID*        SubSegmentCode;                                     
              };                                                                   
/*0x004*/     UINT8        SmallTagIndex;                                          
/*0x005*/     UINT8        Flags;                                                  
/*0x006*/     UINT8        UnusedBytes;                                            
/*0x007*/     UINT8        SegmentIndex;                                           
          }HEAP_ENTRY, *PHEAP_ENTRY;                                               
                                                                                   
          typedef struct _HEAP                                         // 36 elements, 0x588 bytes (sizeof) 
          {                                                                                                 
/*0x000*/     struct _HEAP_ENTRY Entry;                                // 7 elements, 0x8 bytes (sizeof)    
/*0x008*/     ULONG32      Signature;                                                                       
/*0x00C*/     ULONG32      Flags;                                                                           
/*0x010*/     ULONG32      ForceFlags;                                                                      
/*0x014*/     ULONG32      VirtualMemoryThreshold;                                                          
/*0x018*/     ULONG32      SegmentReserve;                                                                  
/*0x01C*/     ULONG32      SegmentCommit;                                                                   
/*0x020*/     ULONG32      DeCommitFreeBlockThreshold;                                                      
/*0x024*/     ULONG32      DeCommitTotalFreeThreshold;                                                      
/*0x028*/     ULONG32      TotalFreeSize;                                                                   
/*0x02C*/     ULONG32      MaximumAllocationSize;                                                           
/*0x030*/     UINT16       ProcessHeapsListIndex;                                                           
/*0x032*/     UINT16       HeaderValidateLength;                                                            
/*0x034*/     VOID*        HeaderValidateCopy;                                                              
/*0x038*/     UINT16       NextAvailableTagIndex;                                                           
/*0x03A*/     UINT16       MaximumTagIndex;                                                                 
/*0x03C*/     struct _HEAP_TAG_ENTRY* TagEntries;                                                           
/*0x040*/     struct _HEAP_UCR_SEGMENT* UCRSegments;                                                        
/*0x044*/     struct _HEAP_UNCOMMMTTED_RANGE* UnusedUnCommittedRanges;                                      
/*0x048*/     ULONG32      AlignRound;                                                                      
/*0x04C*/     ULONG32      AlignMask;                                                                       
/*0x050*/     struct _LIST_ENTRY VirtualAllocdBlocks;                  // 2 elements, 0x8 bytes (sizeof)    
/*0x058*/     struct _HEAP_SEGMENT* Segments[64];                                                           
              union                                                    // 2 elements, 0x10 bytes (sizeof)   
              {                                                                                             
/*0x158*/         ULONG32      FreeListsInUseUlong[4];                                                      
/*0x158*/         UINT8        FreeListsInUseBytes[16];                                                     
              }u;                                                                                           
              union                                                    // 2 elements, 0x2 bytes (sizeof)    
              {                                                                                             
/*0x168*/         UINT16       FreeListsInUseTerminate;                                                     
/*0x168*/         UINT16       DecommitCount;                                                               
              }u2;                                                                                          
/*0x16A*/     UINT16       AllocatorBackTraceIndex;                                                         
/*0x16C*/     ULONG32      NonDedicatedListLength;                                                          
/*0x170*/     VOID*        LargeBlocksIndex;                                                                
/*0x174*/     struct _HEAP_PSEUDO_TAG_ENTRY* PseudoTagEntries;                                              
/*0x178*/     struct _LIST_ENTRY FreeLists[128];                                                            
/*0x578*/     struct _HEAP_LOCK* LockVariable;                                                              
/*0x57C*/     FUNCT_0049_0C6E_CommitRoutine* CommitRoutine;                                                 
/*0x580*/     VOID*        FrontEndHeap;                                                                    
/*0x584*/     UINT16       FrontHeapLockCount;                                                              
/*0x586*/     UINT8        FrontEndHeapType;                                                                
/*0x587*/     UINT8        LastSegmentIndex;                                                                
          }HEAP, *PHEAP;                                                                                    
                                                                                                            
          typedef struct _HEAP_ENTRY_EXTRA                  // 4 elements, 0x8 bytes (sizeof) 
          {                                                                                   
              union                                         // 2 elements, 0x8 bytes (sizeof) 
              {                                                                               
                  struct                                    // 3 elements, 0x8 bytes (sizeof) 
                  {                                                                           
/*0x000*/             UINT16       AllocatorBackTraceIndex;                                   
/*0x002*/             UINT16       TagIndex;                                                  
/*0x004*/             ULONG32      Settable;                                                  
                  };                                                                          
/*0x000*/         UINT64       ZeroInit;                                                      
              };                                                                              
          }HEAP_ENTRY_EXTRA, *PHEAP_ENTRY_EXTRA;                                              
                                                                                              
          typedef struct _HEAP_FREE_ENTRY        // 8 elements, 0x10 bytes (sizeof) 
          {                                                                         
              union                              // 2 elements, 0x4 bytes (sizeof)  
              {                                                                     
                  struct                         // 2 elements, 0x4 bytes (sizeof)  
                  {                                                                 
/*0x000*/             UINT16       Size;                                            
/*0x002*/             UINT16       PreviousSize;                                    
                  };                                                                
/*0x000*/         VOID*        SubSegmentCode;                                      
              };                                                                    
/*0x004*/     UINT8        SmallTagIndex;                                           
/*0x005*/     UINT8        Flags;                                                   
/*0x006*/     UINT8        UnusedBytes;                                             
/*0x007*/     UINT8        SegmentIndex;                                            
/*0x008*/     struct _LIST_ENTRY FreeList;       // 2 elements, 0x8 bytes (sizeof)  
          }HEAP_FREE_ENTRY, *PHEAP_FREE_ENTRY;                                      
                                                                                    
          typedef struct _HEAP_FREE_ENTRY_EXTRA // 2 elements, 0x4 bytes (sizeof) 
          {                                                                       
/*0x000*/     UINT16       TagIndex;                                              
/*0x002*/     UINT16       FreeBackTraceIndex;                                    
          }HEAP_FREE_ENTRY_EXTRA, *PHEAP_FREE_ENTRY_EXTRA;                                  
                                                                                  
          typedef struct _RTL_CRITICAL_SECTION               // 6 elements, 0x18 bytes (sizeof) 
          {                                                                                     
/*0x000*/     struct _RTL_CRITICAL_SECTION_DEBUG* DebugInfo;                                    
/*0x004*/     LONG32       LockCount;                                                           
/*0x008*/     LONG32       RecursionCount;                                                      
/*0x00C*/     VOID*        OwningThread;                                                        
/*0x010*/     VOID*        LockSemaphore;                                                       
/*0x014*/     ULONG32      SpinCount;                                                           
          }RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;                                        
                                                                                                
          typedef struct _HEAP_LOCK                             // 1 elements, 0x38 bytes (sizeof)  
          {                                                                                         
              union                                             // 2 elements, 0x38 bytes (sizeof)  
              {                                                                                     
/*0x000*/         struct _RTL_CRITICAL_SECTION CriticalSection; // 6 elements, 0x18 bytes (sizeof)  
/*0x000*/         struct _ERESOURCE Resource;                   // 13 elements, 0x38 bytes (sizeof) 
              }Lock;                                                                                
          }HEAP_LOCK, *PHEAP_LOCK;                                                                  
                                                                                                    
          typedef struct _HEAP_LOOKASIDE       // 10 elements, 0x30 bytes (sizeof) 
          {                                                                        
/*0x000*/     union _SLIST_HEADER ListHead;    // 4 elements, 0x8 bytes (sizeof)   
/*0x008*/     UINT16       Depth;                                                  
/*0x00A*/     UINT16       MaximumDepth;                                           
/*0x00C*/     ULONG32      TotalAllocates;                                         
/*0x010*/     ULONG32      AllocateMisses;                                         
/*0x014*/     ULONG32      TotalFrees;                                             
/*0x018*/     ULONG32      FreeMisses;                                             
/*0x01C*/     ULONG32      LastTotalAllocates;                                     
/*0x020*/     ULONG32      LastAllocateMisses;                                     
/*0x024*/     ULONG32      Counters[2];                                            
/*0x02C*/     UINT8        _PADDING0_[0x4];                                        
          }HEAP_LOOKASIDE, *PHEAP_LOOKASIDE;                                       
                                                                                   
          typedef struct _HEAP_PSEUDO_TAG_ENTRY // 3 elements, 0xC bytes (sizeof) 
          {                                                                       
/*0x000*/     ULONG32      Allocs;                                                
/*0x004*/     ULONG32      Frees;                                                 
/*0x008*/     ULONG32      Size;                                                  
          }HEAP_PSEUDO_TAG_ENTRY, *PHEAP_PSEUDO_TAG_ENTRY;                                  
                                                                                  
          typedef struct _HEAP_SEGMENT                           // 15 elements, 0x3C bytes (sizeof) 
          {                                                                                          
/*0x000*/     struct _HEAP_ENTRY Entry;                          // 7 elements, 0x8 bytes (sizeof)   
/*0x008*/     ULONG32      Signature;                                                                
/*0x00C*/     ULONG32      Flags;                                                                    
/*0x010*/     struct _HEAP* Heap;                                                                    
/*0x014*/     ULONG32      LargestUnCommittedRange;                                                  
/*0x018*/     VOID*        BaseAddress;                                                              
/*0x01C*/     ULONG32      NumberOfPages;                                                            
/*0x020*/     struct _HEAP_ENTRY* FirstEntry;                                                        
/*0x024*/     struct _HEAP_ENTRY* LastValidEntry;                                                    
/*0x028*/     ULONG32      NumberOfUnCommittedPages;                                                 
/*0x02C*/     ULONG32      NumberOfUnCommittedRanges;                                                
/*0x030*/     struct _HEAP_UNCOMMMTTED_RANGE* UnCommittedRanges;                                     
/*0x034*/     UINT16       AllocatorBackTraceIndex;                                                  
/*0x036*/     UINT16       Reserved;                                                                 
/*0x038*/     struct _HEAP_ENTRY* LastEntryInSegment;                                                
          }HEAP_SEGMENT, *PHEAP_SEGMENT;                                                             
                                                                                                     
          typedef struct _HEAP_STOP_ON_TAG      // 3 elements, 0x4 bytes (sizeof) 
          {                                                                       
              union                             // 2 elements, 0x4 bytes (sizeof) 
              {                                                                   
/*0x000*/         ULONG32      HeapAndTagIndex;                                   
                  struct                        // 2 elements, 0x4 bytes (sizeof) 
                  {                                                               
/*0x000*/             UINT16       TagIndex;                                      
/*0x002*/             UINT16       HeapIndex;                                     
                  };                                                              
              };                                                                  
          }HEAP_STOP_ON_TAG, *PHEAP_STOP_ON_TAG;                                  
                                                                                  
          typedef struct _HEAP_STOP_ON_VALUES      // 6 elements, 0x18 bytes (sizeof) 
          {                                                                           
/*0x000*/     ULONG32      AllocAddress;                                              
/*0x004*/     struct _HEAP_STOP_ON_TAG AllocTag;   // 3 elements, 0x4 bytes (sizeof)  
/*0x008*/     ULONG32      ReAllocAddress;                                            
/*0x00C*/     struct _HEAP_STOP_ON_TAG ReAllocTag; // 3 elements, 0x4 bytes (sizeof)  
/*0x010*/     ULONG32      FreeAddress;                                               
/*0x014*/     struct _HEAP_STOP_ON_TAG FreeTag;    // 3 elements, 0x4 bytes (sizeof)  
          }HEAP_STOP_ON_VALUES, *PHEAP_STOP_ON_VALUES;                                   
                                                                                      
          typedef struct _INTERLOCK_SEQ             // 5 elements, 0x8 bytes (sizeof) 
          {                                                                           
              union                                 // 3 elements, 0x8 bytes (sizeof) 
              {                                                                       
                  struct                            // 2 elements, 0x8 bytes (sizeof) 
                  {                                                                   
/*0x000*/             UINT16       Depth;                                             
/*0x002*/             UINT16       FreeEntryOffset;                                   
/*0x004*/             UINT8        _PADDING0_[0x4];                                   
                  };                                                                  
                  struct                            // 2 elements, 0x8 bytes (sizeof) 
                  {                                                                   
/*0x000*/             ULONG32      OffsetAndDepth;                                    
/*0x004*/             ULONG32      Sequence;                                          
                  };                                                                  
/*0x000*/         INT64        Exchg;                                                 
              };                                                                      
          }INTERLOCK_SEQ, *PINTERLOCK_SEQ;                                            
                                                                                      
          typedef struct _HEAP_SUBSEGMENT               // 11 elements, 0x20 bytes (sizeof) 
          {                                                                                 
/*0x000*/     VOID*        Bucket;                                                          
/*0x004*/     struct _HEAP_USERDATA_HEADER* UserBlocks;                                     
/*0x008*/     struct _INTERLOCK_SEQ AggregateExchg;     // 5 elements, 0x8 bytes (sizeof)   
              union                                     // 2 elements, 0x8 bytes (sizeof)   
              {                                                                             
                  struct                                // 5 elements, 0x8 bytes (sizeof)   
                  {                                                                         
/*0x010*/             UINT16       BlockSize;                                               
/*0x012*/             UINT16       FreeThreshold;                                           
/*0x014*/             UINT16       BlockCount;                                              
/*0x016*/             UINT8        SizeIndex;                                               
/*0x017*/             UINT8        AffinityIndex;                                           
                  };                                                                        
/*0x010*/         ULONG32      Alignment[2];                                                
              };                                                                            
/*0x018*/     struct _SINGLE_LIST_ENTRY SFreeListEntry; // 1 elements, 0x4 bytes (sizeof)   
/*0x01C*/     ULONG32      Lock;                                                            
          }HEAP_SUBSEGMENT, *PHEAP_SUBSEGMENT;                                              
                                                                                            
          typedef struct _HEAP_TAG_ENTRY          // 6 elements, 0x40 bytes (sizeof) 
          {                                                                          
/*0x000*/     ULONG32      Allocs;                                                   
/*0x004*/     ULONG32      Frees;                                                    
/*0x008*/     ULONG32      Size;                                                     
/*0x00C*/     UINT16       TagIndex;                                                 
/*0x00E*/     UINT16       CreatorBackTraceIndex;                                    
/*0x010*/     UINT16       TagName[24];                                              
          }HEAP_TAG_ENTRY, *PHEAP_TAG_ENTRY;                                         
                                                                                     
          typedef struct _HEAP_UCR_SEGMENT    // 4 elements, 0x10 bytes (sizeof) 
          {                                                                      
/*0x000*/     struct _HEAP_UCR_SEGMENT* Next;                                    
/*0x004*/     ULONG32      ReservedSize;                                         
/*0x008*/     ULONG32      CommittedSize;                                        
/*0x00C*/     ULONG32      filler;                                               
          }HEAP_UCR_SEGMENT, *PHEAP_UCR_SEGMENT;                                   
                                                                                 
          typedef struct _HEAP_UNCOMMMTTED_RANGE    // 4 elements, 0x10 bytes (sizeof) 
          {                                                                            
/*0x000*/     struct _HEAP_UNCOMMMTTED_RANGE* Next;                                    
/*0x004*/     ULONG32      Address;                                                    
/*0x008*/     ULONG32      Size;                                                       
/*0x00C*/     ULONG32      filler;                                                     
          }HEAP_UNCOMMMTTED_RANGE, *PHEAP_UNCOMMMTTED_RANGE;                                   
                                                                                       
          typedef struct _HEAP_USERDATA_HEADER              // 5 elements, 0x10 bytes (sizeof) 
          {                                                                                    
              union                                         // 2 elements, 0x4 bytes (sizeof)  
              {                                                                                
/*0x000*/         struct _SINGLE_LIST_ENTRY SFreeListEntry; // 1 elements, 0x4 bytes (sizeof)  
/*0x000*/         struct _HEAP_SUBSEGMENT* SubSegment;                                         
              };                                                                               
/*0x004*/     VOID*        HeapHandle;                                                         
/*0x008*/     ULONG32      SizeIndex;                                                          
/*0x00C*/     ULONG32      Signature;                                                          
          }HEAP_USERDATA_HEADER, *PHEAP_USERDATA_HEADER;                                       
                                                                                               
          typedef struct _HEAP_VIRTUAL_ALLOC_ENTRY // 5 elements, 0x20 bytes (sizeof) 
          {                                                                           
/*0x000*/     struct _LIST_ENTRY Entry;            // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/     struct _HEAP_ENTRY_EXTRA ExtraStuff; // 4 elements, 0x8 bytes (sizeof)  
/*0x010*/     ULONG32      CommitSize;                                                
/*0x014*/     ULONG32      ReserveSize;                                               
/*0x018*/     struct _HEAP_ENTRY BusyBlock;        // 7 elements, 0x8 bytes (sizeof)  
          }HEAP_VIRTUAL_ALLOC_ENTRY, *PHEAP_VIRTUAL_ALLOC_ENTRY;                                   
                                                                                      
          typedef struct _HIVE_LIST_ENTRY   // 8 elements, 0x18 bytes (sizeof) 
          {                                                                    
/*0x000*/     UINT16*      Name;                                               
/*0x004*/     UINT16*      BaseName;                                           
/*0x008*/     struct _CMHIVE* CmHive;                                          
/*0x00C*/     ULONG32      Flags;                                              
/*0x010*/     struct _CMHIVE* CmHive2;                                         
/*0x014*/     UINT8        ThreadFinished;                                     
/*0x015*/     UINT8        ThreadStarted;                                      
/*0x016*/     UINT8        Allocate;                                           
/*0x017*/     UINT8        _PADDING0_[0x1];                                    
          }HIVE_LIST_ENTRY, *PHIVE_LIST_ENTRY;                                   
                                                                               
          typedef struct _HMAP_DIRECTORY           // 1 elements, 0x1000 bytes (sizeof) 
          {                                                                             
/*0x000*/     struct _HMAP_TABLE* Directory[1024];                                      
          }HMAP_DIRECTORY, *PHMAP_DIRECTORY;                                            
                                                                                        
          typedef struct _HMAP_ENTRY           // 4 elements, 0x10 bytes (sizeof) 
          {                                                                       
/*0x000*/     ULONG32      BlockAddress;                                          
/*0x004*/     ULONG32      BinAddress;                                            
/*0x008*/     struct _CM_VIEW_OF_FILE* CmView;                                    
/*0x00C*/     ULONG32      MemAlloc;                                              
          }HMAP_ENTRY, *PHMAP_ENTRY;                                              
                                                                                  
          typedef struct _HMAP_TABLE         // 1 elements, 0x2000 bytes (sizeof) 
          {                                                                       
/*0x000*/     struct _HMAP_ENTRY Table[512];                                      
          }HMAP_TABLE, *PHMAP_TABLE;                                              
                                                                                  
          typedef struct _IMAGE_DATA_DIRECTORY // 2 elements, 0x8 bytes (sizeof) 
          {                                                                      
/*0x000*/     ULONG32      VirtualAddress;                                       
/*0x004*/     ULONG32      Size;                                                 
          }IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;                                  
                                                                                 
          typedef struct _IMAGE_DEBUG_DIRECTORY // 8 elements, 0x1C bytes (sizeof) 
          {                                                                        
/*0x000*/     ULONG32      Characteristics;                                        
/*0x004*/     ULONG32      TimeDateStamp;                                          
/*0x008*/     UINT16       MajorVersion;                                           
/*0x00A*/     UINT16       MinorVersion;                                           
/*0x00C*/     ULONG32      Type;                                                   
/*0x010*/     ULONG32      SizeOfData;                                             
/*0x014*/     ULONG32      AddressOfRawData;                                       
/*0x018*/     ULONG32      PointerToRawData;                                       
          }IMAGE_DEBUG_DIRECTORY, *PIMAGE_DEBUG_DIRECTORY;                                   
                                                                                   
          typedef struct _IMAGE_DOS_HEADER // 19 elements, 0x40 bytes (sizeof) 
          {                                                                    
/*0x000*/     UINT16       e_magic;                                            
/*0x002*/     UINT16       e_cblp;                                             
/*0x004*/     UINT16       e_cp;                                               
/*0x006*/     UINT16       e_crlc;                                             
/*0x008*/     UINT16       e_cparhdr;                                          
/*0x00A*/     UINT16       e_minalloc;                                         
/*0x00C*/     UINT16       e_maxalloc;                                         
/*0x00E*/     UINT16       e_ss;                                               
/*0x010*/     UINT16       e_sp;                                               
/*0x012*/     UINT16       e_csum;                                             
/*0x014*/     UINT16       e_ip;                                               
/*0x016*/     UINT16       e_cs;                                               
/*0x018*/     UINT16       e_lfarlc;                                           
/*0x01A*/     UINT16       e_ovno;                                             
/*0x01C*/     UINT16       e_res[4];                                           
/*0x024*/     UINT16       e_oemid;                                            
/*0x026*/     UINT16       e_oeminfo;                                          
/*0x028*/     UINT16       e_res2[10];                                         
/*0x03C*/     LONG32       e_lfanew;                                           
          }IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;                                    
                                                                               
          typedef struct _IMAGE_FILE_HEADER      // 7 elements, 0x14 bytes (sizeof) 
          {                                                                         
/*0x000*/     UINT16       Machine;                                                 
/*0x002*/     UINT16       NumberOfSections;                                        
/*0x004*/     ULONG32      TimeDateStamp;                                           
/*0x008*/     ULONG32      PointerToSymbolTable;                                    
/*0x00C*/     ULONG32      NumberOfSymbols;                                         
/*0x010*/     UINT16       SizeOfOptionalHeader;                                    
/*0x012*/     UINT16       Characteristics;                                         
          }IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;                                   
                                                                                    
          typedef struct _IMAGE_OPTIONAL_HEADER               // 31 elements, 0xE0 bytes (sizeof) 
          {                                                                                       
/*0x000*/     UINT16       Magic;                                                                 
/*0x002*/     UINT8        MajorLinkerVersion;                                                    
/*0x003*/     UINT8        MinorLinkerVersion;                                                    
/*0x004*/     ULONG32      SizeOfCode;                                                            
/*0x008*/     ULONG32      SizeOfInitializedData;                                                 
/*0x00C*/     ULONG32      SizeOfUninitializedData;                                               
/*0x010*/     ULONG32      AddressOfEntryPoint;                                                   
/*0x014*/     ULONG32      BaseOfCode;                                                            
/*0x018*/     ULONG32      BaseOfData;                                                            
/*0x01C*/     ULONG32      ImageBase;                                                             
/*0x020*/     ULONG32      SectionAlignment;                                                      
/*0x024*/     ULONG32      FileAlignment;                                                         
/*0x028*/     UINT16       MajorOperatingSystemVersion;                                           
/*0x02A*/     UINT16       MinorOperatingSystemVersion;                                           
/*0x02C*/     UINT16       MajorImageVersion;                                                     
/*0x02E*/     UINT16       MinorImageVersion;                                                     
/*0x030*/     UINT16       MajorSubsystemVersion;                                                 
/*0x032*/     UINT16       MinorSubsystemVersion;                                                 
/*0x034*/     ULONG32      Win32VersionValue;                                                     
/*0x038*/     ULONG32      SizeOfImage;                                                           
/*0x03C*/     ULONG32      SizeOfHeaders;                                                         
/*0x040*/     ULONG32      CheckSum;                                                              
/*0x044*/     UINT16       Subsystem;                                                             
/*0x046*/     UINT16       DllCharacteristics;                                                    
/*0x048*/     ULONG32      SizeOfStackReserve;                                                    
/*0x04C*/     ULONG32      SizeOfStackCommit;                                                     
/*0x050*/     ULONG32      SizeOfHeapReserve;                                                     
/*0x054*/     ULONG32      SizeOfHeapCommit;                                                      
/*0x058*/     ULONG32      LoaderFlags;                                                           
/*0x05C*/     ULONG32      NumberOfRvaAndSizes;                                                   
/*0x060*/     struct _IMAGE_DATA_DIRECTORY DataDirectory[16];                                     
          }IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;                                        
                                                                                                  
          typedef struct _IMAGE_NT_HEADERS                  // 3 elements, 0xF8 bytes (sizeof)  
          {                                                                                     
/*0x000*/     ULONG32      Signature;                                                           
/*0x004*/     struct _IMAGE_FILE_HEADER FileHeader;         // 7 elements, 0x14 bytes (sizeof)  
/*0x018*/     struct _IMAGE_OPTIONAL_HEADER OptionalHeader; // 31 elements, 0xE0 bytes (sizeof) 
          }IMAGE_NT_HEADERS, *PIMAGE_NT_HEADERS;                                                
                                                                                                
          typedef struct _IMAGE_ROM_OPTIONAL_HEADER // 13 elements, 0x38 bytes (sizeof) 
          {                                                                             
/*0x000*/     UINT16       Magic;                                                       
/*0x002*/     UINT8        MajorLinkerVersion;                                          
/*0x003*/     UINT8        MinorLinkerVersion;                                          
/*0x004*/     ULONG32      SizeOfCode;                                                  
/*0x008*/     ULONG32      SizeOfInitializedData;                                       
/*0x00C*/     ULONG32      SizeOfUninitializedData;                                     
/*0x010*/     ULONG32      AddressOfEntryPoint;                                         
/*0x014*/     ULONG32      BaseOfCode;                                                  
/*0x018*/     ULONG32      BaseOfData;                                                  
/*0x01C*/     ULONG32      BaseOfBss;                                                   
/*0x020*/     ULONG32      GprMask;                                                     
/*0x024*/     ULONG32      CprMask[4];                                                  
/*0x034*/     ULONG32      GpValue;                                                     
          }IMAGE_ROM_OPTIONAL_HEADER, *PIMAGE_ROM_OPTIONAL_HEADER;                                    
                                                                                        
          typedef struct _IMAGE_SECTION_HEADER   // 10 elements, 0x28 bytes (sizeof) 
          {                                                                          
/*0x000*/     UINT8        Name[8];                                                  
              union                              // 2 elements, 0x4 bytes (sizeof)   
              {                                                                      
/*0x008*/         ULONG32      PhysicalAddress;                                      
/*0x008*/         ULONG32      VirtualSize;                                          
              }Misc;                                                                 
/*0x00C*/     ULONG32      VirtualAddress;                                           
/*0x010*/     ULONG32      SizeOfRawData;                                            
/*0x014*/     ULONG32      PointerToRawData;                                         
/*0x018*/     ULONG32      PointerToRelocations;                                     
/*0x01C*/     ULONG32      PointerToLinenumbers;                                     
/*0x020*/     UINT16       NumberOfRelocations;                                      
/*0x022*/     UINT16       NumberOfLinenumbers;                                      
/*0x024*/     ULONG32      Characteristics;                                          
          }IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;                                    
                                                                                     
          typedef struct _INTERFACE                                                                                                                                                           // 5 elements, 0x10 bytes (sizeof) 
          {                                                                                                                                                                                                                      
/*0x000*/     UINT16       Size;                                                                                                                                                                                                 
/*0x002*/     UINT16       Version;                                                                                                                                                                                              
/*0x004*/     VOID*        Context;                                                                                                                                                                                              
/*0x008*/     FUNCT_00BC_0285_Free_InterfaceReference_InterfaceDereference_DeleteProcedure_ReleaseFromLazyWrite_ReleaseFromReadAhead_WorkerRoutine_Destructor_Callback* InterfaceReference;                                      
/*0x00C*/     FUNCT_00BC_0285_Free_InterfaceReference_InterfaceDereference_DeleteProcedure_ReleaseFromLazyWrite_ReleaseFromReadAhead_WorkerRoutine_Destructor_Callback* InterfaceDereference;                                    
          }INTERFACE, *PINTERFACE;                                                                                                                                                                                               
                                                                                                                                                                                                                                 
          typedef struct _IO_CLIENT_EXTENSION             // 2 elements, 0x8 bytes (sizeof) 
          {                                                                                 
/*0x000*/     struct _IO_CLIENT_EXTENSION* NextExtension;                                   
/*0x004*/     VOID*        ClientIdentificationAddress;                                     
          }IO_CLIENT_EXTENSION, *PIO_CLIENT_EXTENSION;                                      
                                                                                            
          typedef struct _IO_COMPLETION_CONTEXT // 2 elements, 0x8 bytes (sizeof) 
          {                                                                       
/*0x000*/     VOID*        Port;                                                  
/*0x004*/     VOID*        Key;                                                   
          }IO_COMPLETION_CONTEXT, *PIO_COMPLETION_CONTEXT;                                  
                                                                                  
          typedef struct _IO_RESOURCE_DESCRIPTOR           // 7 elements, 0x20 bytes (sizeof) 
          {                                                                                   
/*0x000*/     UINT8        Option;                                                            
/*0x001*/     UINT8        Type;                                                              
/*0x002*/     UINT8        ShareDisposition;                                                  
/*0x003*/     UINT8        Spare1;                                                            
/*0x004*/     UINT16       Flags;                                                             
/*0x006*/     UINT16       Spare2;                                                            
              union                                        // 8 elements, 0x18 bytes (sizeof) 
              {                                                                               
                  struct                                   // 4 elements, 0x18 bytes (sizeof) 
                  {                                                                           
/*0x008*/             ULONG32      Length;                                                    
/*0x00C*/             ULONG32      Alignment;                                                 
/*0x010*/             union _LARGE_INTEGER MinimumAddress; // 4 elements, 0x8 bytes (sizeof)  
/*0x018*/             union _LARGE_INTEGER MaximumAddress; // 4 elements, 0x8 bytes (sizeof)  
                  }Port;                                                                      
                  struct                                   // 4 elements, 0x18 bytes (sizeof) 
                  {                                                                           
/*0x008*/             ULONG32      Length;                                                    
/*0x00C*/             ULONG32      Alignment;                                                 
/*0x010*/             union _LARGE_INTEGER MinimumAddress; // 4 elements, 0x8 bytes (sizeof)  
/*0x018*/             union _LARGE_INTEGER MaximumAddress; // 4 elements, 0x8 bytes (sizeof)  
                  }Memory;                                                                    
                  struct                                   // 2 elements, 0x8 bytes (sizeof)  
                  {                                                                           
/*0x008*/             ULONG32      MinimumVector;                                             
/*0x00C*/             ULONG32      MaximumVector;                                             
                  }Interrupt;                                                                 
                  struct                                   // 2 elements, 0x8 bytes (sizeof)  
                  {                                                                           
/*0x008*/             ULONG32      MinimumChannel;                                            
/*0x00C*/             ULONG32      MaximumChannel;                                            
                  }Dma;                                                                       
                  struct                                   // 4 elements, 0x18 bytes (sizeof) 
                  {                                                                           
/*0x008*/             ULONG32      Length;                                                    
/*0x00C*/             ULONG32      Alignment;                                                 
/*0x010*/             union _LARGE_INTEGER MinimumAddress; // 4 elements, 0x8 bytes (sizeof)  
/*0x018*/             union _LARGE_INTEGER MaximumAddress; // 4 elements, 0x8 bytes (sizeof)  
                  }Generic;                                                                   
                  struct                                   // 1 elements, 0xC bytes (sizeof)  
                  {                                                                           
/*0x008*/             ULONG32      Data[3];                                                   
                  }DevicePrivate;                                                             
                  struct                                   // 4 elements, 0x10 bytes (sizeof) 
                  {                                                                           
/*0x008*/             ULONG32      Length;                                                    
/*0x00C*/             ULONG32      MinBusNumber;                                              
/*0x010*/             ULONG32      MaxBusNumber;                                              
/*0x014*/             ULONG32      Reserved;                                                  
                  }BusNumber;                                                                 
                  struct                                   // 3 elements, 0xC bytes (sizeof)  
                  {                                                                           
/*0x008*/             ULONG32      Priority;                                                  
/*0x00C*/             ULONG32      Reserved1;                                                 
/*0x010*/             ULONG32      Reserved2;                                                 
                  }ConfigData;                                                                
              }u;                                                                             
          }IO_RESOURCE_DESCRIPTOR, *PIO_RESOURCE_DESCRIPTOR;                                   
                                                                                              
          typedef struct _IO_RESOURCE_LIST                   // 4 elements, 0x28 bytes (sizeof) 
          {                                                                                     
/*0x000*/     UINT16       Version;                                                             
/*0x002*/     UINT16       Revision;                                                            
/*0x004*/     ULONG32      Count;                                                               
/*0x008*/     struct _IO_RESOURCE_DESCRIPTOR Descriptors[1];                                    
          }IO_RESOURCE_LIST, *PIO_RESOURCE_LIST;                                                
                                                                                                
          typedef struct _IO_RESOURCE_REQUIREMENTS_LIST // 7 elements, 0x48 bytes (sizeof) 
          {                                                                                
/*0x000*/     ULONG32      ListSize;                                                       
/*0x004*/     enum _INTERFACE_TYPE InterfaceType;                                          
/*0x008*/     ULONG32      BusNumber;                                                      
/*0x00C*/     ULONG32      SlotNumber;                                                     
/*0x010*/     ULONG32      Reserved[3];                                                    
/*0x01C*/     ULONG32      AlternativeLists;                                               
/*0x020*/     struct _IO_RESOURCE_LIST List[1];                                            
          }IO_RESOURCE_REQUIREMENTS_LIST, *PIO_RESOURCE_REQUIREMENTS_LIST;                                   
                                                                                           
          typedef struct _IO_SECURITY_CONTEXT                   // 4 elements, 0x10 bytes (sizeof) 
          {                                                                                        
/*0x000*/     struct _SECURITY_QUALITY_OF_SERVICE* SecurityQos;                                    
/*0x004*/     struct _ACCESS_STATE* AccessState;                                                   
/*0x008*/     ULONG32      DesiredAccess;                                                          
/*0x00C*/     ULONG32      FullCreateOptions;                                                      
          }IO_SECURITY_CONTEXT, *PIO_SECURITY_CONTEXT;                                             
                                                                                                   
          typedef union _POWER_STATE                // 2 elements, 0x4 bytes (sizeof) 
          {                                                                           
/*0x000*/     enum _SYSTEM_POWER_STATE SystemState;                                   
/*0x000*/     enum _DEVICE_POWER_STATE DeviceState;                                   
          }POWER_STATE, *PPOWER_STATE;                                                
                                                                                      
          typedef struct _IO_STACK_LOCATION                                             // 9 elements, 0x24 bytes (sizeof)  
          {                                                                                                                 
/*0x000*/     UINT8        MajorFunction;                                                                                   
/*0x001*/     UINT8        MinorFunction;                                                                                   
/*0x002*/     UINT8        Flags;                                                                                           
/*0x003*/     UINT8        Control;                                                                                         
              union                                                                     // 38 elements, 0x10 bytes (sizeof) 
              {                                                                                                             
                  struct                                                                // 5 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             struct _IO_SECURITY_CONTEXT* SecurityContext;                                                         
/*0x008*/             ULONG32      Options;                                                                                 
/*0x00C*/             UINT16       FileAttributes;                                                                          
/*0x00E*/             UINT16       ShareAccess;                                                                             
/*0x010*/             ULONG32      EaLength;                                                                                
                  }Create;                                                                                                  
                  struct                                                                // 5 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             struct _IO_SECURITY_CONTEXT* SecurityContext;                                                         
/*0x008*/             ULONG32      Options;                                                                                 
/*0x00C*/             UINT16       Reserved;                                                                                
/*0x00E*/             UINT16       ShareAccess;                                                                             
/*0x010*/             struct _NAMED_PIPE_CREATE_PARAMETERS* Parameters;                                                     
                  }CreatePipe;                                                                                              
                  struct                                                                // 5 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             struct _IO_SECURITY_CONTEXT* SecurityContext;                                                         
/*0x008*/             ULONG32      Options;                                                                                 
/*0x00C*/             UINT16       Reserved;                                                                                
/*0x00E*/             UINT16       ShareAccess;                                                                             
/*0x010*/             struct _MAILSLOT_CREATE_PARAMETERS* Parameters;                                                       
                  }CreateMailslot;                                                                                          
                  struct                                                                // 3 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             ULONG32      Length;                                                                                  
/*0x008*/             ULONG32      Key;                                                                                     
/*0x00C*/             union _LARGE_INTEGER ByteOffset;                                  // 4 elements, 0x8 bytes (sizeof)   
                  }Read;                                                                                                    
                  struct                                                                // 3 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             ULONG32      Length;                                                                                  
/*0x008*/             ULONG32      Key;                                                                                     
/*0x00C*/             union _LARGE_INTEGER ByteOffset;                                  // 4 elements, 0x8 bytes (sizeof)   
                  }Write;                                                                                                   
                  struct                                                                // 4 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             ULONG32      Length;                                                                                  
/*0x008*/             struct _STRING* FileName;                                                                             
/*0x00C*/             enum _FILE_INFORMATION_CLASS FileInformationClass;                                                    
/*0x010*/             ULONG32      FileIndex;                                                                               
                  }QueryDirectory;                                                                                          
                  struct                                                                // 2 elements, 0x8 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             ULONG32      Length;                                                                                  
/*0x008*/             ULONG32      CompletionFilter;                                                                        
                  }NotifyDirectory;                                                                                         
                  struct                                                                // 2 elements, 0x8 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             ULONG32      Length;                                                                                  
/*0x008*/             enum _FILE_INFORMATION_CLASS FileInformationClass;                                                    
                  }QueryFile;                                                                                               
                  struct                                                                // 7 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             ULONG32      Length;                                                                                  
/*0x008*/             enum _FILE_INFORMATION_CLASS FileInformationClass;                                                    
/*0x00C*/             struct _FILE_OBJECT* FileObject;                                                                      
                      union                                                             // 3 elements, 0x4 bytes (sizeof)   
                      {                                                                                                     
                          struct                                                        // 2 elements, 0x4 bytes (sizeof)   
                          {                                                                                                 
/*0x010*/                     UINT8        ReplaceIfExists;                                                                 
/*0x011*/                     UINT8        AdvanceOnly;                                                                     
/*0x012*/                     UINT8        _PADDING0_[0x2];                                                                 
                          };                                                                                                
/*0x010*/                 ULONG32      ClusterCount;                                                                        
/*0x010*/                 VOID*        DeleteHandle;                                                                        
                      };                                                                                                    
                  }SetFile;                                                                                                 
                  struct                                                                // 4 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             ULONG32      Length;                                                                                  
/*0x008*/             VOID*        EaList;                                                                                  
/*0x00C*/             ULONG32      EaListLength;                                                                            
/*0x010*/             ULONG32      EaIndex;                                                                                 
                  }QueryEa;                                                                                                 
                  struct                                                                // 1 elements, 0x4 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             ULONG32      Length;                                                                                  
                  }SetEa;                                                                                                   
                  struct                                                                // 2 elements, 0x8 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             ULONG32      Length;                                                                                  
/*0x008*/             enum _FSINFOCLASS FsInformationClass;                                                                 
                  }QueryVolume;                                                                                             
                  struct                                                                // 2 elements, 0x8 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             ULONG32      Length;                                                                                  
/*0x008*/             enum _FSINFOCLASS FsInformationClass;                                                                 
                  }SetVolume;                                                                                               
                  struct                                                                // 4 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             ULONG32      OutputBufferLength;                                                                      
/*0x008*/             ULONG32      InputBufferLength;                                                                       
/*0x00C*/             ULONG32      FsControlCode;                                                                           
/*0x010*/             VOID*        Type3InputBuffer;                                                                        
                  }FileSystemControl;                                                                                       
                  struct                                                                // 3 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             union _LARGE_INTEGER* Length;                                                                         
/*0x008*/             ULONG32      Key;                                                                                     
/*0x00C*/             union _LARGE_INTEGER ByteOffset;                                  // 4 elements, 0x8 bytes (sizeof)   
                  }LockControl;                                                                                             
                  struct                                                                // 4 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             ULONG32      OutputBufferLength;                                                                      
/*0x008*/             ULONG32      InputBufferLength;                                                                       
/*0x00C*/             ULONG32      IoControlCode;                                                                           
/*0x010*/             VOID*        Type3InputBuffer;                                                                        
                  }DeviceIoControl;                                                                                         
                  struct                                                                // 2 elements, 0x8 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             ULONG32      SecurityInformation;                                                                     
/*0x008*/             ULONG32      Length;                                                                                  
                  }QuerySecurity;                                                                                           
                  struct                                                                // 2 elements, 0x8 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             ULONG32      SecurityInformation;                                                                     
/*0x008*/             VOID*        SecurityDescriptor;                                                                      
                  }SetSecurity;                                                                                             
                  struct                                                                // 2 elements, 0x8 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             struct _VPB* Vpb;                                                                                     
/*0x008*/             struct _DEVICE_OBJECT* DeviceObject;                                                                  
                  }MountVolume;                                                                                             
                  struct                                                                // 2 elements, 0x8 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             struct _VPB* Vpb;                                                                                     
/*0x008*/             struct _DEVICE_OBJECT* DeviceObject;                                                                  
                  }VerifyVolume;                                                                                            
                  struct                                                                // 1 elements, 0x4 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             struct _SCSI_REQUEST_BLOCK* Srb;                                                                      
                  }Scsi;                                                                                                    
                  struct                                                                // 4 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             ULONG32      Length;                                                                                  
/*0x008*/             VOID*        StartSid;                                                                                
/*0x00C*/             struct _FILE_GET_QUOTA_INFORMATION* SidList;                                                          
/*0x010*/             ULONG32      SidListLength;                                                                           
                  }QueryQuota;                                                                                              
                  struct                                                                // 1 elements, 0x4 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             ULONG32      Length;                                                                                  
                  }SetQuota;                                                                                                
                  struct                                                                // 1 elements, 0x4 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             enum _DEVICE_RELATION_TYPE Type;                                                                      
                  }QueryDeviceRelations;                                                                                    
                  struct                                                                // 5 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             struct _GUID* InterfaceType;                                                                          
/*0x008*/             UINT16       Size;                                                                                    
/*0x00A*/             UINT16       Version;                                                                                 
/*0x00C*/             struct _INTERFACE* Interface;                                                                         
/*0x010*/             VOID*        InterfaceSpecificData;                                                                   
                  }QueryInterface;                                                                                          
                  struct                                                                // 1 elements, 0x4 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             struct _DEVICE_CAPABILITIES* Capabilities;                                                            
                  }DeviceCapabilities;                                                                                      
                  struct                                                                // 1 elements, 0x4 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             struct _IO_RESOURCE_REQUIREMENTS_LIST* IoResourceRequirementList;                                     
                  }FilterResourceRequirements;                                                                              
                  struct                                                                // 4 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             ULONG32      WhichSpace;                                                                              
/*0x008*/             VOID*        Buffer;                                                                                  
/*0x00C*/             ULONG32      Offset;                                                                                  
/*0x010*/             ULONG32      Length;                                                                                  
                  }ReadWriteConfig;                                                                                         
                  struct                                                                // 1 elements, 0x1 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             UINT8        Lock;                                                                                    
                  }SetLock;                                                                                                 
                  struct                                                                // 1 elements, 0x4 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             enum _BUS_QUERY_ID_TYPE IdType;                                                                       
                  }QueryId;                                                                                                 
                  struct                                                                // 2 elements, 0x8 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             enum _DEVICE_TEXT_TYPE DeviceTextType;                                                                
/*0x008*/             ULONG32      LocaleId;                                                                                
                  }QueryDeviceText;                                                                                         
                  struct                                                                // 3 elements, 0x8 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             UINT8        InPath;                                                                                  
/*0x005*/             UINT8        Reserved[3];                                                                             
/*0x008*/             enum _DEVICE_USAGE_NOTIFICATION_TYPE Type;                                                            
                  }UsageNotification;                                                                                       
                  struct                                                                // 1 elements, 0x4 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             enum _SYSTEM_POWER_STATE PowerState;                                                                  
                  }WaitWake;                                                                                                
                  struct                                                                // 1 elements, 0x4 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             struct _POWER_SEQUENCE* PowerSequence;                                                                
                  }PowerSequence;                                                                                           
                  struct                                                                // 4 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             ULONG32      SystemContext;                                                                           
/*0x008*/             enum _POWER_STATE_TYPE Type;                                                                          
/*0x00C*/             union _POWER_STATE State;                                         // 2 elements, 0x4 bytes (sizeof)   
/*0x010*/             enum _POWER_ACTION ShutdownType;                                                                      
                  }Power;                                                                                                   
                  struct                                                                // 2 elements, 0x8 bytes (sizeof)   
                  {                                                                                                         
/*0x004*/             struct _CM_RESOURCE_LIST* AllocatedResources;                                                         
/*0x008*/             struct _CM_RESOURCE_LIST* AllocatedResourcesTranslated;                                               
                  }StartDevice;                                                                                             
                  struct                                                                // 4 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             ULONG32      ProviderId;                                                                              
/*0x008*/             VOID*        DataPath;                                                                                
/*0x00C*/             ULONG32      BufferSize;                                                                              
/*0x010*/             VOID*        Buffer;                                                                                  
                  }WMI;                                                                                                     
                  struct                                                                // 4 elements, 0x10 bytes (sizeof)  
                  {                                                                                                         
/*0x004*/             VOID*        Argument1;                                                                               
/*0x008*/             VOID*        Argument2;                                                                               
/*0x00C*/             VOID*        Argument3;                                                                               
/*0x010*/             VOID*        Argument4;                                                                               
                  }Others;                                                                                                  
              }Parameters;                                                                                                  
/*0x014*/     struct _DEVICE_OBJECT* DeviceObject;                                                                          
/*0x018*/     struct _FILE_OBJECT* FileObject;                                                                              
/*0x01C*/     FUNCT_0049_0948_CompletionRoutine* CompletionRoutine;                                                         
/*0x020*/     VOID*        Context;                                                                                         
          }IO_STACK_LOCATION, *PIO_STACK_LOCATION;                                                                          
                                                                                                                            
          typedef struct _IO_STATUS_BLOCK // 3 elements, 0x8 bytes (sizeof) 
          {                                                                 
              union                       // 2 elements, 0x4 bytes (sizeof) 
              {                                                             
/*0x000*/         LONG32       Status;                                      
/*0x000*/         VOID*        Pointer;                                     
              };                                                            
/*0x004*/     ULONG32      Information;                                     
          }IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;                                  
                                                                            
          typedef struct _IO_TIMER                        // 6 elements, 0x18 bytes (sizeof) 
          {                                                                                  
/*0x000*/     INT16        Type;                                                             
/*0x002*/     INT16        TimerFlag;                                                        
/*0x004*/     struct _LIST_ENTRY TimerList;               // 2 elements, 0x8 bytes (sizeof)  
/*0x00C*/     FUNCT_00BC_0968_TimerRoutine* TimerRoutine;                                    
/*0x010*/     VOID*        Context;                                                          
/*0x014*/     struct _DEVICE_OBJECT* DeviceObject;                                           
          }IO_TIMER, *PIO_TIMER;                                                             
                                                                                             
          typedef struct _IRP                                                                // 21 elements, 0x70 bytes (sizeof) 
          {                                                                                                                      
/*0x000*/     INT16        Type;                                                                                                 
/*0x002*/     UINT16       Size;                                                                                                 
/*0x004*/     struct _MDL* MdlAddress;                                                                                           
/*0x008*/     ULONG32      Flags;                                                                                                
              union                                                                          // 3 elements, 0x4 bytes (sizeof)   
              {                                                                                                                  
/*0x00C*/         struct _IRP* MasterIrp;                                                                                        
/*0x00C*/         LONG32       IrpCount;                                                                                         
/*0x00C*/         VOID*        SystemBuffer;                                                                                     
              }AssociatedIrp;                                                                                                    
/*0x010*/     struct _LIST_ENTRY ThreadListEntry;                                            // 2 elements, 0x8 bytes (sizeof)   
/*0x018*/     struct _IO_STATUS_BLOCK IoStatus;                                              // 3 elements, 0x8 bytes (sizeof)   
/*0x020*/     CHAR         RequestorMode;                                                                                        
/*0x021*/     UINT8        PendingReturned;                                                                                      
/*0x022*/     CHAR         StackCount;                                                                                           
/*0x023*/     CHAR         CurrentLocation;                                                                                      
/*0x024*/     UINT8        Cancel;                                                                                               
/*0x025*/     UINT8        CancelIrql;                                                                                           
/*0x026*/     CHAR         ApcEnvironment;                                                                                       
/*0x027*/     UINT8        AllocationFlags;                                                                                      
/*0x028*/     struct _IO_STATUS_BLOCK* UserIosb;                                                                                 
/*0x02C*/     struct _KEVENT* UserEvent;                                                                                         
              union                                                                          // 2 elements, 0x8 bytes (sizeof)   
              {                                                                                                                  
                  struct                                                                     // 2 elements, 0x8 bytes (sizeof)   
                  {                                                                                                              
/*0x030*/             FUNCT_00BC_0713_UserApcRoutine* UserApcRoutine;                                                            
/*0x034*/             VOID*        UserApcContext;                                                                               
                  }AsynchronousParameters;                                                                                       
/*0x030*/         union _LARGE_INTEGER AllocationSize;                                       // 4 elements, 0x8 bytes (sizeof)   
              }Overlay;                                                                                                          
/*0x038*/     FUNCT_00BC_0718_DriverStartIo_CancelRoutine_SavedCancelRoutine* CancelRoutine;                                     
/*0x03C*/     VOID*        UserBuffer;                                                                                           
              union                                                                          // 3 elements, 0x30 bytes (sizeof)  
              {                                                                                                                  
                  struct                                                                     // 8 elements, 0x28 bytes (sizeof)  
                  {                                                                                                              
                      union                                                                  // 2 elements, 0x10 bytes (sizeof)  
                      {                                                                                                          
/*0x040*/                 struct _KDEVICE_QUEUE_ENTRY DeviceQueueEntry;                      // 3 elements, 0x10 bytes (sizeof)  
/*0x040*/                 VOID*        DriverContext[4];                                                                         
                      };                                                                                                         
/*0x050*/             struct _ETHREAD* Thread;                                                                                   
/*0x054*/             CHAR*        AuxiliaryBuffer;                                                                              
/*0x058*/             struct _LIST_ENTRY ListEntry;                                          // 2 elements, 0x8 bytes (sizeof)   
                      union                                                                  // 2 elements, 0x4 bytes (sizeof)   
                      {                                                                                                          
/*0x060*/                 struct _IO_STACK_LOCATION* CurrentStackLocation;                                                       
/*0x060*/                 ULONG32      PacketType;                                                                               
                      };                                                                                                         
/*0x064*/             struct _FILE_OBJECT* OriginalFileObject;                                                                   
                  }Overlay;                                                                                                      
/*0x040*/         struct _KAPC Apc;                                                          // 14 elements, 0x30 bytes (sizeof) 
/*0x040*/         VOID*        CompletionKey;                                                                                    
              }Tail;                                                                                                             
          }IRP, *PIRP;                                                                                                           
                                                                                                                                 
          typedef struct _KiIoAccessMap      // 2 elements, 0x2024 bytes (sizeof) 
          {                                                                       
/*0x000*/     UINT8        DirectionMap[32];                                      
/*0x020*/     UINT8        IoMap[8196];                                           
          }KiIoAccessMap, *PKiIoAccessMap;                                        
                                                                                  
          typedef struct _KINTERRUPT                                                                 // 20 elements, 0x1E4 bytes (sizeof) 
          {                                                                                                                               
/*0x000*/     INT16        Type;                                                                                                          
/*0x002*/     INT16        Size;                                                                                                          
/*0x004*/     struct _LIST_ENTRY InterruptListEntry;                                                 // 2 elements, 0x8 bytes (sizeof)    
/*0x00C*/     FUNCT_003D_15EE_ServiceRoutine* ServiceRoutine;                                                                             
/*0x010*/     VOID*        ServiceContext;                                                                                                
/*0x014*/     ULONG32      SpinLock;                                                                                                      
/*0x018*/     ULONG32      TickCount;                                                                                                     
/*0x01C*/     ULONG32*     ActualLock;                                                                                                    
/*0x020*/     FUNCT_00BC_0631_PostProcessInitRoutine_DispatchAddress_FinishRoutine* DispatchAddress;                                      
/*0x024*/     ULONG32      Vector;                                                                                                        
/*0x028*/     UINT8        Irql;                                                                                                          
/*0x029*/     UINT8        SynchronizeIrql;                                                                                               
/*0x02A*/     UINT8        FloatingSave;                                                                                                  
/*0x02B*/     UINT8        Connected;                                                                                                     
/*0x02C*/     CHAR         Number;                                                                                                        
/*0x02D*/     UINT8        ShareVector;                                                                                                   
/*0x02E*/     UINT8        _PADDING0_[0x2];                                                                                               
/*0x030*/     enum _KINTERRUPT_MODE Mode;                                                                                                 
/*0x034*/     ULONG32      ServiceCount;                                                                                                  
/*0x038*/     ULONG32      DispatchCount;                                                                                                 
/*0x03C*/     ULONG32      DispatchCode[106];                                                                                             
          }KINTERRUPT, *PKINTERRUPT;                                                                                                      
                                                                                                                                          
          typedef struct _KSPIN_LOCK_QUEUE    // 2 elements, 0x8 bytes (sizeof) 
          {                                                                     
/*0x000*/     struct _KSPIN_LOCK_QUEUE* Next;                                   
/*0x004*/     ULONG32*     Lock;                                                
          }KSPIN_LOCK_QUEUE, *PKSPIN_LOCK_QUEUE;                                  
                                                                                
          typedef struct _KLOCK_QUEUE_HANDLE      // 2 elements, 0xC bytes (sizeof) 
          {                                                                         
/*0x000*/     struct _KSPIN_LOCK_QUEUE LockQueue; // 2 elements, 0x8 bytes (sizeof) 
/*0x008*/     UINT8        OldIrql;                                                 
/*0x009*/     UINT8        _PADDING0_[0x3];                                         
          }KLOCK_QUEUE_HANDLE, *PKLOCK_QUEUE_HANDLE;                                  
                                                                                    
          typedef struct _KMUTANT                 // 5 elements, 0x20 bytes (sizeof) 
          {                                                                          
/*0x000*/     struct _DISPATCHER_HEADER Header;   // 6 elements, 0x10 bytes (sizeof) 
/*0x010*/     struct _LIST_ENTRY MutantListEntry; // 2 elements, 0x8 bytes (sizeof)  
/*0x018*/     struct _KTHREAD* OwnerThread;                                          
/*0x01C*/     UINT8        Abandoned;                                                
/*0x01D*/     UINT8        ApcDisable;                                               
/*0x01E*/     UINT8        _PADDING0_[0x2];                                          
          }KMUTANT, *PKMUTANT;                                                       
                                                                                     
          typedef struct _KNODE                            // 9 elements, 0x30 bytes (sizeof) 
          {                                                                                   
/*0x000*/     ULONG32      ProcessorMask;                                                     
/*0x004*/     ULONG32      Color;                                                             
/*0x008*/     ULONG32      MmShiftedColor;                                                    
/*0x00C*/     ULONG32      FreeCount[2];                                                      
/*0x014*/     UINT8        _PADDING0_[0x4];                                                   
/*0x018*/     union _SLIST_HEADER DeadStackList;           // 4 elements, 0x8 bytes (sizeof)  
/*0x020*/     union _SLIST_HEADER PfnDereferenceSListHead; // 4 elements, 0x8 bytes (sizeof)  
/*0x028*/     struct _SINGLE_LIST_ENTRY* PfnDeferredList;                                     
/*0x02C*/     UINT8        Seed;                                                              
/*0x02D*/     struct _flags Flags;                         // 1 elements, 0x1 bytes (sizeof)  
/*0x02E*/     UINT8        _PADDING1_[0x2];                                                   
          }KNODE, *PKNODE;                                                                    
                                                                                              
          typedef struct _NT_TIB                                    // 8 elements, 0x1C bytes (sizeof) 
          {                                                                                            
/*0x000*/     struct _EXCEPTION_REGISTRATION_RECORD* ExceptionList;                                    
/*0x004*/     VOID*        StackBase;                                                                  
/*0x008*/     VOID*        StackLimit;                                                                 
/*0x00C*/     VOID*        SubSystemTib;                                                               
              union                                                 // 2 elements, 0x4 bytes (sizeof)  
              {                                                                                        
/*0x010*/         VOID*        FiberData;                                                              
/*0x010*/         ULONG32      Version;                                                                
              };                                                                                       
/*0x014*/     VOID*        ArbitraryUserPointer;                                                       
/*0x018*/     struct _NT_TIB* Self;                                                                    
          }NT_TIB, *PNT_TIB;                                                                           
                                                                                                       
          typedef struct _KSPECIAL_REGISTERS // 15 elements, 0x54 bytes (sizeof) 
          {                                                                      
/*0x000*/     ULONG32      Cr0;                                                  
/*0x004*/     ULONG32      Cr2;                                                  
/*0x008*/     ULONG32      Cr3;                                                  
/*0x00C*/     ULONG32      Cr4;                                                  
/*0x010*/     ULONG32      KernelDr0;                                            
/*0x014*/     ULONG32      KernelDr1;                                            
/*0x018*/     ULONG32      KernelDr2;                                            
/*0x01C*/     ULONG32      KernelDr3;                                            
/*0x020*/     ULONG32      KernelDr6;                                            
/*0x024*/     ULONG32      KernelDr7;                                            
/*0x028*/     struct _DESCRIPTOR Gdtr;       // 3 elements, 0x8 bytes (sizeof)   
/*0x030*/     struct _DESCRIPTOR Idtr;       // 3 elements, 0x8 bytes (sizeof)   
/*0x038*/     UINT16       Tr;                                                   
/*0x03A*/     UINT16       Ldtr;                                                 
/*0x03C*/     ULONG32      Reserved[6];                                          
          }KSPECIAL_REGISTERS, *PKSPECIAL_REGISTERS;                                    
                                                                                 
          typedef struct _KPROCESSOR_STATE                 // 2 elements, 0x320 bytes (sizeof)  
          {                                                                                     
/*0x000*/     struct _CONTEXT ContextFrame;                // 25 elements, 0x2CC bytes (sizeof) 
/*0x2CC*/     struct _KSPECIAL_REGISTERS SpecialRegisters; // 15 elements, 0x54 bytes (sizeof)  
          }KPROCESSOR_STATE, *PKPROCESSOR_STATE;                                                
                                                                                                
          typedef struct _PP_LOOKASIDE_LIST // 2 elements, 0x8 bytes (sizeof) 
          {                                                                   
/*0x000*/     struct _GENERAL_LOOKASIDE* P;                                   
/*0x004*/     struct _GENERAL_LOOKASIDE* L;                                   
          }PP_LOOKASIDE_LIST, *PPP_LOOKASIDE_LIST;                                  
                                                                              
          typedef struct _PROCESSOR_IDLE_TIMES     // 3 elements, 0x20 bytes (sizeof) 
          {                                                                           
/*0x000*/     UINT64       StartTime;                                                 
/*0x008*/     UINT64       EndTime;                                                   
/*0x010*/     ULONG32      IdleHandlerReserved[4];                                    
          }PROCESSOR_IDLE_TIMES, *PPROCESSOR_IDLE_TIMES;                                   
                                                                                      
          typedef struct _PROCESSOR_POWER_STATE                 // 45 elements, 0x120 bytes (sizeof) 
          {                                                                                          
/*0x000*/     FUNCT_00BC_02E9_IdleFunction* IdleFunction;                                            
/*0x004*/     ULONG32      Idle0KernelTimeLimit;                                                     
/*0x008*/     ULONG32      Idle0LastTime;                                                            
/*0x00C*/     VOID*        IdleHandlers;                                                             
/*0x010*/     VOID*        IdleState;                                                                
/*0x014*/     ULONG32      IdleHandlersCount;                                                        
/*0x018*/     UINT64       LastCheck;                                                                
/*0x020*/     struct _PROCESSOR_IDLE_TIMES IdleTimes;           // 3 elements, 0x20 bytes (sizeof)   
/*0x040*/     ULONG32      IdleTime1;                                                                
/*0x044*/     ULONG32      PromotionCheck;                                                           
/*0x048*/     ULONG32      IdleTime2;                                                                
/*0x04C*/     UINT8        CurrentThrottle;                                                          
/*0x04D*/     UINT8        ThermalThrottleLimit;                                                     
/*0x04E*/     UINT8        CurrentThrottleIndex;                                                     
/*0x04F*/     UINT8        ThermalThrottleIndex;                                                     
/*0x050*/     ULONG32      LastKernelUserTime;                                                       
/*0x054*/     ULONG32      LastIdleThreadKernelTime;                                                 
/*0x058*/     ULONG32      PackageIdleStartTime;                                                     
/*0x05C*/     ULONG32      PackageIdleTime;                                                          
/*0x060*/     ULONG32      DebugCount;                                                               
/*0x064*/     ULONG32      LastSysTime;                                                              
/*0x068*/     UINT64       TotalIdleStateTime[3];                                                    
/*0x080*/     ULONG32      TotalIdleTransitions[3];                                                  
/*0x08C*/     UINT8        _PADDING0_[0x4];                                                          
/*0x090*/     UINT64       PreviousC3StateTime;                                                      
/*0x098*/     UINT8        KneeThrottleIndex;                                                        
/*0x099*/     UINT8        ThrottleLimitIndex;                                                       
/*0x09A*/     UINT8        PerfStatesCount;                                                          
/*0x09B*/     UINT8        ProcessorMinThrottle;                                                     
/*0x09C*/     UINT8        ProcessorMaxThrottle;                                                     
/*0x09D*/     UINT8        EnableIdleAccounting;                                                     
/*0x09E*/     UINT8        LastC3Percentage;                                                         
/*0x09F*/     UINT8        LastAdjustedBusyPercentage;                                               
/*0x0A0*/     ULONG32      PromotionCount;                                                           
/*0x0A4*/     ULONG32      DemotionCount;                                                            
/*0x0A8*/     ULONG32      ErrorCount;                                                               
/*0x0AC*/     ULONG32      RetryCount;                                                               
/*0x0B0*/     ULONG32      Flags;                                                                    
/*0x0B4*/     UINT8        _PADDING1_[0x4];                                                          
/*0x0B8*/     union _LARGE_INTEGER PerfCounterFrequency;        // 4 elements, 0x8 bytes (sizeof)    
/*0x0C0*/     ULONG32      PerfTickCount;                                                            
/*0x0C4*/     UINT8        _PADDING2_[0x4];                                                          
/*0x0C8*/     struct _KTIMER PerfTimer;                         // 5 elements, 0x28 bytes (sizeof)   
/*0x0F0*/     struct _KDPC PerfDpc;                             // 9 elements, 0x20 bytes (sizeof)   
/*0x110*/     struct _PROCESSOR_PERF_STATE* PerfStates;                                              
/*0x114*/     FUNCT_0049_0301_PerfSetThrottle* PerfSetThrottle;                                      
/*0x118*/     ULONG32      LastC3KernelUserTime;                                                     
/*0x11C*/     ULONG32      LastPackageIdleTime;                                                      
          }PROCESSOR_POWER_STATE, *PPROCESSOR_POWER_STATE;                                           
                                                                                                     
          typedef struct _KPRCB                                    // 91 elements, 0xC50 bytes (sizeof) 
          {                                                                                             
/*0x000*/     UINT16       MinorVersion;                                                                
/*0x002*/     UINT16       MajorVersion;                                                                
/*0x004*/     struct _KTHREAD* CurrentThread;                                                           
/*0x008*/     struct _KTHREAD* NextThread;                                                              
/*0x00C*/     struct _KTHREAD* IdleThread;                                                              
/*0x010*/     CHAR         Number;                                                                      
/*0x011*/     CHAR         Reserved;                                                                    
/*0x012*/     UINT16       BuildType;                                                                   
/*0x014*/     ULONG32      SetMember;                                                                   
/*0x018*/     CHAR         CpuType;                                                                     
/*0x019*/     CHAR         CpuID;                                                                       
/*0x01A*/     UINT16       CpuStep;                                                                     
/*0x01C*/     struct _KPROCESSOR_STATE ProcessorState;             // 2 elements, 0x320 bytes (sizeof)  
/*0x33C*/     ULONG32      KernelReserved[16];                                                          
/*0x37C*/     ULONG32      HalReserved[16];                                                             
/*0x3BC*/     UINT8        PrcbPad0[92];                                                                
/*0x418*/     struct _KSPIN_LOCK_QUEUE LockQueue[16];                                                   
/*0x498*/     UINT8        PrcbPad1[8];                                                                 
/*0x4A0*/     struct _KTHREAD* NpxThread;                                                               
/*0x4A4*/     ULONG32      InterruptCount;                                                              
/*0x4A8*/     ULONG32      KernelTime;                                                                  
/*0x4AC*/     ULONG32      UserTime;                                                                    
/*0x4B0*/     ULONG32      DpcTime;                                                                     
/*0x4B4*/     ULONG32      DebugDpcTime;                                                                
/*0x4B8*/     ULONG32      InterruptTime;                                                               
/*0x4BC*/     ULONG32      AdjustDpcThreshold;                                                          
/*0x4C0*/     ULONG32      PageColor;                                                                   
/*0x4C4*/     ULONG32      SkipTick;                                                                    
/*0x4C8*/     UINT8        MultiThreadSetBusy;                                                          
/*0x4C9*/     UINT8        Spare2[3];                                                                   
/*0x4CC*/     struct _KNODE* ParentNode;                                                                
/*0x4D0*/     ULONG32      MultiThreadProcessorSet;                                                     
/*0x4D4*/     struct _KPRCB* MultiThreadSetMaster;                                                      
/*0x4D8*/     ULONG32      ThreadStartCount[2];                                                         
/*0x4E0*/     ULONG32      CcFastReadNoWait;                                                            
/*0x4E4*/     ULONG32      CcFastReadWait;                                                              
/*0x4E8*/     ULONG32      CcFastReadNotPossible;                                                       
/*0x4EC*/     ULONG32      CcCopyReadNoWait;                                                            
/*0x4F0*/     ULONG32      CcCopyReadWait;                                                              
/*0x4F4*/     ULONG32      CcCopyReadNoWaitMiss;                                                        
/*0x4F8*/     ULONG32      KeAlignmentFixupCount;                                                       
/*0x4FC*/     ULONG32      KeContextSwitches;                                                           
/*0x500*/     ULONG32      KeDcacheFlushCount;                                                          
/*0x504*/     ULONG32      KeExceptionDispatchCount;                                                    
/*0x508*/     ULONG32      KeFirstLevelTbFills;                                                         
/*0x50C*/     ULONG32      KeFloatingEmulationCount;                                                    
/*0x510*/     ULONG32      KeIcacheFlushCount;                                                          
/*0x514*/     ULONG32      KeSecondLevelTbFills;                                                        
/*0x518*/     ULONG32      KeSystemCalls;                                                               
/*0x51C*/     ULONG32      SpareCounter0[1];                                                            
/*0x520*/     struct _PP_LOOKASIDE_LIST PPLookasideList[16];                                            
/*0x5A0*/     struct _PP_LOOKASIDE_LIST PPNPagedLookasideList[32];                                      
/*0x6A0*/     struct _PP_LOOKASIDE_LIST PPPagedLookasideList[32];                                       
/*0x7A0*/     ULONG32      PacketBarrier;                                                               
/*0x7A4*/     ULONG32      ReverseStall;                                                                
/*0x7A8*/     VOID*        IpiFrame;                                                                    
/*0x7AC*/     UINT8        PrcbPad2[52];                                                                
/*0x7E0*/     VOID*        CurrentPacket[3];                                                            
/*0x7EC*/     ULONG32      TargetSet;                                                                   
/*0x7F0*/     FUNCT_00BC_028D_WorkerRoutine* WorkerRoutine;                                             
/*0x7F4*/     ULONG32      IpiFrozen;                                                                   
/*0x7F8*/     UINT8        PrcbPad3[40];                                                                
/*0x820*/     ULONG32      RequestSummary;                                                              
/*0x824*/     struct _KPRCB* SignalDone;                                                                
/*0x828*/     UINT8        PrcbPad4[56];                                                                
/*0x860*/     struct _LIST_ENTRY DpcListHead;                      // 2 elements, 0x8 bytes (sizeof)    
/*0x868*/     VOID*        DpcStack;                                                                    
/*0x86C*/     ULONG32      DpcCount;                                                                    
/*0x870*/     ULONG32      DpcQueueDepth;                                                               
/*0x874*/     ULONG32      DpcRoutineActive;                                                            
/*0x878*/     ULONG32      DpcInterruptRequested;                                                       
/*0x87C*/     ULONG32      DpcLastCount;                                                                
/*0x880*/     ULONG32      DpcRequestRate;                                                              
/*0x884*/     ULONG32      MaximumDpcQueueDepth;                                                        
/*0x888*/     ULONG32      MinimumDpcRate;                                                              
/*0x88C*/     ULONG32      QuantumEnd;                                                                  
/*0x890*/     UINT8        PrcbPad5[16];                                                                
/*0x8A0*/     ULONG32      DpcLock;                                                                     
/*0x8A4*/     UINT8        PrcbPad6[28];                                                                
/*0x8C0*/     struct _KDPC CallDpc;                                // 9 elements, 0x20 bytes (sizeof)   
/*0x8E0*/     VOID*        ChainedInterruptList;                                                        
/*0x8E4*/     LONG32       LookasideIrpFloat;                                                           
/*0x8E8*/     ULONG32      SpareFields0[6];                                                             
/*0x900*/     UINT8        VendorString[13];                                                            
/*0x90D*/     UINT8        InitialApicId;                                                               
/*0x90E*/     UINT8        LogicalProcessorsPerPhysicalProcessor;                                       
/*0x90F*/     UINT8        _PADDING0_[0x1];                                                             
/*0x910*/     ULONG32      MHz;                                                                         
/*0x914*/     ULONG32      FeatureBits;                                                                 
/*0x918*/     union _LARGE_INTEGER UpdateSignature;                // 4 elements, 0x8 bytes (sizeof)    
/*0x920*/     struct _FX_SAVE_AREA NpxSaveArea;                    // 3 elements, 0x210 bytes (sizeof)  
/*0xB30*/     struct _PROCESSOR_POWER_STATE PowerState;            // 45 elements, 0x120 bytes (sizeof) 
          }KPRCB, *PKPRCB;                                                                              
                                                                                                        
          typedef struct _KPCR                            // 27 elements, 0xD70 bytes (sizeof) 
          {                                                                                    
/*0x000*/     struct _NT_TIB NtTib;                       // 8 elements, 0x1C bytes (sizeof)   
/*0x01C*/     struct _KPCR* SelfPcr;                                                           
/*0x020*/     struct _KPRCB* Prcb;                                                             
/*0x024*/     UINT8        Irql;                                                               
/*0x025*/     UINT8        _PADDING0_[0x3];                                                    
/*0x028*/     ULONG32      IRR;                                                                
/*0x02C*/     ULONG32      IrrActive;                                                          
/*0x030*/     ULONG32      IDR;                                                                
/*0x034*/     VOID*        KdVersionBlock;                                                     
/*0x038*/     struct _KIDTENTRY* IDT;                                                          
/*0x03C*/     struct _KGDTENTRY* GDT;                                                          
/*0x040*/     struct _KTSS* TSS;                                                               
/*0x044*/     UINT16       MajorVersion;                                                       
/*0x046*/     UINT16       MinorVersion;                                                       
/*0x048*/     ULONG32      SetMember;                                                          
/*0x04C*/     ULONG32      StallScaleFactor;                                                   
/*0x050*/     UINT8        DebugActive;                                                        
/*0x051*/     UINT8        Number;                                                             
/*0x052*/     UINT8        Spare0;                                                             
/*0x053*/     UINT8        SecondLevelCacheAssociativity;                                      
/*0x054*/     ULONG32      VdmAlert;                                                           
/*0x058*/     ULONG32      KernelReserved[14];                                                 
/*0x090*/     ULONG32      SecondLevelCacheSize;                                               
/*0x094*/     ULONG32      HalReserved[16];                                                    
/*0x0D4*/     ULONG32      InterruptMode;                                                      
/*0x0D8*/     UINT8        Spare1;                                                             
/*0x0D9*/     UINT8        _PADDING1_[0x3];                                                    
/*0x0DC*/     ULONG32      KernelReserved2[17];                                                
/*0x120*/     struct _KPRCB PrcbData;                     // 91 elements, 0xC50 bytes (sizeof) 
          }KPCR, *PKPCR;                                                                       
                                                                                               
          typedef struct _KSYSTEM_TIME // 3 elements, 0xC bytes (sizeof) 
          {                                                              
/*0x000*/     ULONG32      LowPart;                                      
/*0x004*/     LONG32       High1Time;                                    
/*0x008*/     LONG32       High2Time;                                    
          }KSYSTEM_TIME, *PKSYSTEM_TIME;                                  
                                                                         
          typedef struct _KTRAP_FRAME                               // 35 elements, 0x8C bytes (sizeof) 
          {                                                                                             
/*0x000*/     ULONG32      DbgEbp;                                                                      
/*0x004*/     ULONG32      DbgEip;                                                                      
/*0x008*/     ULONG32      DbgArgMark;                                                                  
/*0x00C*/     ULONG32      DbgArgPointer;                                                               
/*0x010*/     ULONG32      TempSegCs;                                                                   
/*0x014*/     ULONG32      TempEsp;                                                                     
/*0x018*/     ULONG32      Dr0;                                                                         
/*0x01C*/     ULONG32      Dr1;                                                                         
/*0x020*/     ULONG32      Dr2;                                                                         
/*0x024*/     ULONG32      Dr3;                                                                         
/*0x028*/     ULONG32      Dr6;                                                                         
/*0x02C*/     ULONG32      Dr7;                                                                         
/*0x030*/     ULONG32      SegGs;                                                                       
/*0x034*/     ULONG32      SegEs;                                                                       
/*0x038*/     ULONG32      SegDs;                                                                       
/*0x03C*/     ULONG32      Edx;                                                                         
/*0x040*/     ULONG32      Ecx;                                                                         
/*0x044*/     ULONG32      Eax;                                                                         
/*0x048*/     ULONG32      PreviousPreviousMode;                                                        
/*0x04C*/     struct _EXCEPTION_REGISTRATION_RECORD* ExceptionList;                                     
/*0x050*/     ULONG32      SegFs;                                                                       
/*0x054*/     ULONG32      Edi;                                                                         
/*0x058*/     ULONG32      Esi;                                                                         
/*0x05C*/     ULONG32      Ebx;                                                                         
/*0x060*/     ULONG32      Ebp;                                                                         
/*0x064*/     ULONG32      ErrCode;                                                                     
/*0x068*/     ULONG32      Eip;                                                                         
/*0x06C*/     ULONG32      SegCs;                                                                       
/*0x070*/     ULONG32      EFlags;                                                                      
/*0x074*/     ULONG32      HardwareEsp;                                                                 
/*0x078*/     ULONG32      HardwareSegSs;                                                               
/*0x07C*/     ULONG32      V86Es;                                                                       
/*0x080*/     ULONG32      V86Ds;                                                                       
/*0x084*/     ULONG32      V86Fs;                                                                       
/*0x088*/     ULONG32      V86Gs;                                                                       
          }KTRAP_FRAME, *PKTRAP_FRAME;                                                                  
                                                                                                        
           typedef struct _KTSS                  // 35 elements, 0x20AC bytes (sizeof) 
           {                                                                           
/*0x000*/      UINT16       Backlink;                                                  
/*0x002*/      UINT16       Reserved0;                                                 
/*0x004*/      ULONG32      Esp0;                                                      
/*0x008*/      UINT16       Ss0;                                                       
/*0x00A*/      UINT16       Reserved1;                                                 
/*0x00C*/      ULONG32      NotUsed1[4];                                               
/*0x01C*/      ULONG32      CR3;                                                       
/*0x020*/      ULONG32      Eip;                                                       
/*0x024*/      ULONG32      EFlags;                                                    
/*0x028*/      ULONG32      Eax;                                                       
/*0x02C*/      ULONG32      Ecx;                                                       
/*0x030*/      ULONG32      Edx;                                                       
/*0x034*/      ULONG32      Ebx;                                                       
/*0x038*/      ULONG32      Esp;                                                       
/*0x03C*/      ULONG32      Ebp;                                                       
/*0x040*/      ULONG32      Esi;                                                       
/*0x044*/      ULONG32      Edi;                                                       
/*0x048*/      UINT16       Es;                                                        
/*0x04A*/      UINT16       Reserved2;                                                 
/*0x04C*/      UINT16       Cs;                                                        
/*0x04E*/      UINT16       Reserved3;                                                 
/*0x050*/      UINT16       Ss;                                                        
/*0x052*/      UINT16       Reserved4;                                                 
/*0x054*/      UINT16       Ds;                                                        
/*0x056*/      UINT16       Reserved5;                                                 
/*0x058*/      UINT16       Fs;                                                        
/*0x05A*/      UINT16       Reserved6;                                                 
/*0x05C*/      UINT16       Gs;                                                        
/*0x05E*/      UINT16       Reserved7;                                                 
/*0x060*/      UINT16       LDT;                                                       
/*0x062*/      UINT16       Reserved8;                                                 
/*0x064*/      UINT16       Flags;                                                     
/*0x066*/      UINT16       IoMapBase;                                                 
/*0x068*/      struct _KiIoAccessMap IoMaps[1];                                        
/*0x208C*/     UINT8        IntDirectionMap[32];                                       
           }KTSS, *PKTSS;                                                              
                                                                                       
          typedef struct _KUSER_SHARED_DATA                                // 39 elements, 0x338 bytes (sizeof) 
          {                                                                                                     
/*0x000*/     ULONG32      TickCountLow;                                                                        
/*0x004*/     ULONG32      TickCountMultiplier;                                                                 
/*0x008*/     struct _KSYSTEM_TIME InterruptTime;                          // 3 elements, 0xC bytes (sizeof)    
/*0x014*/     struct _KSYSTEM_TIME SystemTime;                             // 3 elements, 0xC bytes (sizeof)    
/*0x020*/     struct _KSYSTEM_TIME TimeZoneBias;                           // 3 elements, 0xC bytes (sizeof)    
/*0x02C*/     UINT16       ImageNumberLow;                                                                      
/*0x02E*/     UINT16       ImageNumberHigh;                                                                     
/*0x030*/     UINT16       NtSystemRoot[260];                                                                   
/*0x238*/     ULONG32      MaxStackTraceDepth;                                                                  
/*0x23C*/     ULONG32      CryptoExponent;                                                                      
/*0x240*/     ULONG32      TimeZoneId;                                                                          
/*0x244*/     ULONG32      Reserved2[8];                                                                        
/*0x264*/     enum _NT_PRODUCT_TYPE NtProductType;                                                              
/*0x268*/     UINT8        ProductTypeIsValid;                                                                  
/*0x269*/     UINT8        _PADDING0_[0x3];                                                                     
/*0x26C*/     ULONG32      NtMajorVersion;                                                                      
/*0x270*/     ULONG32      NtMinorVersion;                                                                      
/*0x274*/     UINT8        ProcessorFeatures[64];                                                               
/*0x2B4*/     ULONG32      Reserved1;                                                                           
/*0x2B8*/     ULONG32      Reserved3;                                                                           
/*0x2BC*/     ULONG32      TimeSlip;                                                                            
/*0x2C0*/     enum _ALTERNATIVE_ARCHITECTURE_TYPE AlternativeArchitecture;                                      
/*0x2C4*/     UINT8        _PADDING1_[0x4];                                                                     
/*0x2C8*/     union _LARGE_INTEGER SystemExpirationDate;                   // 4 elements, 0x8 bytes (sizeof)    
/*0x2D0*/     ULONG32      SuiteMask;                                                                           
/*0x2D4*/     UINT8        KdDebuggerEnabled;                                                                   
/*0x2D5*/     UINT8        NXSupportPolicy;                                                                     
/*0x2D6*/     UINT8        _PADDING2_[0x2];                                                                     
/*0x2D8*/     ULONG32      ActiveConsoleId;                                                                     
/*0x2DC*/     ULONG32      DismountCount;                                                                       
/*0x2E0*/     ULONG32      ComPlusPackage;                                                                      
/*0x2E4*/     ULONG32      LastSystemRITEventTickCount;                                                         
/*0x2E8*/     ULONG32      NumberOfPhysicalPages;                                                               
/*0x2EC*/     UINT8        SafeBootMode;                                                                        
/*0x2ED*/     UINT8        _PADDING3_[0x3];                                                                     
/*0x2F0*/     ULONG32      TraceLogging;                                                                        
/*0x2F4*/     UINT8        _PADDING4_[0x4];                                                                     
/*0x2F8*/     UINT64       TestRetInstruction;                                                                  
/*0x300*/     ULONG32      SystemCall;                                                                          
/*0x304*/     ULONG32      SystemCallReturn;                                                                    
/*0x308*/     UINT64       SystemCallPad[3];                                                                    
              union                                                        // 2 elements, 0x10 bytes (sizeof)   
              {                                                                                                 
/*0x320*/         struct _KSYSTEM_TIME TickCount;                          // 3 elements, 0xC bytes (sizeof)    
/*0x320*/         UINT64       TickCountQuad;                                                                   
              };                                                                                                
/*0x330*/     ULONG32      Cookie;                                                                              
/*0x334*/     UINT8        _PADDING5_[0x4];                                                                     
          }KUSER_SHARED_DATA, *PKUSER_SHARED_DATA;                                                              
                                                                                                                
          typedef struct _LARGE_CONTROL_AREA             // 16 elements, 0x40 bytes (sizeof) 
          {                                                                                  
/*0x000*/     struct _SEGMENT* Segment;                                                      
/*0x004*/     struct _LIST_ENTRY DereferenceList;        // 2 elements, 0x8 bytes (sizeof)   
/*0x00C*/     ULONG32      NumberOfSectionReferences;                                        
/*0x010*/     ULONG32      NumberOfPfnReferences;                                            
/*0x014*/     ULONG32      NumberOfMappedViews;                                              
/*0x018*/     UINT16       NumberOfSubsections;                                              
/*0x01A*/     UINT16       FlushInProgressCount;                                             
/*0x01C*/     ULONG32      NumberOfUserReferences;                                           
              union                                      // 2 elements, 0x4 bytes (sizeof)   
              {                                                                              
/*0x020*/         ULONG32      LongFlags;                                                    
/*0x020*/         struct _MMSECTION_FLAGS Flags;         // 31 elements, 0x4 bytes (sizeof)  
              }u;                                                                            
/*0x024*/     struct _FILE_OBJECT* FilePointer;                                              
/*0x028*/     struct _EVENT_COUNTER* WaitingForDeletion;                                     
/*0x02C*/     UINT16       ModifiedWriteCount;                                               
/*0x02E*/     UINT16       NumberOfSystemCacheViews;                                         
/*0x030*/     ULONG32      StartingFrame;                                                    
/*0x034*/     struct _LIST_ENTRY UserGlobalList;         // 2 elements, 0x8 bytes (sizeof)   
/*0x03C*/     ULONG32      SessionId;                                                        
          }LARGE_CONTROL_AREA, *PLARGE_CONTROL_AREA;                                         
                                                                                             
          typedef struct _LDR_DATA_TABLE_ENTRY               // 18 elements, 0x50 bytes (sizeof) 
          {                                                                                      
/*0x000*/     struct _LIST_ENTRY InLoadOrderLinks;           // 2 elements, 0x8 bytes (sizeof)   
/*0x008*/     struct _LIST_ENTRY InMemoryOrderLinks;         // 2 elements, 0x8 bytes (sizeof)   
/*0x010*/     struct _LIST_ENTRY InInitializationOrderLinks; // 2 elements, 0x8 bytes (sizeof)   
/*0x018*/     VOID*        DllBase;                                                              
/*0x01C*/     VOID*        EntryPoint;                                                           
/*0x020*/     ULONG32      SizeOfImage;                                                          
/*0x024*/     struct _UNICODE_STRING FullDllName;            // 3 elements, 0x8 bytes (sizeof)   
/*0x02C*/     struct _UNICODE_STRING BaseDllName;            // 3 elements, 0x8 bytes (sizeof)   
/*0x034*/     ULONG32      Flags;                                                                
/*0x038*/     UINT16       LoadCount;                                                            
/*0x03A*/     UINT16       TlsIndex;                                                             
              union                                          // 2 elements, 0x8 bytes (sizeof)   
              {                                                                                  
/*0x03C*/         struct _LIST_ENTRY HashLinks;              // 2 elements, 0x8 bytes (sizeof)   
                  struct                                     // 2 elements, 0x8 bytes (sizeof)   
                  {                                                                              
/*0x03C*/             VOID*        SectionPointer;                                               
/*0x040*/             ULONG32      CheckSum;                                                     
                  };                                                                             
              };                                                                                 
              union                                          // 2 elements, 0x4 bytes (sizeof)   
              {                                                                                  
/*0x044*/         ULONG32      TimeDateStamp;                                                    
/*0x044*/         VOID*        LoadedImports;                                                    
              };                                                                                 
/*0x048*/     VOID*        EntryPointActivationContext;                                          
/*0x04C*/     VOID*        PatchInformation;                                                     
          }LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;                                         
                                                                                                 
          typedef struct _LIST_ENTRY32 // 2 elements, 0x8 bytes (sizeof) 
          {                                                              
/*0x000*/     ULONG32      Flink;                                        
/*0x004*/     ULONG32      Blink;                                        
          }LIST_ENTRY32, *PLIST_ENTRY32;                                  
                                                                         
          typedef struct _LIST_ENTRY64 // 2 elements, 0x10 bytes (sizeof) 
          {                                                               
/*0x000*/     UINT64       Flink;                                         
/*0x008*/     UINT64       Blink;                                         
          }LIST_ENTRY64, *PLIST_ENTRY64;                                   
                                                                          
          typedef struct _PORT_MESSAGE             // 7 elements, 0x18 bytes (sizeof) 
          {                                                                           
              union                                // 2 elements, 0x4 bytes (sizeof)  
              {                                                                       
                  struct                           // 2 elements, 0x4 bytes (sizeof)  
                  {                                                                   
/*0x000*/             INT16        DataLength;                                        
/*0x002*/             INT16        TotalLength;                                       
                  }s1;                                                                
/*0x000*/         ULONG32      Length;                                                
              }u1;                                                                    
              union                                // 2 elements, 0x4 bytes (sizeof)  
              {                                                                       
                  struct                           // 2 elements, 0x4 bytes (sizeof)  
                  {                                                                   
/*0x004*/             INT16        Type;                                              
/*0x006*/             INT16        DataInfoOffset;                                    
                  }s2;                                                                
/*0x004*/         ULONG32      ZeroInit;                                              
              }u2;                                                                    
              union                                // 2 elements, 0x8 bytes (sizeof)  
              {                                                                       
/*0x008*/         struct _CLIENT_ID ClientId;      // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/         FLOAT64      DoNotUseThisField;                                     
              };                                                                      
/*0x010*/     ULONG32      MessageId;                                                 
              union                                // 2 elements, 0x4 bytes (sizeof)  
              {                                                                       
/*0x014*/         ULONG32      ClientViewSize;                                        
/*0x014*/         ULONG32      CallbackId;                                            
              };                                                                      
          }PORT_MESSAGE, *PPORT_MESSAGE;                                              
                                                                                      
          typedef struct _LPCP_MESSAGE                     // 7 elements, 0x30 bytes (sizeof) 
          {                                                                                   
              union                                        // 2 elements, 0x8 bytes (sizeof)  
              {                                                                               
/*0x000*/         struct _LIST_ENTRY Entry;                // 2 elements, 0x8 bytes (sizeof)  
                  struct                                   // 2 elements, 0x8 bytes (sizeof)  
                  {                                                                           
/*0x000*/             struct _SINGLE_LIST_ENTRY FreeEntry; // 1 elements, 0x4 bytes (sizeof)  
/*0x004*/             ULONG32      Reserved0;                                                 
                  };                                                                          
              };                                                                              
/*0x008*/     VOID*        SenderPort;                                                        
/*0x00C*/     struct _ETHREAD* RepliedToThread;                                               
/*0x010*/     VOID*        PortContext;                                                       
/*0x014*/     UINT8        _PADDING0_[0x4];                                                   
/*0x018*/     struct _PORT_MESSAGE Request;                // 7 elements, 0x18 bytes (sizeof) 
          }LPCP_MESSAGE, *PLPCP_MESSAGE;                                                      
                                                                                              
          typedef struct _LPCP_NONPAGED_PORT_QUEUE   // 2 elements, 0x18 bytes (sizeof) 
          {                                                                             
/*0x000*/     struct _KSEMAPHORE Semaphore;          // 2 elements, 0x14 bytes (sizeof) 
/*0x014*/     struct _LPCP_PORT_OBJECT* BackPointer;                                    
          }LPCP_NONPAGED_PORT_QUEUE, *PLPCP_NONPAGED_PORT_QUEUE;                                   
                                                                                        
          typedef struct _LPCP_PORT_QUEUE                          // 3 elements, 0x10 bytes (sizeof) 
          {                                                                                           
/*0x000*/     struct _LPCP_NONPAGED_PORT_QUEUE* NonPagedPortQueue;                                    
/*0x004*/     struct _KSEMAPHORE* Semaphore;                                                          
/*0x008*/     struct _LIST_ENTRY ReceiveHead;                      // 2 elements, 0x8 bytes (sizeof)  
          }LPCP_PORT_QUEUE, *PLPCP_PORT_QUEUE;                                                        
                                                                                                      
          typedef struct _SECURITY_QUALITY_OF_SERVICE                // 4 elements, 0xC bytes (sizeof) 
          {                                                                                            
/*0x000*/     ULONG32      Length;                                                                     
/*0x004*/     enum _SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;                                   
/*0x008*/     UINT8        ContextTrackingMode;                                                        
/*0x009*/     UINT8        EffectiveOnly;                                                              
/*0x00A*/     UINT8        _PADDING0_[0x2];                                                            
          }SECURITY_QUALITY_OF_SERVICE, *PSECURITY_QUALITY_OF_SERVICE;                                  
                                                                                                       
          typedef struct _TOKEN_SOURCE       // 2 elements, 0x10 bytes (sizeof) 
          {                                                                     
/*0x000*/     CHAR         SourceName[8];                                       
/*0x008*/     struct _LUID SourceIdentifier; // 2 elements, 0x8 bytes (sizeof)  
          }TOKEN_SOURCE, *PTOKEN_SOURCE;                                        
                                                                                
          typedef struct _TOKEN_CONTROL         // 4 elements, 0x28 bytes (sizeof) 
          {                                                                        
/*0x000*/     struct _LUID TokenId;             // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/     struct _LUID AuthenticationId;    // 2 elements, 0x8 bytes (sizeof)  
/*0x010*/     struct _LUID ModifiedId;          // 2 elements, 0x8 bytes (sizeof)  
/*0x018*/     struct _TOKEN_SOURCE TokenSource; // 2 elements, 0x10 bytes (sizeof) 
          }TOKEN_CONTROL, *PTOKEN_CONTROL;                                         
                                                                                   
          typedef struct _SECURITY_CLIENT_CONTEXT              // 6 elements, 0x3C bytes (sizeof) 
          {                                                                                       
/*0x000*/     struct _SECURITY_QUALITY_OF_SERVICE SecurityQos; // 4 elements, 0xC bytes (sizeof)  
/*0x00C*/     VOID*        ClientToken;                                                           
/*0x010*/     UINT8        DirectlyAccessClientToken;                                             
/*0x011*/     UINT8        DirectAccessEffectiveOnly;                                             
/*0x012*/     UINT8        ServerIsRemote;                                                        
/*0x013*/     UINT8        _PADDING0_[0x1];                                                       
/*0x014*/     struct _TOKEN_CONTROL ClientTokenControl;        // 4 elements, 0x28 bytes (sizeof) 
          }SECURITY_CLIENT_CONTEXT, *PSECURITY_CLIENT_CONTEXT;                                    
                                                                                                  
          typedef struct _LPCP_PORT_OBJECT                     // 18 elements, 0xA4 bytes (sizeof) 
          {                                                                                        
/*0x000*/     struct _LPCP_PORT_OBJECT* ConnectionPort;                                            
/*0x004*/     struct _LPCP_PORT_OBJECT* ConnectedPort;                                             
/*0x008*/     struct _LPCP_PORT_QUEUE MsgQueue;                // 3 elements, 0x10 bytes (sizeof)  
/*0x018*/     struct _CLIENT_ID Creator;                       // 2 elements, 0x8 bytes (sizeof)   
/*0x020*/     VOID*        ClientSectionBase;                                                      
/*0x024*/     VOID*        ServerSectionBase;                                                      
/*0x028*/     VOID*        PortContext;                                                            
/*0x02C*/     struct _ETHREAD* ClientThread;                                                       
/*0x030*/     struct _SECURITY_QUALITY_OF_SERVICE SecurityQos; // 4 elements, 0xC bytes (sizeof)   
/*0x03C*/     struct _SECURITY_CLIENT_CONTEXT StaticSecurity;  // 6 elements, 0x3C bytes (sizeof)  
/*0x078*/     struct _LIST_ENTRY LpcReplyChainHead;            // 2 elements, 0x8 bytes (sizeof)   
/*0x080*/     struct _LIST_ENTRY LpcDataInfoChainHead;         // 2 elements, 0x8 bytes (sizeof)   
              union                                            // 2 elements, 0x4 bytes (sizeof)   
              {                                                                                    
/*0x088*/         struct _EPROCESS* ServerProcess;                                                 
/*0x088*/         struct _EPROCESS* MappingProcess;                                                
              };                                                                                   
/*0x08C*/     UINT16       MaxMessageLength;                                                       
/*0x08E*/     UINT16       MaxConnectionInfoLength;                                                
/*0x090*/     ULONG32      Flags;                                                                  
/*0x094*/     struct _KEVENT WaitEvent;                        // 1 elements, 0x10 bytes (sizeof)  
          }LPCP_PORT_OBJECT, *PLPCP_PORT_OBJECT;                                                   
                                                                                                   
          typedef struct _MAILSLOT_CREATE_PARAMETERS // 4 elements, 0x18 bytes (sizeof) 
          {                                                                             
/*0x000*/     ULONG32      MailslotQuota;                                               
/*0x004*/     ULONG32      MaximumMessageSize;                                          
/*0x008*/     union _LARGE_INTEGER ReadTimeout;      // 4 elements, 0x8 bytes (sizeof)  
/*0x010*/     UINT8        TimeoutSpecified;                                            
/*0x011*/     UINT8        _PADDING0_[0x7];                                             
          }MAILSLOT_CREATE_PARAMETERS, *PMAILSLOT_CREATE_PARAMETERS;                                   
                                                                                        
          typedef struct _MBCB                   // 10 elements, 0x80 bytes (sizeof) 
          {                                                                          
/*0x000*/     INT16        NodeTypeCode;                                             
/*0x002*/     INT16        NodeIsInZone;                                             
/*0x004*/     ULONG32      PagesToWrite;                                             
/*0x008*/     ULONG32      DirtyPages;                                               
/*0x00C*/     ULONG32      Reserved;                                                 
/*0x010*/     struct _LIST_ENTRY BitmapRanges;   // 2 elements, 0x8 bytes (sizeof)   
/*0x018*/     INT64        ResumeWritePage;                                          
/*0x020*/     struct _BITMAP_RANGE BitmapRange1; // 6 elements, 0x20 bytes (sizeof)  
/*0x040*/     struct _BITMAP_RANGE BitmapRange2; // 6 elements, 0x20 bytes (sizeof)  
/*0x060*/     struct _BITMAP_RANGE BitmapRange3; // 6 elements, 0x20 bytes (sizeof)  
          }MBCB, *PMBCB;                                                             
                                                                                     
          typedef struct _MDL              // 8 elements, 0x1C bytes (sizeof) 
          {                                                                   
/*0x000*/     struct _MDL* Next;                                              
/*0x004*/     INT16        Size;                                              
/*0x006*/     INT16        MdlFlags;                                          
/*0x008*/     struct _EPROCESS* Process;                                      
/*0x00C*/     VOID*        MappedSystemVa;                                    
/*0x010*/     VOID*        StartVa;                                           
/*0x014*/     ULONG32      ByteCount;                                         
/*0x018*/     ULONG32      ByteOffset;                                        
          }MDL, *PMDL;                                                        
                                                                              
          typedef struct _MI_VERIFIER_DRIVER_ENTRY         // 22 elements, 0x60 bytes (sizeof) 
          {                                                                                    
/*0x000*/     struct _LIST_ENTRY Links;                    // 2 elements, 0x8 bytes (sizeof)   
/*0x008*/     ULONG32      Loads;                                                              
/*0x00C*/     ULONG32      Unloads;                                                            
/*0x010*/     struct _UNICODE_STRING BaseName;             // 3 elements, 0x8 bytes (sizeof)   
/*0x018*/     VOID*        StartAddress;                                                       
/*0x01C*/     VOID*        EndAddress;                                                         
/*0x020*/     ULONG32      Flags;                                                              
/*0x024*/     ULONG32      Signature;                                                          
/*0x028*/     ULONG32      Reserved;                                                           
/*0x02C*/     ULONG32      VerifierPoolLock;                                                   
/*0x030*/     struct _VI_POOL_ENTRY* PoolHash;                                                 
/*0x034*/     ULONG32      PoolHashSize;                                                       
/*0x038*/     ULONG32      PoolHashFree;                                                       
/*0x03C*/     ULONG32      PoolHashReserved;                                                   
/*0x040*/     ULONG32      CurrentPagedPoolAllocations;                                        
/*0x044*/     ULONG32      CurrentNonPagedPoolAllocations;                                     
/*0x048*/     ULONG32      PeakPagedPoolAllocations;                                           
/*0x04C*/     ULONG32      PeakNonPagedPoolAllocations;                                        
/*0x050*/     ULONG32      PagedBytes;                                                         
/*0x054*/     ULONG32      NonPagedBytes;                                                      
/*0x058*/     ULONG32      PeakPagedBytes;                                                     
/*0x05C*/     ULONG32      PeakNonPagedBytes;                                                  
          }MI_VERIFIER_DRIVER_ENTRY, *PMI_VERIFIER_DRIVER_ENTRY;                                    
                                                                                               
          typedef struct _MI_VERIFIER_POOL_HEADER         // 2 elements, 0x8 bytes (sizeof) 
          {                                                                                 
/*0x000*/     ULONG32      ListIndex;                                                       
/*0x004*/     struct _MI_VERIFIER_DRIVER_ENTRY* Verifier;                                   
          }MI_VERIFIER_POOL_HEADER, *PMI_VERIFIER_POOL_HEADER;                                  
                                                                                            
          typedef struct _MM_DRIVER_VERIFIER_DATA              // 27 elements, 0x70 bytes (sizeof) 
          {                                                                                        
/*0x000*/     ULONG32      Level;                                                                  
/*0x004*/     ULONG32      RaiseIrqls;                                                             
/*0x008*/     ULONG32      AcquireSpinLocks;                                                       
/*0x00C*/     ULONG32      SynchronizeExecutions;                                                  
/*0x010*/     ULONG32      AllocationsAttempted;                                                   
/*0x014*/     ULONG32      AllocationsSucceeded;                                                   
/*0x018*/     ULONG32      AllocationsSucceededSpecialPool;                                        
/*0x01C*/     ULONG32      AllocationsWithNoTag;                                                   
/*0x020*/     ULONG32      TrimRequests;                                                           
/*0x024*/     ULONG32      Trims;                                                                  
/*0x028*/     ULONG32      AllocationsFailed;                                                      
/*0x02C*/     ULONG32      AllocationsFailedDeliberately;                                          
/*0x030*/     ULONG32      Loads;                                                                  
/*0x034*/     ULONG32      Unloads;                                                                
/*0x038*/     ULONG32      UnTrackedPool;                                                          
/*0x03C*/     ULONG32      UserTrims;                                                              
/*0x040*/     ULONG32      CurrentPagedPoolAllocations;                                            
/*0x044*/     ULONG32      CurrentNonPagedPoolAllocations;                                         
/*0x048*/     ULONG32      PeakPagedPoolAllocations;                                               
/*0x04C*/     ULONG32      PeakNonPagedPoolAllocations;                                            
/*0x050*/     ULONG32      PagedBytes;                                                             
/*0x054*/     ULONG32      NonPagedBytes;                                                          
/*0x058*/     ULONG32      PeakPagedBytes;                                                         
/*0x05C*/     ULONG32      PeakNonPagedBytes;                                                      
/*0x060*/     ULONG32      BurstAllocationsFailedDeliberately;                                     
/*0x064*/     ULONG32      SessionTrims;                                                           
/*0x068*/     ULONG32      Reserved[2];                                                            
          }MM_DRIVER_VERIFIER_DATA, *PMM_DRIVER_VERIFIER_DATA;                                     
                                                                                                   
          typedef struct _MM_PAGED_POOL_INFO                          // 9 elements, 0x24 bytes (sizeof) 
          {                                                                                              
/*0x000*/     struct _RTL_BITMAP* PagedPoolAllocationMap;                                                
/*0x004*/     struct _RTL_BITMAP* EndOfPagedPoolBitmap;                                                  
/*0x008*/     struct _RTL_BITMAP* PagedPoolLargeSessionAllocationMap;                                    
/*0x00C*/     struct _MMPTE* FirstPteForPagedPool;                                                       
/*0x010*/     struct _MMPTE* LastPteForPagedPool;                                                        
/*0x014*/     struct _MMPTE* NextPdeForPagedPoolExpansion;                                               
/*0x018*/     ULONG32      PagedPoolHint;                                                                
/*0x01C*/     ULONG32      PagedPoolCommit;                                                              
/*0x020*/     ULONG32      AllocatedPagedPool;                                                           
          }MM_PAGED_POOL_INFO, *PMM_PAGED_POOL_INFO;                                                     
                                                                                                         
          typedef struct _MM_SESSION_SPACE_FLAGS // 5 elements, 0x4 bytes (sizeof) 
          {                                                                        
/*0x000*/     ULONG32      Initialized : 1;      // 0 BitPosition                  
/*0x000*/     ULONG32      Filler0 : 3;          // 1 BitPosition                  
/*0x000*/     ULONG32      HasWsLock : 1;        // 4 BitPosition                  
/*0x000*/     ULONG32      DeletePending : 1;    // 5 BitPosition                  
/*0x000*/     ULONG32      Filler : 26;          // 6 BitPosition                  
          }MM_SESSION_SPACE_FLAGS, *PMM_SESSION_SPACE_FLAGS;                                  
                                                                                   
          typedef struct _MMSESSION                           // 8 elements, 0x3C bytes (sizeof) 
          {                                                                                      
/*0x000*/     struct _FAST_MUTEX SystemSpaceViewLock;         // 5 elements, 0x20 bytes (sizeof) 
/*0x020*/     struct _FAST_MUTEX* SystemSpaceViewLockPointer;                                    
/*0x024*/     CHAR*        SystemSpaceViewStart;                                                 
/*0x028*/     struct _MMVIEW* SystemSpaceViewTable;                                              
/*0x02C*/     ULONG32      SystemSpaceHashSize;                                                  
/*0x030*/     ULONG32      SystemSpaceHashEntries;                                               
/*0x034*/     ULONG32      SystemSpaceHashKey;                                                   
/*0x038*/     struct _RTL_BITMAP* SystemSpaceBitMap;                                             
          }MMSESSION, *PMMSESSION;                                                               
                                                                                                 
          typedef struct _POOL_DESCRIPTOR        // 11 elements, 0x1028 bytes (sizeof) 
          {                                                                            
/*0x000*/     enum _POOL_TYPE PoolType;                                                
/*0x004*/     ULONG32      PoolIndex;                                                  
/*0x008*/     ULONG32      RunningAllocs;                                              
/*0x00C*/     ULONG32      RunningDeAllocs;                                            
/*0x010*/     ULONG32      TotalPages;                                                 
/*0x014*/     ULONG32      TotalBigPages;                                              
/*0x018*/     ULONG32      Threshold;                                                  
/*0x01C*/     VOID*        LockAddress;                                                
/*0x020*/     VOID*        PendingFrees;                                               
/*0x024*/     LONG32       PendingFreeDepth;                                           
/*0x028*/     struct _LIST_ENTRY ListHeads[512];                                       
          }POOL_DESCRIPTOR, *PPOOL_DESCRIPTOR;                                         
                                                                                       
           typedef struct _MM_SESSION_SPACE                    // 38 elements, 0x1278 bytes (sizeof) 
           {                                                                                         
/*0x000*/      ULONG32      ReferenceCount;                                                          
               union                                           // 2 elements, 0x4 bytes (sizeof)     
               {                                                                                     
/*0x004*/          ULONG32      LongFlags;                                                           
/*0x004*/          struct _MM_SESSION_SPACE_FLAGS Flags;       // 5 elements, 0x4 bytes (sizeof)     
               }u;                                                                                   
/*0x008*/      ULONG32      SessionId;                                                               
/*0x00C*/      ULONG32      SessionPageDirectoryIndex;                                               
/*0x010*/      struct _MM_SESSION_SPACE* GlobalVirtualAddress;                                       
/*0x014*/      struct _LIST_ENTRY ProcessList;                 // 2 elements, 0x8 bytes (sizeof)     
/*0x01C*/      ULONG32      NonPagedPoolBytes;                                                       
/*0x020*/      ULONG32      PagedPoolBytes;                                                          
/*0x024*/      ULONG32      NonPagedPoolAllocations;                                                 
/*0x028*/      ULONG32      PagedPoolAllocations;                                                    
/*0x02C*/      ULONG32      NonPagablePages;                                                         
/*0x030*/      ULONG32      CommittedPages;                                                          
/*0x034*/      UINT8        _PADDING0_[0x4];                                                         
/*0x038*/      union _LARGE_INTEGER LastProcessSwappedOutTime; // 4 elements, 0x8 bytes (sizeof)     
/*0x040*/      struct _MMPTE* PageTables;                                                            
/*0x044*/      struct _FAST_MUTEX PagedPoolMutex;              // 5 elements, 0x20 bytes (sizeof)    
/*0x064*/      VOID*        PagedPoolStart;                                                          
/*0x068*/      VOID*        PagedPoolEnd;                                                            
/*0x06C*/      struct _MMPTE* PagedPoolBasePde;                                                      
/*0x070*/      struct _MM_PAGED_POOL_INFO PagedPoolInfo;       // 9 elements, 0x24 bytes (sizeof)    
/*0x094*/      ULONG32      Color;                                                                   
/*0x098*/      ULONG32      ProcessOutSwapCount;                                                     
/*0x09C*/      struct _LIST_ENTRY ImageList;                   // 2 elements, 0x8 bytes (sizeof)     
/*0x0A4*/      struct _MMPTE* GlobalPteEntry;                                                        
/*0x0A8*/      ULONG32      CopyOnWriteCount;                                                        
/*0x0AC*/      ULONG32      SessionPoolAllocationFailures[4];                                        
/*0x0BC*/      ULONG32      AttachCount;                                                             
/*0x0C0*/      struct _KEVENT AttachEvent;                     // 1 elements, 0x10 bytes (sizeof)    
/*0x0D0*/      struct _EPROCESS* LastProcess;                                                        
/*0x0D4*/      UINT8        _PADDING1_[0x4];                                                         
/*0x0D8*/      struct _MMSUPPORT Vm;                           // 14 elements, 0x40 bytes (sizeof)   
/*0x118*/      struct _MMWSLE* Wsle;                                                                 
/*0x11C*/      struct _ERESOURCE WsLock;                       // 13 elements, 0x38 bytes (sizeof)   
/*0x154*/      struct _LIST_ENTRY WsListEntry;                 // 2 elements, 0x8 bytes (sizeof)     
/*0x15C*/      struct _MMSESSION Session;                      // 8 elements, 0x3C bytes (sizeof)    
/*0x198*/      struct _DRIVER_OBJECT Win32KDriverObject;       // 15 elements, 0xA8 bytes (sizeof)   
/*0x240*/      struct _ETHREAD* WorkingSetLockOwner;                                                 
/*0x244*/      struct _POOL_DESCRIPTOR PagedPool;              // 11 elements, 0x1028 bytes (sizeof) 
/*0x126C*/     LONG32       ProcessReferenceToSession;                                               
/*0x1270*/     ULONG32      LocaleId;                                                                
/*0x1274*/     UINT8        _PADDING2_[0x4];                                                         
           }MM_SESSION_SPACE, *PMM_SESSION_SPACE;                                                    
                                                                                                     
          typedef struct _MMADDRESS_LIST // 2 elements, 0x8 bytes (sizeof) 
          {                                                                
/*0x000*/     ULONG32      StartVpn;                                       
/*0x004*/     ULONG32      EndVpn;                                         
          }MMADDRESS_LIST, *PMMADDRESS_LIST;                                  
                                                                           
          typedef struct _MMPTE_HIGHLOW // 2 elements, 0x8 bytes (sizeof) 
          {                                                               
/*0x000*/     ULONG32      LowPart;                                       
/*0x004*/     ULONG32      HighPart;                                      
          }MMPTE_HIGHLOW, *PMMPTE_HIGHLOW;                                  
                                                                          
          typedef struct _MMPTE_HARDWARE         // 14 elements, 0x8 bytes (sizeof) 
          {                                                                         
/*0x000*/     UINT64       Valid : 1;            // 0 BitPosition                   
/*0x000*/     UINT64       Writable : 1;         // 1 BitPosition                   
/*0x000*/     UINT64       Owner : 1;            // 2 BitPosition                   
/*0x000*/     UINT64       WriteThrough : 1;     // 3 BitPosition                   
/*0x000*/     UINT64       CacheDisable : 1;     // 4 BitPosition                   
/*0x000*/     UINT64       Accessed : 1;         // 5 BitPosition                   
/*0x000*/     UINT64       Dirty : 1;            // 6 BitPosition                   
/*0x000*/     UINT64       LargePage : 1;        // 7 BitPosition                   
/*0x000*/     UINT64       Global : 1;           // 8 BitPosition                   
/*0x000*/     UINT64       CopyOnWrite : 1;      // 9 BitPosition                   
/*0x000*/     UINT64       Prototype : 1;        // 10 BitPosition                  
/*0x000*/     UINT64       Write : 1;            // 11 BitPosition                  
/*0x000*/     UINT64       PageFrameNumber : 26; // 12 BitPosition                  
/*0x000*/     UINT64       reserved1 : 26;       // 38 BitPosition                  
          }MMPTE_HARDWARE, *PMMPTE_HARDWARE;                                        
                                                                                    
          typedef struct _MMPTE_PROTOTYPE     // 8 elements, 0x8 bytes (sizeof) 
          {                                                                     
/*0x000*/     UINT64       Valid : 1;         // 0 BitPosition                  
/*0x000*/     UINT64       Unused0 : 7;       // 1 BitPosition                  
/*0x000*/     UINT64       ReadOnly : 1;      // 8 BitPosition                  
/*0x000*/     UINT64       Unused1 : 1;       // 9 BitPosition                  
/*0x000*/     UINT64       Prototype : 1;     // 10 BitPosition                 
/*0x000*/     UINT64       Protection : 5;    // 11 BitPosition                 
/*0x000*/     UINT64       Unused : 16;       // 16 BitPosition                 
/*0x000*/     UINT64       ProtoAddress : 32; // 32 BitPosition                 
          }MMPTE_PROTOTYPE, *PMMPTE_PROTOTYPE;                                  
                                                                                
          typedef struct _MMPTE_SOFTWARE      // 7 elements, 0x8 bytes (sizeof) 
          {                                                                     
/*0x000*/     UINT64       Valid : 1;         // 0 BitPosition                  
/*0x000*/     UINT64       PageFileLow : 4;   // 1 BitPosition                  
/*0x000*/     UINT64       Protection : 5;    // 5 BitPosition                  
/*0x000*/     UINT64       Prototype : 1;     // 10 BitPosition                 
/*0x000*/     UINT64       Transition : 1;    // 11 BitPosition                 
/*0x000*/     UINT64       Unused : 20;       // 12 BitPosition                 
/*0x000*/     UINT64       PageFileHigh : 32; // 32 BitPosition                 
          }MMPTE_SOFTWARE, *PMMPTE_SOFTWARE;                                    
                                                                                
          typedef struct _MMPTE_TRANSITION       // 10 elements, 0x8 bytes (sizeof) 
          {                                                                         
/*0x000*/     UINT64       Valid : 1;            // 0 BitPosition                   
/*0x000*/     UINT64       Write : 1;            // 1 BitPosition                   
/*0x000*/     UINT64       Owner : 1;            // 2 BitPosition                   
/*0x000*/     UINT64       WriteThrough : 1;     // 3 BitPosition                   
/*0x000*/     UINT64       CacheDisable : 1;     // 4 BitPosition                   
/*0x000*/     UINT64       Protection : 5;       // 5 BitPosition                   
/*0x000*/     UINT64       Prototype : 1;        // 10 BitPosition                  
/*0x000*/     UINT64       Transition : 1;       // 11 BitPosition                  
/*0x000*/     UINT64       PageFrameNumber : 26; // 12 BitPosition                  
/*0x000*/     UINT64       Unused : 26;          // 38 BitPosition                  
          }MMPTE_TRANSITION, *PMMPTE_TRANSITION;                                    
                                                                                    
          typedef struct _MMPTE_SUBSECTION         // 6 elements, 0x8 bytes (sizeof) 
          {                                                                          
/*0x000*/     UINT64       Valid : 1;              // 0 BitPosition                  
/*0x000*/     UINT64       Unused0 : 4;            // 1 BitPosition                  
/*0x000*/     UINT64       Protection : 5;         // 5 BitPosition                  
/*0x000*/     UINT64       Prototype : 1;          // 10 BitPosition                 
/*0x000*/     UINT64       Unused1 : 21;           // 11 BitPosition                 
/*0x000*/     UINT64       SubsectionAddress : 32; // 32 BitPosition                 
          }MMPTE_SUBSECTION, *PMMPTE_SUBSECTION;                                     
                                                                                     
          typedef struct _MMPTE_LIST       // 6 elements, 0x8 bytes (sizeof) 
          {                                                                  
/*0x000*/     UINT64       Valid : 1;      // 0 BitPosition                  
/*0x000*/     UINT64       OneEntry : 1;   // 1 BitPosition                  
/*0x000*/     UINT64       filler0 : 8;    // 2 BitPosition                  
/*0x000*/     UINT64       Prototype : 1;  // 10 BitPosition                 
/*0x000*/     UINT64       filler1 : 21;   // 11 BitPosition                 
/*0x000*/     UINT64       NextEntry : 32; // 32 BitPosition                 
          }MMPTE_LIST, *PMMPTE_LIST;                                         
                                                                             
          typedef struct _MMPTE                     // 1 elements, 0x8 bytes (sizeof)  
          {                                                                            
              union                                 // 9 elements, 0x8 bytes (sizeof)  
              {                                                                        
/*0x000*/         UINT64       Long;                                                   
/*0x000*/         struct _MMPTE_HIGHLOW HighLow;    // 2 elements, 0x8 bytes (sizeof)  
/*0x000*/         struct _MMPTE_HARDWARE Hard;      // 14 elements, 0x8 bytes (sizeof) 
/*0x000*/         struct _HARDWARE_PTE Flush;       // 16 elements, 0x8 bytes (sizeof) 
/*0x000*/         struct _MMPTE_PROTOTYPE Proto;    // 8 elements, 0x8 bytes (sizeof)  
/*0x000*/         struct _MMPTE_SOFTWARE Soft;      // 7 elements, 0x8 bytes (sizeof)  
/*0x000*/         struct _MMPTE_TRANSITION Trans;   // 10 elements, 0x8 bytes (sizeof) 
/*0x000*/         struct _MMPTE_SUBSECTION Subsect; // 6 elements, 0x8 bytes (sizeof)  
/*0x000*/         struct _MMPTE_LIST List;          // 6 elements, 0x8 bytes (sizeof)  
              }u;                                                                      
          }MMPTE, *PMMPTE;                                                             
                                                                                       
          typedef struct _MMBANKED_SECTION                  // 8 elements, 0x28 bytes (sizeof) 
          {                                                                                    
/*0x000*/     ULONG32      BasePhysicalPage;                                                   
/*0x004*/     struct _MMPTE* BasedPte;                                                         
/*0x008*/     ULONG32      BankSize;                                                           
/*0x00C*/     ULONG32      BankShift;                                                          
/*0x010*/     FUNCT_00BC_130F_BankedRoutine* BankedRoutine;                                    
/*0x014*/     VOID*        Context;                                                            
/*0x018*/     struct _MMPTE* CurrentMappedPte;                                                 
/*0x01C*/     UINT8        _PADDING0_[0x4];                                                    
/*0x020*/     struct _MMPTE BankTemplate[1];                                                   
          }MMBANKED_SECTION, *PMMBANKED_SECTION;                                               
                                                                                               
          typedef struct _MMCOLOR_TABLES // 3 elements, 0xC bytes (sizeof) 
          {                                                                
/*0x000*/     ULONG32      Flink;                                          
/*0x004*/     VOID*        Blink;                                          
/*0x008*/     ULONG32      Count;                                          
          }MMCOLOR_TABLES, *PMMCOLOR_TABLES;                                  
                                                                           
          typedef struct _MMEXTEND_INFO     // 2 elements, 0x10 bytes (sizeof) 
          {                                                                    
/*0x000*/     UINT64       CommittedSize;                                      
/*0x008*/     ULONG32      ReferenceCount;                                     
/*0x00C*/     UINT8        _PADDING0_[0x4];                                    
          }MMEXTEND_INFO, *PMMEXTEND_INFO;                                     
                                                                               
          typedef struct _MMFREE_POOL_ENTRY     // 4 elements, 0x14 bytes (sizeof) 
          {                                                                        
/*0x000*/     struct _LIST_ENTRY List;          // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/     ULONG32      Size;                                                   
/*0x00C*/     ULONG32      Signature;                                              
/*0x010*/     struct _MMFREE_POOL_ENTRY* Owner;                                    
          }MMFREE_POOL_ENTRY, *PMMFREE_POOL_ENTRY;                                   
                                                                                   
          typedef struct _MMMOD_WRITER_LISTHEAD // 2 elements, 0x18 bytes (sizeof) 
          {                                                                        
/*0x000*/     struct _LIST_ENTRY ListHead;      // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/     struct _KEVENT Event;             // 1 elements, 0x10 bytes (sizeof) 
          }MMMOD_WRITER_LISTHEAD, *PMMMOD_WRITER_LISTHEAD;                                   
                                                                                   
          typedef struct _MMMOD_WRITER_MDL_ENTRY             // 13 elements, 0x58 bytes (sizeof) 
          {                                                                                      
/*0x000*/     struct _LIST_ENTRY Links;                      // 2 elements, 0x8 bytes (sizeof)   
/*0x008*/     union _LARGE_INTEGER WriteOffset;              // 4 elements, 0x8 bytes (sizeof)   
              union                                          // 2 elements, 0x8 bytes (sizeof)   
              {                                                                                  
/*0x010*/         struct _IO_STATUS_BLOCK IoStatus;          // 3 elements, 0x8 bytes (sizeof)   
/*0x010*/         union _LARGE_INTEGER LastByte;             // 4 elements, 0x8 bytes (sizeof)   
              }u;                                                                                
/*0x018*/     struct _IRP* Irp;                                                                  
/*0x01C*/     ULONG32      LastPageToWrite;                                                      
/*0x020*/     struct _MMMOD_WRITER_LISTHEAD* PagingListHead;                                     
/*0x024*/     struct _LIST_ENTRY* CurrentList;                                                   
/*0x028*/     struct _MMPAGING_FILE* PagingFile;                                                 
/*0x02C*/     struct _FILE_OBJECT* File;                                                         
/*0x030*/     struct _CONTROL_AREA* ControlArea;                                                 
/*0x034*/     struct _ERESOURCE* FileResource;                                                   
/*0x038*/     struct _MDL  Mdl;                              // 8 elements, 0x1C bytes (sizeof)  
/*0x054*/     ULONG32      Page[1];                                                              
          }MMMOD_WRITER_MDL_ENTRY, *PMMMOD_WRITER_MDL_ENTRY;                                     
                                                                                                 
          typedef struct _MMPAGING_FILE                 // 17 elements, 0x44 bytes (sizeof) 
          {                                                                                 
/*0x000*/     ULONG32      Size;                                                            
/*0x004*/     ULONG32      MaximumSize;                                                     
/*0x008*/     ULONG32      MinimumSize;                                                     
/*0x00C*/     ULONG32      FreeSpace;                                                       
/*0x010*/     ULONG32      CurrentUsage;                                                    
/*0x014*/     ULONG32      PeakUsage;                                                       
/*0x018*/     ULONG32      Hint;                                                            
/*0x01C*/     ULONG32      HighestPage;                                                     
/*0x020*/     struct _MMMOD_WRITER_MDL_ENTRY* Entry[2];                                     
/*0x028*/     struct _RTL_BITMAP* Bitmap;                                                   
/*0x02C*/     struct _FILE_OBJECT* File;                                                    
/*0x030*/     struct _UNICODE_STRING PageFileName;      // 3 elements, 0x8 bytes (sizeof)   
/*0x038*/     ULONG32      PageFileNumber;                                                  
/*0x03C*/     UINT8        Extended;                                                        
/*0x03D*/     UINT8        HintSetToZero;                                                   
/*0x03E*/     UINT8        BootPartition;                                                   
/*0x03F*/     UINT8        _PADDING0_[0x1];                                                 
/*0x040*/     VOID*        FileHandle;                                                      
          }MMPAGING_FILE, *PMMPAGING_FILE;                                                  
                                                                                            
          typedef struct _MMPFNENTRY             // 12 elements, 0x4 bytes (sizeof) 
          {                                                                         
/*0x000*/     ULONG32      Modified : 1;         // 0 BitPosition                   
/*0x000*/     ULONG32      ReadInProgress : 1;   // 1 BitPosition                   
/*0x000*/     ULONG32      WriteInProgress : 1;  // 2 BitPosition                   
/*0x000*/     ULONG32      PrototypePte : 1;     // 3 BitPosition                   
/*0x000*/     ULONG32      PageColor : 3;        // 4 BitPosition                   
/*0x000*/     ULONG32      ParityError : 1;      // 7 BitPosition                   
/*0x000*/     ULONG32      PageLocation : 3;     // 8 BitPosition                   
/*0x000*/     ULONG32      RemovalRequested : 1; // 11 BitPosition                  
/*0x000*/     ULONG32      CacheAttribute : 2;   // 12 BitPosition                  
/*0x000*/     ULONG32      Rom : 1;              // 14 BitPosition                  
/*0x000*/     ULONG32      LockCharged : 1;      // 15 BitPosition                  
/*0x000*/     ULONG32      DontUse : 16;         // 16 BitPosition                  
          }MMPFNENTRY, *PMMPFNENTRY;                                                
                                                                                    
          typedef struct _MMPFN                            // 6 elements, 0x1C bytes (sizeof) 
          {                                                                                   
              union                                        // 5 elements, 0x4 bytes (sizeof)  
              {                                                                               
/*0x000*/         ULONG32      Flink;                                                         
/*0x000*/         ULONG32      WsIndex;                                                       
/*0x000*/         struct _KEVENT* Event;                                                      
/*0x000*/         LONG32       ReadStatus;                                                    
/*0x000*/         struct _SINGLE_LIST_ENTRY NextStackPfn;  // 1 elements, 0x4 bytes (sizeof)  
              }u1;                                                                            
/*0x004*/     struct _MMPTE* PteAddress;                                                      
              union                                        // 2 elements, 0x4 bytes (sizeof)  
              {                                                                               
/*0x008*/         ULONG32      Blink;                                                         
/*0x008*/         ULONG32      ShareCount;                                                    
              }u2;                                                                            
              union                                        // 2 elements, 0x4 bytes (sizeof)  
              {                                                                               
/*0x00C*/         struct _MMPFNENTRY e1;                   // 12 elements, 0x4 bytes (sizeof) 
                  struct                                   // 2 elements, 0x4 bytes (sizeof)  
                  {                                                                           
/*0x00C*/             UINT16       ShortFlags;                                                
/*0x00E*/             UINT16       ReferenceCount;                                            
                  }e2;                                                                        
              }u3;                                                                            
/*0x010*/     struct _MMPTE OriginalPte;                   // 1 elements, 0x8 bytes (sizeof)  
              union                                        // 8 elements, 0x4 bytes (sizeof)  
              {                                                                               
/*0x018*/         ULONG32      EntireFrame;                                                   
                  struct                                   // 7 elements, 0x4 bytes (sizeof)  
                  {                                                                           
/*0x018*/             ULONG32      PteFrame : 26;          // 0 BitPosition                   
/*0x018*/             ULONG32      InPageError : 1;        // 26 BitPosition                  
/*0x018*/             ULONG32      VerifierAllocation : 1; // 27 BitPosition                  
/*0x018*/             ULONG32      AweAllocation : 1;      // 28 BitPosition                  
/*0x018*/             ULONG32      LockCharged : 1;        // 29 BitPosition                  
/*0x018*/             ULONG32      KernelStack : 1;        // 30 BitPosition                  
/*0x018*/             ULONG32      Reserved : 1;           // 31 BitPosition                  
                  };                                                                          
              }u4;                                                                            
          }MMPFN, *PMMPFN;                                                                    
                                                                                              
          typedef struct _MMPFNLIST   // 4 elements, 0x10 bytes (sizeof) 
          {                                                              
/*0x000*/     ULONG32      Total;                                        
/*0x004*/     enum _MMLISTS ListName;                                    
/*0x008*/     ULONG32      Flink;                                        
/*0x00C*/     ULONG32      Blink;                                        
          }MMPFNLIST, *PMMPFNLIST;                                       
                                                                         
          typedef struct _MMSUBSECTION_FLAGS        // 8 elements, 0x4 bytes (sizeof) 
          {                                                                           
/*0x000*/     UINT32       ReadOnly : 1;            // 0 BitPosition                  
/*0x000*/     UINT32       ReadWrite : 1;           // 1 BitPosition                  
/*0x000*/     UINT32       SubsectionStatic : 1;    // 2 BitPosition                  
/*0x000*/     UINT32       GlobalMemory : 1;        // 3 BitPosition                  
/*0x000*/     UINT32       Protection : 5;          // 4 BitPosition                  
/*0x000*/     UINT32       LargePages : 1;          // 9 BitPosition                  
/*0x000*/     UINT32       StartingSector4132 : 10; // 10 BitPosition                 
/*0x000*/     UINT32       SectorEndOffset : 12;    // 20 BitPosition                 
          }MMSUBSECTION_FLAGS, *PMMSUBSECTION_FLAGS;                                  
                                                                                      
          typedef struct _MMVAD_FLAGS             // 10 elements, 0x4 bytes (sizeof) 
          {                                                                          
/*0x000*/     ULONG32      CommitCharge : 19;     // 0 BitPosition                   
/*0x000*/     ULONG32      PhysicalMapping : 1;   // 19 BitPosition                  
/*0x000*/     ULONG32      ImageMap : 1;          // 20 BitPosition                  
/*0x000*/     ULONG32      UserPhysicalPages : 1; // 21 BitPosition                  
/*0x000*/     ULONG32      NoChange : 1;          // 22 BitPosition                  
/*0x000*/     ULONG32      WriteWatch : 1;        // 23 BitPosition                  
/*0x000*/     ULONG32      Protection : 5;        // 24 BitPosition                  
/*0x000*/     ULONG32      LargePages : 1;        // 29 BitPosition                  
/*0x000*/     ULONG32      MemCommit : 1;         // 30 BitPosition                  
/*0x000*/     ULONG32      PrivateMemory : 1;     // 31 BitPosition                  
          }MMVAD_FLAGS, *PMMVAD_FLAGS;                                               
                                                                                     
          typedef struct _MMVAD_FLAGS2          // 9 elements, 0x4 bytes (sizeof) 
          {                                                                       
/*0x000*/     UINT32       FileOffset : 24;     // 0 BitPosition                  
/*0x000*/     UINT32       SecNoChange : 1;     // 24 BitPosition                 
/*0x000*/     UINT32       OneSecured : 1;      // 25 BitPosition                 
/*0x000*/     UINT32       MultipleSecured : 1; // 26 BitPosition                 
/*0x000*/     UINT32       ReadOnly : 1;        // 27 BitPosition                 
/*0x000*/     UINT32       LongVad : 1;         // 28 BitPosition                 
/*0x000*/     UINT32       ExtendableFile : 1;  // 29 BitPosition                 
/*0x000*/     UINT32       Inherit : 1;         // 30 BitPosition                 
/*0x000*/     UINT32       CopyOnWrite : 1;     // 31 BitPosition                 
          }MMVAD_FLAGS2, *PMMVAD_FLAGS2;                                          
                                                                                  
          typedef struct _MMVAD                   // 10 elements, 0x28 bytes (sizeof) 
          {                                                                           
/*0x000*/     ULONG32      StartingVpn;                                               
/*0x004*/     ULONG32      EndingVpn;                                                 
/*0x008*/     struct _MMVAD* Parent;                                                  
/*0x00C*/     struct _MMVAD* LeftChild;                                               
/*0x010*/     struct _MMVAD* RightChild;                                              
              union                               // 2 elements, 0x4 bytes (sizeof)   
              {                                                                       
/*0x014*/         ULONG32      LongFlags;                                             
/*0x014*/         struct _MMVAD_FLAGS VadFlags;   // 10 elements, 0x4 bytes (sizeof)  
              }u;                                                                     
/*0x018*/     struct _CONTROL_AREA* ControlArea;                                      
/*0x01C*/     struct _MMPTE* FirstPrototypePte;                                       
/*0x020*/     struct _MMPTE* LastContiguousPte;                                       
              union                               // 2 elements, 0x4 bytes (sizeof)   
              {                                                                       
/*0x024*/         ULONG32      LongFlags2;                                            
/*0x024*/         struct _MMVAD_FLAGS2 VadFlags2; // 9 elements, 0x4 bytes (sizeof)   
              }u2;                                                                    
          }MMVAD, *PMMVAD;                                                            
                                                                                      
          typedef struct _MMVAD_LONG                   // 12 elements, 0x34 bytes (sizeof) 
          {                                                                                
/*0x000*/     ULONG32      StartingVpn;                                                    
/*0x004*/     ULONG32      EndingVpn;                                                      
/*0x008*/     struct _MMVAD* Parent;                                                       
/*0x00C*/     struct _MMVAD* LeftChild;                                                    
/*0x010*/     struct _MMVAD* RightChild;                                                   
              union                                    // 2 elements, 0x4 bytes (sizeof)   
              {                                                                            
/*0x014*/         ULONG32      LongFlags;                                                  
/*0x014*/         struct _MMVAD_FLAGS VadFlags;        // 10 elements, 0x4 bytes (sizeof)  
              }u;                                                                          
/*0x018*/     struct _CONTROL_AREA* ControlArea;                                           
/*0x01C*/     struct _MMPTE* FirstPrototypePte;                                            
/*0x020*/     struct _MMPTE* LastContiguousPte;                                            
              union                                    // 2 elements, 0x4 bytes (sizeof)   
              {                                                                            
/*0x024*/         ULONG32      LongFlags2;                                                 
/*0x024*/         struct _MMVAD_FLAGS2 VadFlags2;      // 9 elements, 0x4 bytes (sizeof)   
              }u2;                                                                         
              union                                    // 2 elements, 0x8 bytes (sizeof)   
              {                                                                            
/*0x028*/         struct _LIST_ENTRY List;             // 2 elements, 0x8 bytes (sizeof)   
/*0x028*/         struct _MMADDRESS_LIST Secured;      // 2 elements, 0x8 bytes (sizeof)   
              }u3;                                                                         
              union                                    // 2 elements, 0x4 bytes (sizeof)   
              {                                                                            
/*0x030*/         struct _MMBANKED_SECTION* Banked;                                        
/*0x030*/         struct _MMEXTEND_INFO* ExtendedInfo;                                     
              }u4;                                                                         
          }MMVAD_LONG, *PMMVAD_LONG;                                                       
                                                                                           
          typedef struct _MMVAD_SHORT           // 6 elements, 0x18 bytes (sizeof) 
          {                                                                        
/*0x000*/     ULONG32      StartingVpn;                                            
/*0x004*/     ULONG32      EndingVpn;                                              
/*0x008*/     struct _MMVAD* Parent;                                               
/*0x00C*/     struct _MMVAD* LeftChild;                                            
/*0x010*/     struct _MMVAD* RightChild;                                           
              union                             // 2 elements, 0x4 bytes (sizeof)  
              {                                                                    
/*0x014*/         ULONG32      LongFlags;                                          
/*0x014*/         struct _MMVAD_FLAGS VadFlags; // 10 elements, 0x4 bytes (sizeof) 
              }u;                                                                  
          }MMVAD_SHORT, *PMMVAD_SHORT;                                             
                                                                                   
          typedef struct _MMVIEW                 // 2 elements, 0x8 bytes (sizeof) 
          {                                                                        
/*0x000*/     ULONG32      Entry;                                                  
/*0x004*/     struct _CONTROL_AREA* ControlArea;                                   
          }MMVIEW, *PMMVIEW;                                                       
                                                                                   
          typedef struct _MMWSL                         // 17 elements, 0xCFC bytes (sizeof) 
          {                                                                                  
/*0x000*/     ULONG32      Quota;                                                            
/*0x004*/     ULONG32      FirstFree;                                                        
/*0x008*/     ULONG32      FirstDynamic;                                                     
/*0x00C*/     ULONG32      LastEntry;                                                        
/*0x010*/     ULONG32      NextSlot;                                                         
/*0x014*/     struct _MMWSLE* Wsle;                                                          
/*0x018*/     ULONG32      LastInitializedWsle;                                              
/*0x01C*/     ULONG32      NonDirectCount;                                                   
/*0x020*/     struct _MMWSLE_HASH* HashTable;                                                
/*0x024*/     ULONG32      HashTableSize;                                                    
/*0x028*/     ULONG32      NumberOfCommittedPageTables;                                      
/*0x02C*/     VOID*        HashTableStart;                                                   
/*0x030*/     VOID*        HighestPermittedHashAddress;                                      
/*0x034*/     ULONG32      NumberOfImageWaiters;                                             
/*0x038*/     ULONG32      VadBitMapHint;                                                    
/*0x03C*/     UINT16       UsedPageTableEntries[1536];                                       
/*0xC3C*/     ULONG32      CommittedPageTables[48];                                          
          }MMWSL, *PMMWSL;                                                                   
                                                                                             
          typedef struct _MMWSLENTRY               // 8 elements, 0x4 bytes (sizeof) 
          {                                                                          
/*0x000*/     ULONG32      Valid : 1;              // 0 BitPosition                  
/*0x000*/     ULONG32      LockedInWs : 1;         // 1 BitPosition                  
/*0x000*/     ULONG32      LockedInMemory : 1;     // 2 BitPosition                  
/*0x000*/     ULONG32      Protection : 5;         // 3 BitPosition                  
/*0x000*/     ULONG32      Hashed : 1;             // 8 BitPosition                  
/*0x000*/     ULONG32      Direct : 1;             // 9 BitPosition                  
/*0x000*/     ULONG32      Age : 2;                // 10 BitPosition                 
/*0x000*/     ULONG32      VirtualPageNumber : 20; // 12 BitPosition                 
          }MMWSLENTRY, *PMMWSLENTRY;                                                 
                                                                                     
          typedef struct _MMWSLE               // 1 elements, 0x4 bytes (sizeof) 
          {                                                                      
              union                            // 3 elements, 0x4 bytes (sizeof) 
              {                                                                  
/*0x000*/         VOID*        VirtualAddress;                                   
/*0x000*/         ULONG32      Long;                                             
/*0x000*/         struct _MMWSLENTRY e1;       // 8 elements, 0x4 bytes (sizeof) 
              }u1;                                                               
          }MMWSLE, *PMMWSLE;                                                     
                                                                                 
          typedef struct _MMWSLE_HASH // 2 elements, 0x8 bytes (sizeof) 
          {                                                             
/*0x000*/     VOID*        Key;                                         
/*0x004*/     ULONG32      Index;                                       
          }MMWSLE_HASH, *PMMWSLE_HASH;                                  
                                                                        
          typedef struct _NAMED_PIPE_CREATE_PARAMETERS // 8 elements, 0x28 bytes (sizeof) 
          {                                                                               
/*0x000*/     ULONG32      NamedPipeType;                                                 
/*0x004*/     ULONG32      ReadMode;                                                      
/*0x008*/     ULONG32      CompletionMode;                                                
/*0x00C*/     ULONG32      MaximumInstances;                                              
/*0x010*/     ULONG32      InboundQuota;                                                  
/*0x014*/     ULONG32      OutboundQuota;                                                 
/*0x018*/     union _LARGE_INTEGER DefaultTimeout;     // 4 elements, 0x8 bytes (sizeof)  
/*0x020*/     UINT8        TimeoutSpecified;                                              
/*0x021*/     UINT8        _PADDING0_[0x7];                                               
          }NAMED_PIPE_CREATE_PARAMETERS, *PNAMED_PIPE_CREATE_PARAMETERS;                                   
                                                                                          
          typedef struct _NPAGED_LOOKASIDE_LIST          // 2 elements, 0x100 bytes (sizeof) 
          {                                                                                  
/*0x000*/     struct _GENERAL_LOOKASIDE L;               // 19 elements, 0x80 bytes (sizeof) 
/*0x080*/     ULONG32      Lock__ObsoleteButDoNotDelete;                                     
/*0x084*/     UINT8        _PADDING0_[0x7C];                                                 
          }NPAGED_LOOKASIDE_LIST, *PNPAGED_LOOKASIDE_LIST;                                    
                                                                                             
          typedef struct _OBJECT_ATTRIBUTES          // 6 elements, 0x18 bytes (sizeof) 
          {                                                                             
/*0x000*/     ULONG32      Length;                                                      
/*0x004*/     VOID*        RootDirectory;                                               
/*0x008*/     struct _UNICODE_STRING* ObjectName;                                       
/*0x00C*/     ULONG32      Attributes;                                                  
/*0x010*/     VOID*        SecurityDescriptor;                                          
/*0x014*/     VOID*        SecurityQualityOfService;                                    
          }OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;                                      
                                                                                        
          typedef struct _OBJECT_CREATE_INFORMATION                         // 10 elements, 0x30 bytes (sizeof) 
          {                                                                                                     
/*0x000*/     ULONG32      Attributes;                                                                          
/*0x004*/     VOID*        RootDirectory;                                                                       
/*0x008*/     VOID*        ParseContext;                                                                        
/*0x00C*/     CHAR         ProbeMode;                                                                           
/*0x00D*/     UINT8        _PADDING0_[0x3];                                                                     
/*0x010*/     ULONG32      PagedPoolCharge;                                                                     
/*0x014*/     ULONG32      NonPagedPoolCharge;                                                                  
/*0x018*/     ULONG32      SecurityDescriptorCharge;                                                            
/*0x01C*/     VOID*        SecurityDescriptor;                                                                  
/*0x020*/     struct _SECURITY_QUALITY_OF_SERVICE* SecurityQos;                                                 
/*0x024*/     struct _SECURITY_QUALITY_OF_SERVICE SecurityQualityOfService; // 4 elements, 0xC bytes (sizeof)   
          }OBJECT_CREATE_INFORMATION, *POBJECT_CREATE_INFORMATION;                                              
                                                                                                                
          typedef struct _OBJECT_DIRECTORY                     // 6 elements, 0xA4 bytes (sizeof) 
          {                                                                                       
/*0x000*/     struct _OBJECT_DIRECTORY_ENTRY* HashBuckets[37];                                    
/*0x094*/     struct _EX_PUSH_LOCK Lock;                       // 5 elements, 0x4 bytes (sizeof)  
/*0x098*/     struct _DEVICE_MAP* DeviceMap;                                                      
/*0x09C*/     ULONG32      SessionId;                                                             
/*0x0A0*/     UINT16       Reserved;                                                              
/*0x0A2*/     UINT16       SymbolicLinkUsageCount;                                                
          }OBJECT_DIRECTORY, *POBJECT_DIRECTORY;                                                  
                                                                                                  
          typedef struct _OBJECT_DIRECTORY_ENTRY         // 2 elements, 0x8 bytes (sizeof) 
          {                                                                                
/*0x000*/     struct _OBJECT_DIRECTORY_ENTRY* ChainLink;                                   
/*0x004*/     VOID*        Object;                                                         
          }OBJECT_DIRECTORY_ENTRY, *POBJECT_DIRECTORY_ENTRY;                                  
                                                                                           
          typedef struct _OBJECT_DUMP_CONTROL // 2 elements, 0x8 bytes (sizeof) 
          {                                                                     
/*0x000*/     VOID*        Stream;                                              
/*0x004*/     ULONG32      Detail;                                              
          }OBJECT_DUMP_CONTROL, *POBJECT_DUMP_CONTROL;                                  
                                                                                
          typedef struct _OBJECT_HANDLE_INFORMATION // 2 elements, 0x8 bytes (sizeof) 
          {                                                                           
/*0x000*/     ULONG32      HandleAttributes;                                          
/*0x004*/     ULONG32      GrantedAccess;                                             
          }OBJECT_HANDLE_INFORMATION, *POBJECT_HANDLE_INFORMATION;                                  
                                                                                      
          typedef struct _QUAD                // 1 elements, 0x8 bytes (sizeof) 
          {                                                                     
/*0x000*/     FLOAT64      DoNotUseThisField;                                   
          }QUAD, *PQUAD;                                                        
                                                                                
          typedef struct _OBJECT_HEADER                                // 12 elements, 0x20 bytes (sizeof) 
          {                                                                                                
/*0x000*/     LONG32       PointerCount;                                                                   
              union                                                    // 2 elements, 0x4 bytes (sizeof)   
              {                                                                                            
/*0x004*/         LONG32       HandleCount;                                                                
/*0x004*/         VOID*        NextToFree;                                                                 
              };                                                                                           
/*0x008*/     struct _OBJECT_TYPE* Type;                                                                   
/*0x00C*/     UINT8        NameInfoOffset;                                                                 
/*0x00D*/     UINT8        HandleInfoOffset;                                                               
/*0x00E*/     UINT8        QuotaInfoOffset;                                                                
/*0x00F*/     UINT8        Flags;                                                                          
              union                                                    // 2 elements, 0x4 bytes (sizeof)   
              {                                                                                            
/*0x010*/         struct _OBJECT_CREATE_INFORMATION* ObjectCreateInfo;                                     
/*0x010*/         VOID*        QuotaBlockCharged;                                                          
              };                                                                                           
/*0x014*/     VOID*        SecurityDescriptor;                                                             
/*0x018*/     struct _QUAD Body;                                       // 1 elements, 0x8 bytes (sizeof)   
          }OBJECT_HEADER, *POBJECT_HEADER;                                                                 
                                                                                                           
          typedef struct _OBJECT_HEADER_CREATOR_INFO // 4 elements, 0x10 bytes (sizeof) 
          {                                                                             
/*0x000*/     struct _LIST_ENTRY TypeList;           // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/     VOID*        CreatorUniqueProcess;                                        
/*0x00C*/     UINT16       CreatorBackTraceIndex;                                       
/*0x00E*/     UINT16       Reserved;                                                    
          }OBJECT_HEADER_CREATOR_INFO, *POBJECT_HEADER_CREATOR_INFO;                                   
                                                                                        
          typedef struct _OBJECT_HEADER_NAME_INFO  // 3 elements, 0x10 bytes (sizeof) 
          {                                                                           
/*0x000*/     struct _OBJECT_DIRECTORY* Directory;                                    
/*0x004*/     struct _UNICODE_STRING Name;         // 3 elements, 0x8 bytes (sizeof)  
/*0x00C*/     ULONG32      QueryReferences;                                           
          }OBJECT_HEADER_NAME_INFO, *POBJECT_HEADER_NAME_INFO;                                   
                                                                                      
          typedef struct _OBJECT_NAME_INFORMATION // 1 elements, 0x8 bytes (sizeof) 
          {                                                                         
/*0x000*/     struct _UNICODE_STRING Name;        // 3 elements, 0x8 bytes (sizeof) 
          }OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;                                  
                                                                                    
          typedef struct _OBJECT_SYMBOLIC_LINK            // 5 elements, 0x20 bytes (sizeof) 
          {                                                                                  
/*0x000*/     union _LARGE_INTEGER CreationTime;          // 4 elements, 0x8 bytes (sizeof)  
/*0x008*/     struct _UNICODE_STRING LinkTarget;          // 3 elements, 0x8 bytes (sizeof)  
/*0x010*/     struct _UNICODE_STRING LinkTargetRemaining; // 3 elements, 0x8 bytes (sizeof)  
/*0x018*/     VOID*        LinkTargetObject;                                                 
/*0x01C*/     ULONG32      DosDeviceDriveIndex;                                              
          }OBJECT_SYMBOLIC_LINK, *POBJECT_SYMBOLIC_LINK;                                     
                                                                                             
          typedef struct _OBJECT_TYPE_INITIALIZER                                                                                                                                        // 20 elements, 0x4C bytes (sizeof) 
          {                                                                                                                                                                                                                  
/*0x000*/     UINT16       Length;                                                                                                                                                                                           
/*0x002*/     UINT8        UseDefaultObject;                                                                                                                                                                                 
/*0x003*/     UINT8        CaseInsensitive;                                                                                                                                                                                  
/*0x004*/     ULONG32      InvalidAttributes;                                                                                                                                                                                
/*0x008*/     struct _GENERIC_MAPPING GenericMapping;                                                                                                                                    // 4 elements, 0x10 bytes (sizeof)  
/*0x018*/     ULONG32      ValidAccessMask;                                                                                                                                                                                  
/*0x01C*/     UINT8        SecurityRequired;                                                                                                                                                                                 
/*0x01D*/     UINT8        MaintainHandleCount;                                                                                                                                                                              
/*0x01E*/     UINT8        MaintainTypeList;                                                                                                                                                                                 
/*0x01F*/     UINT8        _PADDING0_[0x1];                                                                                                                                                                                  
/*0x020*/     enum _POOL_TYPE PoolType;                                                                                                                                                                                      
/*0x024*/     ULONG32      DefaultPagedPoolCharge;                                                                                                                                                                           
/*0x028*/     ULONG32      DefaultNonPagedPoolCharge;                                                                                                                                                                        
/*0x02C*/     FUNCT_00BC_09D8_DumpProcedure* DumpProcedure;                                                                                                                                                                  
/*0x030*/     FUNCT_0049_09E0_OpenProcedure* OpenProcedure;                                                                                                                                                                  
/*0x034*/     FUNCT_00BC_09ED_CloseProcedure* CloseProcedure;                                                                                                                                                                
/*0x038*/     FUNCT_00BC_0285_Free_InterfaceReference_InterfaceDereference_DeleteProcedure_ReleaseFromLazyWrite_ReleaseFromReadAhead_WorkerRoutine_Destructor_Callback* DeleteProcedure;                                     
/*0x03C*/     FUNCT_0049_09F4_ParseProcedure* ParseProcedure;                                                                                                                                                                
/*0x040*/     FUNCT_0049_0A00_SecurityProcedure* SecurityProcedure;                                                                                                                                                          
/*0x044*/     FUNCT_0049_0A11_QueryNameProcedure* QueryNameProcedure;                                                                                                                                                        
/*0x048*/     FUNCT_003D_0A18_OkayToCloseProcedure* OkayToCloseProcedure;                                                                                                                                                    
          }OBJECT_TYPE_INITIALIZER, *POBJECT_TYPE_INITIALIZER;                                                                                                                                                               
                                                                                                                                                                                                                             
          typedef struct _OBJECT_TYPE                   // 12 elements, 0x190 bytes (sizeof) 
          {                                                                                  
/*0x000*/     struct _ERESOURCE Mutex;                  // 13 elements, 0x38 bytes (sizeof)  
/*0x038*/     struct _LIST_ENTRY TypeList;              // 2 elements, 0x8 bytes (sizeof)    
/*0x040*/     struct _UNICODE_STRING Name;              // 3 elements, 0x8 bytes (sizeof)    
/*0x048*/     VOID*        DefaultObject;                                                    
/*0x04C*/     ULONG32      Index;                                                            
/*0x050*/     ULONG32      TotalNumberOfObjects;                                             
/*0x054*/     ULONG32      TotalNumberOfHandles;                                             
/*0x058*/     ULONG32      HighWaterNumberOfObjects;                                         
/*0x05C*/     ULONG32      HighWaterNumberOfHandles;                                         
/*0x060*/     struct _OBJECT_TYPE_INITIALIZER TypeInfo; // 20 elements, 0x4C bytes (sizeof)  
/*0x0AC*/     ULONG32      Key;                                                              
/*0x0B0*/     struct _ERESOURCE ObjectLocks[4];                                              
          }OBJECT_TYPE, *POBJECT_TYPE;                                                       
                                                                                             
          typedef struct _PAGED_LOOKASIDE_LIST                 // 2 elements, 0x100 bytes (sizeof) 
          {                                                                                        
/*0x000*/     struct _GENERAL_LOOKASIDE L;                     // 19 elements, 0x80 bytes (sizeof) 
/*0x080*/     struct _FAST_MUTEX Lock__ObsoleteButDoNotDelete; // 5 elements, 0x20 bytes (sizeof)  
/*0x0A0*/     UINT8        _PADDING0_[0x60];                                                       
          }PAGED_LOOKASIDE_LIST, *PPAGED_LOOKASIDE_LIST;                                           
                                                                                                   
          typedef struct _PROCESS_WS_WATCH_INFORMATION // 2 elements, 0x8 bytes (sizeof) 
          {                                                                              
/*0x000*/     VOID*        FaultingPc;                                                   
/*0x004*/     VOID*        FaultingVa;                                                   
          }PROCESS_WS_WATCH_INFORMATION, *PPROCESS_WS_WATCH_INFORMATION;                                  
                                                                                         
          typedef struct _PAGEFAULT_HISTORY                      // 5 elements, 0x18 bytes (sizeof) 
          {                                                                                         
/*0x000*/     ULONG32      CurrentIndex;                                                            
/*0x004*/     ULONG32      MaxIndex;                                                                
/*0x008*/     ULONG32      SpinLock;                                                                
/*0x00C*/     VOID*        Reserved;                                                                
/*0x010*/     struct _PROCESS_WS_WATCH_INFORMATION WatchInfo[1];                                    
          }PAGEFAULT_HISTORY, *PPAGEFAULT_HISTORY;                                                  
                                                                                                    
          typedef struct _PCI_SECONDARY_EXTENSION                                                                                                                                   // 3 elements, 0xC bytes (sizeof) 
          {                                                                                                                                                                                                           
/*0x000*/     struct _SINGLE_LIST_ENTRY List;                                                                                                                                       // 1 elements, 0x4 bytes (sizeof) 
/*0x004*/     enum _PCI_SIGNATURE ExtensionType;                                                                                                                                                                      
/*0x008*/     FUNCT_00BC_0285_Free_InterfaceReference_InterfaceDereference_DeleteProcedure_ReleaseFromLazyWrite_ReleaseFromReadAhead_WorkerRoutine_Destructor_Callback* Destructor;                                   
          }PCI_SECONDARY_EXTENSION, *PPCI_SECONDARY_EXTENSION;                                                                                                                                                        
                                                                                                                                                                                                                      
          typedef struct _PCI_ARBITER_INSTANCE            // 5 elements, 0xE0 bytes (sizeof)  
          {                                                                                   
/*0x000*/     struct _PCI_SECONDARY_EXTENSION Header;     // 3 elements, 0xC bytes (sizeof)   
/*0x00C*/     struct _PCI_INTERFACE* Interface;                                               
/*0x010*/     struct _PCI_FDO_EXTENSION* BusFdoExtension;                                     
/*0x014*/     UINT16       InstanceName[24];                                                  
/*0x044*/     struct _ARBITER_INSTANCE CommonInstance;    // 37 elements, 0x9C bytes (sizeof) 
          }PCI_ARBITER_INSTANCE, *PPCI_ARBITER_INSTANCE;                                      
                                                                                              
          typedef struct _PCI_BUS_INTERFACE_STANDARD                                                                                                                                          // 9 elements, 0x20 bytes (sizeof) 
          {                                                                                                                                                                                                                      
/*0x000*/     UINT16       Size;                                                                                                                                                                                                 
/*0x002*/     UINT16       Version;                                                                                                                                                                                              
/*0x004*/     VOID*        Context;                                                                                                                                                                                              
/*0x008*/     FUNCT_00BC_0285_Free_InterfaceReference_InterfaceDereference_DeleteProcedure_ReleaseFromLazyWrite_ReleaseFromReadAhead_WorkerRoutine_Destructor_Callback* InterfaceReference;                                      
/*0x00C*/     FUNCT_00BC_0285_Free_InterfaceReference_InterfaceDereference_DeleteProcedure_ReleaseFromLazyWrite_ReleaseFromReadAhead_WorkerRoutine_Destructor_Callback* InterfaceDereference;                                    
/*0x010*/     FUNCT_0009_13CC_ReadConfig_WriteConfig* ReadConfig;                                                                                                                                                                
/*0x014*/     FUNCT_0009_13CC_ReadConfig_WriteConfig* WriteConfig;                                                                                                                                                               
/*0x018*/     FUNCT_00BC_13D4_PinToLine* PinToLine;                                                                                                                                                                              
/*0x01C*/     FUNCT_00BC_1422_LineToPin* LineToPin;                                                                                                                                                                              
          }PCI_BUS_INTERFACE_STANDARD, *PPCI_BUS_INTERFACE_STANDARD;                                                                                                                                                             
                                                                                                                                                                                                                                 
          typedef struct _PCI_HEADER_TYPE_0  // 12 elements, 0x30 bytes (sizeof) 
          {                                                                      
/*0x000*/     ULONG32      BaseAddresses[6];                                     
/*0x018*/     ULONG32      CIS;                                                  
/*0x01C*/     UINT16       SubVendorID;                                          
/*0x01E*/     UINT16       SubSystemID;                                          
/*0x020*/     ULONG32      ROMBaseAddress;                                       
/*0x024*/     UINT8        CapabilitiesPtr;                                      
/*0x025*/     UINT8        Reserved1[3];                                         
/*0x028*/     ULONG32      Reserved2;                                            
/*0x02C*/     UINT8        InterruptLine;                                        
/*0x02D*/     UINT8        InterruptPin;                                         
/*0x02E*/     UINT8        MinimumGrant;                                         
/*0x02F*/     UINT8        MaximumLatency;                                       
          }PCI_HEADER_TYPE_0, *PPCI_HEADER_TYPE_0;                                    
                                                                                 
          typedef struct _PCI_HEADER_TYPE_1      // 22 elements, 0x30 bytes (sizeof) 
          {                                                                          
/*0x000*/     ULONG32      BaseAddresses[2];                                         
/*0x008*/     UINT8        PrimaryBus;                                               
/*0x009*/     UINT8        SecondaryBus;                                             
/*0x00A*/     UINT8        SubordinateBus;                                           
/*0x00B*/     UINT8        SecondaryLatency;                                         
/*0x00C*/     UINT8        IOBase;                                                   
/*0x00D*/     UINT8        IOLimit;                                                  
/*0x00E*/     UINT16       SecondaryStatus;                                          
/*0x010*/     UINT16       MemoryBase;                                               
/*0x012*/     UINT16       MemoryLimit;                                              
/*0x014*/     UINT16       PrefetchBase;                                             
/*0x016*/     UINT16       PrefetchLimit;                                            
/*0x018*/     ULONG32      PrefetchBaseUpper32;                                      
/*0x01C*/     ULONG32      PrefetchLimitUpper32;                                     
/*0x020*/     UINT16       IOBaseUpper16;                                            
/*0x022*/     UINT16       IOLimitUpper16;                                           
/*0x024*/     UINT8        CapabilitiesPtr;                                          
/*0x025*/     UINT8        Reserved1[3];                                             
/*0x028*/     ULONG32      ROMBaseAddress;                                           
/*0x02C*/     UINT8        InterruptLine;                                            
/*0x02D*/     UINT8        InterruptPin;                                             
/*0x02E*/     UINT16       BridgeControl;                                            
          }PCI_HEADER_TYPE_1, *PPCI_HEADER_TYPE_1;                                    
                                                                                     
          typedef struct __201304220D382601B88D00D78581753BF_Range // 2 elements, 0x8 bytes (sizeof) 
          {                                                                                          
/*0x000*/     ULONG32      Base;                                                                     
/*0x004*/     ULONG32      Limit;                                                                    
          }_201304220D382601B88D00D78581753BF_Range, *P_201304220D382601B88D00D78581753BF_Range;                                  
                                                                                                     
          typedef struct _PCI_HEADER_TYPE_2                              // 12 elements, 0x30 bytes (sizeof) 
          {                                                                                                  
/*0x000*/     ULONG32      SocketRegistersBaseAddress;                                                       
/*0x004*/     UINT8        CapabilitiesPtr;                                                                  
/*0x005*/     UINT8        Reserved;                                                                         
/*0x006*/     UINT16       SecondaryStatus;                                                                  
/*0x008*/     UINT8        PrimaryBus;                                                                       
/*0x009*/     UINT8        SecondaryBus;                                                                     
/*0x00A*/     UINT8        SubordinateBus;                                                                   
/*0x00B*/     UINT8        SecondaryLatency;                                                                 
/*0x00C*/     struct __201304220D382601B88D00D78581753BF_Range Range[4];                                     
/*0x02C*/     UINT8        InterruptLine;                                                                    
/*0x02D*/     UINT8        InterruptPin;                                                                     
/*0x02E*/     UINT16       BridgeControl;                                                                    
          }PCI_HEADER_TYPE_2, *PPCI_HEADER_TYPE_2;                                                           
                                                                                                             
          typedef struct _PCI_COMMON_CONFIG        // 14 elements, 0x100 bytes (sizeof) 
          {                                                                             
/*0x000*/     UINT16       VendorID;                                                    
/*0x002*/     UINT16       DeviceID;                                                    
/*0x004*/     UINT16       Command;                                                     
/*0x006*/     UINT16       Status;                                                      
/*0x008*/     UINT8        RevisionID;                                                  
/*0x009*/     UINT8        ProgIf;                                                      
/*0x00A*/     UINT8        SubClass;                                                    
/*0x00B*/     UINT8        BaseClass;                                                   
/*0x00C*/     UINT8        CacheLineSize;                                               
/*0x00D*/     UINT8        LatencyTimer;                                                
/*0x00E*/     UINT8        HeaderType;                                                  
/*0x00F*/     UINT8        BIST;                                                        
              union                                // 3 elements, 0x30 bytes (sizeof)   
              {                                                                         
/*0x010*/         struct _PCI_HEADER_TYPE_0 type0; // 12 elements, 0x30 bytes (sizeof)  
/*0x010*/         struct _PCI_HEADER_TYPE_1 type1; // 22 elements, 0x30 bytes (sizeof)  
/*0x010*/         struct _PCI_HEADER_TYPE_2 type2; // 12 elements, 0x30 bytes (sizeof)  
              }u;                                                                       
/*0x040*/     UINT8        DeviceSpecific[192];                                         
          }PCI_COMMON_CONFIG, *PPCI_COMMON_CONFIG;                                      
                                                                                        
          typedef struct _PCI_COMMON_EXTENSION                 // 6 elements, 0x20 bytes (sizeof) 
          {                                                                                       
/*0x000*/     VOID*        Next;                                                                  
/*0x004*/     enum _PCI_SIGNATURE ExtensionType;                                                  
/*0x008*/     struct _PCI_MJ_DISPATCH_TABLE* IrpDispatchTable;                                    
/*0x00C*/     UINT8        DeviceState;                                                           
/*0x00D*/     UINT8        TentativeNextState;                                                    
/*0x00E*/     UINT8        _PADDING0_[0x2];                                                       
/*0x010*/     struct _KEVENT SecondaryExtLock;                 // 1 elements, 0x10 bytes (sizeof) 
          }PCI_COMMON_EXTENSION, *PPCI_COMMON_EXTENSION;                                          
                                                                                                  
          typedef struct _PCI_POWER_STATE                                                         // 10 elements, 0x40 bytes (sizeof) 
          {                                                                                                                           
/*0x000*/     enum _SYSTEM_POWER_STATE CurrentSystemState;                                                                            
/*0x004*/     enum _DEVICE_POWER_STATE CurrentDeviceState;                                                                            
/*0x008*/     enum _SYSTEM_POWER_STATE SystemWakeLevel;                                                                               
/*0x00C*/     enum _DEVICE_POWER_STATE DeviceWakeLevel;                                                                               
/*0x010*/     enum _DEVICE_POWER_STATE SystemStateMapping[7];                                                                         
/*0x02C*/     struct _IRP* WaitWakeIrp;                                                                                               
/*0x030*/     FUNCT_00BC_0718_DriverStartIo_CancelRoutine_SavedCancelRoutine* SavedCancelRoutine;                                     
/*0x034*/     LONG32       Paging;                                                                                                    
/*0x038*/     LONG32       Hibernate;                                                                                                 
/*0x03C*/     LONG32       CrashDump;                                                                                                 
          }PCI_POWER_STATE, *PPCI_POWER_STATE;                                                                                        
                                                                                                                                      
          typedef struct _PCI_LOCK          // 2 elements, 0x8 bytes (sizeof) 
          {                                                                   
/*0x000*/     ULONG32      Atom;                                              
/*0x004*/     UINT8        OldIrql;                                           
/*0x005*/     UINT8        _PADDING0_[0x3];                                   
          }PCI_LOCK, *PPCI_LOCK;                                              
                                                                              
          typedef struct _PCI_FDO_EXTENSION                        // 31 elements, 0xC0 bytes (sizeof) 
          {                                                                                            
/*0x000*/     struct _SINGLE_LIST_ENTRY List;                      // 1 elements, 0x4 bytes (sizeof)   
/*0x004*/     enum _PCI_SIGNATURE ExtensionType;                                                       
/*0x008*/     struct _PCI_MJ_DISPATCH_TABLE* IrpDispatchTable;                                         
/*0x00C*/     UINT8        DeviceState;                                                                
/*0x00D*/     UINT8        TentativeNextState;                                                         
/*0x00E*/     UINT8        _PADDING0_[0x2];                                                            
/*0x010*/     struct _KEVENT SecondaryExtLock;                     // 1 elements, 0x10 bytes (sizeof)  
/*0x020*/     struct _DEVICE_OBJECT* PhysicalDeviceObject;                                             
/*0x024*/     struct _DEVICE_OBJECT* FunctionalDeviceObject;                                           
/*0x028*/     struct _DEVICE_OBJECT* AttachedDeviceObject;                                             
/*0x02C*/     struct _KEVENT ChildListLock;                        // 1 elements, 0x10 bytes (sizeof)  
/*0x03C*/     struct _PCI_PDO_EXTENSION* ChildPdoList;                                                 
/*0x040*/     struct _PCI_FDO_EXTENSION* BusRootFdoExtension;                                          
/*0x044*/     struct _PCI_FDO_EXTENSION* ParentFdoExtension;                                           
/*0x048*/     struct _PCI_PDO_EXTENSION* ChildBridgePdoList;                                           
/*0x04C*/     struct _PCI_BUS_INTERFACE_STANDARD* PciBusInterface;                                     
/*0x050*/     UINT8        MaxSubordinateBus;                                                          
/*0x051*/     UINT8        _PADDING1_[0x3];                                                            
/*0x054*/     struct _BUS_HANDLER* BusHandler;                                                         
/*0x058*/     UINT8        BaseBus;                                                                    
/*0x059*/     UINT8        Fake;                                                                       
/*0x05A*/     UINT8        ChildDelete;                                                                
/*0x05B*/     UINT8        Scanned;                                                                    
/*0x05C*/     UINT8        ArbitersInitialized;                                                        
/*0x05D*/     UINT8        BrokenVideoHackApplied;                                                     
/*0x05E*/     UINT8        Hibernated;                                                                 
/*0x05F*/     UINT8        _PADDING2_[0x1];                                                            
/*0x060*/     struct _PCI_POWER_STATE PowerState;                  // 10 elements, 0x40 bytes (sizeof) 
/*0x0A0*/     struct _SINGLE_LIST_ENTRY SecondaryExtension;        // 1 elements, 0x4 bytes (sizeof)   
/*0x0A4*/     ULONG32      ChildWaitWakeCount;                                                         
/*0x0A8*/     struct _PCI_COMMON_CONFIG* PreservedConfig;                                              
/*0x0AC*/     struct _PCI_LOCK Lock;                               // 2 elements, 0x8 bytes (sizeof)   
              struct                                               // 5 elements, 0x5 bytes (sizeof)   
              {                                                                                        
/*0x0B4*/         UINT8        Acquired;                                                               
/*0x0B5*/         UINT8        CacheLineSize;                                                          
/*0x0B6*/         UINT8        LatencyTimer;                                                           
/*0x0B7*/         UINT8        EnablePERR;                                                             
/*0x0B8*/         UINT8        EnableSERR;                                                             
              }HotPlugParameters;                                                                      
/*0x0BC*/     ULONG32      BusHackFlags;                                                               
          }PCI_FDO_EXTENSION, *PPCI_FDO_EXTENSION;                                                     
                                                                                                       
          typedef struct _PCI_FUNCTION_RESOURCES                 // 2 elements, 0x150 bytes (sizeof) 
          {                                                                                          
/*0x000*/     struct _IO_RESOURCE_DESCRIPTOR Limit[7];                                               
/*0x0E0*/     struct _CM_PARTIAL_RESOURCE_DESCRIPTOR Current[7];                                     
          }PCI_FUNCTION_RESOURCES, *PPCI_FUNCTION_RESOURCES;                                         
                                                                                                     
          typedef union _PCI_HEADER_TYPE_DEPENDENT          // 3 elements, 0x4 bytes (sizeof) 
          {                                                                                   
              struct                                        // 1 elements, 0x4 bytes (sizeof) 
              {                                                                               
/*0x000*/         UINT8        Spare[4];                                                      
              }type0;                                                                         
              struct                                        // 8 elements, 0x4 bytes (sizeof) 
              {                                                                               
/*0x000*/         UINT8        PrimaryBus;                                                    
/*0x001*/         UINT8        SecondaryBus;                                                  
/*0x002*/         UINT8        SubordinateBus;                                                
                  struct                                    // 5 elements, 0x1 bytes (sizeof) 
                  {                                                                           
/*0x003*/             UINT8        SubtractiveDecode : 1;   // 0 BitPosition                  
/*0x003*/             UINT8        IsaBitSet : 1;           // 1 BitPosition                  
/*0x003*/             UINT8        VgaBitSet : 1;           // 2 BitPosition                  
/*0x003*/             UINT8        WeChangedBusNumbers : 1; // 3 BitPosition                  
/*0x003*/             UINT8        IsaBitRequired : 1;      // 4 BitPosition                  
                  };                                                                          
              }type1;                                                                         
              struct                                        // 8 elements, 0x4 bytes (sizeof) 
              {                                                                               
/*0x000*/         UINT8        PrimaryBus;                                                    
/*0x001*/         UINT8        SecondaryBus;                                                  
/*0x002*/         UINT8        SubordinateBus;                                                
                  struct                                    // 5 elements, 0x1 bytes (sizeof) 
                  {                                                                           
/*0x003*/             UINT8        SubtractiveDecode : 1;   // 0 BitPosition                  
/*0x003*/             UINT8        IsaBitSet : 1;           // 1 BitPosition                  
/*0x003*/             UINT8        VgaBitSet : 1;           // 2 BitPosition                  
/*0x003*/             UINT8        WeChangedBusNumbers : 1; // 3 BitPosition                  
/*0x003*/             UINT8        IsaBitRequired : 1;      // 4 BitPosition                  
                  };                                                                          
              }type2;                                                                         
          }PCI_HEADER_TYPE_DEPENDENT, *PPCI_HEADER_TYPE_DEPENDENT;                                  
                                                                                              
          typedef struct _PCI_INTERFACE                 // 9 elements, 0x1C bytes (sizeof) 
          {                                                                                
/*0x000*/     struct _GUID* InterfaceType;                                                 
/*0x004*/     UINT16       MinSize;                                                        
/*0x006*/     UINT16       MinVersion;                                                     
/*0x008*/     UINT16       MaxVersion;                                                     
/*0x00A*/     UINT16       Flags;                                                          
/*0x00C*/     LONG32       ReferenceCount;                                                 
/*0x010*/     enum _PCI_SIGNATURE Signature;                                               
/*0x014*/     FUNCT_0049_160F_Constructor* Constructor;                                    
/*0x018*/     FUNCT_0049_1617_Initializer* Initializer;                                    
          }PCI_INTERFACE, *PPCI_INTERFACE;                                                 
                                                                                           
          typedef struct _PCI_MJ_DISPATCH_TABLE                                                                                             // 8 elements, 0x20 bytes (sizeof) 
          {                                                                                                                                                                    
/*0x000*/     ULONG32      PnpIrpMaximumMinorFunction;                                                                                                                         
/*0x004*/     struct _PCI_MN_DISPATCH_TABLE* PnpIrpDispatchTable;                                                                                                              
/*0x008*/     ULONG32      PowerIrpMaximumMinorFunction;                                                                                                                       
/*0x00C*/     struct _PCI_MN_DISPATCH_TABLE* PowerIrpDispatchTable;                                                                                                            
/*0x010*/     enum _PCI_DISPATCH_STYLE SystemControlIrpDispatchStyle;                                                                                                          
/*0x014*/     FUNCT_0049_138A_SystemControlIrpDispatchFunction_OtherIrpDispatchFunction_DispatchFunction* SystemControlIrpDispatchFunction;                                    
/*0x018*/     enum _PCI_DISPATCH_STYLE OtherIrpDispatchStyle;                                                                                                                  
/*0x01C*/     FUNCT_0049_138A_SystemControlIrpDispatchFunction_OtherIrpDispatchFunction_DispatchFunction* OtherIrpDispatchFunction;                                            
          }PCI_MJ_DISPATCH_TABLE, *PPCI_MJ_DISPATCH_TABLE;                                                                                                                     
                                                                                                                                                                               
          typedef struct _PCI_MN_DISPATCH_TABLE                                                                             // 2 elements, 0x8 bytes (sizeof) 
          {                                                                                                                                                   
/*0x000*/     enum _PCI_DISPATCH_STYLE DispatchStyle;                                                                                                         
/*0x004*/     FUNCT_0049_138A_SystemControlIrpDispatchFunction_OtherIrpDispatchFunction_DispatchFunction* DispatchFunction;                                   
          }PCI_MN_DISPATCH_TABLE, *PPCI_MN_DISPATCH_TABLE;                                                                                                    
                                                                                                                                                              
          typedef struct _PCI_SLOT_NUMBER              // 1 elements, 0x4 bytes (sizeof) 
          {                                                                              
              union                                    // 2 elements, 0x4 bytes (sizeof) 
              {                                                                          
                  struct                               // 3 elements, 0x4 bytes (sizeof) 
                  {                                                                      
/*0x000*/             ULONG32      DeviceNumber : 5;   // 0 BitPosition                  
/*0x000*/             ULONG32      FunctionNumber : 3; // 5 BitPosition                  
/*0x000*/             ULONG32      Reserved : 24;      // 8 BitPosition                  
                  }bits;                                                                 
/*0x000*/         ULONG32      AsULONG;                                                  
              }u;                                                                        
          }PCI_SLOT_NUMBER, *PPCI_SLOT_NUMBER;                                           
                                                                                         
          typedef struct _PM_SUPPORT      // 8 elements, 0x1 bytes (sizeof) 
          {                                                                 
/*0x000*/     UINT8        Rsvd2 : 1;     // 0 BitPosition                  
/*0x000*/     UINT8        D1 : 1;        // 1 BitPosition                  
/*0x000*/     UINT8        D2 : 1;        // 2 BitPosition                  
/*0x000*/     UINT8        PMED0 : 1;     // 3 BitPosition                  
/*0x000*/     UINT8        PMED1 : 1;     // 4 BitPosition                  
/*0x000*/     UINT8        PMED2 : 1;     // 5 BitPosition                  
/*0x000*/     UINT8        PMED3Hot : 1;  // 6 BitPosition                  
/*0x000*/     UINT8        PMED3Cold : 1; // 7 BitPosition                  
          }PM_SUPPORT, *PPM_SUPPORT;                                        
                                                                            
          typedef struct _PCI_PMC                                // 6 elements, 0x2 bytes (sizeof) 
          {                                                                                        
              struct                                             // 5 elements, 0x1 bytes (sizeof) 
              {                                                                                    
/*0x000*/         UINT8        Version : 3;                      // 0 BitPosition                  
/*0x000*/         UINT8        PMEClock : 1;                     // 3 BitPosition                  
/*0x000*/         UINT8        Rsvd1 : 1;                        // 4 BitPosition                  
/*0x000*/         UINT8        DeviceSpecificInitialization : 1; // 5 BitPosition                  
/*0x000*/         UINT8        Rsvd2 : 2;                        // 6 BitPosition                  
              };                                                                                   
/*0x001*/     struct _PM_SUPPORT Support;                        // 8 elements, 0x1 bytes (sizeof) 
          }PCI_PMC, *PPCI_PMC;                                                                     
                                                                                                   
          typedef struct _PCI_PDO_EXTENSION                    // 53 elements, 0xC8 bytes (sizeof) 
          {                                                                                        
/*0x000*/     struct _PCI_PDO_EXTENSION* Next;                                                     
/*0x004*/     enum _PCI_SIGNATURE ExtensionType;                                                   
/*0x008*/     struct _PCI_MJ_DISPATCH_TABLE* IrpDispatchTable;                                     
/*0x00C*/     UINT8        DeviceState;                                                            
/*0x00D*/     UINT8        TentativeNextState;                                                     
/*0x00E*/     UINT8        _PADDING0_[0x2];                                                        
/*0x010*/     struct _KEVENT SecondaryExtLock;                 // 1 elements, 0x10 bytes (sizeof)  
/*0x020*/     struct _PCI_SLOT_NUMBER Slot;                    // 1 elements, 0x4 bytes (sizeof)   
/*0x024*/     struct _DEVICE_OBJECT* PhysicalDeviceObject;                                         
/*0x028*/     struct _PCI_FDO_EXTENSION* ParentFdoExtension;                                       
/*0x02C*/     struct _SINGLE_LIST_ENTRY SecondaryExtension;    // 1 elements, 0x4 bytes (sizeof)   
/*0x030*/     ULONG32      BusInterfaceReferenceCount;                                             
/*0x034*/     ULONG32      AgpInterfaceReferenceCount;                                             
/*0x038*/     UINT16       VendorId;                                                               
/*0x03A*/     UINT16       DeviceId;                                                               
/*0x03C*/     UINT16       SubsystemVendorId;                                                      
/*0x03E*/     UINT16       SubsystemId;                                                            
/*0x040*/     UINT8        RevisionId;                                                             
/*0x041*/     UINT8        ProgIf;                                                                 
/*0x042*/     UINT8        SubClass;                                                               
/*0x043*/     UINT8        BaseClass;                                                              
/*0x044*/     UINT8        AdditionalResourceCount;                                                
/*0x045*/     UINT8        AdjustedInterruptLine;                                                  
/*0x046*/     UINT8        InterruptPin;                                                           
/*0x047*/     UINT8        RawInterruptLine;                                                       
/*0x048*/     UINT8        CapabilitiesPtr;                                                        
/*0x049*/     UINT8        SavedLatencyTimer;                                                      
/*0x04A*/     UINT8        SavedCacheLineSize;                                                     
/*0x04B*/     UINT8        HeaderType;                                                             
/*0x04C*/     UINT8        NotPresent;                                                             
/*0x04D*/     UINT8        ReportedMissing;                                                        
/*0x04E*/     UINT8        ExpectedWritebackFailure;                                               
/*0x04F*/     UINT8        NoTouchPmeEnable;                                                       
/*0x050*/     UINT8        LegacyDriver;                                                           
/*0x051*/     UINT8        UpdateHardware;                                                         
/*0x052*/     UINT8        MovedDevice;                                                            
/*0x053*/     UINT8        DisablePowerDown;                                                       
/*0x054*/     UINT8        NeedsHotPlugConfiguration;                                              
/*0x055*/     UINT8        SwitchedIDEToNativeMode;                                                
/*0x056*/     UINT8        BIOSAllowsIDESwitchToNativeMode;                                        
/*0x057*/     UINT8        IoSpaceUnderNativeIdeControl;                                           
/*0x058*/     UINT8        OnDebugPath;                                                            
/*0x059*/     UINT8        _PADDING1_[0x3];                                                        
/*0x05C*/     struct _PCI_POWER_STATE PowerState;              // 10 elements, 0x40 bytes (sizeof) 
/*0x09C*/     union _PCI_HEADER_TYPE_DEPENDENT Dependent;      // 3 elements, 0x4 bytes (sizeof)   
/*0x0A0*/     UINT64       HackFlags;                                                              
/*0x0A8*/     struct _PCI_FUNCTION_RESOURCES* Resources;                                           
/*0x0AC*/     struct _PCI_FDO_EXTENSION* BridgeFdoExtension;                                       
/*0x0B0*/     struct _PCI_PDO_EXTENSION* NextBridge;                                               
/*0x0B4*/     struct _PCI_PDO_EXTENSION* NextHashEntry;                                            
/*0x0B8*/     struct _PCI_LOCK Lock;                           // 2 elements, 0x8 bytes (sizeof)   
/*0x0C0*/     struct _PCI_PMC PowerCapabilities;               // 6 elements, 0x2 bytes (sizeof)   
/*0x0C2*/     UINT8        TargetAgpCapabilityId;                                                  
/*0x0C3*/     UINT8        _PADDING2_[0x1];                                                        
/*0x0C4*/     UINT16       CommandEnables;                                                         
/*0x0C6*/     UINT16       InitialCommand;                                                         
          }PCI_PDO_EXTENSION, *PPCI_PDO_EXTENSION;                                                 
                                                                                                   
          typedef struct _PEB                                                                               // 65 elements, 0x210 bytes (sizeof) 
          {                                                                                                                                      
/*0x000*/     UINT8        InheritedAddressSpace;                                                                                                
/*0x001*/     UINT8        ReadImageFileExecOptions;                                                                                             
/*0x002*/     UINT8        BeingDebugged;                                                                                                        
/*0x003*/     UINT8        SpareBool;                                                                                                            
/*0x004*/     VOID*        Mutant;                                                                                                               
/*0x008*/     VOID*        ImageBaseAddress;                                                                                                     
/*0x00C*/     struct _PEB_LDR_DATA* Ldr;                                                                                                         
/*0x010*/     struct _RTL_USER_PROCESS_PARAMETERS* ProcessParameters;                                                                            
/*0x014*/     VOID*        SubSystemData;                                                                                                        
/*0x018*/     VOID*        ProcessHeap;                                                                                                          
/*0x01C*/     struct _RTL_CRITICAL_SECTION* FastPebLock;                                                                                         
/*0x020*/     VOID*        FastPebLockRoutine;                                                                                                   
/*0x024*/     VOID*        FastPebUnlockRoutine;                                                                                                 
/*0x028*/     ULONG32      EnvironmentUpdateCount;                                                                                               
/*0x02C*/     VOID*        KernelCallbackTable;                                                                                                  
/*0x030*/     ULONG32      SystemReserved[1];                                                                                                    
/*0x034*/     ULONG32      AtlThunkSListPtr32;                                                                                                   
/*0x038*/     struct _PEB_FREE_BLOCK* FreeList;                                                                                                  
/*0x03C*/     ULONG32      TlsExpansionCounter;                                                                                                  
/*0x040*/     VOID*        TlsBitmap;                                                                                                            
/*0x044*/     ULONG32      TlsBitmapBits[2];                                                                                                     
/*0x04C*/     VOID*        ReadOnlySharedMemoryBase;                                                                                             
/*0x050*/     VOID*        ReadOnlySharedMemoryHeap;                                                                                             
/*0x054*/     VOID**       ReadOnlyStaticServerData;                                                                                             
/*0x058*/     VOID*        AnsiCodePageData;                                                                                                     
/*0x05C*/     VOID*        OemCodePageData;                                                                                                      
/*0x060*/     VOID*        UnicodeCaseTableData;                                                                                                 
/*0x064*/     ULONG32      NumberOfProcessors;                                                                                                   
/*0x068*/     ULONG32      NtGlobalFlag;                                                                                                         
/*0x06C*/     UINT8        _PADDING0_[0x4];                                                                                                      
/*0x070*/     union _LARGE_INTEGER CriticalSectionTimeout;                                                  // 4 elements, 0x8 bytes (sizeof)    
/*0x078*/     ULONG32      HeapSegmentReserve;                                                                                                   
/*0x07C*/     ULONG32      HeapSegmentCommit;                                                                                                    
/*0x080*/     ULONG32      HeapDeCommitTotalFreeThreshold;                                                                                       
/*0x084*/     ULONG32      HeapDeCommitFreeBlockThreshold;                                                                                       
/*0x088*/     ULONG32      NumberOfHeaps;                                                                                                        
/*0x08C*/     ULONG32      MaximumNumberOfHeaps;                                                                                                 
/*0x090*/     VOID**       ProcessHeaps;                                                                                                         
/*0x094*/     VOID*        GdiSharedHandleTable;                                                                                                 
/*0x098*/     VOID*        ProcessStarterHelper;                                                                                                 
/*0x09C*/     ULONG32      GdiDCAttributeList;                                                                                                   
/*0x0A0*/     VOID*        LoaderLock;                                                                                                           
/*0x0A4*/     ULONG32      OSMajorVersion;                                                                                                       
/*0x0A8*/     ULONG32      OSMinorVersion;                                                                                                       
/*0x0AC*/     UINT16       OSBuildNumber;                                                                                                        
/*0x0AE*/     UINT16       OSCSDVersion;                                                                                                         
/*0x0B0*/     ULONG32      OSPlatformId;                                                                                                         
/*0x0B4*/     ULONG32      ImageSubsystem;                                                                                                       
/*0x0B8*/     ULONG32      ImageSubsystemMajorVersion;                                                                                           
/*0x0BC*/     ULONG32      ImageSubsystemMinorVersion;                                                                                           
/*0x0C0*/     ULONG32      ImageProcessAffinityMask;                                                                                             
/*0x0C4*/     ULONG32      GdiHandleBuffer[34];                                                                                                  
/*0x14C*/     FUNCT_00BC_0631_PostProcessInitRoutine_DispatchAddress_FinishRoutine* PostProcessInitRoutine;                                      
/*0x150*/     VOID*        TlsExpansionBitmap;                                                                                                   
/*0x154*/     ULONG32      TlsExpansionBitmapBits[32];                                                                                           
/*0x1D4*/     ULONG32      SessionId;                                                                                                            
/*0x1D8*/     union _ULARGE_INTEGER AppCompatFlags;                                                         // 4 elements, 0x8 bytes (sizeof)    
/*0x1E0*/     union _ULARGE_INTEGER AppCompatFlagsUser;                                                     // 4 elements, 0x8 bytes (sizeof)    
/*0x1E8*/     VOID*        pShimData;                                                                                                            
/*0x1EC*/     VOID*        AppCompatInfo;                                                                                                        
/*0x1F0*/     struct _UNICODE_STRING CSDVersion;                                                            // 3 elements, 0x8 bytes (sizeof)    
/*0x1F8*/     VOID*        ActivationContextData;                                                                                                
/*0x1FC*/     VOID*        ProcessAssemblyStorageMap;                                                                                            
/*0x200*/     VOID*        SystemDefaultActivationContextData;                                                                                   
/*0x204*/     VOID*        SystemAssemblyStorageMap;                                                                                             
/*0x208*/     ULONG32      MinimumStackCommit;                                                                                                   
/*0x20C*/     UINT8        _PADDING1_[0x4];                                                                                                      
          }PEB, *PPEB;                                                                                                                           
                                                                                                                                                 
          typedef struct _PEB_FREE_BLOCK    // 2 elements, 0x8 bytes (sizeof) 
          {                                                                   
/*0x000*/     struct _PEB_FREE_BLOCK* Next;                                   
/*0x004*/     ULONG32      Size;                                              
          }PEB_FREE_BLOCK, *PPEB_FREE_BLOCK;                                  
                                                                              
          typedef struct _PEB_LDR_DATA                            // 7 elements, 0x28 bytes (sizeof) 
          {                                                                                          
/*0x000*/     ULONG32      Length;                                                                   
/*0x004*/     UINT8        Initialized;                                                              
/*0x005*/     UINT8        _PADDING0_[0x3];                                                          
/*0x008*/     VOID*        SsHandle;                                                                 
/*0x00C*/     struct _LIST_ENTRY InLoadOrderModuleList;           // 2 elements, 0x8 bytes (sizeof)  
/*0x014*/     struct _LIST_ENTRY InMemoryOrderModuleList;         // 2 elements, 0x8 bytes (sizeof)  
/*0x01C*/     struct _LIST_ENTRY InInitializationOrderModuleList; // 2 elements, 0x8 bytes (sizeof)  
/*0x024*/     VOID*        EntryInProgress;                                                          
          }PEB_LDR_DATA, *PPEB_LDR_DATA;                                                             
                                                                                                     
          typedef struct _PHYSICAL_MEMORY_RUN // 2 elements, 0x8 bytes (sizeof) 
          {                                                                     
/*0x000*/     ULONG32      BasePage;                                            
/*0x004*/     ULONG32      PageCount;                                           
          }PHYSICAL_MEMORY_RUN, *PPHYSICAL_MEMORY_RUN;                                  
                                                                                
          typedef struct _PHYSICAL_MEMORY_DESCRIPTOR // 3 elements, 0x10 bytes (sizeof) 
          {                                                                             
/*0x000*/     ULONG32      NumberOfRuns;                                                
/*0x004*/     ULONG32      NumberOfPages;                                               
/*0x008*/     struct _PHYSICAL_MEMORY_RUN Run[1];                                       
          }PHYSICAL_MEMORY_DESCRIPTOR, *PPHYSICAL_MEMORY_DESCRIPTOR;                                   
                                                                                        
          typedef struct _PI_BUS_EXTENSION                // 17 elements, 0x44 bytes (sizeof) 
          {                                                                                   
/*0x000*/     ULONG32      Flags;                                                             
/*0x004*/     ULONG32      NumberCSNs;                                                        
/*0x008*/     UINT8*       ReadDataPort;                                                      
/*0x00C*/     UINT8        DataPortMapped;                                                    
/*0x00D*/     UINT8        _PADDING0_[0x3];                                                   
/*0x010*/     UINT8*       AddressPort;                                                       
/*0x014*/     UINT8        AddrPortMapped;                                                    
/*0x015*/     UINT8        _PADDING1_[0x3];                                                   
/*0x018*/     UINT8*       CommandPort;                                                       
/*0x01C*/     UINT8        CmdPortMapped;                                                     
/*0x01D*/     UINT8        _PADDING2_[0x3];                                                   
/*0x020*/     ULONG32      NextSlotNumber;                                                    
/*0x024*/     struct _SINGLE_LIST_ENTRY DeviceList;       // 1 elements, 0x4 bytes (sizeof)   
/*0x028*/     struct _SINGLE_LIST_ENTRY CardList;         // 1 elements, 0x4 bytes (sizeof)   
/*0x02C*/     struct _DEVICE_OBJECT* PhysicalBusDevice;                                       
/*0x030*/     struct _DEVICE_OBJECT* FunctionalBusDevice;                                     
/*0x034*/     struct _DEVICE_OBJECT* AttachedDevice;                                          
/*0x038*/     ULONG32      BusNumber;                                                         
/*0x03C*/     enum _SYSTEM_POWER_STATE SystemPowerState;                                      
/*0x040*/     enum _DEVICE_POWER_STATE DevicePowerState;                                      
          }PI_BUS_EXTENSION, *PPI_BUS_EXTENSION;                                              
                                                                                              
          typedef struct _PI_RESOURCE_ARBITER_ENTRY        // 10 elements, 0x38 bytes (sizeof) 
          {                                                                                    
/*0x000*/     struct _LIST_ENTRY DeviceArbiterList;        // 2 elements, 0x8 bytes (sizeof)   
/*0x008*/     UINT8        ResourceType;                                                       
/*0x009*/     UINT8        _PADDING0_[0x3];                                                    
/*0x00C*/     struct _ARBITER_INTERFACE* ArbiterInterface;                                     
/*0x010*/     ULONG32      Level;                                                              
/*0x014*/     struct _LIST_ENTRY ResourceList;             // 2 elements, 0x8 bytes (sizeof)   
/*0x01C*/     struct _LIST_ENTRY BestResourceList;         // 2 elements, 0x8 bytes (sizeof)   
/*0x024*/     struct _LIST_ENTRY BestConfig;               // 2 elements, 0x8 bytes (sizeof)   
/*0x02C*/     struct _LIST_ENTRY ActiveArbiterList;        // 2 elements, 0x8 bytes (sizeof)   
/*0x034*/     UINT8        State;                                                              
/*0x035*/     UINT8        ResourcesChanged;                                                   
/*0x036*/     UINT8        _PADDING1_[0x2];                                                    
          }PI_RESOURCE_ARBITER_ENTRY, *PPI_RESOURCE_ARBITER_ENTRY;                                    
                                                                                               
          typedef struct _PLUGPLAY_EVENT_BLOCK                // 7 elements, 0x38 bytes (sizeof) 
          {                                                                                      
/*0x000*/     struct _GUID EventGuid;                         // 4 elements, 0x10 bytes (sizeof) 
/*0x010*/     enum _PLUGPLAY_EVENT_CATEGORY EventCategory;                                       
/*0x014*/     ULONG32*     Result;                                                               
/*0x018*/     ULONG32      Flags;                                                                
/*0x01C*/     ULONG32      TotalSize;                                                            
/*0x020*/     VOID*        DeviceObject;                                                         
              union                                           // 8 elements, 0x14 bytes (sizeof) 
              {                                                                                  
                  struct                                      // 2 elements, 0x14 bytes (sizeof) 
                  {                                                                              
/*0x024*/             struct _GUID ClassGuid;                 // 4 elements, 0x10 bytes (sizeof) 
/*0x034*/             UINT16       SymbolicLinkName[1];                                          
/*0x036*/             UINT8        _PADDING0_[0x2];                                              
                  }DeviceClass;                                                                  
                  struct                                      // 1 elements, 0x2 bytes (sizeof)  
                  {                                                                              
/*0x024*/             UINT16       DeviceIds[1];                                                 
                  }TargetDevice;                                                                 
                  struct                                      // 1 elements, 0x2 bytes (sizeof)  
                  {                                                                              
/*0x024*/             UINT16       DeviceId[1];                                                  
                  }InstallDevice;                                                                
                  struct                                      // 2 elements, 0x8 bytes (sizeof)  
                  {                                                                              
/*0x024*/             VOID*        NotificationStructure;                                        
/*0x028*/             UINT16       DeviceIds[1];                                                 
/*0x02A*/             UINT8        _PADDING0_[0x2];                                              
                  }CustomNotification;                                                           
                  struct                                      // 1 elements, 0x4 bytes (sizeof)  
                  {                                                                              
/*0x024*/             VOID*        Notification;                                                 
                  }ProfileNotification;                                                          
                  struct                                      // 2 elements, 0x8 bytes (sizeof)  
                  {                                                                              
/*0x024*/             ULONG32      NotificationCode;                                             
/*0x028*/             ULONG32      NotificationData;                                             
                  }PowerNotification;                                                            
                  struct                                      // 2 elements, 0x8 bytes (sizeof)  
                  {                                                                              
/*0x024*/             enum _PNP_VETO_TYPE VetoType;                                              
/*0x028*/             UINT16       DeviceIdVetoNameBuffer[1];                                    
/*0x02A*/             UINT8        _PADDING0_[0x2];                                              
                  }VetoNotification;                                                             
                  struct                                      // 1 elements, 0x10 bytes (sizeof) 
                  {                                                                              
/*0x024*/             struct _GUID BlockedDriverGuid;         // 4 elements, 0x10 bytes (sizeof) 
                  }BlockedDriverNotification;                                                    
              }u;                                                                                
          }PLUGPLAY_EVENT_BLOCK, *PPLUGPLAY_EVENT_BLOCK;                                         
                                                                                                 
          typedef struct _PNP_DEVICE_EVENT_ENTRY                                                                                                                                  // 8 elements, 0x58 bytes (sizeof) 
          {                                                                                                                                                                                                          
/*0x000*/     struct _LIST_ENTRY ListEntry;                                                                                                                                       // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/     ULONG32      Argument;                                                                                                                                                                                 
/*0x00C*/     struct _KEVENT* CallerEvent;                                                                                                                                                                           
/*0x010*/     FUNCT_00BC_0285_Free_InterfaceReference_InterfaceDereference_DeleteProcedure_ReleaseFromLazyWrite_ReleaseFromReadAhead_WorkerRoutine_Destructor_Callback* Callback;                                    
/*0x014*/     VOID*        Context;                                                                                                                                                                                  
/*0x018*/     enum _PNP_VETO_TYPE* VetoType;                                                                                                                                                                         
/*0x01C*/     struct _UNICODE_STRING* VetoName;                                                                                                                                                                      
/*0x020*/     struct _PLUGPLAY_EVENT_BLOCK Data;                                                                                                                                  // 7 elements, 0x38 bytes (sizeof) 
          }PNP_DEVICE_EVENT_ENTRY, *PPNP_DEVICE_EVENT_ENTRY;                                                                                                                                                         
                                                                                                                                                                                                                     
          typedef struct _PNP_DEVICE_EVENT_LIST // 4 elements, 0x4C bytes (sizeof) 
          {                                                                        
/*0x000*/     LONG32       Status;                                                 
/*0x004*/     struct _KMUTANT EventQueueMutex;  // 5 elements, 0x20 bytes (sizeof) 
/*0x024*/     struct _FAST_MUTEX Lock;          // 5 elements, 0x20 bytes (sizeof) 
/*0x044*/     struct _LIST_ENTRY List;          // 2 elements, 0x8 bytes (sizeof)  
          }PNP_DEVICE_EVENT_LIST, *PPNP_DEVICE_EVENT_LIST;                                   
                                                                                   
          typedef struct _PO_DEVICE_NOTIFY         // 10 elements, 0x28 bytes (sizeof) 
          {                                                                            
/*0x000*/     struct _LIST_ENTRY Link;             // 2 elements, 0x8 bytes (sizeof)   
/*0x008*/     struct _DEVICE_OBJECT* TargetDevice;                                     
/*0x00C*/     UINT8        WakeNeeded;                                                 
/*0x00D*/     UINT8        OrderLevel;                                                 
/*0x00E*/     UINT8        _PADDING0_[0x2];                                            
/*0x010*/     struct _DEVICE_OBJECT* DeviceObject;                                     
/*0x014*/     VOID*        Node;                                                       
/*0x018*/     UINT16*      DeviceName;                                                 
/*0x01C*/     UINT16*      DriverName;                                                 
/*0x020*/     ULONG32      ChildCount;                                                 
/*0x024*/     ULONG32      ActiveChild;                                                
          }PO_DEVICE_NOTIFY, *PPO_DEVICE_NOTIFY;                                       
                                                                                       
          typedef struct _PO_NOTIFY_ORDER_LEVEL // 9 elements, 0x48 bytes (sizeof) 
          {                                                                        
/*0x000*/     struct _KEVENT LevelReady;        // 1 elements, 0x10 bytes (sizeof) 
/*0x010*/     ULONG32      DeviceCount;                                            
/*0x014*/     ULONG32      ActiveCount;                                            
/*0x018*/     struct _LIST_ENTRY WaitSleep;     // 2 elements, 0x8 bytes (sizeof)  
/*0x020*/     struct _LIST_ENTRY ReadySleep;    // 2 elements, 0x8 bytes (sizeof)  
/*0x028*/     struct _LIST_ENTRY Pending;       // 2 elements, 0x8 bytes (sizeof)  
/*0x030*/     struct _LIST_ENTRY Complete;      // 2 elements, 0x8 bytes (sizeof)  
/*0x038*/     struct _LIST_ENTRY ReadyS0;       // 2 elements, 0x8 bytes (sizeof)  
/*0x040*/     struct _LIST_ENTRY WaitS0;        // 2 elements, 0x8 bytes (sizeof)  
          }PO_NOTIFY_ORDER_LEVEL, *PPO_NOTIFY_ORDER_LEVEL;                                   
                                                                                   
          typedef struct _PO_DEVICE_NOTIFY_ORDER           // 3 elements, 0x248 bytes (sizeof) 
          {                                                                                    
/*0x000*/     ULONG32      DevNodeSequence;                                                    
/*0x004*/     struct _DEVICE_OBJECT** WarmEjectPdoPointer;                                     
/*0x008*/     struct _PO_NOTIFY_ORDER_LEVEL OrderLevel[8];                                     
          }PO_DEVICE_NOTIFY_ORDER, *PPO_DEVICE_NOTIFY_ORDER;                                    
                                                                                               
          typedef struct _PO_HIBER_PERF     // 13 elements, 0x48 bytes (sizeof) 
          {                                                                     
/*0x000*/     UINT64       IoTicks;                                             
/*0x008*/     UINT64       InitTicks;                                           
/*0x010*/     UINT64       CopyTicks;                                           
/*0x018*/     UINT64       StartCount;                                          
/*0x020*/     ULONG32      ElapsedTime;                                         
/*0x024*/     ULONG32      IoTime;                                              
/*0x028*/     ULONG32      CopyTime;                                            
/*0x02C*/     ULONG32      InitTime;                                            
/*0x030*/     ULONG32      PagesWritten;                                        
/*0x034*/     ULONG32      PagesProcessed;                                      
/*0x038*/     ULONG32      BytesCopied;                                         
/*0x03C*/     ULONG32      DumpCount;                                           
/*0x040*/     ULONG32      FileRuns;                                            
/*0x044*/     UINT8        _PADDING0_[0x4];                                     
          }PO_HIBER_PERF, *PPO_HIBER_PERF;                                      
                                                                                
          typedef struct _PO_MEMORY_IMAGE      // 22 elements, 0xA8 bytes (sizeof) 
          {                                                                        
/*0x000*/     ULONG32      Signature;                                              
/*0x004*/     ULONG32      Version;                                                
/*0x008*/     ULONG32      CheckSum;                                               
/*0x00C*/     ULONG32      LengthSelf;                                             
/*0x010*/     ULONG32      PageSelf;                                               
/*0x014*/     ULONG32      PageSize;                                               
/*0x018*/     ULONG32      ImageType;                                              
/*0x01C*/     UINT8        _PADDING0_[0x4];                                        
/*0x020*/     union _LARGE_INTEGER SystemTime; // 4 elements, 0x8 bytes (sizeof)   
/*0x028*/     UINT64       InterruptTime;                                          
/*0x030*/     ULONG32      FeatureFlags;                                           
/*0x034*/     UINT8        HiberFlags;                                             
/*0x035*/     UINT8        spare[3];                                               
/*0x038*/     ULONG32      NoHiberPtes;                                            
/*0x03C*/     ULONG32      HiberVa;                                                
/*0x040*/     union _LARGE_INTEGER HiberPte;   // 4 elements, 0x8 bytes (sizeof)   
/*0x048*/     ULONG32      NoFreePages;                                            
/*0x04C*/     ULONG32      FreeMapCheck;                                           
/*0x050*/     ULONG32      WakeCheck;                                              
/*0x054*/     ULONG32      TotalPages;                                             
/*0x058*/     ULONG32      FirstTablePage;                                         
/*0x05C*/     ULONG32      LastFilePage;                                           
/*0x060*/     struct _PO_HIBER_PERF PerfInfo;  // 13 elements, 0x48 bytes (sizeof) 
          }PO_MEMORY_IMAGE, *PPO_MEMORY_IMAGE;                                     
                                                                                   
          typedef struct _PO_MEMORY_RANGE_ARRAY            // 2 elements, 0x10 bytes (sizeof) 
          {                                                                                   
              union                                        // 2 elements, 0x10 bytes (sizeof) 
              {                                                                               
                  struct                                   // 4 elements, 0x10 bytes (sizeof) 
                  {                                                                           
/*0x000*/             ULONG32      PageNo;                                                    
/*0x004*/             ULONG32      StartPage;                                                 
/*0x008*/             ULONG32      EndPage;                                                   
/*0x00C*/             ULONG32      CheckSum;                                                  
                  }Range;                                                                     
                  struct                                   // 4 elements, 0x10 bytes (sizeof) 
                  {                                                                           
/*0x000*/             struct _PO_MEMORY_RANGE_ARRAY* Next;                                    
/*0x004*/             ULONG32      NextTable;                                                 
/*0x008*/             ULONG32      CheckSum;                                                  
/*0x00C*/             ULONG32      EntryCount;                                                
                  }Link;                                                                      
              };                                                                              
          }PO_MEMORY_RANGE_ARRAY, *PPO_MEMORY_RANGE_ARRAY;                                    
                                                                                              
          typedef struct _POOL_HEADER                       // 9 elements, 0x8 bytes (sizeof) 
          {                                                                                   
              union                                         // 2 elements, 0x4 bytes (sizeof) 
              {                                                                               
                  struct                                    // 2 elements, 0x4 bytes (sizeof) 
                  {                                                                           
                      struct                                // 2 elements, 0x2 bytes (sizeof) 
                      {                                                                       
/*0x000*/                 UINT16       PreviousSize : 9;    // 0 BitPosition                  
/*0x000*/                 UINT16       PoolIndex : 7;       // 9 BitPosition                  
                      };                                                                      
                      struct                                // 2 elements, 0x2 bytes (sizeof) 
                      {                                                                       
/*0x002*/                 UINT16       BlockSize : 9;       // 0 BitPosition                  
/*0x002*/                 UINT16       PoolType : 7;        // 9 BitPosition                  
                      };                                                                      
                  };                                                                          
/*0x000*/         ULONG32      Ulong1;                                                        
              };                                                                              
              union                                         // 3 elements, 0x4 bytes (sizeof) 
              {                                                                               
/*0x004*/         struct _EPROCESS* ProcessBilled;                                            
/*0x004*/         ULONG32      PoolTag;                                                       
                  struct                                    // 2 elements, 0x4 bytes (sizeof) 
                  {                                                                           
/*0x004*/             UINT16       AllocatorBackTraceIndex;                                   
/*0x006*/             UINT16       PoolTagHash;                                               
                  };                                                                          
              };                                                                              
          }POOL_HEADER, *PPOOL_HEADER;                                                        
                                                                                              
          typedef struct _POOL_BLOCK_HEAD // 2 elements, 0x10 bytes (sizeof) 
          {                                                                  
/*0x000*/     struct _POOL_HEADER Header; // 9 elements, 0x8 bytes (sizeof)  
/*0x008*/     struct _LIST_ENTRY List;    // 2 elements, 0x8 bytes (sizeof)  
          }POOL_BLOCK_HEAD, *PPOOL_BLOCK_HEAD;                                   
                                                                             
          typedef struct _POOL_HACKER     // 2 elements, 0x28 bytes (sizeof) 
          {                                                                  
/*0x000*/     struct _POOL_HEADER Header; // 9 elements, 0x8 bytes (sizeof)  
/*0x008*/     ULONG32      Contents[8];                                      
          }POOL_HACKER, *PPOOL_HACKER;                                       
                                                                             
          typedef struct _POOL_TRACKER_BIG_PAGES // 3 elements, 0xC bytes (sizeof) 
          {                                                                        
/*0x000*/     VOID*        Va;                                                     
/*0x004*/     ULONG32      Key;                                                    
/*0x008*/     ULONG32      NumberOfPages;                                          
          }POOL_TRACKER_BIG_PAGES, *PPOOL_TRACKER_BIG_PAGES;                                  
                                                                                   
          typedef struct _POOL_TRACKER_TABLE // 7 elements, 0x1C bytes (sizeof) 
          {                                                                     
/*0x000*/     ULONG32      Key;                                                 
/*0x004*/     ULONG32      NonPagedAllocs;                                      
/*0x008*/     ULONG32      NonPagedFrees;                                       
/*0x00C*/     ULONG32      NonPagedBytes;                                       
/*0x010*/     ULONG32      PagedAllocs;                                         
/*0x014*/     ULONG32      PagedFrees;                                          
/*0x018*/     ULONG32      PagedBytes;                                          
          }POOL_TRACKER_TABLE, *PPOOL_TRACKER_TABLE;                                   
                                                                                
          typedef struct _POP_ACTION_TRIGGER      // 5 elements, 0xC bytes (sizeof) 
          {                                                                         
/*0x000*/     enum _POP_POLICY_DEVICE_TYPE Type;                                    
/*0x004*/     UINT8        Flags;                                                   
/*0x005*/     UINT8        Spare[3];                                                
              union                               // 2 elements, 0x4 bytes (sizeof) 
              {                                                                     
                  struct                          // 1 elements, 0x4 bytes (sizeof) 
                  {                                                                 
/*0x008*/             ULONG32      Level;                                           
                  }Battery;                                                         
/*0x008*/         struct _POP_TRIGGER_WAIT* Wait;                                   
              };                                                                    
          }POP_ACTION_TRIGGER, *PPOP_ACTION_TRIGGER;                                  
                                                                                    
          typedef struct _POP_DEVICE_POWER_IRP  // 7 elements, 0x2C bytes (sizeof) 
          {                                                                        
/*0x000*/     struct _SINGLE_LIST_ENTRY Free;   // 1 elements, 0x4 bytes (sizeof)  
/*0x004*/     struct _IRP* Irp;                                                    
/*0x008*/     struct _PO_DEVICE_NOTIFY* Notify;                                    
/*0x00C*/     struct _LIST_ENTRY Pending;       // 2 elements, 0x8 bytes (sizeof)  
/*0x014*/     struct _LIST_ENTRY Complete;      // 2 elements, 0x8 bytes (sizeof)  
/*0x01C*/     struct _LIST_ENTRY Abort;         // 2 elements, 0x8 bytes (sizeof)  
/*0x024*/     struct _LIST_ENTRY Failed;        // 2 elements, 0x8 bytes (sizeof)  
          }POP_DEVICE_POWER_IRP, *PPOP_DEVICE_POWER_IRP;                                   
                                                                                   
          typedef struct _POP_DEVICE_SYS_STATE                // 18 elements, 0x620 bytes (sizeof) 
          {                                                                                        
/*0x000*/     UINT8        IrpMinor;                                                               
/*0x001*/     UINT8        _PADDING0_[0x3];                                                        
/*0x004*/     enum _SYSTEM_POWER_STATE SystemState;                                                
/*0x008*/     struct _KEVENT Event;                           // 1 elements, 0x10 bytes (sizeof)   
/*0x018*/     ULONG32      SpinLock;                                                               
/*0x01C*/     struct _KTHREAD* Thread;                                                             
/*0x020*/     UINT8        GetNewDeviceList;                                                       
/*0x021*/     UINT8        _PADDING1_[0x3];                                                        
/*0x024*/     struct _PO_DEVICE_NOTIFY_ORDER Order;           // 3 elements, 0x248 bytes (sizeof)  
/*0x26C*/     LONG32       Status;                                                                 
/*0x270*/     struct _DEVICE_OBJECT* FailedDevice;                                                 
/*0x274*/     UINT8        Waking;                                                                 
/*0x275*/     UINT8        Cancelled;                                                              
/*0x276*/     UINT8        IgnoreErrors;                                                           
/*0x277*/     UINT8        IgnoreNotImplemented;                                                   
/*0x278*/     UINT8        WaitAny;                                                                
/*0x279*/     UINT8        WaitAll;                                                                
/*0x27A*/     UINT8        _PADDING2_[0x2];                                                        
/*0x27C*/     struct _LIST_ENTRY PresentIrpQueue;             // 2 elements, 0x8 bytes (sizeof)    
/*0x284*/     struct _POP_DEVICE_POWER_IRP Head;              // 7 elements, 0x2C bytes (sizeof)   
/*0x2B0*/     struct _POP_DEVICE_POWER_IRP PowerIrpState[20];                                      
          }POP_DEVICE_SYS_STATE, *PPOP_DEVICE_SYS_STATE;                                           
                                                                                                   
          typedef struct _POP_HIBER_CONTEXT             // 38 elements, 0xE0 bytes (sizeof) 
          {                                                                                 
/*0x000*/     UINT8        WriteToFile;                                                     
/*0x001*/     UINT8        ReserveLoaderMemory;                                             
/*0x002*/     UINT8        ReserveFreeMemory;                                               
/*0x003*/     UINT8        VerifyOnWake;                                                    
/*0x004*/     UINT8        Reset;                                                           
/*0x005*/     UINT8        HiberFlags;                                                      
/*0x006*/     UINT8        LinkFile;                                                        
/*0x007*/     UINT8        _PADDING0_[0x1];                                                 
/*0x008*/     VOID*        LinkFileHandle;                                                  
/*0x00C*/     ULONG32      Lock;                                                            
/*0x010*/     UINT8        MapFrozen;                                                       
/*0x011*/     UINT8        _PADDING1_[0x3];                                                 
/*0x014*/     struct _RTL_BITMAP MemoryMap;             // 2 elements, 0x8 bytes (sizeof)   
/*0x01C*/     struct _LIST_ENTRY ClonedRanges;          // 2 elements, 0x8 bytes (sizeof)   
/*0x024*/     ULONG32      ClonedRangeCount;                                                
/*0x028*/     struct _LIST_ENTRY* NextCloneRange;                                           
/*0x02C*/     ULONG32      NextPreserve;                                                    
/*0x030*/     struct _MDL* LoaderMdl;                                                       
/*0x034*/     struct _MDL* Clones;                                                          
/*0x038*/     UINT8*       NextClone;                                                       
/*0x03C*/     ULONG32      NoClones;                                                        
/*0x040*/     struct _MDL* Spares;                                                          
/*0x044*/     UINT8        _PADDING2_[0x4];                                                 
/*0x048*/     UINT64       PagesOut;                                                        
/*0x050*/     VOID*        IoPage;                                                          
/*0x054*/     VOID*        CurrentMcb;                                                      
/*0x058*/     struct _DUMP_STACK_CONTEXT* DumpStack;                                        
/*0x05C*/     struct _KPROCESSOR_STATE* WakeState;                                          
/*0x060*/     ULONG32      NoRanges;                                                        
/*0x064*/     ULONG32      HiberVa;                                                         
/*0x068*/     union _LARGE_INTEGER HiberPte;            // 4 elements, 0x8 bytes (sizeof)   
/*0x070*/     LONG32       Status;                                                          
/*0x074*/     struct _PO_MEMORY_IMAGE* MemoryImage;                                         
/*0x078*/     struct _PO_MEMORY_RANGE_ARRAY* TableHead;                                     
/*0x07C*/     UINT8*       CompressionWorkspace;                                            
/*0x080*/     UINT8*       CompressedWriteBuffer;                                           
/*0x084*/     ULONG32*     PerformanceStats;                                                
/*0x088*/     VOID*        CompressionBlock;                                                
/*0x08C*/     VOID*        DmaIO;                                                           
/*0x090*/     VOID*        TemporaryHeap;                                                   
/*0x094*/     UINT8        _PADDING3_[0x4];                                                 
/*0x098*/     struct _PO_HIBER_PERF PerfInfo;           // 13 elements, 0x48 bytes (sizeof) 
          }POP_HIBER_CONTEXT, *PPOP_HIBER_CONTEXT;                                          
                                                                                            
          typedef struct _POP_IDLE_HANDLER                // 12 elements, 0x20 bytes (sizeof) 
          {                                                                                   
/*0x000*/     ULONG32      Latency;                                                           
/*0x004*/     ULONG32      TimeCheck;                                                         
/*0x008*/     ULONG32      DemoteLimit;                                                       
/*0x00C*/     ULONG32      PromoteLimit;                                                      
/*0x010*/     ULONG32      PromoteCount;                                                      
/*0x014*/     UINT8        Demote;                                                            
/*0x015*/     UINT8        Promote;                                                           
/*0x016*/     UINT8        PromotePercent;                                                    
/*0x017*/     UINT8        DemotePercent;                                                     
/*0x018*/     UINT8        State;                                                             
/*0x019*/     UINT8        Spare[3];                                                          
/*0x01C*/     FUNCT_003D_1759_IdleFunction* IdleFunction;                                     
          }POP_IDLE_HANDLER, *PPOP_IDLE_HANDLER;                                              
                                                                                              
          typedef struct _POP_POWER_ACTION                     // 16 elements, 0x40 bytes (sizeof) 
          {                                                                                        
/*0x000*/     UINT8        Updates;                                                                
/*0x001*/     UINT8        State;                                                                  
/*0x002*/     UINT8        Shutdown;                                                               
/*0x003*/     UINT8        _PADDING0_[0x1];                                                        
/*0x004*/     enum _POWER_ACTION Action;                                                           
/*0x008*/     enum _SYSTEM_POWER_STATE LightestState;                                              
/*0x00C*/     ULONG32      Flags;                                                                  
/*0x010*/     LONG32       Status;                                                                 
/*0x014*/     UINT8        IrpMinor;                                                               
/*0x015*/     UINT8        _PADDING1_[0x3];                                                        
/*0x018*/     enum _SYSTEM_POWER_STATE SystemState;                                                
/*0x01C*/     enum _SYSTEM_POWER_STATE NextSystemState;                                            
/*0x020*/     struct _POP_SHUTDOWN_BUG_CHECK* ShutdownBugCode;                                     
/*0x024*/     struct _POP_DEVICE_SYS_STATE* DevState;                                              
/*0x028*/     struct _POP_HIBER_CONTEXT* HiberContext;                                             
/*0x02C*/     enum _SYSTEM_POWER_STATE LastWakeState;                                              
/*0x030*/     UINT64       WakeTime;                                                               
/*0x038*/     UINT64       SleepTime;                                                              
          }POP_POWER_ACTION, *PPOP_POWER_ACTION;                                                   
                                                                                                   
          typedef struct _POP_SHUTDOWN_BUG_CHECK // 5 elements, 0x14 bytes (sizeof) 
          {                                                                         
/*0x000*/     ULONG32      Code;                                                    
/*0x004*/     ULONG32      Parameter1;                                              
/*0x008*/     ULONG32      Parameter2;                                              
/*0x00C*/     ULONG32      Parameter3;                                              
/*0x010*/     ULONG32      Parameter4;                                              
          }POP_SHUTDOWN_BUG_CHECK, *PPOP_SHUTDOWN_BUG_CHECK;                                   
                                                                                    
          typedef struct _THERMAL_INFORMATION    // 10 elements, 0x4C bytes (sizeof) 
          {                                                                          
/*0x000*/     ULONG32      ThermalStamp;                                             
/*0x004*/     ULONG32      ThermalConstant1;                                         
/*0x008*/     ULONG32      ThermalConstant2;                                         
/*0x00C*/     ULONG32      Processors;                                               
/*0x010*/     ULONG32      SamplingPeriod;                                           
/*0x014*/     ULONG32      CurrentTemperature;                                       
/*0x018*/     ULONG32      PassiveTripPoint;                                         
/*0x01C*/     ULONG32      CriticalTripPoint;                                        
/*0x020*/     UINT8        ActiveTripPointCount;                                     
/*0x021*/     UINT8        _PADDING0_[0x3];                                          
/*0x024*/     ULONG32      ActiveTripPoint[10];                                      
          }THERMAL_INFORMATION, *PTHERMAL_INFORMATION;                                    
                                                                                     
          typedef struct _POP_THERMAL_ZONE              // 16 elements, 0xD0 bytes (sizeof) 
          {                                                                                 
/*0x000*/     struct _LIST_ENTRY Link;                  // 2 elements, 0x8 bytes (sizeof)   
/*0x008*/     UINT8        State;                                                           
/*0x009*/     UINT8        Flags;                                                           
/*0x00A*/     UINT8        Mode;                                                            
/*0x00B*/     UINT8        PendingMode;                                                     
/*0x00C*/     UINT8        ActivePoint;                                                     
/*0x00D*/     UINT8        PendingActivePoint;                                              
/*0x00E*/     UINT8        _PADDING0_[0x2];                                                 
/*0x010*/     LONG32       Throttle;                                                        
/*0x014*/     UINT8        _PADDING1_[0x4];                                                 
/*0x018*/     UINT64       LastTime;                                                        
/*0x020*/     ULONG32      SampleRate;                                                      
/*0x024*/     ULONG32      LastTemp;                                                        
/*0x028*/     struct _KTIMER PassiveTimer;              // 5 elements, 0x28 bytes (sizeof)  
/*0x050*/     struct _KDPC PassiveDpc;                  // 9 elements, 0x20 bytes (sizeof)  
/*0x070*/     struct _POP_ACTION_TRIGGER OverThrottled; // 5 elements, 0xC bytes (sizeof)   
/*0x07C*/     struct _IRP* Irp;                                                             
/*0x080*/     struct _THERMAL_INFORMATION Info;         // 10 elements, 0x4C bytes (sizeof) 
/*0x0CC*/     UINT8        _PADDING2_[0x4];                                                 
          }POP_THERMAL_ZONE, *PPOP_THERMAL_ZONE;                                            
                                                                                            
          typedef struct _POP_TRIGGER_WAIT         // 4 elements, 0x20 bytes (sizeof) 
          {                                                                           
/*0x000*/     struct _KEVENT Event;                // 1 elements, 0x10 bytes (sizeof) 
/*0x010*/     LONG32       Status;                                                    
/*0x014*/     struct _LIST_ENTRY Link;             // 2 elements, 0x8 bytes (sizeof)  
/*0x01C*/     struct _POP_ACTION_TRIGGER* Trigger;                                    
          }POP_TRIGGER_WAIT, *PPOP_TRIGGER_WAIT;                                      
                                                                                      
          typedef struct _POWER_ACTION_POLICY // 3 elements, 0xC bytes (sizeof) 
          {                                                                     
/*0x000*/     enum _POWER_ACTION Action;                                        
/*0x004*/     ULONG32      Flags;                                               
/*0x008*/     ULONG32      EventCode;                                           
          }POWER_ACTION_POLICY, *PPOWER_ACTION_POLICY;                                  
                                                                                
          typedef struct _POWER_SEQUENCE // 3 elements, 0xC bytes (sizeof) 
          {                                                                
/*0x000*/     ULONG32      SequenceD1;                                     
/*0x004*/     ULONG32      SequenceD2;                                     
/*0x008*/     ULONG32      SequenceD3;                                     
          }POWER_SEQUENCE, *PPOWER_SEQUENCE;                                  
                                                                           
          typedef struct _PRIVATE_CACHE_MAP_FLAGS // 4 elements, 0x4 bytes (sizeof) 
          {                                                                         
/*0x000*/     ULONG32      DontUse : 16;          // 0 BitPosition                  
/*0x000*/     ULONG32      ReadAheadActive : 1;   // 16 BitPosition                 
/*0x000*/     ULONG32      ReadAheadEnabled : 1;  // 17 BitPosition                 
/*0x000*/     ULONG32      Available : 14;        // 18 BitPosition                 
          }PRIVATE_CACHE_MAP_FLAGS, *PPRIVATE_CACHE_MAP_FLAGS;                                  
                                                                                    
          typedef struct _PRIVATE_CACHE_MAP              // 13 elements, 0x58 bytes (sizeof) 
          {                                                                                  
              union                                      // 3 elements, 0x4 bytes (sizeof)   
              {                                                                              
/*0x000*/         INT16        NodeTypeCode;                                                 
/*0x000*/         struct _PRIVATE_CACHE_MAP_FLAGS Flags; // 4 elements, 0x4 bytes (sizeof)   
/*0x000*/         ULONG32      UlongFlags;                                                   
              };                                                                             
/*0x004*/     ULONG32      ReadAheadMask;                                                    
/*0x008*/     struct _FILE_OBJECT* FileObject;                                               
/*0x00C*/     UINT8        _PADDING0_[0x4];                                                  
/*0x010*/     union _LARGE_INTEGER FileOffset1;          // 4 elements, 0x8 bytes (sizeof)   
/*0x018*/     union _LARGE_INTEGER BeyondLastByte1;      // 4 elements, 0x8 bytes (sizeof)   
/*0x020*/     union _LARGE_INTEGER FileOffset2;          // 4 elements, 0x8 bytes (sizeof)   
/*0x028*/     union _LARGE_INTEGER BeyondLastByte2;      // 4 elements, 0x8 bytes (sizeof)   
/*0x030*/     union _LARGE_INTEGER ReadAheadOffset[2];                                       
/*0x040*/     ULONG32      ReadAheadLength[2];                                               
/*0x048*/     ULONG32      ReadAheadSpinLock;                                                
/*0x04C*/     struct _LIST_ENTRY PrivateLinks;           // 2 elements, 0x8 bytes (sizeof)   
/*0x054*/     UINT8        _PADDING1_[0x4];                                                  
          }PRIVATE_CACHE_MAP, *PPRIVATE_CACHE_MAP;                                           
                                                                                             
          typedef struct _PROCESSOR_PERF_STATE // 11 elements, 0x20 bytes (sizeof) 
          {                                                                        
/*0x000*/     UINT8        PercentFrequency;                                       
/*0x001*/     UINT8        MinCapacity;                                            
/*0x002*/     UINT16       Power;                                                  
/*0x004*/     UINT8        IncreaseLevel;                                          
/*0x005*/     UINT8        DecreaseLevel;                                          
/*0x006*/     UINT16       Flags;                                                  
/*0x008*/     ULONG32      IncreaseTime;                                           
/*0x00C*/     ULONG32      DecreaseTime;                                           
/*0x010*/     ULONG32      IncreaseCount;                                          
/*0x014*/     ULONG32      DecreaseCount;                                          
/*0x018*/     UINT64       PerformanceTime;                                        
          }PROCESSOR_PERF_STATE, *PPROCESSOR_PERF_STATE;                                    
                                                                                   
          typedef struct _PROCESSOR_POWER_POLICY_INFO // 9 elements, 0x14 bytes (sizeof) 
          {                                                                              
/*0x000*/     ULONG32      TimeCheck;                                                    
/*0x004*/     ULONG32      DemoteLimit;                                                  
/*0x008*/     ULONG32      PromoteLimit;                                                 
/*0x00C*/     UINT8        DemotePercent;                                                
/*0x00D*/     UINT8        PromotePercent;                                               
/*0x00E*/     UINT8        Spare[2];                                                     
              struct                                  // 3 elements, 0x4 bytes (sizeof)  
              {                                                                          
/*0x010*/         ULONG32      AllowDemotion : 1;     // 0 BitPosition                   
/*0x010*/         ULONG32      AllowPromotion : 1;    // 1 BitPosition                   
/*0x010*/         ULONG32      Reserved : 30;         // 2 BitPosition                   
              };                                                                         
          }PROCESSOR_POWER_POLICY_INFO, *PPROCESSOR_POWER_POLICY_INFO;                                   
                                                                                         
          typedef struct _PROCESSOR_POWER_POLICY             // 7 elements, 0x4C bytes (sizeof) 
          {                                                                                     
/*0x000*/     ULONG32      Revision;                                                            
/*0x004*/     UINT8        DynamicThrottle;                                                     
/*0x005*/     UINT8        Spare[3];                                                            
              struct                                         // 2 elements, 0x4 bytes (sizeof)  
              {                                                                                 
/*0x008*/         ULONG32      DisableCStates : 1;           // 0 BitPosition                   
/*0x008*/         ULONG32      Reserved : 31;                // 1 BitPosition                   
              };                                                                                
/*0x00C*/     ULONG32      PolicyCount;                                                         
/*0x010*/     struct _PROCESSOR_POWER_POLICY_INFO Policy[3];                                    
          }PROCESSOR_POWER_POLICY, *PPROCESSOR_POWER_POLICY;                                    
                                                                                                
          typedef struct _PS_IMPERSONATION_INFORMATION               // 4 elements, 0xC bytes (sizeof) 
          {                                                                                            
/*0x000*/     VOID*        Token;                                                                      
/*0x004*/     UINT8        CopyOnOpen;                                                                 
/*0x005*/     UINT8        EffectiveOnly;                                                              
/*0x006*/     UINT8        _PADDING0_[0x2];                                                            
/*0x008*/     enum _SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;                                   
          }PS_IMPERSONATION_INFORMATION, *PPS_IMPERSONATION_INFORMATION;                                  
                                                                                                       
          typedef struct _PS_JOB_TOKEN_FILTER                  // 9 elements, 0x24 bytes (sizeof) 
          {                                                                                       
/*0x000*/     ULONG32      CapturedSidCount;                                                      
/*0x004*/     struct _SID_AND_ATTRIBUTES* CapturedSids;                                           
/*0x008*/     ULONG32      CapturedSidsLength;                                                    
/*0x00C*/     ULONG32      CapturedGroupCount;                                                    
/*0x010*/     struct _SID_AND_ATTRIBUTES* CapturedGroups;                                         
/*0x014*/     ULONG32      CapturedGroupsLength;                                                  
/*0x018*/     ULONG32      CapturedPrivilegeCount;                                                
/*0x01C*/     struct _LUID_AND_ATTRIBUTES* CapturedPrivileges;                                    
/*0x020*/     ULONG32      CapturedPrivilegesLength;                                              
          }PS_JOB_TOKEN_FILTER, *PPS_JOB_TOKEN_FILTER;                                            
                                                                                                  
          typedef struct _RTL_HANDLE_TABLE                        // 7 elements, 0x20 bytes (sizeof) 
          {                                                                                          
/*0x000*/     ULONG32      MaximumNumberOfHandles;                                                   
/*0x004*/     ULONG32      SizeOfHandleTableEntry;                                                   
/*0x008*/     ULONG32      Reserved[2];                                                              
/*0x010*/     struct _RTL_HANDLE_TABLE_ENTRY* FreeHandles;                                           
/*0x014*/     struct _RTL_HANDLE_TABLE_ENTRY* CommittedHandles;                                      
/*0x018*/     struct _RTL_HANDLE_TABLE_ENTRY* UnCommittedHandles;                                    
/*0x01C*/     struct _RTL_HANDLE_TABLE_ENTRY* MaxReservedHandles;                                    
          }RTL_HANDLE_TABLE, *PRTL_HANDLE_TABLE;                                                     
                                                                                                     
          typedef struct _RTL_ATOM_TABLE                    // 5 elements, 0x44 bytes (sizeof) 
          {                                                                                    
/*0x000*/     ULONG32      Signature;                                                          
/*0x004*/     struct _RTL_CRITICAL_SECTION CriticalSection; // 6 elements, 0x18 bytes (sizeof) 
/*0x01C*/     struct _RTL_HANDLE_TABLE RtlHandleTable;      // 7 elements, 0x20 bytes (sizeof) 
/*0x03C*/     ULONG32      NumberOfBuckets;                                                    
/*0x040*/     struct _RTL_ATOM_TABLE_ENTRY* Buckets[1];                                        
          }RTL_ATOM_TABLE, *PRTL_ATOM_TABLE;                                                   
                                                                                               
          typedef struct _RTL_ATOM_TABLE_ENTRY        // 7 elements, 0x10 bytes (sizeof) 
          {                                                                              
/*0x000*/     struct _RTL_ATOM_TABLE_ENTRY* HashLink;                                    
/*0x004*/     UINT16       HandleIndex;                                                  
/*0x006*/     UINT16       Atom;                                                         
/*0x008*/     UINT16       ReferenceCount;                                               
/*0x00A*/     UINT8        Flags;                                                        
/*0x00B*/     UINT8        NameLength;                                                   
/*0x00C*/     UINT16       Name[1];                                                      
/*0x00E*/     UINT8        _PADDING0_[0x2];                                              
          }RTL_ATOM_TABLE_ENTRY, *PRTL_ATOM_TABLE_ENTRY;                                   
                                                                                         
          typedef struct _RTL_CRITICAL_SECTION_DEBUG         // 7 elements, 0x20 bytes (sizeof) 
          {                                                                                     
/*0x000*/     UINT16       Type;                                                                
/*0x002*/     UINT16       CreatorBackTraceIndex;                                               
/*0x004*/     struct _RTL_CRITICAL_SECTION* CriticalSection;                                    
/*0x008*/     struct _LIST_ENTRY ProcessLocksList;           // 2 elements, 0x8 bytes (sizeof)  
/*0x010*/     ULONG32      EntryCount;                                                          
/*0x014*/     ULONG32      ContentionCount;                                                     
/*0x018*/     ULONG32      Spare[2];                                                            
          }RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG;                                   
                                                                                                
          typedef struct _RTL_DRIVE_LETTER_CURDIR // 4 elements, 0x10 bytes (sizeof) 
          {                                                                          
/*0x000*/     UINT16       Flags;                                                    
/*0x002*/     UINT16       Length;                                                   
/*0x004*/     ULONG32      TimeStamp;                                                
/*0x008*/     struct _STRING DosPath;             // 3 elements, 0x8 bytes (sizeof)  
          }RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;                                   
                                                                                     
          typedef struct _RTL_HANDLE_TABLE_ENTRY            // 2 elements, 0x4 bytes (sizeof) 
          {                                                                                   
              union                                         // 2 elements, 0x4 bytes (sizeof) 
              {                                                                               
/*0x000*/         ULONG32      Flags;                                                         
/*0x000*/         struct _RTL_HANDLE_TABLE_ENTRY* NextFree;                                   
              };                                                                              
          }RTL_HANDLE_TABLE_ENTRY, *PRTL_HANDLE_TABLE_ENTRY;                                  
                                                                                              
          typedef struct _RTL_RANGE         // 6 elements, 0x20 bytes (sizeof) 
          {                                                                    
/*0x000*/     UINT64       Start;                                              
/*0x008*/     UINT64       End;                                                
/*0x010*/     VOID*        UserData;                                           
/*0x014*/     VOID*        Owner;                                              
/*0x018*/     UINT8        Attributes;                                         
/*0x019*/     UINT8        Flags;                                              
/*0x01A*/     UINT8        _PADDING0_[0x6];                                    
          }RTL_RANGE, *PRTL_RANGE;                                             
                                                                               
          typedef struct _RTL_RANGE_LIST   // 4 elements, 0x14 bytes (sizeof) 
          {                                                                   
/*0x000*/     struct _LIST_ENTRY ListHead; // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/     ULONG32      Flags;                                             
/*0x00C*/     ULONG32      Count;                                             
/*0x010*/     ULONG32      Stamp;                                             
          }RTL_RANGE_LIST, *PRTL_RANGE_LIST;                                   
                                                                              
          typedef struct _RTL_USER_PROCESS_PARAMETERS                // 28 elements, 0x290 bytes (sizeof) 
          {                                                                                               
/*0x000*/     ULONG32      MaximumLength;                                                                 
/*0x004*/     ULONG32      Length;                                                                        
/*0x008*/     ULONG32      Flags;                                                                         
/*0x00C*/     ULONG32      DebugFlags;                                                                    
/*0x010*/     VOID*        ConsoleHandle;                                                                 
/*0x014*/     ULONG32      ConsoleFlags;                                                                  
/*0x018*/     VOID*        StandardInput;                                                                 
/*0x01C*/     VOID*        StandardOutput;                                                                
/*0x020*/     VOID*        StandardError;                                                                 
/*0x024*/     struct _CURDIR CurrentDirectory;                       // 2 elements, 0xC bytes (sizeof)    
/*0x030*/     struct _UNICODE_STRING DllPath;                        // 3 elements, 0x8 bytes (sizeof)    
/*0x038*/     struct _UNICODE_STRING ImagePathName;                  // 3 elements, 0x8 bytes (sizeof)    
/*0x040*/     struct _UNICODE_STRING CommandLine;                    // 3 elements, 0x8 bytes (sizeof)    
/*0x048*/     VOID*        Environment;                                                                   
/*0x04C*/     ULONG32      StartingX;                                                                     
/*0x050*/     ULONG32      StartingY;                                                                     
/*0x054*/     ULONG32      CountX;                                                                        
/*0x058*/     ULONG32      CountY;                                                                        
/*0x05C*/     ULONG32      CountCharsX;                                                                   
/*0x060*/     ULONG32      CountCharsY;                                                                   
/*0x064*/     ULONG32      FillAttribute;                                                                 
/*0x068*/     ULONG32      WindowFlags;                                                                   
/*0x06C*/     ULONG32      ShowWindowFlags;                                                               
/*0x070*/     struct _UNICODE_STRING WindowTitle;                    // 3 elements, 0x8 bytes (sizeof)    
/*0x078*/     struct _UNICODE_STRING DesktopInfo;                    // 3 elements, 0x8 bytes (sizeof)    
/*0x080*/     struct _UNICODE_STRING ShellInfo;                      // 3 elements, 0x8 bytes (sizeof)    
/*0x088*/     struct _UNICODE_STRING RuntimeData;                    // 3 elements, 0x8 bytes (sizeof)    
/*0x090*/     struct _RTL_DRIVE_LETTER_CURDIR CurrentDirectores[32];                                      
          }RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;                                     
                                                                                                          
          typedef struct _RTLP_RANGE_LIST_ENTRY    // 8 elements, 0x28 bytes (sizeof) 
          {                                                                           
/*0x000*/     UINT64       Start;                                                     
/*0x008*/     UINT64       End;                                                       
              union                                // 2 elements, 0x8 bytes (sizeof)  
              {                                                                       
                  struct                           // 2 elements, 0x8 bytes (sizeof)  
                  {                                                                   
/*0x010*/             VOID*        UserData;                                          
/*0x014*/             VOID*        Owner;                                             
                  }Allocated;                                                         
                  struct                           // 1 elements, 0x8 bytes (sizeof)  
                  {                                                                   
/*0x010*/             struct _LIST_ENTRY ListHead; // 2 elements, 0x8 bytes (sizeof)  
                  }Merged;                                                            
              };                                                                      
/*0x018*/     UINT8        Attributes;                                                
/*0x019*/     UINT8        PublicFlags;                                               
/*0x01A*/     UINT16       PrivateFlags;                                              
/*0x01C*/     struct _LIST_ENTRY ListEntry;        // 2 elements, 0x8 bytes (sizeof)  
/*0x024*/     UINT8        _PADDING0_[0x4];                                           
          }RTLP_RANGE_LIST_ENTRY, *PRTLP_RANGE_LIST_ENTRY;                                   
                                                                                      
typedef struct _SCSI_REQUEST_BLOCK // 0 elements, 0x0 bytes (sizeof) 
{                                                                    
}SCSI_REQUEST_BLOCK, *PSCSI_REQUEST_BLOCK;                                  
                                                                     
          typedef struct _SECTION_IMAGE_INFORMATION       // 17 elements, 0x30 bytes (sizeof) 
          {                                                                                   
/*0x000*/     VOID*        TransferAddress;                                                   
/*0x004*/     ULONG32      ZeroBits;                                                          
/*0x008*/     ULONG32      MaximumStackSize;                                                  
/*0x00C*/     ULONG32      CommittedStackSize;                                                
/*0x010*/     ULONG32      SubSystemType;                                                     
              union                                       // 2 elements, 0x4 bytes (sizeof)   
              {                                                                               
                  struct                                  // 2 elements, 0x4 bytes (sizeof)   
                  {                                                                           
/*0x014*/             UINT16       SubSystemMinorVersion;                                     
/*0x016*/             UINT16       SubSystemMajorVersion;                                     
                  };                                                                          
/*0x014*/         ULONG32      SubSystemVersion;                                              
              };                                                                              
/*0x018*/     ULONG32      GpValue;                                                           
/*0x01C*/     UINT16       ImageCharacteristics;                                              
/*0x01E*/     UINT16       DllCharacteristics;                                                
/*0x020*/     UINT16       Machine;                                                           
/*0x022*/     UINT8        ImageContainsCode;                                                 
/*0x023*/     UINT8        Spare1;                                                            
/*0x024*/     ULONG32      LoaderFlags;                                                       
/*0x028*/     ULONG32      ImageFileSize;                                                     
/*0x02C*/     ULONG32      Reserved[1];                                                       
          }SECTION_IMAGE_INFORMATION, *PSECTION_IMAGE_INFORMATION;                                    
                                                                                              
          typedef struct _SECTION_OBJECT       // 6 elements, 0x18 bytes (sizeof) 
          {                                                                       
/*0x000*/     VOID*        StartingVa;                                            
/*0x004*/     VOID*        EndingVa;                                              
/*0x008*/     VOID*        Parent;                                                
/*0x00C*/     VOID*        LeftChild;                                             
/*0x010*/     VOID*        RightChild;                                            
/*0x014*/     struct _SEGMENT_OBJECT* Segment;                                    
          }SECTION_OBJECT, *PSECTION_OBJECT;                                      
                                                                                  
          typedef struct _SECTION_OBJECT_POINTERS // 3 elements, 0xC bytes (sizeof) 
          {                                                                         
/*0x000*/     VOID*        DataSectionObject;                                       
/*0x004*/     VOID*        SharedCacheMap;                                          
/*0x008*/     VOID*        ImageSectionObject;                                      
          }SECTION_OBJECT_POINTERS, *PSECTION_OBJECT_POINTERS;                                  
                                                                                    
          typedef struct _SECURITY_DESCRIPTOR // 7 elements, 0x14 bytes (sizeof) 
          {                                                                      
/*0x000*/     UINT8        Revision;                                             
/*0x001*/     UINT8        Sbz1;                                                 
/*0x002*/     UINT16       Control;                                              
/*0x004*/     VOID*        Owner;                                                
/*0x008*/     VOID*        Group;                                                
/*0x00C*/     struct _ACL* Sacl;                                                 
/*0x010*/     struct _ACL* Dacl;                                                 
          }SECURITY_DESCRIPTOR, *PSECURITY_DESCRIPTOR;                                   
                                                                                 
          typedef struct _SECURITY_TOKEN_AUDIT_DATA // 3 elements, 0xC bytes (sizeof) 
          {                                                                           
/*0x000*/     ULONG32      Length;                                                    
/*0x004*/     ULONG32      GrantMask;                                                 
/*0x008*/     ULONG32      DenyMask;                                                  
          }SECURITY_TOKEN_AUDIT_DATA, *PSECURITY_TOKEN_AUDIT_DATA;                                  
                                                                                      
          typedef struct _SECURITY_TOKEN_PROXY_DATA // 5 elements, 0x18 bytes (sizeof) 
          {                                                                            
/*0x000*/     ULONG32      Length;                                                     
/*0x004*/     enum _PROXY_CLASS ProxyClass;                                            
/*0x008*/     struct _UNICODE_STRING PathInfo;      // 3 elements, 0x8 bytes (sizeof)  
/*0x010*/     ULONG32      ContainerMask;                                              
/*0x014*/     ULONG32      ObjectMask;                                                 
          }SECURITY_TOKEN_PROXY_DATA, *PSECURITY_TOKEN_PROXY_DATA;                                   
                                                                                       
          typedef struct _SEGMENT                                      // 14 elements, 0x48 bytes (sizeof) 
          {                                                                                                
/*0x000*/     struct _CONTROL_AREA* ControlArea;                                                           
/*0x004*/     ULONG32      TotalNumberOfPtes;                                                              
/*0x008*/     ULONG32      NonExtendedPtes;                                                                
/*0x00C*/     ULONG32      WritableUserReferences;                                                         
/*0x010*/     UINT64       SizeOfSegment;                                                                  
/*0x018*/     struct _MMPTE SegmentPteTemplate;                        // 1 elements, 0x8 bytes (sizeof)   
/*0x020*/     ULONG32      NumberOfCommittedPages;                                                         
/*0x024*/     struct _MMEXTEND_INFO* ExtendInfo;                                                           
/*0x028*/     VOID*        SystemImageBase;                                                                
/*0x02C*/     VOID*        BasedAddress;                                                                   
              union                                                    // 2 elements, 0x4 bytes (sizeof)   
              {                                                                                            
/*0x030*/         ULONG32      ImageCommitment;                                                            
/*0x030*/         struct _EPROCESS* CreatingProcess;                                                       
              }u1;                                                                                         
              union                                                    // 2 elements, 0x4 bytes (sizeof)   
              {                                                                                            
/*0x034*/         struct _SECTION_IMAGE_INFORMATION* ImageInformation;                                     
/*0x034*/         VOID*        FirstMappedVa;                                                              
              }u2;                                                                                         
/*0x038*/     struct _MMPTE* PrototypePte;                                                                 
/*0x03C*/     UINT8        _PADDING0_[0x4];                                                                
/*0x040*/     struct _MMPTE ThePtes[1];                                                                    
          }SEGMENT, *PSEGMENT;                                                                             
                                                                                                           
          typedef struct _SEGMENT_OBJECT                     // 10 elements, 0x30 bytes (sizeof) 
          {                                                                                      
/*0x000*/     VOID*        BaseAddress;                                                          
/*0x004*/     ULONG32      TotalNumberOfPtes;                                                    
/*0x008*/     union _LARGE_INTEGER SizeOfSegment;            // 4 elements, 0x8 bytes (sizeof)   
/*0x010*/     ULONG32      NonExtendedPtes;                                                      
/*0x014*/     ULONG32      ImageCommitment;                                                      
/*0x018*/     struct _CONTROL_AREA* ControlArea;                                                 
/*0x01C*/     struct _SUBSECTION* Subsection;                                                    
/*0x020*/     struct _LARGE_CONTROL_AREA* LargeControlArea;                                      
/*0x024*/     struct _MMSECTION_FLAGS* MmSectionFlags;                                           
/*0x028*/     struct _MMSUBSECTION_FLAGS* MmSubSectionFlags;                                     
/*0x02C*/     UINT8        _PADDING0_[0x4];                                                      
          }SEGMENT_OBJECT, *PSEGMENT_OBJECT;                                                     
                                                                                                 
          typedef struct _SEP_AUDIT_POLICY_CATEGORIES      // 9 elements, 0x8 bytes (sizeof) 
          {                                                                                  
              struct                                       // 8 elements, 0x4 bytes (sizeof) 
              {                                                                              
/*0x000*/         ULONG32      System : 4;                 // 0 BitPosition                  
/*0x000*/         ULONG32      Logon : 4;                  // 4 BitPosition                  
/*0x000*/         ULONG32      ObjectAccess : 4;           // 8 BitPosition                  
/*0x000*/         ULONG32      PrivilegeUse : 4;           // 12 BitPosition                 
/*0x000*/         ULONG32      DetailedTracking : 4;       // 16 BitPosition                 
/*0x000*/         ULONG32      PolicyChange : 4;           // 20 BitPosition                 
/*0x000*/         ULONG32      AccountManagement : 4;      // 24 BitPosition                 
/*0x000*/         ULONG32      DirectoryServiceAccess : 4; // 28 BitPosition                 
              };                                                                             
/*0x004*/     ULONG32      AccountLogon : 4;               // 0 BitPosition                  
          }SEP_AUDIT_POLICY_CATEGORIES, *PSEP_AUDIT_POLICY_CATEGORIES;                                  
                                                                                             
          typedef struct _SEP_AUDIT_POLICY_OVERLAY // 2 elements, 0x8 bytes (sizeof) 
          {                                                                          
/*0x000*/     UINT64       PolicyBits : 36;        // 0 BitPosition                  
/*0x000*/     UINT64       SetBit : 1;             // 36 BitPosition                 
          }SEP_AUDIT_POLICY_OVERLAY, *PSEP_AUDIT_POLICY_OVERLAY;                                  
                                                                                     
          typedef struct _SEP_AUDIT_POLICY                            // 3 elements, 0x8 bytes (sizeof) 
          {                                                                                             
              union                                                   // 3 elements, 0x8 bytes (sizeof) 
              {                                                                                         
/*0x000*/         struct _SEP_AUDIT_POLICY_CATEGORIES PolicyElements; // 9 elements, 0x8 bytes (sizeof) 
/*0x000*/         struct _SEP_AUDIT_POLICY_OVERLAY PolicyOverlay;     // 2 elements, 0x8 bytes (sizeof) 
/*0x000*/         UINT64       Overlay;                                                                 
              };                                                                                        
          }SEP_AUDIT_POLICY, *PSEP_AUDIT_POLICY;                                                        
                                                                                                        
          typedef struct _SHARED_CACHE_MAP                          // 41 elements, 0x130 bytes (sizeof) 
          {                                                                                              
/*0x000*/     INT16        NodeTypeCode;                                                                 
/*0x002*/     INT16        NodeByteSize;                                                                 
/*0x004*/     ULONG32      OpenCount;                                                                    
/*0x008*/     union _LARGE_INTEGER FileSize;                        // 4 elements, 0x8 bytes (sizeof)    
/*0x010*/     struct _LIST_ENTRY BcbList;                           // 2 elements, 0x8 bytes (sizeof)    
/*0x018*/     union _LARGE_INTEGER SectionSize;                     // 4 elements, 0x8 bytes (sizeof)    
/*0x020*/     union _LARGE_INTEGER ValidDataLength;                 // 4 elements, 0x8 bytes (sizeof)    
/*0x028*/     union _LARGE_INTEGER ValidDataGoal;                   // 4 elements, 0x8 bytes (sizeof)    
/*0x030*/     struct _VACB* InitialVacbs[4];                                                             
/*0x040*/     struct _VACB** Vacbs;                                                                      
/*0x044*/     struct _FILE_OBJECT* FileObject;                                                           
/*0x048*/     struct _VACB* ActiveVacb;                                                                  
/*0x04C*/     VOID*        NeedToZero;                                                                   
/*0x050*/     ULONG32      ActivePage;                                                                   
/*0x054*/     ULONG32      NeedToZeroPage;                                                               
/*0x058*/     ULONG32      ActiveVacbSpinLock;                                                           
/*0x05C*/     ULONG32      VacbActiveCount;                                                              
/*0x060*/     ULONG32      DirtyPages;                                                                   
/*0x064*/     struct _LIST_ENTRY SharedCacheMapLinks;               // 2 elements, 0x8 bytes (sizeof)    
/*0x06C*/     ULONG32      Flags;                                                                        
/*0x070*/     LONG32       Status;                                                                       
/*0x074*/     struct _MBCB* Mbcb;                                                                        
/*0x078*/     VOID*        Section;                                                                      
/*0x07C*/     struct _KEVENT* CreateEvent;                                                               
/*0x080*/     struct _KEVENT* WaitOnActiveCount;                                                         
/*0x084*/     ULONG32      PagesToWrite;                                                                 
/*0x088*/     INT64        BeyondLastFlush;                                                              
/*0x090*/     struct _CACHE_MANAGER_CALLBACKS* Callbacks;                                                
/*0x094*/     VOID*        LazyWriteContext;                                                             
/*0x098*/     struct _LIST_ENTRY PrivateList;                       // 2 elements, 0x8 bytes (sizeof)    
/*0x0A0*/     VOID*        LogHandle;                                                                    
/*0x0A4*/     FUNCT_00BC_0BDF_FlushToLsnRoutine* FlushToLsnRoutine;                                      
/*0x0A8*/     ULONG32      DirtyPageThreshold;                                                           
/*0x0AC*/     ULONG32      LazyWritePassCount;                                                           
/*0x0B0*/     struct _CACHE_UNINITIALIZE_EVENT* UninitializeEvent;                                       
/*0x0B4*/     struct _VACB* NeedToZeroVacb;                                                              
/*0x0B8*/     ULONG32      BcbSpinLock;                                                                  
/*0x0BC*/     VOID*        Reserved;                                                                     
/*0x0C0*/     struct _KEVENT Event;                                 // 1 elements, 0x10 bytes (sizeof)   
/*0x0D0*/     struct _EX_PUSH_LOCK VacbPushLock;                    // 5 elements, 0x4 bytes (sizeof)    
/*0x0D4*/     UINT8        _PADDING0_[0x4];                                                              
/*0x0D8*/     struct _PRIVATE_CACHE_MAP PrivateCacheMap;            // 13 elements, 0x58 bytes (sizeof)  
          }SHARED_CACHE_MAP, *PSHARED_CACHE_MAP;                                                         
                                                                                                         
          typedef struct _SID_AND_ATTRIBUTES // 2 elements, 0x8 bytes (sizeof) 
          {                                                                    
/*0x000*/     VOID*        Sid;                                                
/*0x004*/     ULONG32      Attributes;                                         
          }SID_AND_ATTRIBUTES, *PSID_AND_ATTRIBUTES;                                  
                                                                               
          typedef struct _SUBSECTION                          // 8 elements, 0x20 bytes (sizeof) 
          {                                                                                      
/*0x000*/     struct _CONTROL_AREA* ControlArea;                                                 
              union                                           // 2 elements, 0x4 bytes (sizeof)  
              {                                                                                  
/*0x004*/         ULONG32      LongFlags;                                                        
/*0x004*/         struct _MMSUBSECTION_FLAGS SubsectionFlags; // 8 elements, 0x4 bytes (sizeof)  
              }u;                                                                                
/*0x008*/     ULONG32      StartingSector;                                                       
/*0x00C*/     ULONG32      NumberOfFullSectors;                                                  
/*0x010*/     struct _MMPTE* SubsectionBase;                                                     
/*0x014*/     ULONG32      UnusedPtes;                                                           
/*0x018*/     ULONG32      PtesInSubsection;                                                     
/*0x01C*/     struct _SUBSECTION* NextSubsection;                                                
          }SUBSECTION, *PSUBSECTION;                                                             
                                                                                                 
          typedef struct _SUPPORTED_RANGE      // 5 elements, 0x20 bytes (sizeof) 
          {                                                                       
/*0x000*/     struct _SUPPORTED_RANGE* Next;                                      
/*0x004*/     ULONG32      SystemAddressSpace;                                    
/*0x008*/     INT64        SystemBase;                                            
/*0x010*/     INT64        Base;                                                  
/*0x018*/     INT64        Limit;                                                 
          }SUPPORTED_RANGE, *PSUPPORTED_RANGE;                                    
                                                                                  
          typedef struct _SUPPORTED_RANGES            // 11 elements, 0xA0 bytes (sizeof) 
          {                                                                               
/*0x000*/     UINT16       Version;                                                       
/*0x002*/     UINT8        Sorted;                                                        
/*0x003*/     UINT8        Reserved;                                                      
/*0x004*/     ULONG32      NoIO;                                                          
/*0x008*/     struct _SUPPORTED_RANGE IO;             // 5 elements, 0x20 bytes (sizeof)  
/*0x028*/     ULONG32      NoMemory;                                                      
/*0x02C*/     UINT8        _PADDING0_[0x4];                                               
/*0x030*/     struct _SUPPORTED_RANGE Memory;         // 5 elements, 0x20 bytes (sizeof)  
/*0x050*/     ULONG32      NoPrefetchMemory;                                              
/*0x054*/     UINT8        _PADDING1_[0x4];                                               
/*0x058*/     struct _SUPPORTED_RANGE PrefetchMemory; // 5 elements, 0x20 bytes (sizeof)  
/*0x078*/     ULONG32      NoDma;                                                         
/*0x07C*/     UINT8        _PADDING2_[0x4];                                               
/*0x080*/     struct _SUPPORTED_RANGE Dma;            // 5 elements, 0x20 bytes (sizeof)  
          }SUPPORTED_RANGES, *PSUPPORTED_RANGES;                                          
                                                                                          
          typedef struct _SYSPTES_HEADER   // 2 elements, 0xC bytes (sizeof) 
          {                                                                  
/*0x000*/     struct _LIST_ENTRY ListHead; // 2 elements, 0x8 bytes (sizeof) 
/*0x008*/     ULONG32      Count;                                            
          }SYSPTES_HEADER, *PSYSPTES_HEADER;                                  
                                                                             
          typedef struct _SYSTEM_POWER_CAPABILITIES            // 28 elements, 0x4C bytes (sizeof) 
          {                                                                                        
/*0x000*/     UINT8        PowerButtonPresent;                                                     
/*0x001*/     UINT8        SleepButtonPresent;                                                     
/*0x002*/     UINT8        LidPresent;                                                             
/*0x003*/     UINT8        SystemS1;                                                               
/*0x004*/     UINT8        SystemS2;                                                               
/*0x005*/     UINT8        SystemS3;                                                               
/*0x006*/     UINT8        SystemS4;                                                               
/*0x007*/     UINT8        SystemS5;                                                               
/*0x008*/     UINT8        HiberFilePresent;                                                       
/*0x009*/     UINT8        FullWake;                                                               
/*0x00A*/     UINT8        VideoDimPresent;                                                        
/*0x00B*/     UINT8        ApmPresent;                                                             
/*0x00C*/     UINT8        UpsPresent;                                                             
/*0x00D*/     UINT8        ThermalControl;                                                         
/*0x00E*/     UINT8        ProcessorThrottle;                                                      
/*0x00F*/     UINT8        ProcessorMinThrottle;                                                   
/*0x010*/     UINT8        ProcessorMaxThrottle;                                                   
/*0x011*/     UINT8        spare2[4];                                                              
/*0x015*/     UINT8        DiskSpinDown;                                                           
/*0x016*/     UINT8        spare3[8];                                                              
/*0x01E*/     UINT8        SystemBatteriesPresent;                                                 
/*0x01F*/     UINT8        BatteriesAreShortTerm;                                                  
/*0x020*/     struct _BATTERY_REPORTING_SCALE BatteryScale[3];                                     
/*0x038*/     enum _SYSTEM_POWER_STATE AcOnLineWake;                                               
/*0x03C*/     enum _SYSTEM_POWER_STATE SoftLidWake;                                                
/*0x040*/     enum _SYSTEM_POWER_STATE RtcWake;                                                    
/*0x044*/     enum _SYSTEM_POWER_STATE MinDeviceWakeState;                                         
/*0x048*/     enum _SYSTEM_POWER_STATE DefaultLowLatencyWake;                                      
          }SYSTEM_POWER_CAPABILITIES, *PSYSTEM_POWER_CAPABILITIES;                                    
                                                                                                   
          typedef struct _SYSTEM_POWER_LEVEL           // 5 elements, 0x18 bytes (sizeof) 
          {                                                                               
/*0x000*/     UINT8        Enable;                                                        
/*0x001*/     UINT8        Spare[3];                                                      
/*0x004*/     ULONG32      BatteryLevel;                                                  
/*0x008*/     struct _POWER_ACTION_POLICY PowerPolicy; // 3 elements, 0xC bytes (sizeof)  
/*0x014*/     enum _SYSTEM_POWER_STATE MinSystemState;                                    
          }SYSTEM_POWER_LEVEL, *PSYSTEM_POWER_LEVEL;                                      
                                                                                          
          typedef struct _SYSTEM_POWER_POLICY                // 28 elements, 0xE8 bytes (sizeof) 
          {                                                                                      
/*0x000*/     ULONG32      Revision;                                                             
/*0x004*/     struct _POWER_ACTION_POLICY PowerButton;       // 3 elements, 0xC bytes (sizeof)   
/*0x010*/     struct _POWER_ACTION_POLICY SleepButton;       // 3 elements, 0xC bytes (sizeof)   
/*0x01C*/     struct _POWER_ACTION_POLICY LidClose;          // 3 elements, 0xC bytes (sizeof)   
/*0x028*/     enum _SYSTEM_POWER_STATE LidOpenWake;                                              
/*0x02C*/     ULONG32      Reserved;                                                             
/*0x030*/     struct _POWER_ACTION_POLICY Idle;              // 3 elements, 0xC bytes (sizeof)   
/*0x03C*/     ULONG32      IdleTimeout;                                                          
/*0x040*/     UINT8        IdleSensitivity;                                                      
/*0x041*/     UINT8        DynamicThrottle;                                                      
/*0x042*/     UINT8        Spare2[2];                                                            
/*0x044*/     enum _SYSTEM_POWER_STATE MinSleep;                                                 
/*0x048*/     enum _SYSTEM_POWER_STATE MaxSleep;                                                 
/*0x04C*/     enum _SYSTEM_POWER_STATE ReducedLatencySleep;                                      
/*0x050*/     ULONG32      WinLogonFlags;                                                        
/*0x054*/     ULONG32      Spare3;                                                               
/*0x058*/     ULONG32      DozeS4Timeout;                                                        
/*0x05C*/     ULONG32      BroadcastCapacityResolution;                                          
/*0x060*/     struct _SYSTEM_POWER_LEVEL DischargePolicy[4];                                     
/*0x0C0*/     ULONG32      VideoTimeout;                                                         
/*0x0C4*/     UINT8        VideoDimDisplay;                                                      
/*0x0C5*/     UINT8        _PADDING0_[0x3];                                                      
/*0x0C8*/     ULONG32      VideoReserved[3];                                                     
/*0x0D4*/     ULONG32      SpindownTimeout;                                                      
/*0x0D8*/     UINT8        OptimizeForPower;                                                     
/*0x0D9*/     UINT8        FanThrottleTolerance;                                                 
/*0x0DA*/     UINT8        ForcedThrottle;                                                       
/*0x0DB*/     UINT8        MinThrottle;                                                          
/*0x0DC*/     struct _POWER_ACTION_POLICY OverThrottled;     // 3 elements, 0xC bytes (sizeof)   
          }SYSTEM_POWER_POLICY, *PSYSTEM_POWER_POLICY;                                           
                                                                                                 
          typedef struct _Wx86ThreadState   // 4 elements, 0xC bytes (sizeof) 
          {                                                                   
/*0x000*/     ULONG32*     CallBx86Eip;                                       
/*0x004*/     VOID*        DeallocationCpu;                                   
/*0x008*/     UINT8        UseKnownWx86Dll;                                   
/*0x009*/     CHAR         OleStubInvoked;                                    
/*0x00A*/     UINT8        _PADDING0_[0x2];                                   
          }Wx86ThreadState, *PWx86ThreadState;                                  
                                                                              
          typedef struct _TEB                                          // 66 elements, 0xFB8 bytes (sizeof) 
          {                                                                                                 
/*0x000*/     struct _NT_TIB NtTib;                                    // 8 elements, 0x1C bytes (sizeof)   
/*0x01C*/     VOID*        EnvironmentPointer;                                                              
/*0x020*/     struct _CLIENT_ID ClientId;                              // 2 elements, 0x8 bytes (sizeof)    
/*0x028*/     VOID*        ActiveRpcHandle;                                                                 
/*0x02C*/     VOID*        ThreadLocalStoragePointer;                                                       
/*0x030*/     struct _PEB* ProcessEnvironmentBlock;                                                         
/*0x034*/     ULONG32      LastErrorValue;                                                                  
/*0x038*/     ULONG32      CountOfOwnedCriticalSections;                                                    
/*0x03C*/     VOID*        CsrClientThread;                                                                 
/*0x040*/     VOID*        Win32ThreadInfo;                                                                 
/*0x044*/     ULONG32      User32Reserved[26];                                                              
/*0x0AC*/     ULONG32      UserReserved[5];                                                                 
/*0x0C0*/     VOID*        WOW32Reserved;                                                                   
/*0x0C4*/     ULONG32      CurrentLocale;                                                                   
/*0x0C8*/     ULONG32      FpSoftwareStatusRegister;                                                        
/*0x0CC*/     VOID*        SystemReserved1[54];                                                             
/*0x1A4*/     LONG32       ExceptionCode;                                                                   
/*0x1A8*/     struct _ACTIVATION_CONTEXT_STACK ActivationContextStack; // 4 elements, 0x14 bytes (sizeof)   
/*0x1BC*/     UINT8        SpareBytes1[24];                                                                 
/*0x1D4*/     struct _GDI_TEB_BATCH GdiTebBatch;                       // 3 elements, 0x4E0 bytes (sizeof)  
/*0x6B4*/     struct _CLIENT_ID RealClientId;                          // 2 elements, 0x8 bytes (sizeof)    
/*0x6BC*/     VOID*        GdiCachedProcessHandle;                                                          
/*0x6C0*/     ULONG32      GdiClientPID;                                                                    
/*0x6C4*/     ULONG32      GdiClientTID;                                                                    
/*0x6C8*/     VOID*        GdiThreadLocalInfo;                                                              
/*0x6CC*/     ULONG32      Win32ClientInfo[62];                                                             
/*0x7C4*/     VOID*        glDispatchTable[233];                                                            
/*0xB68*/     ULONG32      glReserved1[29];                                                                 
/*0xBDC*/     VOID*        glReserved2;                                                                     
/*0xBE0*/     VOID*        glSectionInfo;                                                                   
/*0xBE4*/     VOID*        glSection;                                                                       
/*0xBE8*/     VOID*        glTable;                                                                         
/*0xBEC*/     VOID*        glCurrentRC;                                                                     
/*0xBF0*/     VOID*        glContext;                                                                       
/*0xBF4*/     ULONG32      LastStatusValue;                                                                 
/*0xBF8*/     struct _UNICODE_STRING StaticUnicodeString;              // 3 elements, 0x8 bytes (sizeof)    
/*0xC00*/     UINT16       StaticUnicodeBuffer[261];                                                        
/*0xE0A*/     UINT8        _PADDING0_[0x2];                                                                 
/*0xE0C*/     VOID*        DeallocationStack;                                                               
/*0xE10*/     VOID*        TlsSlots[64];                                                                    
/*0xF10*/     struct _LIST_ENTRY TlsLinks;                             // 2 elements, 0x8 bytes (sizeof)    
/*0xF18*/     VOID*        Vdm;                                                                             
/*0xF1C*/     VOID*        ReservedForNtRpc;                                                                
/*0xF20*/     VOID*        DbgSsReserved[2];                                                                
/*0xF28*/     ULONG32      HardErrorsAreDisabled;                                                           
/*0xF2C*/     VOID*        Instrumentation[16];                                                             
/*0xF6C*/     VOID*        WinSockData;                                                                     
/*0xF70*/     ULONG32      GdiBatchCount;                                                                   
/*0xF74*/     UINT8        InDbgPrint;                                                                      
/*0xF75*/     UINT8        FreeStackOnTermination;                                                          
/*0xF76*/     UINT8        HasFiberData;                                                                    
/*0xF77*/     UINT8        IdealProcessor;                                                                  
/*0xF78*/     ULONG32      Spare3;                                                                          
/*0xF7C*/     VOID*        ReservedForPerf;                                                                 
/*0xF80*/     VOID*        ReservedForOle;                                                                  
/*0xF84*/     ULONG32      WaitingOnLoaderLock;                                                             
/*0xF88*/     struct _Wx86ThreadState Wx86Thread;                      // 4 elements, 0xC bytes (sizeof)    
/*0xF94*/     VOID**       TlsExpansionSlots;                                                               
/*0xF98*/     ULONG32      ImpersonationLocale;                                                             
/*0xF9C*/     ULONG32      IsImpersonating;                                                                 
/*0xFA0*/     VOID*        NlsCache;                                                                        
/*0xFA4*/     VOID*        pShimData;                                                                       
/*0xFA8*/     ULONG32      HeapVirtualAffinity;                                                             
/*0xFAC*/     VOID*        CurrentTransactionHandle;                                                        
/*0xFB0*/     struct _TEB_ACTIVE_FRAME* ActiveFrame;                                                        
/*0xFB4*/     UINT8        SafeThunkCall;                                                                   
/*0xFB5*/     UINT8        BooleanSpare[3];                                                                 
          }TEB, *PTEB;                                                                                      
                                                                                                            
          typedef struct _TEB_ACTIVE_FRAME               // 3 elements, 0xC bytes (sizeof) 
          {                                                                                
/*0x000*/     ULONG32      Flags;                                                          
/*0x004*/     struct _TEB_ACTIVE_FRAME* Previous;                                          
/*0x008*/     struct _TEB_ACTIVE_FRAME_CONTEXT* Context;                                   
          }TEB_ACTIVE_FRAME, *PTEB_ACTIVE_FRAME;                                           
                                                                                           
          typedef struct _TEB_ACTIVE_FRAME_CONTEXT // 2 elements, 0x8 bytes (sizeof) 
          {                                                                          
/*0x000*/     ULONG32      Flags;                                                    
/*0x004*/     CHAR*        FrameName;                                                
          }TEB_ACTIVE_FRAME_CONTEXT, *PTEB_ACTIVE_FRAME_CONTEXT;                                  
                                                                                     
          typedef struct _TERMINATION_PORT    // 2 elements, 0x8 bytes (sizeof) 
          {                                                                     
/*0x000*/     struct _TERMINATION_PORT* Next;                                   
/*0x004*/     VOID*        Port;                                                
          }TERMINATION_PORT, *PTERMINATION_PORT;                                  
                                                                                
          typedef struct _TOKEN                                      // 30 elements, 0xA8 bytes (sizeof) 
          {                                                                                              
/*0x000*/     struct _TOKEN_SOURCE TokenSource;                      // 2 elements, 0x10 bytes (sizeof)  
/*0x010*/     struct _LUID TokenId;                                  // 2 elements, 0x8 bytes (sizeof)   
/*0x018*/     struct _LUID AuthenticationId;                         // 2 elements, 0x8 bytes (sizeof)   
/*0x020*/     struct _LUID ParentTokenId;                            // 2 elements, 0x8 bytes (sizeof)   
/*0x028*/     union _LARGE_INTEGER ExpirationTime;                   // 4 elements, 0x8 bytes (sizeof)   
/*0x030*/     struct _ERESOURCE* TokenLock;                                                              
/*0x034*/     UINT8        _PADDING0_[0x4];                                                              
/*0x038*/     struct _SEP_AUDIT_POLICY AuditPolicy;                  // 3 elements, 0x8 bytes (sizeof)   
/*0x040*/     struct _LUID ModifiedId;                               // 2 elements, 0x8 bytes (sizeof)   
/*0x048*/     ULONG32      SessionId;                                                                    
/*0x04C*/     ULONG32      UserAndGroupCount;                                                            
/*0x050*/     ULONG32      RestrictedSidCount;                                                           
/*0x054*/     ULONG32      PrivilegeCount;                                                               
/*0x058*/     ULONG32      VariableLength;                                                               
/*0x05C*/     ULONG32      DynamicCharged;                                                               
/*0x060*/     ULONG32      DynamicAvailable;                                                             
/*0x064*/     ULONG32      DefaultOwnerIndex;                                                            
/*0x068*/     struct _SID_AND_ATTRIBUTES* UserAndGroups;                                                 
/*0x06C*/     struct _SID_AND_ATTRIBUTES* RestrictedSids;                                                
/*0x070*/     VOID*        PrimaryGroup;                                                                 
/*0x074*/     struct _LUID_AND_ATTRIBUTES* Privileges;                                                   
/*0x078*/     ULONG32*     DynamicPart;                                                                  
/*0x07C*/     struct _ACL* DefaultDacl;                                                                  
/*0x080*/     enum _TOKEN_TYPE TokenType;                                                                
/*0x084*/     enum _SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;                                     
/*0x088*/     ULONG32      TokenFlags;                                                                   
/*0x08C*/     UINT8        TokenInUse;                                                                   
/*0x08D*/     UINT8        _PADDING1_[0x3];                                                              
/*0x090*/     struct _SECURITY_TOKEN_PROXY_DATA* ProxyData;                                              
/*0x094*/     struct _SECURITY_TOKEN_AUDIT_DATA* AuditData;                                              
/*0x098*/     struct _LUID OriginatingLogonSession;                  // 2 elements, 0x8 bytes (sizeof)   
/*0x0A0*/     ULONG32      VariablePart;                                                                 
/*0x0A4*/     UINT8        _PADDING2_[0x4];                                                              
          }TOKEN, *PTOKEN;                                                                               
                                                                                                         
          typedef struct _TRACE_ENABLE_FLAG_EXTENSION // 3 elements, 0x4 bytes (sizeof) 
          {                                                                             
/*0x000*/     UINT16       Offset;                                                      
/*0x002*/     UINT8        Length;                                                      
/*0x003*/     UINT8        Flag;                                                        
          }TRACE_ENABLE_FLAG_EXTENSION, *PTRACE_ENABLE_FLAG_EXTENSION;                                  
                                                                                        
          typedef struct _VACB                          // 4 elements, 0x18 bytes (sizeof) 
          {                                                                                
/*0x000*/     VOID*        BaseAddress;                                                    
/*0x004*/     struct _SHARED_CACHE_MAP* SharedCacheMap;                                    
              union                                     // 2 elements, 0x8 bytes (sizeof)  
              {                                                                            
/*0x008*/         union _LARGE_INTEGER FileOffset;      // 4 elements, 0x8 bytes (sizeof)  
/*0x008*/         UINT16       ActiveCount;                                                
              }Overlay;                                                                    
/*0x010*/     struct _LIST_ENTRY LruList;               // 2 elements, 0x8 bytes (sizeof)  
          }VACB, *PVACB;                                                                   
                                                                                           
          typedef struct _VACB_LEVEL_REFERENCE // 2 elements, 0x8 bytes (sizeof) 
          {                                                                      
/*0x000*/     LONG32       Reference;                                            
/*0x004*/     LONG32       SpecialReference;                                     
          }VACB_LEVEL_REFERENCE, *PVACB_LEVEL_REFERENCE;                                  
                                                                                 
          typedef struct _VI_DEADLOCK_GLOBALS            // 29 elements, 0x110 bytes (sizeof) 
          {                                                                                   
/*0x000*/     ULONG32      Nodes[2];                                                          
/*0x008*/     ULONG32      Resources[2];                                                      
/*0x010*/     ULONG32      Threads[2];                                                        
/*0x018*/     INT64        TimeAcquire;                                                       
/*0x020*/     INT64        TimeRelease;                                                       
/*0x028*/     ULONG32      BytesAllocated;                                                    
/*0x02C*/     struct _LIST_ENTRY* ResourceDatabase;                                           
/*0x030*/     struct _LIST_ENTRY* ThreadDatabase;                                             
/*0x034*/     ULONG32      AllocationFailures;                                                
/*0x038*/     ULONG32      NodesTrimmedBasedOnAge;                                            
/*0x03C*/     ULONG32      NodesTrimmedBasedOnCount;                                          
/*0x040*/     ULONG32      NodesSearched;                                                     
/*0x044*/     ULONG32      MaxNodesSearched;                                                  
/*0x048*/     ULONG32      SequenceNumber;                                                    
/*0x04C*/     ULONG32      RecursionDepthLimit;                                               
/*0x050*/     ULONG32      SearchedNodesLimit;                                                
/*0x054*/     ULONG32      DepthLimitHits;                                                    
/*0x058*/     ULONG32      SearchLimitHits;                                                   
/*0x05C*/     ULONG32      ABC_ACB_Skipped;                                                   
/*0x060*/     struct _LIST_ENTRY FreeResourceList;       // 2 elements, 0x8 bytes (sizeof)    
/*0x068*/     struct _LIST_ENTRY FreeThreadList;         // 2 elements, 0x8 bytes (sizeof)    
/*0x070*/     struct _LIST_ENTRY FreeNodeList;           // 2 elements, 0x8 bytes (sizeof)    
/*0x078*/     ULONG32      FreeResourceCount;                                                 
/*0x07C*/     ULONG32      FreeThreadCount;                                                   
/*0x080*/     ULONG32      FreeNodeCount;                                                     
/*0x084*/     VOID*        Instigator;                                                        
/*0x088*/     ULONG32      NumberOfParticipants;                                              
/*0x08C*/     struct _VI_DEADLOCK_NODE* Participant[32];                                      
/*0x10C*/     ULONG32      CacheReductionInProgress;                                          
          }VI_DEADLOCK_GLOBALS, *PVI_DEADLOCK_GLOBALS;                                        
                                                                                              
          typedef struct _VI_DEADLOCK_NODE             // 12 elements, 0x68 bytes (sizeof) 
          {                                                                                
/*0x000*/     struct _VI_DEADLOCK_NODE* Parent;                                            
/*0x004*/     struct _LIST_ENTRY ChildrenList;         // 2 elements, 0x8 bytes (sizeof)   
/*0x00C*/     struct _LIST_ENTRY SiblingsList;         // 2 elements, 0x8 bytes (sizeof)   
              union                                    // 2 elements, 0x8 bytes (sizeof)   
              {                                                                            
/*0x014*/         struct _LIST_ENTRY ResourceList;     // 2 elements, 0x8 bytes (sizeof)   
/*0x014*/         struct _LIST_ENTRY FreeListEntry;    // 2 elements, 0x8 bytes (sizeof)   
              };                                                                           
/*0x01C*/     struct _VI_DEADLOCK_RESOURCE* Root;                                          
/*0x020*/     struct _VI_DEADLOCK_THREAD* ThreadEntry;                                     
              struct                                   // 3 elements, 0x4 bytes (sizeof)   
              {                                                                            
/*0x024*/         ULONG32      Active : 1;             // 0 BitPosition                    
/*0x024*/         ULONG32      OnlyTryAcquireUsed : 1; // 1 BitPosition                    
/*0x024*/         ULONG32      SequenceNumber : 30;    // 2 BitPosition                    
              };                                                                           
/*0x028*/     VOID*        StackTrace[8];                                                  
/*0x048*/     VOID*        ParentStackTrace[8];                                            
          }VI_DEADLOCK_NODE, *PVI_DEADLOCK_NODE;                                           
                                                                                           
          typedef struct _VI_DEADLOCK_RESOURCE         // 11 elements, 0x80 bytes (sizeof) 
          {                                                                                
/*0x000*/     enum _VI_DEADLOCK_RESOURCE_TYPE Type;                                        
              struct                                   // 2 elements, 0x4 bytes (sizeof)   
              {                                                                            
/*0x004*/         ULONG32      NodeCount : 16;         // 0 BitPosition                    
/*0x004*/         ULONG32      RecursionCount : 16;    // 16 BitPosition                   
              };                                                                           
/*0x008*/     VOID*        ResourceAddress;                                                
/*0x00C*/     struct _VI_DEADLOCK_THREAD* ThreadOwner;                                     
/*0x010*/     struct _LIST_ENTRY ResourceList;         // 2 elements, 0x8 bytes (sizeof)   
              union                                    // 2 elements, 0x8 bytes (sizeof)   
              {                                                                            
/*0x018*/         struct _LIST_ENTRY HashChainList;    // 2 elements, 0x8 bytes (sizeof)   
/*0x018*/         struct _LIST_ENTRY FreeListEntry;    // 2 elements, 0x8 bytes (sizeof)   
              };                                                                           
/*0x020*/     VOID*        StackTrace[8];                                                  
/*0x040*/     VOID*        LastAcquireTrace[8];                                            
/*0x060*/     VOID*        LastReleaseTrace[8];                                            
          }VI_DEADLOCK_RESOURCE, *PVI_DEADLOCK_RESOURCE;                                    
                                                                                           
          typedef struct _VI_DEADLOCK_THREAD              // 7 elements, 0x1C bytes (sizeof) 
          {                                                                                  
/*0x000*/     struct _KTHREAD* Thread;                                                       
/*0x004*/     struct _VI_DEADLOCK_NODE* CurrentSpinNode;                                     
/*0x008*/     struct _VI_DEADLOCK_NODE* CurrentOtherNode;                                    
              union                                       // 2 elements, 0x8 bytes (sizeof)  
              {                                                                              
/*0x00C*/         struct _LIST_ENTRY ListEntry;           // 2 elements, 0x8 bytes (sizeof)  
/*0x00C*/         struct _LIST_ENTRY FreeListEntry;       // 2 elements, 0x8 bytes (sizeof)  
              };                                                                             
/*0x014*/     ULONG32      NodeCount;                                                        
/*0x018*/     ULONG32      PagingCount;                                                      
          }VI_DEADLOCK_THREAD, *PVI_DEADLOCK_THREAD;                                         
                                                                                             
          typedef struct _VI_POOL_ENTRY_INUSE // 4 elements, 0x10 bytes (sizeof) 
          {                                                                      
/*0x000*/     VOID*        VirtualAddress;                                       
/*0x004*/     VOID*        CallingAddress;                                       
/*0x008*/     ULONG32      NumberOfBytes;                                        
/*0x00C*/     ULONG32      Tag;                                                  
          }VI_POOL_ENTRY_INUSE, *PVI_POOL_ENTRY_INUSE;                                   
                                                                                 
          typedef struct _VI_POOL_ENTRY              // 2 elements, 0x10 bytes (sizeof) 
          {                                                                             
              union                                  // 2 elements, 0x10 bytes (sizeof) 
              {                                                                         
/*0x000*/         struct _VI_POOL_ENTRY_INUSE InUse; // 4 elements, 0x10 bytes (sizeof) 
/*0x000*/         ULONG32      FreeListNext;                                            
              };                                                                        
          }VI_POOL_ENTRY, *PVI_POOL_ENTRY;                                              
                                                                                        
          typedef struct _VPB                      // 9 elements, 0x58 bytes (sizeof) 
          {                                                                           
/*0x000*/     INT16        Type;                                                      
/*0x002*/     INT16        Size;                                                      
/*0x004*/     UINT16       Flags;                                                     
/*0x006*/     UINT16       VolumeLabelLength;                                         
/*0x008*/     struct _DEVICE_OBJECT* DeviceObject;                                    
/*0x00C*/     struct _DEVICE_OBJECT* RealDevice;                                      
/*0x010*/     ULONG32      SerialNumber;                                              
/*0x014*/     ULONG32      ReferenceCount;                                            
/*0x018*/     UINT16       VolumeLabel[32];                                           
          }VPB, *PVPB;                                                                
                                                                                      
          typedef struct _WNODE_HEADER            // 11 elements, 0x30 bytes (sizeof) 
          {                                                                           
/*0x000*/     ULONG32      BufferSize;                                                
/*0x004*/     ULONG32      ProviderId;                                                
              union                               // 2 elements, 0x8 bytes (sizeof)   
              {                                                                       
/*0x008*/         UINT64       HistoricalContext;                                     
                  struct                          // 2 elements, 0x8 bytes (sizeof)   
                  {                                                                   
/*0x008*/             ULONG32      Version;                                           
/*0x00C*/             ULONG32      Linkage;                                           
                  };                                                                  
              };                                                                      
              union                               // 3 elements, 0x8 bytes (sizeof)   
              {                                                                       
/*0x010*/         ULONG32      CountLost;                                             
/*0x010*/         VOID*        KernelHandle;                                          
/*0x010*/         union _LARGE_INTEGER TimeStamp; // 4 elements, 0x8 bytes (sizeof)   
              };                                                                      
/*0x018*/     struct _GUID Guid;                  // 4 elements, 0x10 bytes (sizeof)  
/*0x028*/     ULONG32      ClientContext;                                             
/*0x02C*/     ULONG32      Flags;                                                     
          }WNODE_HEADER, *PWNODE_HEADER;                                              
                                                                                      
          typedef struct _WMI_CLIENT_CONTEXT // 3 elements, 0x4 bytes (sizeof) 
          {                                                                    
/*0x000*/     UINT8        ProcessorNumber;                                    
/*0x001*/     UINT8        Alignment;                                          
/*0x002*/     UINT16       LoggerId;                                           
          }WMI_CLIENT_CONTEXT, *PWMI_CLIENT_CONTEXT;                                  
                                                                               
          typedef struct _WMI_BUFFER_STATE // 4 elements, 0x4 bytes (sizeof) 
          {                                                                  
/*0x000*/     ULONG32      Free : 1;       // 0 BitPosition                  
/*0x000*/     ULONG32      InUse : 1;      // 1 BitPosition                  
/*0x000*/     ULONG32      Flush : 1;      // 2 BitPosition                  
/*0x000*/     ULONG32      Unused : 29;    // 3 BitPosition                  
          }WMI_BUFFER_STATE, *PWMI_BUFFER_STATE;                                  
                                                                             
          typedef struct _WMI_BUFFER_HEADER                         // 21 elements, 0x48 bytes (sizeof) 
          {                                                                                             
              union                                                 // 3 elements, 0x30 bytes (sizeof)  
              {                                                                                         
/*0x000*/         struct _WNODE_HEADER Wnode;                       // 11 elements, 0x30 bytes (sizeof) 
                  struct                                            // 4 elements, 0x30 bytes (sizeof)  
                  {                                                                                     
/*0x000*/             UINT64       Reserved1;                                                           
/*0x008*/             UINT64       Reserved2;                                                           
/*0x010*/             union _LARGE_INTEGER Reserved3;               // 4 elements, 0x8 bytes (sizeof)   
                      union                                         // 2 elements, 0x8 bytes (sizeof)   
                      {                                                                                 
                          struct                                    // 2 elements, 0x8 bytes (sizeof)   
                          {                                                                             
/*0x018*/                     VOID*        Alignment;                                                   
/*0x01C*/                     struct _SINGLE_LIST_ENTRY SlistEntry; // 1 elements, 0x4 bytes (sizeof)   
                          };                                                                            
/*0x018*/                 struct _LIST_ENTRY Entry;                 // 2 elements, 0x8 bytes (sizeof)   
                      };                                                                                
                  };                                                                                    
                  struct                                            // 8 elements, 0x30 bytes (sizeof)  
                  {                                                                                     
/*0x000*/             LONG32       ReferenceCount;                                                      
/*0x004*/             ULONG32      SavedOffset;                                                         
/*0x008*/             ULONG32      CurrentOffset;                                                       
/*0x00C*/             ULONG32      UsePerfClock;                                                        
/*0x010*/             union _LARGE_INTEGER TimeStamp;               // 4 elements, 0x8 bytes (sizeof)   
/*0x018*/             struct _GUID Guid;                            // 4 elements, 0x10 bytes (sizeof)  
/*0x028*/             struct _WMI_CLIENT_CONTEXT ClientContext;     // 3 elements, 0x4 bytes (sizeof)   
                      union                                         // 2 elements, 0x4 bytes (sizeof)   
                      {                                                                                 
/*0x02C*/                 struct _WMI_BUFFER_STATE State;           // 4 elements, 0x4 bytes (sizeof)   
/*0x02C*/                 ULONG32      Flags;                                                           
                      };                                                                                
                  };                                                                                    
              };                                                                                        
/*0x030*/     ULONG32      Offset;                                                                      
/*0x034*/     ULONG32      EventsLost;                                                                  
              union                                                 // 2 elements, 0x10 bytes (sizeof)  
              {                                                                                         
/*0x038*/         struct _GUID InstanceGuid;                        // 4 elements, 0x10 bytes (sizeof)  
                  struct                                            // 2 elements, 0x10 bytes (sizeof)  
                  {                                                                                     
/*0x038*/             VOID*        LoggerContext;                                                       
/*0x03C*/             struct _SINGLE_LIST_ENTRY GlobalEntry;        // 1 elements, 0x4 bytes (sizeof)   
/*0x040*/             UINT8        _PADDING0_[0x8];                                                     
                  };                                                                                    
              };                                                                                        
          }WMI_BUFFER_HEADER, *PWMI_BUFFER_HEADER;                                                      
                                                                                                        
          typedef struct _WMI_LOGGER_MODE         // 13 elements, 0x4 bytes (sizeof) 
          {                                                                          
/*0x000*/     ULONG32      SequentialFile : 1;    // 0 BitPosition                   
/*0x000*/     ULONG32      CircularFile : 1;      // 1 BitPosition                   
/*0x000*/     ULONG32      AppendFile : 1;        // 2 BitPosition                   
/*0x000*/     ULONG32      Unused1 : 5;           // 3 BitPosition                   
/*0x000*/     ULONG32      RealTime : 1;          // 8 BitPosition                   
/*0x000*/     ULONG32      DelayOpenFile : 1;     // 9 BitPosition                   
/*0x000*/     ULONG32      BufferOnly : 1;        // 10 BitPosition                  
/*0x000*/     ULONG32      PrivateLogger : 1;     // 11 BitPosition                  
/*0x000*/     ULONG32      AddHeader : 1;         // 12 BitPosition                  
/*0x000*/     ULONG32      UseExisting : 1;       // 13 BitPosition                  
/*0x000*/     ULONG32      UseGlobalSequence : 1; // 14 BitPosition                  
/*0x000*/     ULONG32      UseLocalSequence : 1;  // 15 BitPosition                  
/*0x000*/     ULONG32      Unused2 : 16;          // 16 BitPosition                  
          }WMI_LOGGER_MODE, *PWMI_LOGGER_MODE;                                       
                                                                                     
          typedef struct _WMI_LOGGER_CONTEXT                           // 65 elements, 0x1C8 bytes (sizeof) 
          {                                                                                                 
/*0x000*/     ULONG32      BufferSpinLock;                                                                  
/*0x004*/     UINT8        _PADDING0_[0x4];                                                                 
/*0x008*/     union _LARGE_INTEGER StartTime;                          // 4 elements, 0x8 bytes (sizeof)    
/*0x010*/     VOID*        LogFileHandle;                                                                   
/*0x014*/     struct _KSEMAPHORE LoggerSemaphore;                      // 2 elements, 0x14 bytes (sizeof)   
/*0x028*/     struct _ETHREAD* LoggerThread;                                                                
/*0x02C*/     struct _KEVENT LoggerEvent;                              // 1 elements, 0x10 bytes (sizeof)   
/*0x03C*/     struct _KEVENT FlushEvent;                               // 1 elements, 0x10 bytes (sizeof)   
/*0x04C*/     LONG32       LoggerStatus;                                                                    
/*0x050*/     ULONG32      LoggerId;                                                                        
/*0x054*/     LONG32       BuffersAvailable;                                                                
/*0x058*/     ULONG32      UsePerfClock;                                                                    
/*0x05C*/     ULONG32      WriteFailureLimit;                                                               
/*0x060*/     ULONG32      BuffersDirty;                                                                    
/*0x064*/     ULONG32      BuffersInUse;                                                                    
/*0x068*/     ULONG32      SwitchingInProgress;                                                             
/*0x06C*/     UINT8        _PADDING1_[0x4];                                                                 
/*0x070*/     union _SLIST_HEADER FreeList;                            // 4 elements, 0x8 bytes (sizeof)    
/*0x078*/     union _SLIST_HEADER FlushList;                           // 4 elements, 0x8 bytes (sizeof)    
/*0x080*/     union _SLIST_HEADER GlobalList;                          // 4 elements, 0x8 bytes (sizeof)    
/*0x088*/     union _SLIST_HEADER* ProcessorBuffers;                                                        
/*0x08C*/     struct _UNICODE_STRING LoggerName;                       // 3 elements, 0x8 bytes (sizeof)    
/*0x094*/     struct _UNICODE_STRING LogFileName;                      // 3 elements, 0x8 bytes (sizeof)    
/*0x09C*/     struct _UNICODE_STRING LogFilePattern;                   // 3 elements, 0x8 bytes (sizeof)    
/*0x0A4*/     struct _UNICODE_STRING NewLogFileName;                   // 3 elements, 0x8 bytes (sizeof)    
/*0x0AC*/     UINT8*       EndPageMarker;                                                                   
/*0x0B0*/     LONG32       CollectionOn;                                                                    
/*0x0B4*/     ULONG32      KernelTraceOn;                                                                   
/*0x0B8*/     LONG32       PerfLogInTransition;                                                             
/*0x0BC*/     ULONG32      RequestFlag;                                                                     
/*0x0C0*/     ULONG32      EnableFlags;                                                                     
/*0x0C4*/     ULONG32      MaximumFileSize;                                                                 
              union                                                    // 2 elements, 0x4 bytes (sizeof)    
              {                                                                                             
/*0x0C8*/         ULONG32      LoggerMode;                                                                  
/*0x0C8*/         struct _WMI_LOGGER_MODE LoggerModeFlags;             // 13 elements, 0x4 bytes (sizeof)   
              };                                                                                            
/*0x0CC*/     ULONG32      LastFlushedBuffer;                                                               
/*0x0D0*/     ULONG32      RefCount;                                                                        
/*0x0D4*/     ULONG32      FlushTimer;                                                                      
/*0x0D8*/     union _LARGE_INTEGER FirstBufferOffset;                  // 4 elements, 0x8 bytes (sizeof)    
/*0x0E0*/     union _LARGE_INTEGER ByteOffset;                         // 4 elements, 0x8 bytes (sizeof)    
/*0x0E8*/     union _LARGE_INTEGER BufferAgeLimit;                     // 4 elements, 0x8 bytes (sizeof)    
/*0x0F0*/     ULONG32      MaximumBuffers;                                                                  
/*0x0F4*/     ULONG32      MinimumBuffers;                                                                  
/*0x0F8*/     ULONG32      EventsLost;                                                                      
/*0x0FC*/     ULONG32      BuffersWritten;                                                                  
/*0x100*/     ULONG32      LogBuffersLost;                                                                  
/*0x104*/     ULONG32      RealTimeBuffersLost;                                                             
/*0x108*/     ULONG32      BufferSize;                                                                      
/*0x10C*/     LONG32       NumberOfBuffers;                                                                 
/*0x110*/     LONG32*      SequencePtr;                                                                     
/*0x114*/     struct _GUID InstanceGuid;                               // 4 elements, 0x10 bytes (sizeof)   
/*0x124*/     VOID*        LoggerHeader;                                                                    
/*0x128*/     FUNCT_004D_0E28_GetCpuClock* GetCpuClock;                                                     
/*0x12C*/     struct _SECURITY_CLIENT_CONTEXT ClientSecurityContext;   // 6 elements, 0x3C bytes (sizeof)   
/*0x168*/     VOID*        LoggerExtension;                                                                 
/*0x16C*/     LONG32       ReleaseQueue;                                                                    
/*0x170*/     struct _TRACE_ENABLE_FLAG_EXTENSION EnableFlagExtension; // 3 elements, 0x4 bytes (sizeof)    
/*0x174*/     ULONG32      LocalSequence;                                                                   
/*0x178*/     ULONG32      MaximumIrql;                                                                     
/*0x17C*/     ULONG32*     EnableFlagArray;                                                                 
/*0x180*/     struct _KMUTANT LoggerMutex;                             // 5 elements, 0x20 bytes (sizeof)   
/*0x1A0*/     LONG32       MutexCount;                                                                      
/*0x1A4*/     ULONG32      FileCounter;                                                                     
/*0x1A8*/     FUNCT_00BC_0E44_BufferCallback* BufferCallback;                                               
/*0x1AC*/     VOID*        CallbackContext;                                                                 
/*0x1B0*/     enum _POOL_TYPE PoolType;                                                                     
/*0x1B4*/     UINT8        _PADDING2_[0x4];                                                                 
/*0x1B8*/     union _LARGE_INTEGER ReferenceSystemTime;                // 4 elements, 0x8 bytes (sizeof)    
/*0x1C0*/     union _LARGE_INTEGER ReferenceTimeStamp;                 // 4 elements, 0x8 bytes (sizeof)    
          }WMI_LOGGER_CONTEXT, *PWMI_LOGGER_CONTEXT;                                                        
                                                                                                            
          typedef struct _WORK_QUEUE_ITEM                                                                                                                                              // 3 elements, 0x10 bytes (sizeof) 
          {                                                                                                                                                                                                               
/*0x000*/     struct _LIST_ENTRY List;                                                                                                                                                 // 2 elements, 0x8 bytes (sizeof)  
/*0x008*/     FUNCT_00BC_0285_Free_InterfaceReference_InterfaceDereference_DeleteProcedure_ReleaseFromLazyWrite_ReleaseFromReadAhead_WorkerRoutine_Destructor_Callback* WorkerRoutine;                                    
/*0x00C*/     VOID*        Parameter;                                                                                                                                                                                     
          }WORK_QUEUE_ITEM, *PWORK_QUEUE_ITEM;                                                                                                                                                                            
                                                                                                                                                                                                                          

#ifdef __cplusplus
}
#endif

