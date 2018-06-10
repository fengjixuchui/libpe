/*********************************************************************
* Copyright 2018 Jovibor: https://github.com/jovibor/
* PE viewer library for x86 (PE32) and x64 (PE32+) binares.
* Include this header into your project along with libpe.lib
* Additional info can be found at github.com/jovibor/libpe
*********************************************************************/
#pragma once
static_assert(_MSC_VER >= 1914, "MSVS 15.7 (C+17) or higher needed.");

#include <Imagehlp.h>

typedef const DWORD* PCDWORD;
typedef const IMAGE_DOS_HEADER *PLIBPE_DOSHEADER;

//Vector of undocumented DOUBLE DWORDs of "Rich" structure.
typedef std::vector<std::tuple<WORD, WORD, DWORD>> LIBPE_RICH_VEC;
typedef const LIBPE_RICH_VEC *PLIBPE_RICH_VEC;

//Only one IMAGE_OPTIONAL_HEADER structure of tuple will be filled, 
//x86 or x64 — depending on file type. Second will be zeroed.
typedef std::tuple<IMAGE_NT_HEADERS32, IMAGE_NT_HEADERS64> LIBPE_NTHEADER;
typedef const LIBPE_NTHEADER *PLIBPE_NTHEADER;

typedef const IMAGE_FILE_HEADER *PLIBPE_FILEHEADER;

//Only one structure of tuple will be filled depending on file type
//x86 or x64. Second will be zeroed.
typedef std::tuple<IMAGE_OPTIONAL_HEADER32, IMAGE_OPTIONAL_HEADER64> LIBPE_OPTHEADER;
typedef const LIBPE_OPTHEADER *PLIBPE_OPTHEADER;

//Vector of IMAGE_DATA_DIRECTORY and section name this dir resides in.
typedef std::vector<std::tuple<IMAGE_DATA_DIRECTORY, std::string>> LIBPE_DATADIRS_VEC;
typedef const LIBPE_DATADIRS_VEC *PLIBPE_DATADIRS_VEC;

//Sections.
typedef std::vector<IMAGE_SECTION_HEADER> LIBPE_SECHEADER_VEC;
typedef LIBPE_SECHEADER_VEC *PLIBPE_SECHEADER_VEC;

//Tuple of: IMAGE_EXPORT_DIRECTORY, Actual export module name
//and vector of exported funcs: RVA, ordinal, func name, func forwarder name.
typedef std::tuple<IMAGE_EXPORT_DIRECTORY, std::string, std::vector<std::tuple<DWORD, DWORD, std::string, std::string>>> LIBPE_EXPORT;
typedef const LIBPE_EXPORT *PLIBPE_EXPORT;

//Vector of import modules:
//IMAGE_IMPORT_DESCRIPTOR, import module name, vector of:
//Ordinal/Hint (depending on import type), func name, import thunk RVA.
typedef std::vector<std::tuple<IMAGE_IMPORT_DESCRIPTOR, std::string, std::vector<std::tuple<LONGLONG, std::string, LONGLONG>>>> LIBPE_IMPORT_VEC;
typedef const LIBPE_IMPORT_VEC *PLIBPE_IMPORT_VEC;

/*****************************Resources by Levels**************************************
* Totally 3 levels of resources. Highest level include lowest levels.
**************************************************************************************/
//Level 3 (the lowest) Resources.
typedef std::vector<std::tuple<IMAGE_RESOURCE_DIRECTORY_ENTRY, std::wstring/*ResName*/,
	IMAGE_RESOURCE_DATA_ENTRY, std::vector<std::byte>/*resource LVL3 RAW data*/>> LIBPE_RESOURCE_LVL3_VEC;
typedef const LIBPE_RESOURCE_LVL3_VEC *PLIBPE_RESOURCE_LVL3_VEC;
typedef std::tuple<IMAGE_RESOURCE_DIRECTORY, LIBPE_RESOURCE_LVL3_VEC> LIBPE_RESOURCE_LVL3;
typedef const LIBPE_RESOURCE_LVL3 *PLIBPE_RESOURCE_LVL3;

