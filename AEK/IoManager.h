#pragma once
#include <Windows.h>
#include <iostream>
#include <winternl.h>
#include <Psapi.h>

namespace aek {
	class IO {
	private:
		HANDLE handle = NULL;
		DWORD dw_ret;
		UCHAR* in_buffer, * out_buffer;

		class Payload {
		private:
			UINT max_len;
			UINT idx;
			BYTE* buffer;

		public:
			Payload(UINT max_len) : max_len(max_len) {
				buffer = new BYTE[max_len];
			};

			~Payload() {
				delete[] buffer;
			};

			void clear();
			void operator+(INT64);
			void operator+(INT32);
			void operator+(INT16);
			void operator+(INT8);
			void operator+(UINT64);
		};
	public:
		IO() {};
		bool create(const WCHAR* deviceLink);
		bool create(const CHAR* deviceLink);
		bool ioctl(DWORD ctlcode, LPVOID i_buf, DWORD i_size, LPVOID o_buf, DWORD o_size);

	};
}