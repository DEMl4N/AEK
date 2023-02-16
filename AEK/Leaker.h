#pragma once
#include <Windows.h>
#include <iostream>
#include <winternl.h>
#include <Psapi.h>
#include <functional>

namespace aek {
	class Leaker {
	private:
		typedef struct SYSTEM_MODULE {
			ULONG                Reserved1;
			ULONG                Reserved2;
			ULONG				 Reserved3;
			PVOID                ImageBaseAddress;
			ULONG                ImageSize;
			ULONG                Flags;
			WORD                 Id;
			WORD                 Rank;
			WORD                 LoadCount;
			WORD                 NameOffset;
			CHAR                 Name[256];
		}SYSTEM_MODULE, * PSYSTEM_MODULE;

		typedef struct SYSTEM_MODULE_INFORMATION {
			ULONG                ModulesCount;
			SYSTEM_MODULE        Modules[1];
		} SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;

		typedef enum _SYSTEM_INFORMATION_CLASS {
			SystemModuleInformation = 0xb
		} SYSTEM_INFORMATION_CLASS;

		typedef NTSTATUS(WINAPI* PNtQuerySystemInformation)(
			__in SYSTEM_INFORMATION_CLASS SystemInformationClass,
			__inout PVOID SystemInformation,
			__in ULONG SystemInformationLength,
			__out_opt PULONG ReturnLength
			);

	public:
		Leaker() {};
		UINT64 getKernelBase();
		UINT64 getImageBase(const char image_name[]);

		UINT64 calcPteAddr(UINT64 addr, UINT64 pte_base);
		std::function<UINT64()> KernelBaseLeakRoutine;				// Leak under Low integrity
		std::function<UINT64(const char[])> ImageBaseLeakRoutine;	// Leak under Low integrity
	};
}