//Level 2 Resources — Includes LVL3 Resourses
typedef std::vector<std::tuple<IMAGE_RESOURCE_DIRECTORY_ENTRY, std::wstring/*ResName*/,
	IMAGE_RESOURCE_DATA_ENTRY, std::vector<std::byte>/*LVL2 RAW data*/, LIBPE_RESOURCE_LVL3>> LIBPE_RESOURCE_LVL2_VEC;
typedef const LIBPE_RESOURCE_LVL2_VEC *PLIBPE_RESOURCE_LVL2_VEC;
typedef std::tuple<IMAGE_RESOURCE_DIRECTORY, LIBPE_RESOURCE_LVL2_VEC> LIBPE_RESOURCE_LVL2;
typedef const LIBPE_RESOURCE_LVL2 *PLIBPE_RESOURCE_LVL2;

//Level 1 (Root) Resources — Includes LVL2 Resources
typedef std::vector<std::tuple<IMAGE_RESOURCE_DIRECTORY_ENTRY, std::wstring/*ResName*/,
	IMAGE_RESOURCE_DATA_ENTRY, std::vector<std::byte>/*LVL1 RAW data*/, LIBPE_RESOURCE_LVL2>> LIBPE_RESOURCE_ROOT_VEC;
typedef const LIBPE_RESOURCE_ROOT_VEC *PLIBPE_RESOURCE_LVL1_VEC;
typedef std::tuple<IMAGE_RESOURCE_DIRECTORY, LIBPE_RESOURCE_ROOT_VEC> LIBPE_RESOURCE_ROOT;
typedef const LIBPE_RESOURCE_ROOT *PLIBPE_RESOURCE_ROOT;
/***************************************************************************************
***************************************************************************************/

//Vector of Exception table
typedef std::vector<_IMAGE_RUNTIME_FUNCTION_ENTRY> LIBPE_EXCEPTION_VEC;
typedef const LIBPE_EXCEPTION_VEC *PLIBPE_EXCEPTION_VEC;

//Vector of security table:
//WIN_CERTIFICATE and vector of actual data in form of std::bytes
typedef std::vector<std::tuple<WIN_CERTIFICATE, std::vector<std::byte>>> LIBPE_SECURITY_VEC;
typedef const LIBPE_SECURITY_VEC *PLIBPE_SECURITY_VEC;

//Vector of relocations:
//IMAGE_BASE_RELOCATION, vector of: Reloc type and Offset
typedef std::vector<std::tuple<IMAGE_BASE_RELOCATION, std::vector<std::tuple<WORD, WORD>>>> LIBPE_RELOCATION_VEC;
typedef const LIBPE_RELOCATION_VEC *PLIBPE_RELOCATION_VEC;

//Vector of Debug entries
typedef std::vector<IMAGE_DEBUG_DIRECTORY> LIBPE_DEBUG_VEC;
typedef LIBPE_DEBUG_VEC *PLIBPE_DEBUG_VEC;

//TLS tuple. Only one structure is filled depending on file type - x86 or x64, second is zeroed.
//vector of std::byte — TLS Raw data, vector of TLS Callbacks. 
typedef std::tuple<IMAGE_TLS_DIRECTORY32, IMAGE_TLS_DIRECTORY64, std::vector<std::byte>/*Raw Data*/, std::vector<DWORD>> LIBPE_TLS;
typedef const LIBPE_TLS *PLIBPE_TLS;

//Filled depending on file type - x86 or x64, second is zeroed.
typedef std::tuple<IMAGE_LOAD_CONFIG_DIRECTORY32, IMAGE_LOAD_CONFIG_DIRECTORY64> LIBPE_LOADCONFIGTABLE;
typedef const LIBPE_LOADCONFIGTABLE *PLIBPE_LOADCONFIGTABLE;

//Vector of: IMAGE_BOUND_IMPORT_DESCRIPTOR, import module name, 
//vector of: IMAGE_BOUND_FORWARDER_REF, forwarder module name
typedef std::vector<std::tuple<IMAGE_BOUND_IMPORT_DESCRIPTOR, std::string,
	std::vector<std::tuple<IMAGE_BOUND_FORWARDER_REF, std::string>>>> LIBPE_BOUNDIMPORT_VEC;
