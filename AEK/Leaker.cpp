#include "Leaker.h"

namespace aek {

	// get ntoskrnl.exe base
	UINT64 Leaker::getKernelBase() {
		if (KernelBaseLeakRoutine)
			return KernelBaseLeakRoutine();

		//https://github.com/koczkatamas/CVE-2016-0051/blob/master/EoP/Shellcode/Shellcode.cpp

		PNtQuerySystemInformation NtQuerySystemInformation =
			(PNtQuerySystemInformation)GetProcAddress(GetModuleHandleA("ntdll.dll"),
				"NtQuerySystemInformation");

		if (!NtQuerySystemInformation) {

			std::cout << "[!] Failed to get the address of NtQuerySystemInformation." << std::endl;
			std::cout << "[!] Last error " << GetLastError() << std::endl;
			exit(1);
		}

		ULONG len = 0;
		NtQuerySystemInformation(SystemModuleInformation,
			NULL,
			0,
			&len);

		PSYSTEM_MODULE_INFORMATION pModuleInfo = (PSYSTEM_MODULE_INFORMATION)
			VirtualAlloc(NULL,
				len,
				MEM_RESERVE | MEM_COMMIT,
				PAGE_EXECUTE_READWRITE);

		if (!pModuleInfo) {
			std::cout << "[-] VirtaulAlloc Failed\n";
			return 0;
		}

		NTSTATUS status = NtQuerySystemInformation(SystemModuleInformation,
			pModuleInfo,
			len,
			&len);

		if (status != (NTSTATUS)0x0) {	// if not ntsuccess
			std::cout << "[!] NtQuerySystemInformation failed!" << std::endl;
			exit(1);
		}

		PVOID kernelImageBase = pModuleInfo->Modules[0].ImageBaseAddress;

		std::cout << "[+] ntoskrnl.exe base address: 0x" << std::hex << kernelImageBase << std::endl;

		return (UINT64)kernelImageBase;
	}

	// get module base by name
	UINT64 Leaker::getImageBase(const char image_name[]) {
		if (ImageBaseLeakRoutine)
			return ImageBaseLeakRoutine(image_name);

		// Defining EnumDeviceDrivers() and GetDeviceDriverBaseNameA() parameters
		LPVOID lpImageBase[1024];
		DWORD lpcbNeeded;
		int drivers;
		char lpFileName[0x100];
		UINT64 imageBase = 0;

		BOOL baseofDrivers = EnumDeviceDrivers(
			lpImageBase,
			sizeof(lpImageBase),
			&lpcbNeeded
		);

		// Error handling
		if (!baseofDrivers) {
			printf("[-] Error! Unable to invoke EnumDeviceDrivers(). Error: %d\n", GetLastError());
			exit(1);
		}

		// Defining number of drivers for GetDeviceDriverBaseNameA()
		drivers = lpcbNeeded / sizeof(lpImageBase[0]);

		// Parsing loaded drivers
		for (int i = 0; i < drivers; i++) {
			GetDeviceDriverBaseNameA(
				lpImageBase[i],
				lpFileName,
				sizeof(lpFileName) / sizeof(char)
			);

			// Keep looping, until found, to find user supplied driver base address
			if (!strcmp(image_name, lpFileName)) {
				imageBase = (unsigned long long)lpImageBase[i];
				std::cout << "[+] Found " << lpFileName << " : " << std::hex << imageBase << '\n';
				// Exit loop
				break;
			}
		}
		if (!imageBase) {
			std::cout << "No Image Named " << image_name << " Found\n";
		}

		return imageBase;
	}

	UINT64 Leaker::calcPteAddr(UINT64 addr, UINT64 pte_base) {
		UINT64 pteAddr = addr >> 9;
		pteAddr &= 0x7FFFFFFFF8;
		pteAddr += pte_base;
		return pteAddr;
	}
}