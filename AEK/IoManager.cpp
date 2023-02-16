#pragma once
#include "IoManager.h"

namespace aek {
	bool IO::create(const WCHAR* deviceLink) {
		handle = CreateFileW(deviceLink,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
		if (handle == INVALID_HANDLE_VALUE) {
			return false;
		}
		return true;
	}

	bool IO::create(const CHAR* deviceLink) {
		handle = CreateFileA(deviceLink,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
		if (handle == INVALID_HANDLE_VALUE) {
			std::cout << "[-] Handle Open Failed\n";
			exit(-1);
		}
		return true;
	}

	bool IO::ioctl(DWORD ctlcode, LPVOID i_buf, DWORD i_size, LPVOID o_buf, DWORD o_size) {
		return DeviceIoControl(handle, ctlcode, i_buf, i_size, o_buf, o_size, &dw_ret, 0);

	}
}