typedef const LIBPE_BOUNDIMPORT_VEC *PLIBPE_BOUNDIMPORT_VEC;

//Delay import vector: IMAGE_DELAYLOAD_DESCRIPTOR, module name, vector of:
//Hint/Ordinal, Func name, ThunkName RVA, ThunkIAT RVA, ThunkBoundIAT RVA, ThunkUnloadedInfoIAT RVA.
typedef std::vector<std::tuple<IMAGE_DELAYLOAD_DESCRIPTOR, std::string,
	std::vector<std::tuple<LONGLONG, std::string, LONGLONG, LONGLONG, LONGLONG, LONGLONG>>>> LIBPE_DELAYIMPORT_VEC;
typedef const LIBPE_DELAYIMPORT_VEC *PLIBPE_DELAYIMPORT_VEC;

//COM descriptor table
typedef const IMAGE_COR20_HEADER *PLIBPE_COM_DESCRIPTOR;

//Pure Virtual base class Ilibpe
class __declspec(novtable) Ilibpe
{
public:
	virtual HRESULT LoadPe(LPCWSTR lpszFileName) = 0;
	virtual HRESULT GetFileSummary(PCDWORD*) = 0;
	virtual HRESULT GetMSDOSHeader(PLIBPE_DOSHEADER*) = 0;
	virtual HRESULT GetMSDOSRichHeader(PLIBPE_RICH_VEC*) = 0;
	virtual HRESULT GetNTHeader(PLIBPE_NTHEADER*) = 0;
	virtual HRESULT GetFileHeader(PLIBPE_FILEHEADER*) = 0;
	virtual HRESULT GetOptionalHeader(PLIBPE_OPTHEADER*) = 0;
	virtual HRESULT GetDataDirectories(PLIBPE_DATADIRS_VEC*) = 0;
	virtual HRESULT GetSectionHeaders(PLIBPE_SECHEADER_VEC*) = 0;
	virtual HRESULT GetExportTable(PLIBPE_EXPORT*) = 0;
	virtual HRESULT GetImportTable(PLIBPE_IMPORT_VEC*) = 0;
	virtual HRESULT GetResourceTable(PLIBPE_RESOURCE_ROOT*) = 0;
	virtual HRESULT GetExceptionTable(PLIBPE_EXCEPTION_VEC*) = 0;
	virtual HRESULT GetSecurityTable(PLIBPE_SECURITY_VEC*) = 0;
	virtual HRESULT GetRelocationTable(PLIBPE_RELOCATION_VEC*) = 0;
	virtual HRESULT GetDebugTable(PLIBPE_DEBUG_VEC*) = 0;
	virtual HRESULT GetTLSTable(PLIBPE_TLS*) = 0;
	virtual HRESULT GetLoadConfigTable(PLIBPE_LOADCONFIGTABLE*) = 0;
	virtual HRESULT GetBoundImportTable(PLIBPE_BOUNDIMPORT_VEC*) = 0;
	virtual HRESULT GetDelayImportTable(PLIBPE_DELAYIMPORT_VEC*) = 0;
	virtual HRESULT GetCOMDescriptorTable(PLIBPE_COM_DESCRIPTOR*) = 0;

	virtual HRESULT Release() = 0;
};

#if defined(ILIBPE_EXPORT)
#define ILIBPEAPI __declspec(dllexport) __cdecl
#else 
#define ILIBPEAPI __declspec(dllimport) __cdecl
#endif

extern "C" HRESULT ILIBPEAPI Getlibpe(Ilibpe**);

/*
*  Return errors
*/
#define	CALL_LOADPE_FIRST					0xFFFF
#define	FILE_OPEN_FAILED					0x0010
#define	FILE_SIZE_TOO_SMALL					0x0011
#define	FILE_CREATE_FILE_MAPPING_FAILED		0x0012
#define	FILE_MAP_VIEW_OF_FILE_FAILED		0x0013
#define	IMAGE_TYPE_UNSUPPORTED				0x0014
#define	IMAGE_DOS_SIGNATURE_MISMATCH		0x0015
#define	IMAGE_HAS_NO_DOS_HEADER				0x0016
#define	IMAGE_HAS_NO_RICH_HEADER			0x0017
#define	IMAGE_NT_SIGNATURE_MISMATCH			0x0018
#define	IMAGE_HAS_NO_NT_HEADER				0x0019
#define	IMAGE_HAS_NO_FILE_HEADER			0x001A
#define	IMAGE_HAS_NO_OPTIONAL_HEADER		0x001B
#define	IMAGE_HAS_NO_DATA_DIRECTORIES		0x001C
#define	IMAGE_HAS_NO_SECTIONS				0x001D
#define	IMAGE_HAS_NO_EXPORT_DIR				0x001E
#define	IMAGE_HAS_NO_IMPORT_DIR				0x001F
#define	IMAGE_HAS_NO_RESOURCE_DIR			0x0020
#define	IMAGE_HAS_NO_EXCEPTION_DIR			0x0021
#define	IMAGE_HAS_NO_SECURITY_DIR			0x0022
#define	IMAGE_HAS_NO_BASERELOC_DIR			0x0023
#define	IMAGE_HAS_NO_DEBUG_DIR				0x0024
#define	IMAGE_HAS_NO_ARCHITECTURE_DIR		0x0025
#define	IMAGE_HAS_NO_GLOBALPTR_DIR			0x0026
#define	IMAGE_HAS_NO_TLS_DIR				0x0027
#define	IMAGE_HAS_NO_LOADCONFIG_DIR			0x0028
#define	IMAGE_HAS_NO_BOUNDIMPORT_DIR		0x0029
#define	IMAGE_HAS_NO_IAT_DIR				0x002A
#define	IMAGE_HAS_NO_DELAY_IMPORT_DIR		0x002B
#define	IMAGE_HAS_NO_COMDESCRIPTOR_DIR		0x002C

/*
* Flags according to loaded PE file properties
*/
#define IMAGE_HAS_FLAG(dword, flag) ((dword) & (flag))
#define IMAGE_PE32_FLAG						0x00000001
#define IMAGE_PE64_FLAG						0x00000002
#define IMAGE_DOS_HEADER_FLAG				0x00000004
#define IMAGE_RICH_HEADER_FLAG				0x00000008
#define IMAGE_NT_HEADER_FLAG				0x00000010
#define IMAGE_FILE_HEADER_FLAG				0x00000020
#define IMAGE_OPTIONAL_HEADER_FLAG			0x00000040
#define IMAGE_DATA_DIRECTORIES_FLAG			0x00000080
#define IMAGE_SECTION_HEADERS_FLAG			0x00000100
#define IMAGE_EXPORT_DIRECTORY_FLAG			0x00000200
#define IMAGE_IMPORT_DIRECTORY_FLAG			0x00000400
#define IMAGE_RESOURCE_DIRECTORY_FLAG		0x00000800
#define IMAGE_EXCEPTION_DIRECTORY_FLAG		0x00001000
#define IMAGE_SECURITY_DIRECTORY_FLAG		0x00002000
#define IMAGE_BASERELOC_DIRECTORY_FLAG		0x00004000
#define IMAGE_DEBUG_DIRECTORY_FLAG			0x00008000
#define IMAGE_ARCHITECTURE_DIRECTORY_FLAG	0x00010000
#define IMAGE_GLOBALPTR_DIRECTORY_FLAG		0x00020000
#define IMAGE_TLS_DIRECTORY_FLAG			0x00040000
#define IMAGE_LOADCONFIG_DIRECTORY_FLAG		0x00080000
#define IMAGE_BOUNDIMPORT_DIRECTORY_FLAG	0x00100000
#define IMAGE_IAT_DIRECTORY_FLAG			0x00200000
#define IMAGE_DELAYIMPORT_DIRECTORY_FLAG	0x00400000
#define IMAGE_COMDESCRIPTOR_DIRECTORY_FLAG	0x00800000